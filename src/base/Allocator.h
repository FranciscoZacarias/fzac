#ifndef ALLOCATOR_H
#define ALLOCATOR_H

// @File: Implements memory allocators and thread context (used for temporary memory)

// @Section: Arena Allocator

#ifndef ARENA_RESERVE_SIZE
# define ARENA_RESERVE_SIZE megabytes(64)
#endif
#ifndef ARENA_COMMIT_SIZE
# define ARENA_COMMIT_SIZE kilobytes(64)
#endif

typedef struct Arena Arena;
struct Arena
{
  u64 reserved;    /* Reserved memory */
  u64 commited;    /* Commited memory */
  u64 commit_size; /* Size for each commit on this arena */
  u64 position;    /* Current position of the arena */
  u64 align;       /* Arena's memory alignment */
};
#define ARENA_HEADER_SIZE align_power_of_two(sizeof(Arena), os_memory_get_page_size())

function Arena* arena_alloc(); /* Allocates an arena with the default reserve and commit size */
function Arena* arena_alloc_sized(u64 reserve, u64 commit); /* Allocates an arena with specific reserve and commit size */

function void* arena_push(Arena* arena, u64 size); /* Pushes data into the arena */
function void* arena_push_no_zero(Arena* arena, u64 size); /* Pushes data into the arena without zeroing the memory */
function void  arena_pop(Arena* arena, u64 size); /* Moves the arena pointer back by <size> bytes */
function void  arena_pop_to(Arena* arena, u64 pos); /* Moves the arena pointer to the specific <pos> position */
function void  arena_clear(Arena* arena); /* Resets the arena position */
function void  arena_free(Arena* arena); /* Frees the arena's memory */
function void  print_arena(Arena *arena, const u8* label);

#define push_array(arena, type, count)         (type*) arena_push((arena), sizeof(type)*(count))
#define push_array_no_zero(arena, type, count) (type*) arena_push_no_zero((arena), sizeof(type)*(count))

typedef struct Scratch Scratch;
struct Scratch
{
  Arena* arena;
  u64 temp_position;
};

function Scratch arena_temp_begin(Arena* arena); /* Starts a temporary arena. Saves the current position. */
function void    arena_temp_end(Scratch* temp); /* Ends the temporary arena and pops to the position saved in arena_temp_begin */

// @Section: Thread context

#define DEFAULT_ARENAS_PER_THREAD_CONTEXT 2
static_assert(DEFAULT_ARENAS_PER_THREAD_CONTEXT > 0, "There must be at least 1 arena per thread context")

typedef struct Thread_Context Thread_Context;
struct Thread_Context
{
  Arena* arena; /* Persistant arena for lifetime allocations */
  Arena* temporary_arenas[DEFAULT_ARENAS_PER_THREAD_CONTEXT]; /* Used for scratches arenas */
};

C_LINKAGE thread_static Thread_Context* ThreadContextThreadLocal = 0;
global Thread_Context MainThreadContext;

function void            thread_context_init_and_attach(Thread_Context* thread_context); /* Initializes a thread context with DEFAULT_ARENAS_PER_THREAD_CONTEXT arenas */
function void            thread_context_free(); /* Frees the thread context */
function Thread_Context* thread_context_get(); /* Returns current thread context */
function Arena*         _thread_context_get_scratch(Arena** conflicts, u64 count); /* Returns a scratch arena */

#define scratch_begin(conflicts, count) arena_temp_begin(_thread_context_get_scratch((conflicts), (count)))
#define scratch_end(scratch) arena_temp_end(scratch)

// @Section: Implementation

function void
thread_context_init_and_attach(Thread_Context* thread_context)
{
  memory_zero_struct(thread_context);

  thread_context->arena = arena_alloc();
  for (u64 i = 0; i < array_count(thread_context->temporary_arenas); i += 1)
  {
    thread_context->temporary_arenas[i] = arena_alloc();
  }
  ThreadContextThreadLocal = thread_context;
}

function void
thread_context_free()
{
  for(u64 i = 0; i < array_count(ThreadContextThreadLocal->temporary_arenas); i += 1)
  {
    arena_free(ThreadContextThreadLocal->temporary_arenas[i]);
  }
}

function Thread_Context*
thread_context_get_equipped()
{
  return ThreadContextThreadLocal;
}

function Arena*
_thread_context_get_scratch(Arena **conflicts, u64 count)
{
  Thread_Context *thread_context = thread_context_get_equipped();
  assert(thread_context);

  Arena* result = 0;
  for (u64 i = 0; i < array_count(thread_context->temporary_arenas); i += 1)
  {
    Arena* candidate = thread_context->temporary_arenas[i];
    b32 has_conflict = 0;

    for (u64 j = 0; j < count; j += 1)
    {
      if (candidate == conflicts[j])
      {
        has_conflict = 1;
        break;
      }
    }

    if (!has_conflict)
    {
      result = candidate;
      break;
    }
  }

  if (result == 0)
  {
    result = thread_context->temporary_arenas[0];
  }

  return result;
}

function Arena*
arena_alloc()
{
  Arena* arena = arena_alloc_sized(ARENA_RESERVE_SIZE, ARENA_COMMIT_SIZE);
  return arena;
}

function Arena*
arena_alloc_sized(u64 reserve, u64 commit)
{
  void* memory = NULL;
  
  u64 page_size = os_memory_get_page_size();
  reserve = align_power_of_two(reserve, page_size);
  commit  = align_power_of_two(commit,  page_size);
  
  assert(ARENA_HEADER_SIZE < commit && commit <= reserve);
  
  memory = os_memory_reserve(reserve);
  if(!os_memory_commit(memory, commit))
  {
    memory = NULL;
    os_memory_free(memory, reserve);
  }
  
  Arena* arena = (Arena*) memory;
  
  if (arena)
  {
    arena->reserved    = reserve;
    arena->commited    = commit;
    arena->commit_size = commit;
    arena->position    = ARENA_HEADER_SIZE;
    arena->align       = DEFAULT_ALIGNMENT;
  }
  else
  {
    // @TODO(Fz): Error
    //emit_fatal(S("Error setting arena's memory"));
  }
  
  return arena;
}

function void*
arena_push(Arena* arena, u64 size)
{
  void* result =  arena_push_no_zero(arena, size);
  memory_zero(result, size);
  return result;
}

function void*
arena_push_no_zero(Arena* arena, u64 size)
{
  void* result = NULL;

  if (arena->position + size <= arena->reserved)
  {
    u64 position_memory = align_power_of_two(arena->position, arena->align);
    u64 new_position    = position_memory + size;
  
    if (arena->commited < new_position)
    {
      u64 commit_aligned = align_power_of_two(new_position, arena->commit_size);
      u64 commit_clamped = clamp_top(commit_aligned, arena->reserved);
      u64 commit_size    = commit_clamped - arena->commited;
      if (os_memory_commit((u8*)arena + arena->commited, commit_size))
      {
        arena->commited = commit_clamped;
      }
      else
      {
        // @TODO(Fz): Error
        //emit_error(S("Could not commit memory when increasing the arena's committed memory."));
      }
    }
    result = (u8*)arena + position_memory;
    arena->position = new_position;
  }
  else
  {
    // @TODO(Fz): Error
    //Scratch scratch = scratch_begin(0,0);
    //emit_error(Sf(scratch.arena, "Trying to allocate too much memory to a non dynamic arena.\nSize: %llu\nArena->Position: %llu\nArena->reserved: %llu\nArena->Position+Size: %llu", size, arena->position, arena->reserved, arena->position+size));
    //scratch_end(&scratch);
  }
  
  return result;
}

function void
arena_pop(Arena* arena, u64 size)
{
  if (size > arena->position)
  {
    // @TODO(Fz): Error
    //Scratch scratch = scratch_begin(0,0);
    //emit_warn(Sf(scratch.arena, "Warning :: Arena :: Trying to pop %lld bytes from arena with %lld allocated. Will pop %lld instead of %lld.\n", size, arena->position, arena->position, size));
    //scratch_end(&scratch);
    size = arena->position;
  }
  arena->position -= size;
}

function void
arena_pop_to(Arena* arena, u64 pos)
{
  if (pos > arena->reserved)
  {
    // @TODO(Fz): Error
    //Scratch scratch = scratch_begin(0,0);
    //emit_warn(Sf(scratch.arena, "Warning :: Arena :: Trying to pop over arena's reserved. Will pop only to %lld instead of %lld", arena->reserved, pos));
    //scratch_end(&scratch);
    pos = arena->reserved;
  }
  else if (pos < ARENA_HEADER_SIZE)
  {
    // @TODO(Fz): Error
    //Scratch scratch = scratch_begin(0,0);
    //emit_warn(Sf(scratch.arena, "Warning :: Arena :: Trying to pop arena under it's header size (pos -> %llu). Truncating it to header size", pos));
    //scratch_end(&scratch);
    pos = ARENA_HEADER_SIZE;
  }
  arena->position = pos;
}

function void
arena_clear(Arena* arena)
{
  arena_pop_to(arena, ARENA_HEADER_SIZE);
}

function void
arena_free(Arena* arena)
{
  os_memory_free((u8*)arena, arena->reserved);
}

function void
print_arena(Arena *arena, const u8* label)
{
  Scratch scratch = scratch_begin(0,0);

  // f64 committed_percentage = ((f64)arena->position / arena->commited) * 100.0f;
  // f64 reserved_percentage  = ((f64)arena->position / arena->reserved) * 100.0f;
  // @TODO(Fz): Print
  //emit_info(Sf(scratch.arena, "%s: Arena { reserved: %llu, commited: %llu, commit_size: %llu, position: %llu, align: %llu, committed_percentage: %.2f%%, reserved_percentage: %.2f%% }\n",
         //label, arena->reserved, arena->commited, arena->commit_size, arena->position, arena->align, committed_percentage, reserved_percentage));
  scratch_end(&scratch);
}

function Scratch
arena_temp_begin(Arena* arena)
{
  Scratch temp;
  temp.arena = arena;
  temp.temp_position = arena->position;
  return temp;
}

function void
arena_temp_end(Scratch* temp)
{
  arena_pop_to(temp->arena, temp->temp_position);
}

#endif // ALLOCATOR_H
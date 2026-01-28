#ifndef ALLOCATOR_H
#define ALLOCATOR_H

/* @File: Implements memory allocators and thread context (used for temporary memory) */ 

// @Section: Allocator
typedef struct Allocator Allocator; /* Allocator slot structure that holds function pointers to an actual allocator */
struct Allocator
{
  void* (*alloc)(u64 bytes, void* context);            /* Allocates memory zeroed out */
  void* (*alloc_no_zero)(u64 bytes, void* context);    /* Allocates memory without necessairly zeroing it out */
  void* (*free) (u64 bytes, void* ptr, void* context); /* Frees allocated memory */
  void* context;                                       /* Allocator specific context */
};

// @Section: Malloc Allocator
function void* _stdlib_alloc(u64 bytes, void* context); /* Allocates memory with the stdlib calloc function */
function void*
_stdlib_alloc(u64 bytes, void* context)
{
  return calloc(1, (size_t)bytes);
}
function void* _stdlib_alloc_no_zero(u64 bytes, void* context); /* Allocates memory with the stdlib malloc function */
function void*
_stdlib_alloc_no_zero(u64 bytes, void* context)
{
  return malloc((size_t)bytes);
}
function void* _stdlib_free(u64 bytes, void* ptr, void* context); /* Calls free on memory allocated with stdlib allocator */
function void*
_stdlib_free(u64 bytes, void* ptr, void* context)
{
  free(ptr); return NULL;
}

global Allocator MallocAllocator = {
  .alloc         = _stdlib_alloc,
  .alloc_no_zero = _stdlib_alloc_no_zero,
  .free          = _stdlib_free,
  .context       = NULL,
};

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
#define ARENA_HEADER_SIZE align_power_of_two(sizeof(Arena), memory_get_page_size())

function Arena* arena_alloc(); /* Allocates an arena with the default reserve and commit size */
function Arena* arena_alloc_sized(u64 reserve, u64 commit); /* Allocates an arena with specific reserve and commit size */

#define        arena_push(arena, type, count)         (type*) _arena_push((arena), sizeof(type)*(count))
#define        arena_push_no_zero(arena, type, count) (type*) _arena_push_no_zero((arena), sizeof(type)*(count))
function void  arena_pop(Arena* arena, u64 size); /* Moves the arena pointer back by <size> bytes */
function void  arena_pop_to(Arena* arena, u64 pos); /* Moves the arena pointer to the specific <pos> position */
function void  arena_clear(Arena* arena); /* Resets the arena position */
function void  arena_free(Arena* arena); /* Frees the arena's memory */

function void* _arena_push(Arena* arena, u64 size); /* Pushes data into the arena */
function void* _arena_push_no_zero(Arena* arena, u64 size); /* Pushes data into the arena without zeroing the memory */

typedef struct Scratch Scratch;
struct Scratch
{
  Arena* arena;
  u64 temp_position;
};

function Scratch arena_temp_begin(Arena* arena); /* Starts a temporary arena. Saves the current position. */
function void    arena_temp_end(Scratch* temp); /* Ends the temporary arena and pops to the position saved in arena_temp_begin */

// Helper to push data into an arena backed array
#define ARENA_ARRAY_PUSH(ptr, count, cap) &(ptr)[(count)++]

// @Section: Implementation

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
  
  u64 page_size = memory_get_page_size();
  reserve = align_power_of_two(reserve, page_size);
  commit  = align_power_of_two(commit,  page_size);
  
  assert(ARENA_HEADER_SIZE < commit && commit <= reserve);
  
  memory = memory_reserve(reserve);
  if(!memory_commit(memory, commit))
  {
    memory = NULL;
    memory_free(memory, reserve);
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
_arena_push(Arena* arena, u64 size)
{
  void* result = _arena_push_no_zero(arena, size);
  memory_zero(result, size);
  return result;
}

function void*
_arena_push_no_zero(Arena* arena, u64 size)
{
  void* result = NULL;

  if (size != 0)
  {
    if (arena->position + size <= arena->reserved)
    {
      u64 position_memory = align_power_of_two(arena->position, arena->align);
      u64 new_position    = position_memory + size;
  
      if (arena->commited < new_position)
      {
        u64 commit_aligned = align_power_of_two(new_position, arena->commit_size);
        u64 commit_clamped = clamp_top(commit_aligned, arena->reserved);
        u64 commit_size    = commit_clamped - arena->commited;
        if (memory_commit((u8*)arena + arena->commited, commit_size))
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
  memory_free((u8*)arena, arena->reserved);
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
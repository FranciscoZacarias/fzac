#ifndef ARENA_H
#define ARENA_H

#ifndef ARENA_RESERVE_SIZE
# define ARENA_RESERVE_SIZE megabytes(64)
#endif
#ifndef ARENA_COMMIT_SIZE
# define ARENA_COMMIT_SIZE kilobytes(64)
#endif

typedef struct Arena Arena;
struct Arena
{
  u64 reserved;     /* Reserved memory */
  u64 committed;    /* Committed memory */
  u64 commit_size;  /* Size for each commit on this arena */
  u64 position;     /* Current position of the arena */
  u64 align;        /* Arena's memory alignment */
};

/* NOTE: ARENA_HEADER_SIZE must be computed at runtime because memory_get_page_size()
   is a runtime value, not a compile-time constant. Use arena_header_size() instead. */
function u64 arena_header_size();

function Arena* arena_alloc();                           /* Allocates an arena with the default reserve and commit size */
function Arena* arena_alloc_sized(u64 reserve, u64 commit); /* Allocates an arena with specific reserve and commit size */

#define        push_array(arena, type, count)         (type*) _push_array((arena), sizeof(type)*(count))
#define        push_struct(arena, type)               (type*) _push_array((arena), sizeof(type))
#define        push_array_no_zero(arena, type, count) (type*) _push_array_no_zero((arena), sizeof(type)*(count))
function void  arena_pop(Arena* arena, u64 size);        /* Moves the arena pointer back by <size> bytes */
function void  arena_pop_to(Arena* arena, u64 pos);      /* Moves the arena pointer to the specific <pos> position */
function void  arena_clear(Arena* arena);                /* Resets the arena position */
function void  arena_free(Arena* arena);                 /* Frees the arena's memory */

function void* _push_array(Arena* arena, u64 size);          /* Pushes zeroed data into the arena */
function void* _push_array_no_zero(Arena* arena, u64 size);  /* Pushes data into the arena without zeroing */

typedef struct Scratch Scratch;
struct Scratch
{
  Arena* arena;
  u64 temp_position;
};

function Scratch arena_temp_begin(Arena* arena); /* Saves the current position for later rollback */
function void    arena_temp_end(Scratch* temp);  /* Rolls back to the position saved in arena_temp_begin */

// Helper to push data into an arena backed array

#define Array(name, type) \
  type *name;             \
  u32 name##_count;       \
  u32 name##_capacity

#define array_init(arena, name, type, capacity)  \
  statement(                                           \
    assert((arena) != NULL);                           \
    name##_count = 0;                                  \
    name##_capacity = (capacity);                      \
    name = push_array((arena), type, name##_capacity); \
  )

#define array_add(name) _array_add_impl((void*)name, &(name##_count), name##_capacity, sizeof((name)[0]))

#define array_pop(out_ptr, name)       \
  statement(                                 \
    if ((name##_count) == 0)                 \
    {                                        \
      message_box(S("Arena Array Underflow"),\
        St("array_pop on empty array"),\
        S(__FILE__), __LINE__);              \
      assert(false);                         \
    }                                        \
    (out_ptr) = &(name)[--(name##_count)];   \
  )

#define each_array(idx, arr) (u32 (idx)=0; (idx) < glue(arr,_count); (idx) += 1)

// @Section: Implementation

function void*
_array_add_impl(void* array, u32* count, u32 capacity, u64 element_size)
{
  if (*count >= capacity)
  {
    // @TODO(Fz): error
    assert(false);
  }

  void* result = (u8*)array + (*count * element_size);
  *count += 1;
  return result;
}

function u64
arena_header_size()
{
  return align_power_of_two(sizeof(Arena), memory_get_page_size());
}

function Arena*
arena_alloc()
{
  return arena_alloc_sized(ARENA_RESERVE_SIZE, ARENA_COMMIT_SIZE);
}

function Arena*
arena_alloc_sized(u64 reserve, u64 commit)
{
  u64 page_size = memory_get_page_size();
  u64 header_size = arena_header_size();

  reserve = align_power_of_two(reserve, page_size);
  commit  = align_power_of_two(commit,  page_size);

  assert(header_size < commit && commit <= reserve);

  void* memory = memory_reserve(reserve);
  if (!memory)
  {
    // @TODO(Fz): emit_fatal(S("Failed to reserve virtual memory for arena."));
    assert(!"arena_alloc_sized: memory_reserve failed");
    return NULL;
  }

  if (!memory_commit(memory, commit))
  {
    // @TODO(Fz): emit_fatal(S("Failed to commit initial memory for arena."));
    assert(!"arena_alloc_sized: memory_commit failed");
    memory_free(memory, reserve); // free BEFORE nulling the pointer
    return NULL;
  }

  Arena* arena      = (Arena*) memory;
  arena->reserved   = reserve;
  arena->committed  = commit;
  arena->commit_size = commit;
  arena->position   = header_size;
  arena->align      = DEFAULT_ALIGNMENT;

  return arena;
}

function void*
_push_array(Arena* arena, u64 size)
{
  void* result = _push_array_no_zero(arena, size);
  if (result)
  {
    memory_zero(result, size);
  }
  return result;
}

function void*
_push_array_no_zero(Arena* arena, u64 size)
{
  if (!arena || size == 0)
  {
    return NULL;
  }

  /* Align the current position first, THEN check if the total fits in reserved. */
  u64 position_aligned = align_power_of_two(arena->position, arena->align);
  u64 new_position     = position_aligned + size;

  /* Check for integer overflow and reserved overflow in one place. */
  if (new_position < position_aligned || new_position > arena->reserved)
  {
    // @TODO(Fz): emit_error(Sf(..., "Arena out of memory. size=%llu pos=%llu reserved=%llu", size, arena->position, arena->reserved));
    assert(!"_push_array_no_zero: arena out of memory");
    return NULL;
  }

  if (arena->committed < new_position)
  {
    u64 commit_aligned = align_power_of_two(new_position, arena->commit_size);
    u64 commit_clamped = clamp_top(commit_aligned, arena->reserved);
    u64 commit_size    = commit_clamped - arena->committed;

    if (!memory_commit((u8*)arena + arena->committed, commit_size))
    {
      // @TODO(Fz): emit_error(S("Arena failed to commit more memory."));
      assert(!"_push_array_no_zero: memory_commit failed during grow");
      return NULL;
    }

    arena->committed = commit_clamped;
  }

  void* result    = (u8*)arena + position_aligned;
  arena->position = new_position;
  return result;
}

function void
arena_pop(Arena* arena, u64 size)
{
  if (!arena) { return; }

  u64 header_size = arena_header_size();
  if (size > arena->position - header_size)
  {
    // @TODO(Fz): emit_warn(..., "arena_pop: clamping pop size to available bytes");
    size = arena->position - header_size;
  }
  arena->position -= size;
}

function void
arena_pop_to(Arena* arena, u64 pos)
{
  if (!arena) { return; }

  u64 header_size = arena_header_size();
  if (pos > arena->reserved)
  {
    // @TODO(Fz): emit_warn(..., "arena_pop_to: pos exceeds reserved, clamping");
    pos = arena->reserved;
  }
  else if (pos < header_size)
  {
    // @TODO(Fz): emit_warn(..., "arena_pop_to: pos is below header, clamping to header");
    pos = header_size;
  }
  arena->position = pos;
}

function void
arena_clear(Arena* arena)
{
  arena_pop_to(arena, arena_header_size());
}

function void
arena_free(Arena* arena)
{
  if (!arena) { return; }
  memory_free((u8*)arena, arena->reserved);
}

function Scratch
arena_temp_begin(Arena* arena)
{
  Scratch temp;
  temp.arena         = arena;
  temp.temp_position = arena ? arena->position : 0;
  return temp;
}

function void
arena_temp_end(Scratch* temp)
{
  if (!temp || !temp->arena) { return; }
  arena_pop_to(temp->arena, temp->temp_position);
}

#endif // ARENA_H
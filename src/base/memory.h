#ifndef MEMORY_H
#define MEMORY_H

// @Section: Arena Allocator

#ifndef ArenaReserveSize
# define ArenaReserveSize megabytes(64)
#endif
#ifndef ArenaCommitSize
# define ArenaCommitSize kilobytes(64)
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
#define ArenaHeaderSize align_power_of_two(sizeof(Arena), os_memory_get_page_size())

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

#endif // MEMORY_H
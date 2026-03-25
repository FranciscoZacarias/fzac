#ifndef POOL_H
#define POOL_H

typedef struct Pool Pool;
struct Pool
{
  void* free_list;   /* Head of the embedded free list of available chunks */
  u64   chunk_size;  /* Size of each chunk in bytes (aligned) */
  u64   chunk_count; /* Total number of chunks in the pool */
  u64   used;        /* Number of chunks currently in use */
  u64   reserved;    /* Total reserved virtual memory in bytes */
  u64   committed;   /* Total committed virtual memory in bytes */
  u8*   memory;      /* Pointer to the start of the chunk block */
};

/* NOTE: Pool header size must be computed at runtime, same reason as Arena. */
function u64   pool_header_size();

function Pool* pool_alloc(u64 chunk_size, u64 chunk_count); /* Allocates a pool, reserving and committing virtual memory directly. chunk_size  - size of each individual element in bytes chunk_count - total number of elements the pool can hold */
function void* pool_get(Pool* pool); /* Returns a zeroed chunk from the pool. Asserts and returns NULL if full. */
function void* pool_get_no_zero(Pool* pool); /* Returns an unzeroed chunk from the pool. Asserts and returns NULL if full. */
function void  pool_free_chunk(Pool* pool, void* chunk); /* Returns a chunk back to the pool for reuse. Safe to call with NULL. */
function void  pool_clear(Pool* pool); /* Resets the pool so all chunks are available again. Does not zero memory. */
function void  pool_free(Pool* pool); /* Releases all virtual memory. Pool pointer and all chunks become invalid. */

// @Section: Implementation

function u64
pool_header_size()
{
  return align_power_of_two(sizeof(Pool), memory_get_page_size());
}

function Pool*
pool_alloc(u64 chunk_size, u64 chunk_count)
{
  if (chunk_count == 0)
  {
    assert(!"pool_alloc: chunk_count must be >= 1");
    return NULL;
  }

  /* Enforce minimum chunk size so the free list pointer always fits. */
  if (chunk_size < sizeof(void*))
  {
    chunk_size = sizeof(void*);
  }

  /* Align chunk size so every chunk is naturally aligned. */
  chunk_size = align_power_of_two(chunk_size, DEFAULT_ALIGNMENT);

  u64 page_size   = memory_get_page_size();
  u64 header_size = pool_header_size();
  u64 block_size  = chunk_size * chunk_count;
  u64 total_size  = align_power_of_two(header_size + block_size, page_size);

  void* memory = memory_reserve(total_size);
  if (!memory)
  {
    assert(!"pool_alloc: memory_reserve failed");
    return NULL;
  }

  if (!memory_commit(memory, total_size))
  {
    assert(!"pool_alloc: memory_commit failed");
    memory_free(memory, total_size);
    return NULL;
  }

  Pool* pool      = (Pool*) memory;
  pool->chunk_size  = chunk_size;
  pool->chunk_count = chunk_count;
  pool->used        = 0;
  pool->reserved    = total_size;
  pool->committed   = total_size;
  pool->memory      = (u8*)memory + header_size;
  pool->free_list   = NULL;

  /* Build the initial free list */
  for (u64 i = 0; i < chunk_count; i++)
  {
    void** chunk    = (void**)(pool->memory + i * chunk_size);
    *chunk          = pool->free_list;
    pool->free_list = chunk;
  }

  return pool;
}

function void*
pool_get_no_zero(Pool* pool)
{
  if (!pool)
  {
    assert(!"pool_get_no_zero: pool is NULL");
    return NULL;
  }
  if (!pool->free_list)
  {
    assert(!"pool_get_no_zero: pool is full");
    return NULL;
  }

  /* Pop from the head of the free list. */
  void** chunk    = (void**) pool->free_list;
  pool->free_list = *chunk;
  pool->used     += 1;

  return (void*) chunk;
}

function void*
pool_get(Pool* pool)
{
  void* chunk = pool_get_no_zero(pool);
  if (chunk)
  {
    memory_zero(chunk, pool->chunk_size);
  }
  return chunk;
}

function void
pool_free_chunk(Pool* pool, void* chunk)
{
  if (!pool || !chunk)
  {
    return;
  }

  /* Bounds check: ensure the pointer belongs to this pool. */
  u8* start = pool->memory;
  u8* end   = pool->memory + pool->chunk_size * pool->chunk_count;
  if ((u8*)chunk < start || (u8*)chunk >= end)
  {
    assert(!"pool_free_chunk: chunk does not belong to this pool");
    return;
  }

  /* Alignment check: pointer must land exactly on a chunk boundary. */
  u64 offset = (u8*)chunk - start;
  if (offset % pool->chunk_size != 0)
  {
    assert(!"pool_free_chunk: chunk pointer is not aligned to a chunk boundary");
    return;
  }

  /* Push onto the head of the free list. */
  void** free_chunk = (void**) chunk;
  *free_chunk     = pool->free_list;
  pool->free_list = free_chunk;
  pool->used     -= 1;
}

function void
pool_clear(Pool* pool)
{
  if (!pool)
  {
    return;
  }

  pool->free_list = NULL;
  pool->used      = 0;

  for (u64 i = 0; i < pool->chunk_count; i++)
  {
    void** chunk    = (void**)(pool->memory + i * pool->chunk_size);
    *chunk          = pool->free_list;
    pool->free_list = chunk;
  }
}

function void
pool_free(Pool* pool)
{
  if (!pool)
  {
    return;
  }
  memory_free((u8*)pool, pool->reserved);
}

#endif // POOL_H
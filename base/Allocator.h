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

#endif // ALLOCATOR_H
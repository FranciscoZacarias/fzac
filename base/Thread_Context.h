#ifndef THREAD_CONTEXT_H
#define THREAD_CONTEXT_H

#define DEFAULT_ARENAS_PER_THREAD_CONTEXT 2

typedef struct Thread_Context Thread_Context;
struct Thread_Context
{
  Arena *temporary_storage; /* Used for allocating without thinking too hard about where the allocation goes to. Use get_temporary_storage() and clear_temporary_storage(); */
  Arena *temporary_arenas[DEFAULT_ARENAS_PER_THREAD_CONTEXT]; /* Used for scratches arenas */
};

C_LINKAGE thread_static Thread_Context* ThreadContextThreadLocal = 0;
fz_global Thread_Context MainThreadContext;

fz_function void            thread_context_init_and_attach(Thread_Context* thread_context); /* Initializes a thread context with DEFAULT_ARENAS_PER_THREAD_CONTEXT arenas */
fz_function void            thread_context_free(); /* Frees the thread context */
fz_function Arena*         _thread_context_get_scratch(Arena** conflicts, u64 count); /* Returns a scratch arena */
fz_function Thread_Context* thread_context_get_equipped(); /* Returns currently attached thread_context */

fz_function Arena* get_temporary_storage();
fz_function void   clear_temporary_storage();

#define scratch_begin(conflicts, count) arena_temp_begin(_thread_context_get_scratch((conflicts), (count)))
#define scratch_end(scratch) arena_temp_end(scratch)

fz_function Arena* 
get_temporary_storage()
{
  Thread_Context *ctx = thread_context_get_equipped();
  return ctx->temporary_storage;
}

fz_function void   
clear_temporary_storage()
{
  Thread_Context *ctx = thread_context_get_equipped();
  arena_clear(ctx->temporary_storage);
}

fz_function void
thread_context_init_and_attach(Thread_Context* thread_context)
{
  memory_zero_struct(thread_context);

  thread_context->temporary_storage = arena_alloc();
  for (u64 i = 0; i < c_array_count(thread_context->temporary_arenas); i += 1)
  {
    thread_context->temporary_arenas[i] = arena_alloc();
  }
  ThreadContextThreadLocal = thread_context;
}

fz_function void
thread_context_free()
{
  for(u64 i = 0; i < c_array_count(ThreadContextThreadLocal->temporary_arenas); i += 1)
  {
    arena_free(ThreadContextThreadLocal->temporary_arenas[i]);
  }
}

fz_function Thread_Context*
thread_context_get_equipped()
{
  return ThreadContextThreadLocal;
}

fz_function Arena*
_thread_context_get_scratch(Arena **conflicts, u64 count)
{
  Thread_Context *thread_context = thread_context_get_equipped();
  assert(thread_context);

  Arena* result = 0;
  for (u64 i = 0; i < c_array_count(thread_context->temporary_arenas); i += 1)
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

#endif // THREAD_CONTEXT_H
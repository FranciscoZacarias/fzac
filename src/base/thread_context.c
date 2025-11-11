function void
thread_context_init_and_attach(Thread_Context* thread_context)
{
  memory_zero_struct(thread_context);
  Arena** arena_ptr = thread_context->arenas;
  for (u64 i = 0; i < array_count(thread_context->arenas); i += 1, arena_ptr += 1)
  {
    *arena_ptr = arena_alloc();
  }
  ThreadContextThreadLocal = thread_context;
}

function void
thread_context_free()
{
  for(u64 i = 0; i < array_count(ThreadContextThreadLocal->arenas); i += 1)
  {
    arena_free(ThreadContextThreadLocal->arenas[i]);
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
  for (u64 i = 0; i < array_count(thread_context->arenas); i += 1)
  {
    Arena* candidate = thread_context->arenas[i];
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
    result = thread_context->arenas[0];
  }

  return result;
}
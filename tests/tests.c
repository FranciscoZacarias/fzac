#include "../fzac.h"

function void
string_test(Test_Result* test)
{
  Arena* arena = arena_alloc();
  (void)test;

  arena_free(arena);
}

int main(void)
{
  return 0;
}

#include <stdio.h>
#include <signal.h>

fz_internal void
message_box(String title, String content, String file, u32 line)
{
  // Linux doesn´t really have a MessageBox like windows.
  // I think we could use 'zenity' but we woulde have to spawn a process? Maybe.
  fprintf(stderr, "[%.*s] %.*s line:%u\n%.*s\n", S_ARG(title), S_ARG(file), line, S_ARG(content));
}

fz_internal void
message_box_with_debugger(String title, String content, String file, u32 line)
{
  Scratch scratch = scratch_begin(0, 0);

  fprintf(stderr, "\n========================================\n");
  fprintf(stderr, "[%.*s] %.*s line:%u\n%.*s\n", S_ARG(title), S_ARG(file), line, S_ARG(content));
  fprintf(stderr, "========================================\n");

  for(;;)
  {
    if (raddbg_is_attached())
    {
      fprintf(stderr, "Debugger attached. Trapping into debugger...\n");
      scratch_end(&scratch);
      
#if defined(__GNUC__) || defined(__clang__)
      __builtin_trap();
#else
      raise(SIGTRAP);
#endif
      return;
    }
    else
    {
      fprintf(stderr, "No debugger attached. [r]etry check / [c]ontinue? ");
      fflush(stderr);

      int c = getchar();
      while (c != '\n' && c != EOF && getchar() != '\n');
      if (c == 'c' || c == 'C' || c == EOF)
      {
        break;
      }
    }
  }

  scratch_end(&scratch);
}
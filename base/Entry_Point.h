#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#include "..\modules\Command_Line.h"

function void entry_point(Command_Line* command_line); /* Application entry point, defined by user. */

function void
main_thread_base_entry_point(String command_line)
{
  local_persist Thread_Context thread_context;
  thread_context_init_and_attach(&thread_context);

  Command_Line cmd_line = command_line_parse(command_line);
  entry_point(&cmd_line);
}

#if OS_WINDOWS

# pragma comment(lib, "user32.lib")
# pragma comment(lib, "gdi32.lib")
# pragma comment(lib, "kernel32.lib")

# define WIN32_LEAN_AND_MEAN
# include <windows.h>

int WINAPI
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  String command_line = S("");

  // Convert PWSTR to String
  s32 needed = WideCharToMultiByte(CP_UTF8, 0, pCmdLine, -1, 0, 0, 0, 0);
  if (needed > 1)
  {
    u8* buf = (u8*)VirtualAlloc(0, needed, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    WideCharToMultiByte(CP_UTF8, 0, pCmdLine, -1, (LPSTR)buf, needed, 0, 0);
    command_line.count   = (u64)needed - 1;
    command_line.cstring = buf;
  }

  main_thread_base_entry_point(command_line);
  return 0;
}

#elif OS_LINUX
# error fzac with Linux not supported
#elif OS_MACOS
# error Macos not supported
#else
# error fzac with Operating System not supported
#endif

#endif // ENTRY_POINT_H
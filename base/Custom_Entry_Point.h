#ifndef CUSTOM_ENTRY_POINT_H
#define CUSTOM_ENTRY_POINT_H

#include "..\modules\Command_Line.h"

#if METAPROGRAM
function void metaprogram_entry_point(Arena* arena,Command_Line *command_line, String project_path); /* Metaprogram entry point, defined by user. */
raddbg_entry_point(metaprogram_entry_point);
function void metaprogram_main_thread_base_entry_point(String command_line); /* Internal entry point for the main thread in the 'fzac' codebase */
#else
function void entry_point(Command_Line *command_line); /* Application entry point, defined by user. */
raddbg_entry_point(entry_point);
function void main_thread_base_entry_point(String command_line); /* Internal entry point for the main thread in the 'fzac' codebase */
#endif

// @Section: Implementation

#if METAPROGRAM
#include "Code_Generation.h"
#include "Platform.h"
function void
metaprogram_main_thread_base_entry_point(String command_line)
{
  local_persist Thread_Context thread_context;
  thread_context_init_and_attach(&thread_context);
  Command_Line cmd_line = command_line_parse(command_line);
  
  // Default metaprogram 
  Arena* arena = arena_alloc();
  {
    console_attach();
    String path = full_path_from_relative_path(arena, S("../src"));
    if (cmd_line.args_count > 0)
    {
      for (u32 i = 0; i < cmd_line.args_count; i += 1)
      {
        Command_Line_Arg arg = cmd_line.args[i];
        if (string_equals(arg.value, S("cgen"), false))
        {
          CGen_Context cgen = cgen_run(path);
          cgen_execute_commands(&cgen);
        }
      }
    }
  }
  
  metaprogram_entry_point(arena, &cmd_line, S("../src"));
}
#else
function void
main_thread_base_entry_point(String command_line)
{
  local_persist Thread_Context thread_context;
  thread_context_init_and_attach(&thread_context);
  Command_Line cmd_line = command_line_parse(command_line);
  entry_point(&cmd_line);
}
#endif

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

#if METAPROGRAM
  metaprogram_main_thread_base_entry_point(command_line);
#else
  main_thread_base_entry_point(command_line);
#endif
  return 0;
}

#elif OS_LINUX
# error fzac with Linux not supported
#elif OS_MACOS
# error Macos not supported
#else
# error fzac with Operating System not supported
#endif

#endif // CUSTOM_ENTRY_POINT_H
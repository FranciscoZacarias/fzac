#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

function void entry_point(Command_Line command_line); /* Application entry point, defined by user. */

function void
main_thread_base_entry_point(int argc, char **argv)
{
  Scratch scratch = scratch_begin(0, 0);
  os_time_init();

  // Setup log system
  {
    String8 log_path = os_executable_path(scratch.arena);
    log_path = os_directory_pop(log_path); // Pop the applicatin.exe file
    log_path = os_directory_pop(log_path); // Pop from build directory
    log_init(log_path);
    emit_info(S(">>>> Entry point <<<<"));
  }

  Command_Line cmd_line = command_line_parse_from_argc_argv(argc, argv);
  entry_point(&cmd_line);
  scratch_end(&scratch);
}

#if OS_WINDOWS

# pragma comment(lib, "user32.lib")
# pragma comment(lib, "gdi32.lib")
# pragma comment(lib, "kernel32.lib")

# define WIN32_LEAN_AND_MEAN
# include <windows.h>

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
  local_persist Thread_Context thread_context;
  thread_context_init_and_attach(&thread_context);

  main_thread_base_entry_point(__argc, __argv);
  return _g_application_return;
}

#elif OS_LINUX
# error fzac with Linux not supported
#elif OS_MACOS
# error Macos not supported
#else
# error fzac with Operating System not supported
#endif

#endif // ENTRY_POINT_H
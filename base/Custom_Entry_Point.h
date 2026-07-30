#ifndef CUSTOM_ENTRY_POINT_H
#define CUSTOM_ENTRY_POINT_H

#include "..\modules\Command_Line.h"
#define enum_type(name, type, to_string) enum // This macro is so that the metaprogram knows what enum is being defined

#if METAPROGRAM
  #include "Code_Generation.h"
  #include "Platform.h"
  #include "Default_Metaprogram.h"
  
  fz_global Default_Metaprogram DefaultMetaprogram;
  
  // Metaprogram
  #define METAPROGRAM_SRC_DIRECTORY S("../src")
  fz_internal void metaprogram_entry_point(Default_Metaprogram *dm, Command_Line *command_line, String project_path, b32 *run_default_metaprogram, String *global_headers_extra_data);  /* Metaprogram entry point, defined by user. */
  raddbg_entry_point(metaprogram_entry_point);
  fz_internal void metaprogram_main_thread_base_entry_point(String command_line); /* Internal entry point for the main thread in the 'fzac' codebase */
  
  fz_internal void
  metaprogram_main_thread_base_entry_point(String command_line)
  {
    fz_local_persist Thread_Context thread_context;
    thread_context_init_and_attach(&thread_context);
    Command_Line cmd_line = command_line_parse(command_line);
  
    console_attach();
  
    DefaultMetaprogram.arena = arena_alloc();
    DefaultMetaprogram.files_capacity = 64;
  
    // Metaprogram optional flags
    // Default Metaprogram
    
    // User can toggle this off if doesn't want to run default metaprogram
    b32 run_default_metaprogram = true; 

    // If this string pointer is not NULL, it will be added to fz_global headers. 
    // Must be allocated by the user and not cleaned up, so the metaprogram can use it.
    // Only writes to global_headers.h!! The metaprogram_global_headers cannot be modified
    String global_headers_extra_data = S(""); 

    // @TODO(fz): project_path should pass the full path, not relative
    metaprogram_entry_point(&DefaultMetaprogram, &cmd_line, S("../src"), &run_default_metaprogram, &global_headers_extra_data);
    if (run_default_metaprogram)
    {
      default_metaprogram(&DefaultMetaprogram, &cmd_line, METAPROGRAM_SRC_DIRECTORY, &global_headers_extra_data);  
    }
    printf("Metaprogram Finished!\n");
    arena_free(DefaultMetaprogram.arena);
  }
#else
  fz_internal void entry_point(Command_Line *command_line); /* Application entry point, defined by user. */
  raddbg_entry_point(entry_point);
  fz_internal void main_thread_base_entry_point(String command_line); /* Internal entry point for the main thread in the 'fzac' codebase */
  
  fz_internal void
  main_thread_base_entry_point(String command_line)
  {
    fz_local_persist Thread_Context thread_context;
    thread_context_init_and_attach(&thread_context);
    Command_Line cmd_line = command_line_parse(command_line);
    entry_point(&cmd_line);
  }
#endif // METAPROGRAM


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
# error fzac with Macos not supported
#else
# error fzac doesn't even know what OS this is
#endif

#endif // CUSTOM_ENTRY_POINT_H
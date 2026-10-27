#if defined(NOUSER)
# define MB_OK         0x00000000L
# define MB_OKCANCEL   0x00000001L
# define MB_ICONERROR  0x00000010L
# define MB_TASKMODAL  0x00002000L
# define IDOK          1
# define IDCANCEL      2
__declspec(dllimport) int __stdcall MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
#endif

fz_internal void
console_attach()
{
  b32 attached = AttachConsole(ATTACH_PARENT_PROCESS);

  if (!attached)
  {
    AllocConsole();
  }

  FILE *fp;

  freopen_s(&fp, "CONOUT$", "w", stdout);
  freopen_s(&fp, "CONOUT$", "w", stderr);
  freopen_s(&fp, "CONIN$",  "r", stdin);

  setvbuf(stdout, 0, _IONBF, 0);
  setvbuf(stderr, 0, _IONBF, 0);

  // Enable ANSI escape sequences
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  if (handle != INVALID_HANDLE_VALUE)
  {
    DWORD mode = 0;
    if (GetConsoleMode(handle, &mode))
    {
      SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
  }
}

fz_internal b32
debugger_is_attached()
{
  return IsDebuggerPresent() || raddbg_is_attached();
}

fz_internal void
fatal_error(const char* message)
{
  OutputDebugStringA(message);
  OutputDebugStringA("\n");
  if (debugger_is_attached())
  {
    __debugbreak();
  }
  MessageBoxA(NULL, message, "Fatal Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
  ExitProcess(1);
}

fz_internal void
message_box(String title, String content, String file, u32 line)
{
  Scratch scratch = scratch_begin(0,0);
  String body = Sf(scratch.arena, S_FMT" line:%u\n"S_FMT, S_ARG(file), line, S_ARG(content));
  MessageBoxA(NULL, (LPCSTR)body.cstring, (LPCSTR)title.cstring, MB_OK);
  scratch_end(&scratch);
}

fz_internal void
message_box_with_debugger(String title, String content, String file, u32 line)
{
  Scratch scratch = scratch_begin(0, 0);
  String body;

  for(;;)
  {
    if(debugger_is_attached())
    {
      body = Sf(scratch.arena, S_FMT " line:%u\n" S_FMT "\n\nDebugger attached. Press 'OK' to trap, or 'Cancel' to dismiss.", S_ARG(file), line, S_ARG(content));
      int result = MessageBoxA(NULL, (LPCSTR)body.cstring, (LPCSTR)title.cstring, MB_OKCANCEL | MB_ICONERROR | MB_TASKMODAL);
      if(result == IDOK)
      {
        scratch_end(&scratch);
        __debugbreak();
        return;
      }
      break;
    }
    else
    {
      body = Sf(scratch.arena, S_FMT " line:%u\n" S_FMT "\n\nNo debugger attached.\n'OK' to check again, 'Cancel' to close.", S_ARG(file), line, S_ARG(content));
      int result = MessageBoxA(NULL, (LPCSTR)body.cstring, (LPCSTR)title.cstring, MB_OKCANCEL | MB_ICONERROR | MB_TASKMODAL);
      if(result == IDCANCEL) 
      { 
        break; 
      }
      if (result == IDOK)
      {
        // IDOK: loop again and re-check
      }
    }
  }

  scratch_end(&scratch);
}
function void
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


function void
message_box(String title, String content, String file, u32 line)
{
  Scratch scratch = scratch_begin(0,0);
  String body = Sf(scratch.arena, S_FMT" line:%u\n"S_FMT, S_ARG(file), line, S_ARG(content));
  MessageBoxA(NULL, (LPCSTR)body.cstring, (LPCSTR)title.cstring, MB_OK);
  scratch_end(&scratch);
}
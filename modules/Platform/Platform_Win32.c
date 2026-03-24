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
clipboard_write(String str)
{
  if(!OpenClipboard(0))
  {
    return;
  }

  EmptyClipboard();
  s32 wchar_count = MultiByteToWideChar(CP_UTF8, 0, (char*)str.cstring, (int)str.count, 0, 0);
  HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, (wchar_count + 1) * sizeof(wchar_t));
  if(!mem)
  {
    CloseClipboard();
    return;
  }

  wchar_t *buffer = (wchar_t*)GlobalLock(mem);
  MultiByteToWideChar(CP_UTF8, 0, (char*)str.cstring, (int)str.count, buffer, wchar_count);
  buffer[wchar_count] = 0;
  GlobalUnlock(mem);
  SetClipboardData(CF_UNICODETEXT, mem);
  CloseClipboard();
}

function String
clipboard_read(Arena *arena)
{
  String result = {0};

  if(!OpenClipboard(0))
  {
    return result;
  }

  HANDLE handle = GetClipboardData(CF_UNICODETEXT);
  if(!handle)
  {
    CloseClipboard();
    return result;
  }

  wchar_t *wstr = (wchar_t*)GlobalLock(handle);
  if(!wstr)
  {
    CloseClipboard();
    return result;
  }

  s32 wchar_count = (s32)wcslen(wstr);
  s32 utf8_size = WideCharToMultiByte(CP_UTF8, 0, wstr, wchar_count, 0, 0, 0, 0);
  u8 *buffer = push_array(arena, u8, utf8_size + 1);
  WideCharToMultiByte(CP_UTF8, 0, wstr, wchar_count, (char*)buffer, utf8_size, 0, 0);
  buffer[utf8_size] = 0;
  result.count   = utf8_size;
  result.cstring = buffer;
  GlobalUnlock(handle);
  CloseClipboard();

  return result;
}

function void
message_box(String title, String content, String file, u32 line)
{
  Scratch scratch = scratch_begin(0,0);
  String body = Sf(scratch.arena, S_FMT" line:%u\n"S_FMT, S_ARG(file), line, S_ARG(content));
  MessageBoxA(NULL, (LPCSTR)body.cstring, (LPCSTR)title.cstring, MB_OK);
  scratch_end(&scratch);
}
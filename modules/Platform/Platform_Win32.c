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

fz_internal void
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

fz_internal String
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

fz_internal V2f32 
cursor_get_position(Window *window)
{
  V2f32 result = {0};

  POINT point;
  GetCursorPos(&point);

  ScreenToClient(window->hwnd, &point);

  result.x = point.x;
  result.y = point.y;

  return result;
}

fz_internal void
cursor_lock(Window *window)
{
  if (window->input.is_cursor_locked)
  {
    return;
  }

  RECT rect;
  GetClientRect(window->hwnd, &rect);

  POINT top_left  = { rect.left,  rect.top };
  POINT bot_right = { rect.right, rect.bottom };

  ClientToScreen(window->hwnd, &top_left);
  ClientToScreen(window->hwnd, &bot_right);

  RECT clip =
  {
    top_left.x,
    top_left.y,
    bot_right.x,
    bot_right.y
  };

  ClipCursor(&clip);

  window->input.is_cursor_locked = true;
}

fz_internal void
cursor_unlock()
{
  Window *window = &GlobalWindow;

  if (!window->input.is_cursor_locked)
  {
    return;
  }

  ClipCursor(NULL);

  window->input.is_cursor_locked = false;
}

fz_internal b32
cursor_is_locked()
{
  return GlobalWindow.input.is_cursor_locked;
}

fz_internal void
cursor_hide(b32 hide)
{
  // Win32 quirk. It has an internal counter required to show the cursor.
  // The while loops just make sure it exhausts the counter and applies immediately.
  while (ShowCursor(hide ? false : true) >= 0 &&  hide);
  while (ShowCursor(hide ? false : true) < 0  && !hide);
}

fz_internal void
cursor_set_position(Window *window, s32 x, s32 y)
{
  POINT point;
  point.x = x;
  point.y = y;

  ClientToScreen(window->hwnd, &point);

  SetCursorPos(point.x, point.y);
}

fz_internal void
cursor_set_icon(Cursor_Type type)
{
  HCURSOR hCursor = NULL;

  switch (type)
  {
    case Cursor_Arrow:
    {
      hCursor = LoadCursor(NULL, IDC_ARROW);
    }
    break;
    case Cursor_Hand:
    {
      hCursor = LoadCursor(NULL, IDC_HAND);
    }
    break;
    case Cursor_Crosshair:
    {
      hCursor = LoadCursor(NULL, IDC_CROSS);
    }
    break;
    case Cursor_Ibeam:
    {
      hCursor = LoadCursor(NULL, IDC_IBEAM);
    }
    break;
    case Cursor_Wait:
    { 
      hCursor = LoadCursor(NULL, IDC_WAIT);
    }
    break;
    case Cursor_Size_All:
    {
      hCursor = LoadCursor(NULL, IDC_SIZEALL);
    }
    break;
    default:
    {
      hCursor = LoadCursor(NULL, IDC_ARROW);
    }
    break;
  }

  if (hCursor)
  {
    SetCursor(hCursor);
  }
}
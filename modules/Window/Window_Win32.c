
#define WINDOW_CLASS_NAME L"DefaultWindowClass"

#define WINDOWED_STYLE         WS_OVERLAPPEDWINDOW;
#define FULLSCREEN_STYLE       WS_VISIBLE | WS_POPUP;
#define SECONDARY_WINDOW_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;

function Window*
window_create(Window* parent, String title, u32 width, u32 height, u32 x, u32 y)
{
  if (!WindowClassInited)
  {
    _init_window_class();
  }

  DWORD style = WINDOWED_STYLE;
  if (parent) style = SECONDARY_WINDOW_STYLE;

  if (x == -1 || y == -1)
  {
    RECT work_area = {0};
    b32 work_area_success = SystemParametersInfoW(SPI_GETWORKAREA, 0, &work_area, 0);
    if (work_area_success)
    {
      if (x == -1) x = work_area.left;
      if (y == -1) y = work_area.top;
    }
  }

  RECT rect = {0};
  rect.right  = (LONG)width;
  rect.bottom = (LONG)height;

  AdjustWindowRect(&rect, style, false);

  u32 client_width  = (u32)(rect.right  - rect.left);
  u32 client_height = (u32)(rect.bottom - rect.top);
  
  if (title.count >= 126)
  {
    // @TODO(fz): Error, title too long
    return NULL;
  }

  wchar_t title_w[126];
  {
    char* src = (char*)title.cstring;
    s32 required = MultiByteToWideChar(CP_UTF8, 0, src, (s32)title.count, NULL, 0);
    if (required == 0 || required > 256)
    {
      // @TODO(fz): Error
      return NULL;
    }
    MultiByteToWideChar(CP_UTF8, 0, src, (s32)title.count, title_w, required);
    title_w[required] = 0;
  }

  HWND hwnd = CreateWindowExW(0, WINDOW_CLASS_NAME, title_w, style, x, y, client_width, client_height, (parent) ? parent->hwnd : NULL, NULL, (HINSTANCE)0, NULL);
  if (hwnd == NULL)
  {
    // @TODO(fz): Error CreateWindowExW return 0
    return NULL;
  }

  HANDLE heap = GetProcessHeap();
  Window* result = HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(Window)); // @Leak: @TODO(Fz): We may just want to pass an arena to this function
  if (!result)
  {
    // @TODO(fz): Error allocating 
    return NULL;
  }

  result->next   = parent;
  result->title  = title;
  result->width  = width;
  result->height = height;
  result->x      = x;
  result->y      = y;
  result->hwnd   = hwnd;

  UpdateWindow(hwnd);
  ShowWindow(hwnd, SW_SHOW);

  return result;
}

function LRESULT CALLBACK
_window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
  switch (message)
  {
    case WM_CLOSE:
    {
      DestroyWindow(hwnd);
      return false;
    }

    case WM_DESTROY: 
    {
      ReleaseDC(hwnd, GetDC(hwnd));
      PostQuitMessage(0);
      return false;
    }
  }

  return DefWindowProcW(hwnd, message, wparam, lparam);
}

function void
_init_window_class()
{
  if(WindowClassInited) return;
  assert_no_reentry();

  HMODULE hInstance = GetModuleHandle(NULL);

  WNDCLASSEXW wc = {0};
  wc.cbSize        = sizeof(WNDCLASSEXW);
  wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc   = _window_proc;
  wc.hInstance     = hInstance;
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = WINDOW_CLASS_NAME;

  RegisterClassExW(&wc);
  WindowClassInited = 1;
}

function void
update_window_events()
{
  for (;;)
  {
    MSG message;
    BOOL result = PeekMessageW(&message, NULL, 0, 0, PM_REMOVE);
    if (!result) break;
    if (message.message == WM_QUIT) return 0;

    switch (message.message)
    {
      case WM_KEYDOWN:
      {
        
      }
      break;
    }

    TranslateMessage(&message);
    DispatchMessageW(&message);
  }

  return true;
}
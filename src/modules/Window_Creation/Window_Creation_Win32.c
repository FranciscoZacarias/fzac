
#define WINDOW_CLASS_NAME L"DefaultWindowClass"

#define WINDOWED_STYLE         WS_OVERLAPPEDWINDOW;
#define FULLSCREEN_STYLE       WS_VISIBLE | WS_POPUP;
#define SECONDARY_WINDOW_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;

struct Window
{
  Window *next;

  String title;

  u32 width;
  u32 height;

  u32 x;
  u32 y;

  HWND hwnd;
};

global Window* WindowListHead = NULL;
global b32 WindowClassInited = 0;

function void _init_window_class(); /* Only needs to be called one time per process. */

void PrintLastError(const wchar_t* msg)
{
    DWORD error = GetLastError();
    if (error == 0)
    {
        wprintf(L"%s: No error.\n", msg);
        return;
    }

    LPWSTR buffer = NULL;

    DWORD size = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&buffer,
        0,
        NULL
    );

    if (size == 0)
    {
        wprintf(L"%s: Unknown error %lu\n", msg, error);
        return;
    }

    wprintf(L"%s failed with error %lu: %s\n", msg, error, buffer);

    LocalFree(buffer);
}

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
  
  if (title.size >= 126)
  {
    // @TODO(fz): Error, title too long
    return NULL;
  }

  wchar_t title_w[126];
  {
    char* src = (char*)title.cstring;
    s32 required = MultiByteToWideChar(CP_UTF8, 0, src, (s32)title.size, NULL, 0);
    if (required == 0 || required > 256)
    {
      // @TODO(fz): Error
      return NULL;
    }
    MultiByteToWideChar(CP_UTF8, 0, src, (s32)title.size, title_w, required);
    title_w[required] = 0;
  }


  HWND hwnd = CreateWindowExW(
    0,
    WINDOW_CLASS_NAME,
    title_w,
    style,
    x,
    y,
    client_width,
    client_height,
    (parent) ? parent->hwnd : NULL,
    NULL,
    (HINSTANCE)0,
    NULL
  );

  if (hwnd == NULL)
  {
    PrintLastError(L"CreateWindowExW");
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
    case WM_QUIT:
    {
    }
    break;
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


#define WINDOWED_STYLE         WS_OVERLAPPEDWINDOW;
#define FULLSCREEN_STYLE       WS_VISIBLE | WS_POPUP;
#define SECONDARY_WINDOW_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;

struct Window
{
  Window *next;

  String title;
  u32 width;
  u32 height;
  b32 should_close;

  HWND hwnd;
};

global wchar_t const* WindowClassName = L"Default Window Class";
global Window*        WindowListHead = NULL;
global b32            WindowClassInited = 0;
global WNDCLASSEXW    WndClass;

function void init_window_class(); /* Only needs to be one time per process. */

function Window*
window_create(Window* parent, String title, u32 width, u32 height, u32 x, u32 y)
{
  Window* result;
  memory_zero_struct(result);

  DWORD style = WINDOWED_STYLE;
  if (parent != NULL) style = SECONDARY_WINDOW_STYLE;

  if (x == -1)
  {
    Rect work_area;
    b32 work_area_success = SystemParametersInfoW(SPI_GETWORKAREA, 0, &work_area, 0);
    if (work_area_success)
    {
      x = work_area.left;
      y = work_area.top;
    }
  }

  LPRECT rect;
  rect->right  = (LONG) width;
  rect->bottom = (LONG) height;

  AdjustWindowRect(rect, style, false);

  s32 client_width  = rect->right  - rect->left;
  s32 client_height = rect->bottom - rect->top;

  result->hwnd = CreateWindowExW(
  );
  
  if (result.hwnd == NULL)
  {
    // TODO(Fz): Handle failed window creation
    return NULL;
  }

  UpdateWindow(result->hwnd);
  ShowWindow(result->hwnd, SW_SHOW);

  return hwnd;
}

function void
init_window_class()
{
  if(WindowClassInited) return;

  HMODULE hInstance = GetModuleHandle(NULL);

  memory_zero_struct(WndClass);
  WndClass.cbSize        = sizeof(WNDCLASSEXW);
  WndClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  WndClass.lpfnWndProc   = window_proc;
  WndClass.hInstance     = hInstance;
  WndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
  WndClass.lpszClassName = WindowClassName;

  RegisterClassExW(&WndClass);
  WindowClassInited = 1;
}
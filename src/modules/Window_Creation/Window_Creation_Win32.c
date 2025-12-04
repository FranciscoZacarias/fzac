
#define WINDOW_CLASS_NAME L"DefaultWindowClass";

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

global Window *WindowListHead = NULL;
global b32 WindowClassInited = 0;

function void init_window_class(); /* Only needs to be one time per process. */

function Window*
window_create(String title, u32 width, u32 height, u32 x, u32 y, Window* parent, )
{
  Window* result;
  memory_zero_struct(window);

  DWORD style = WINDOWED_STYLE;

}

function void
init_window_class()
{
  if(WindowClassInited) return;

  HMODULE hInstance = GetModuleHandle(NULL);

  WNDCLASSEXW wc = {0};
  wc.cbSize        = sizeof(WNDCLASSEXW);
  wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc   = window_proc;
  wc.hInstance     = hInstance;
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = WINDOW_CLASS_NAME;

  RegisterClassExW(&wc);
  WindowClassInited = 1;
}
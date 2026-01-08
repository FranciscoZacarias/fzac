
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
  Window_Context* ctx = &WindowContext;
  Event_Array* events = &ctx->events_this_frame;

  switch (message)
  {
    case WM_CLOSE:
    {
      DestroyWindow(hwnd);
      return 0;
    }

    case WM_DESTROY:
    {
      // Only post quit when last window closes (simplified here)
      PostQuitMessage(0);
      return 0;
    }

    case WM_KEYDOWN:
    {
      Event* e = _event_push(events);
      e->kind = Event_Keyboard;

      e->modifiers.shift_down = (GetKeyState(VK_SHIFT)   & 0x8000) != 0;
      e->modifiers.ctrl_down  = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
      e->modifiers.alt_down   = (GetKeyState(VK_MENU)    & 0x8000) != 0;

      return 0;
    }

    case WM_CHAR:
    {
      Event* e = _event_push(events);
      e->kind = Event_Text_Input;
      // store UTF-32 char from wparam
      return 0;
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

  WindowContext.frame_arena = arena_alloc();
  WindowContext.events_this_frame.count    = 0;
  WindowContext.events_this_frame.capacity = 4096;
  WindowContext.events_this_frame.data     = push_array(WindowContext.frame_arena, Event, WindowContext.events_this_frame.capacity);
}

function void
update_window_events()
{
  Window_Context* ctx = &WindowContext;

  arena_clear(ctx->frame_arena);
  ctx->events_this_frame.count = 0;

  for (;;)
  {
    MSG msg;
    if (!PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) break;

    if (msg.message == WM_QUIT)
    {
      Event* event = _event_push(&ctx->events_this_frame);
      event->kind = Event_Quit;
    }

    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }
}

function Event*
_event_push(Event_Array* array)
{
  assert(array->count < array->capacity);
  return &array->data[array->count++];
}

function u32
get_total_events_this_frame()
{
  return WindowContext.events_this_frame.count;
}

function Event*
get_event_this_frame(u32 index)
{
  assert(index < WindowContext.events_this_frame.count);
  return &(WindowContext.events_this_frame.data[index]);
}
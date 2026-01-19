
#define WINDOW_CLASS_NAME L"DefaultWindowClass"

#define WINDOWED_STYLE         WS_OVERLAPPEDWINDOW;
#define FULLSCREEN_STYLE       WS_VISIBLE | WS_POPUP;
#define SECONDARY_WINDOW_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;

global Keyboard_Key _win32_vk_to_key[Keyboard_Key_Count];
global u32 _win32_key_table[Keyboard_Key_Count] =
{
  0x08, // Keyboard_Key_BACKSPACE
  0x0D, // Keyboard_Key_ENTER
  0x09, // Keyboard_Key_TAB
  0x10, // Keyboard_Key_SHIFT
  0x11, // Keyboard_Key_CONTROL
  0x13, // Keyboard_Key_PAUSE
  0x14, // Keyboard_Key_CAPS_LOCK
  0x1B, // Keyboard_Key_ESCAPE
  0x1C, // Keyboard_Key_CONVERT
  0x1D, // Keyboard_Key_NONCONVERT
  0x1E, // Keyboard_Key_ACCEPT
  0x1F, // Keyboard_Key_MODECHANGE
  0x20, // Keyboard_Key_SPACE
  0x21, // Keyboard_Key_PRIOR
  0x22, // Keyboard_Key_NEXT
  0x23, // Keyboard_Key_END
  0x24, // Keyboard_Key_HOME
  0x25, // Keyboard_Key_ARROW_LEFT
  0x26, // Keyboard_Key_ARROW_UP
  0x27, // Keyboard_Key_ARROW_RIGHT
  0x28, // Keyboard_Key_ARROW_DOWN
  0x29, // Keyboard_Key_SELECT
  0x2A, // Keyboard_Key_PRINT
  0x2B, // Keyboard_Key_EXECUTE
  0x2C, // Keyboard_Key_SNAPSHOT
  0x2D, // Keyboard_Key_INSERT
  0x2E, // Keyboard_Key_DELETE
  0x2F, // Keyboard_Key_HELP
  0x30, // Keyboard_Key_0
  0x31, // Keyboard_Key_1
  0x32, // Keyboard_Key_2
  0x33, // Keyboard_Key_3
  0x34, // Keyboard_Key_4
  0x35, // Keyboard_Key_5
  0x36, // Keyboard_Key_6
  0x37, // Keyboard_Key_7
  0x38, // Keyboard_Key_8
  0x39, // Keyboard_Key_9
  0x41, // Keyboard_Key_A
  0x42, // Keyboard_Key_B
  0x43, // Keyboard_Key_C
  0x44, // Keyboard_Key_D
  0x45, // Keyboard_Key_E
  0x46, // Keyboard_Key_F
  0x47, // Keyboard_Key_G
  0x48, // Keyboard_Key_H
  0x49, // Keyboard_Key_I
  0x4A, // Keyboard_Key_J
  0x4B, // Keyboard_Key_K
  0x4C, // Keyboard_Key_L
  0x4D, // Keyboard_Key_M
  0x4E, // Keyboard_Key_N
  0x4F, // Keyboard_Key_O
  0x50, // Keyboard_Key_P
  0x51, // Keyboard_Key_Q
  0x52, // Keyboard_Key_R
  0x53, // Keyboard_Key_S
  0x54, // Keyboard_Key_T
  0x55, // Keyboard_Key_U
  0x56, // Keyboard_Key_V
  0x57, // Keyboard_Key_W
  0x58, // Keyboard_Key_X
  0x59, // Keyboard_Key_Y
  0x5A, // Keyboard_Key_Z
  0x5B, // Keyboard_Key_LEFT_WIN
  0x5C, // Keyboard_Key_RIGHT_WIN
  0x5D, // Keyboard_Key_APPS
  0x5F, // Keyboard_Key_SLEEP
  0x60, // Keyboard_Key_NUMPAD0
  0x61, // Keyboard_Key_NUMPAD1
  0x62, // Keyboard_Key_NUMPAD2
  0x63, // Keyboard_Key_NUMPAD3
  0x64, // Keyboard_Key_NUMPAD4
  0x65, // Keyboard_Key_NUMPAD5
  0x66, // Keyboard_Key_NUMPAD6
  0x67, // Keyboard_Key_NUMPAD7
  0x68, // Keyboard_Key_NUMPAD8
  0x69, // Keyboard_Key_NUMPAD9
  0x6A, // Keyboard_Key_NUMPAD_MULTIPLY
  0x6B, // Keyboard_Key_NUMPAD_ADD
  0x6C, // Keyboard_Key_NUMPAD_SEPARATOR
  0x6D, // Keyboard_Key_NUMPAD_SUBTRACT
  0x6E, // Keyboard_Key_NUMPAD_DECIMAL
  0x6F, // Keyboard_Key_NUMPAD_DIVIDE
  0x70, // Keyboard_Key_F1
  0x71, // Keyboard_Key_F2
  0x72, // Keyboard_Key_F3
  0x73, // Keyboard_Key_F4
  0x74, // Keyboard_Key_F5
  0x75, // Keyboard_Key_F6
  0x76, // Keyboard_Key_F7
  0x77, // Keyboard_Key_F8
  0x78, // Keyboard_Key_F9
  0x79, // Keyboard_Key_F10
  0x7A, // Keyboard_Key_F11
  0x7B, // Keyboard_Key_F12
  0x7C, // Keyboard_Key_F13
  0x7D, // Keyboard_Key_F14
  0x7E, // Keyboard_Key_F15
  0x7F, // Keyboard_Key_F16
  0x80, // Keyboard_Key_F17
  0x81, // Keyboard_Key_F18
  0x82, // Keyboard_Key_F19
  0x83, // Keyboard_Key_F20
  0x84, // Keyboard_Key_F21
  0x85, // Keyboard_Key_F22
  0x86, // Keyboard_Key_F23
  0x87, // Keyboard_Key_F24
  0x90, // Keyboard_Key_NUMLOCK
  0x91, // Keyboard_Key_SCROLL
  0x92, // Keyboard_Key_NUMPAD_EQUAL
  0xA0, // Keyboard_Key_LEFT_SHIFT
  0xA1, // Keyboard_Key_RIGHT_SHIFT
  0xA2, // Keyboard_Key_LEFT_CONTROL
  0xA3, // Keyboard_Key_RIGHT_CONTROL
  0xA4, // Keyboard_Key_LEFT_MENU
  0xA5, // Keyboard_Key_RIGHT_MENU
  0xBA, // Keyboard_Key_SEMICOLON
  0xBB, // Keyboard_Key_PLUS
  0xBC, // Keyboard_Key_COMMA
  0xBD, // Keyboard_Key_MINUS
  0xBE, // Keyboard_Key_PERIOD
  0xBF, // Keyboard_Key_SLASH
  0xC0, // Keyboard_Key_GRAVE
  0xDB, // Keyboard_Key_LEFT_BRACKET
  0xDC, // Keyboard_Key_BACKSLASH
  0xDD, // Keyboard_Key_RIGHT_BRACKET
  0xDE, // Keyboard_Key_QUOTE
};

function LRESULT CALLBACK _window_procedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

// @TODO(Fz): Add resize callback

function Window* 
window_create(Window* parent, String title, u32 width, u32 height, u32 x, u32 y)
{
  if (!WindowClassInited) _init_window_class();

  DWORD style = WINDOWED_STYLE;
  if (parent) style = SECONDARY_WINDOW_STYLE;

  if (x == -1 || y == -1)
  {
    RECT work_area = {0};
    if (SystemParametersInfoW(SPI_GETWORKAREA, 0, &work_area, 0))
    {
      if (x == -1) x = work_area.left;
      if (y == -1) y = work_area.top;
    }
  }

  RECT rect = {0};
  rect.right  = (LONG)width;
  rect.bottom = (LONG)height;
  AdjustWindowRect(&rect, style, false);
  u32 client_width  = (u32)(rect.right - rect.left);
  u32 client_height = (u32)(rect.bottom - rect.top);

  if (title.count >= 126) return NULL;

  wchar_t title_w[126];
  s32 required = MultiByteToWideChar(CP_UTF8, 0, (char*)title.cstring, (s32)title.count, NULL, 0);
  if (required == 0 || required > 256) return NULL;
  MultiByteToWideChar(CP_UTF8, 0, (char*)title.cstring, (s32)title.count, title_w, required);
  title_w[required] = 0;

  HWND hwnd = CreateWindowExW(
    0, WINDOW_CLASS_NAME, title_w, style, x, y, client_width, client_height,
    parent ? parent->hwnd : NULL, NULL, (HINSTANCE)0, NULL
  );
  if (!hwnd) return NULL;

  Window* window = push_array(WindowContext.arena, Window, 1);
  memory_zero_struct(window);

  window->next   = NULL;
  window->title  = title;
  window->width  = width;
  window->height = height;
  window->x      = x;
  window->y      = y;
  window->hwnd   = hwnd;

  // Add to WindowContext linked list
  if (!WindowContext.window_list)
  {
    WindowContext.window_list = window;
  }
  else
  {
    Window* it = WindowContext.window_list;
    while (it->next) it = it->next;
    it->next = window;
  }

  // Device context
  window->dc = GetDC(window->hwnd);
  if (!window->dc)
  {
    // TODO(fz): Handle error
    assert(0);
  }

  WindowContext.total_windows += 1;
  UpdateWindow(hwnd);
  ShowWindow(hwnd, SW_SHOW);

  return window;
}

function void
window_swap_buffers(Window* window)
{
  SwapBuffers(window->dc);
}

function void
window_destroy(Window* window)
{
  if (!window)
  {
    return;
  }

  WindowContext.total_windows -= 1;
  DestroyWindow(window->hwnd);
  
  Window* prev = NULL;
  Window* it = WindowContext.window_list;
  while (it)
  {
    if (it == window)
    {
      if (prev)
      {
        prev->next = it->next;
      }
      else
      {
        WindowContext.window_list = it->next;
      }
      break;
    }
    prev = it;
    it = it->next;
  }
}

function LRESULT CALLBACK
_window_procedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
  Window_Context* ctx = &WindowContext;
  Event_Array* events = &ctx->events_this_frame;

  Window* window = WindowContext.window_list;
  while (window && window->hwnd != hwnd)
  {
    window = window->next;
  }

  if (window) switch (message)
  {
    case WM_CLOSE:
    {
      WindowContext.any_window_pending_close = true;
      window->should_close = true;
      return 0;
    }
    break;
    case WM_DESTROY:
    { 
      if (WindowContext.total_windows == 0) 
      {
        PostQuitMessage(0);
      }
      return 0;
    } break;
    
    // Window
    case WM_SETFOCUS:
    {
      window->is_focused = true;
      WindowContext.focused_window = window;
      return 0;
    }
    break;

    case WM_KILLFOCUS:
    {
      window->is_focused = false;
      if (WindowContext.focused_window == window)
      {
        WindowContext.focused_window = NULL;
      }
      return 0;
    }
    break;

    case WM_SIZE:
    {
      RECT rect;
      if (GetClientRect(hwnd, &rect))
      {
        window->width  = (u32)(rect.right - rect.left);
        window->height = (u32)(rect.bottom - rect.top);
      }
      return 0;
    }
    break;

    case WM_MOVE:
    {
      RECT rect;
      if (GetWindowRect(hwnd, &rect))
      {
        window->x = (u32)rect.left;
        window->y = (u32)rect.top;
      }
      return 0;
    }
    break;


    // Mouse
    case WM_LBUTTONDOWN: { _input_process_mouse_button(Mouse_Button_Left,   true);  return 0; } break;
    case WM_LBUTTONUP:   { _input_process_mouse_button(Mouse_Button_Left,   false); return 0; } break;
    case WM_RBUTTONDOWN: { _input_process_mouse_button(Mouse_Button_Right,  true);  return 0; } break;
    case WM_RBUTTONUP:   { _input_process_mouse_button(Mouse_Button_Right,  false); return 0; } break;
    case WM_MBUTTONDOWN: { _input_process_mouse_button(Mouse_Button_Middle, true);  return 0; } break;
    case WM_MBUTTONUP:   { _input_process_mouse_button(Mouse_Button_Middle, false); return 0; } break;

    case WM_MOUSEMOVE:
    {
      s32 x = GET_X_LPARAM(lparam);
      s32 y = GET_Y_LPARAM(lparam);
      WindowContext.input.mouse_current.screen_space.x = (f32)x; 
      WindowContext.input.mouse_current.screen_space.y = (f32)y;
      return 0;
    }
    break;

    case WM_MOUSEWHEEL:
    {
      short delta = GET_WHEEL_DELTA_WPARAM(wparam);
      WindowContext.input.mouse_current.wheel_delta += delta;
      return 0;
    }
    break;

    // Keyboard 
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
      if (lparam & (1 << 30)) break; // ignore auto-repeat

      u32 vk = (u32)wparam;
      Keyboard_Key key = _key_from_native_key(vk);

      if (key != Keyboard_Key_Count)
      {
        _input_process_keyboard_key(key, true);
      }

      Window_Event* event = _event_push(events);
      event->kind = Event_Keyboard;

      event->modifiers.shift_down = input_is_key_down(Keyboard_Key_SHIFT);
      event->modifiers.ctrl_down  = input_is_key_down(Keyboard_Key_CONTROL);
      event->modifiers.alt_down   = input_is_key_down(Keyboard_Key_LEFT_MENU) || input_is_key_down(Keyboard_Key_RIGHT_MENU);

      event->payload.key = key;

      return 0;
    }
    break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
      u32 vk = (u32)wparam;
      Keyboard_Key key = _key_from_native_key(vk);

      if (key != Keyboard_Key_Count)
      {
        _input_process_keyboard_key(key, false);
      }

      Window_Event* event = _event_push(events);
      event->kind = Event_Keyboard;
      event->payload.key = key;

      return 0;
    }
    break;

    case WM_CHAR:
    {
      Window_Event* event = _event_push(events);
      event->kind = Event_Text_Input;
      event->payload.codepoint = (u32)wparam; // UTF-32

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

  _input_init();
  _init_win32_key_tables();

  HMODULE hInstance = GetModuleHandle(NULL);

  WNDCLASSEXW wc = {0};
  wc.cbSize        = sizeof(WNDCLASSEXW);
  wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc   = _window_procedure;
  wc.hInstance     = hInstance;
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = WINDOW_CLASS_NAME;

  RegisterClassExW(&wc);
  WindowClassInited = 1;

  WindowContext.arena = arena_alloc();
  WindowContext.frame_arena = arena_alloc();
  WindowContext.events_this_frame.count    = 0;
  WindowContext.events_this_frame.capacity = 4096;
  WindowContext.events_this_frame.data     = push_array(WindowContext.frame_arena, Window_Event, WindowContext.events_this_frame.capacity);
}

function void
update_window_events()
{
  Window_Context* ctx = &WindowContext;

  ctx->events_this_frame.count = 0;
  arena_clear(ctx->frame_arena);

  _input_update(&ctx->input);

  for (;;)
  {
    MSG msg;
    if (!PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) break;

    if (msg.message == WM_QUIT)
    {
      Window_Event* event = _event_push(&ctx->events_this_frame);
      event->kind = Event_Quit;
      continue;
    }

    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  if (WindowContext.any_window_pending_close)
  {
    Window* it = WindowContext.window_list;
    Window* next = NULL;
    while (it)
    {
      next = it->next;
      if (it->should_close) window_destroy(it);
      it = next;
    }

    WindowContext.any_window_pending_close = false;
  }
}

function Window_Event*
_event_push(Event_Array* array)
{
  assert(array->count < array->capacity);
  Window_Event* result = &array->data[array->count++];
  memory_zero_struct(result);
  return result;
}

function u32
get_total_events_this_frame()
{
  return WindowContext.events_this_frame.count;
}

function Window_Event*
get_event_this_frame(u32 index)
{
  assert(index < WindowContext.events_this_frame.count);
  return &(WindowContext.events_this_frame.data[index]);
}

function void
_init_win32_key_tables()
{
  for (u32 i = 0; i < 256; ++i)
  {
    _win32_vk_to_key[i] = Keyboard_Key_Count;
  }

  for (u32 key = 0; key < Keyboard_Key_Count; ++key)
  {
    u32 vk = _win32_key_table[key];
    if (vk < 256)
    {
      _win32_vk_to_key[vk] = (Keyboard_Key)key;
    }
  }
}

function u32
_native_key_from_key(Keyboard_Key key)
{
  assert(key < Keyboard_Key_Count);
  return _win32_key_table[key];
}

function Keyboard_Key
_key_from_native_key(u32 native_key)
{
  if (native_key < 256)
  {
    return _win32_vk_to_key[native_key];
  }
  return Keyboard_Key_Count;
}

function void
_input_init()
{
  assert_no_reentry();
  memory_zero_struct(&WindowContext.input);

  WindowContext.input.ignore_next_mouse_move = false;
  WindowContext.input.is_cursor_locked       = false;
}

function void
_input_update()
{
  // Compute new deltas
  WindowContext.input.mouse_current.delta.x = WindowContext.input.mouse_current.screen_space.x - WindowContext.input.mouse_previous.screen_space.x; 
  WindowContext.input.mouse_current.delta.y = WindowContext.input.mouse_current.screen_space.y - WindowContext.input.mouse_previous.screen_space.y;
  WindowContext.input.mouse_current.wheel_delta = 0;

  memory_copy(&(WindowContext.input.keyboard_previous), &(WindowContext.input.keyboard_current), sizeof(Keyboard_State));
  memory_copy(&(WindowContext.input.mouse_previous),    &(WindowContext.input.mouse_current),    sizeof(Mouse_State));
}

function b32
input_is_key_up(Keyboard_Key key)
{
  b32 result = WindowContext.input.keyboard_current.keys[key] == false;
  return result;
}

function b32
input_is_key_down(Keyboard_Key key)
{
  b32 result = WindowContext.input.keyboard_current.keys[key] == true;
  return result;
}

function b32
input_was_key_up(Keyboard_Key key)
{
  b32 result = WindowContext.input.keyboard_previous.keys[key] == false;
  return result;
}

function b32
input_was_key_down(Keyboard_Key key)
{
  b32 result = WindowContext.input.keyboard_previous.keys[key] == true;
  return result;
}

function b32
input_is_key_clicked(Keyboard_Key key)
{
  return input_is_key_down(key) && input_was_key_up(key);
}

function void
_input_process_keyboard_key(Keyboard_Key key, b8 is_pressed)
{
  if (WindowContext.input.keyboard_current.keys[key] != is_pressed)
  {
    WindowContext.input.keyboard_current.keys[key] = is_pressed;
  }
}

function b32
input_is_button_up(Mouse_Button button)
{
  b32 result = WindowContext.input.mouse_current.buttons[button] == false;
  return result;
}

function b32
input_is_button_down(Mouse_Button button)
{
  b32 result = WindowContext.input.mouse_current.buttons[button] == true;
  return result;
}

function b32
input_was_button_up(Mouse_Button button)
{
  b32 result = WindowContext.input.mouse_previous.buttons[button] == false;
  return result;
}

function b32
input_was_button_down(Mouse_Button button)
{
  b32 result = WindowContext.input.mouse_previous.buttons[button] == true;
  return result;
}

function b32
input_is_button_clicked(Mouse_Button button)
{
  b32 result = input_is_button_down(button) && input_was_button_up(button);
  return result;
}

function void
_input_process_mouse_button(Mouse_Button button, b32 is_pressed)
{
  if (WindowContext.input.mouse_current.buttons[button] != (b8)is_pressed)
  {
    WindowContext.input.mouse_current.buttons[button] = (b8)is_pressed;
  }
}

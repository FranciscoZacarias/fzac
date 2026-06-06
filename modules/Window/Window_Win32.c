
#define WINDOW_CLASS_NAME "DefaultWindowClass"
#define WINDOW_CLASS_NAMEW L"DefaultWindowClass"

#define WINDOWED_STYLE         WS_OVERLAPPEDWINDOW
#define FULLSCREEN_STYLE       WS_VISIBLE | WS_POPUP
#define SECONDARY_WINDOW_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME

global Keyboard_Key _win32_vk_to_key[256];

function LRESULT CALLBACK _window_procedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

// @TODO(Fz): Add resize callback

function Window*
window_create(String title, u32 width, u32 height, u32 x, u32 y)
{
  if (!WindowClassInited) _init_window_class();

  Window *window = &GlobalWindow;
  memory_zero_struct(window);

  window->title  = title;
  window->width  = width;
  window->height = height;
  window->x      = x;
  window->y      = y;

  DWORD style = WINDOWED_STYLE;

  RECT rect = {0};
  rect.right  = width;
  rect.bottom = height;
  AdjustWindowRect(&rect, style, false);

  HWND hwnd    = CreateWindowExA(0, WINDOW_CLASS_NAME, (char*)title.cstring, style, x, y, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, GetModuleHandle(NULL), NULL);
  window->hwnd = hwnd;
  window->dc   = GetDC(hwnd);

  window->frame_arena                = arena_alloc();
  window->events_this_frame.capacity = 4096;
  window->events_this_frame.data     =  push_array(window->frame_arena, Window_Event, window->events_this_frame.capacity);

  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);

  return window;
}

function void
window_swap_buffers()
{
  SwapBuffers(GlobalWindow.dc);
}

function void
window_destroy()
{
  if (!GlobalWindow.hwnd) return;

  DestroyWindow(GlobalWindow.hwnd);
  GlobalWindow.should_close = true;
}

function LRESULT CALLBACK
_window_procedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
  Event_Array* events = &GlobalWindow.events_this_frame;
  Window *window = &GlobalWindow;

  switch (message)
  {
    case WM_CLOSE:
    {
      window->should_close = true;
      return 0;
    }
    break;
    case WM_DESTROY:
    { 
      PostQuitMessage(0);
      return 0;
    } break;
    
    // Window
    case WM_SETFOCUS:
    {
      Window_Event* event = _event_push(events);
      event->kind = Event_Window;

      window->is_focused = true;
      return 0;
    }
    break;

    case WM_KILLFOCUS:
    {
      Window_Event* event = _event_push(events);
      event->kind = Event_Window;

      window->is_focused = false;
      // Reset input
      {
        memory_zero_struct(&GlobalWindow.input);
        GlobalWindow.input.mouse_previous.screen_space.x = -1;
        GlobalWindow.input.mouse_previous.screen_space.y = -1;
      }
      return 0;
    }
    break;

    case WM_SIZE:
    {
      RECT rect;
      if (GetClientRect(hwnd, &rect))
      {
        
        Window_Event* event = _event_push(events);
        event->kind = Event_Window;

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
      GlobalWindow.input.mouse_current.screen_space.x = x; 
      GlobalWindow.input.mouse_current.screen_space.y = y;
      return 0;
    }
    break;

    case WM_MOUSEWHEEL:
    {
      short delta = GET_WHEEL_DELTA_WPARAM(wparam);
      GlobalWindow.input.mouse_current.wheel_delta += delta;
      return 0;
    }
    break;

    // Keyboard 
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
      if (lparam & (1 << 30))
      {
        return 0;
      }

      u32 vk = (u32)wparam;
      Keyboard_Key key = _key_from_native_key(vk);

      if (key != Keyboard_Key_Count)
      {
        _input_process_keyboard_key(key, true);
      }

      Window_Event* event = _event_push(events);
      event->kind = Event_Keyboard;

      event->modifiers.shift_pressed = is_key_down(Keyboard_Key_SHIFT);
      event->modifiers.ctrl_pressed  = is_key_down(Keyboard_Key_CONTROL);
      event->modifiers.alt_pressed   = is_key_down(Keyboard_Key_LEFT_MENU) || is_key_down(Keyboard_Key_RIGHT_MENU);

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
      event->payload.codepoint = (u32)wparam;

      return 0;
    }
  }

  return DefWindowProcW(hwnd, message, wparam, lparam);
}

function void
_init_window_class()
{
  if(WindowClassInited) return;

  _input_init();
  _init_win32_key_table();

  HMODULE hInstance = GetModuleHandle(NULL);

  WNDCLASSEXW wc = {0};
  wc.cbSize        = sizeof(WNDCLASSEXW);
  wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc   = _window_procedure;
  wc.hInstance     = hInstance;
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = WINDOW_CLASS_NAMEW;

  RegisterClassExW(&wc);
  WindowClassInited = 1;

  GlobalWindow.frame_arena = arena_alloc();
  GlobalWindow.events_this_frame.count    = 0;
  GlobalWindow.events_this_frame.capacity = 4096;
  GlobalWindow.events_this_frame.data     = push_array(GlobalWindow.frame_arena, Window_Event, GlobalWindow.events_this_frame.capacity);
}

function void
window_update_events()
{
  Window *window = &GlobalWindow;

  window->events_this_frame.count = 0;
  arena_clear(window->frame_arena);

  _input_update();

  for (;;)
  {
    MSG msg;
    if (!PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
      break;

    if (msg.message == WM_QUIT)
    {
      Window_Event* event = _event_push(&window->events_this_frame);
      event->kind = Event_Quit;
      continue;
    }

    TranslateMessage(&msg);
    DispatchMessageW(&msg);
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
  return GlobalWindow.events_this_frame.count;
}

function Window_Event*
get_event_this_frame(u32 index)
{
  return &GlobalWindow.events_this_frame.data[index];
}

function void
_init_win32_key_table()
{
  for (u32 i = 0; i < 256; ++i)
  {
    _win32_vk_to_key[i] = Keyboard_Key_Count;
  }

  for (u32 i = 'A'; i <= 'Z'; ++i)
  {
    _win32_vk_to_key[i] = Keyboard_Key_A + (i - 'A');
  }
  for (u32 i = '0'; i <= '9'; ++i)
  {
    _win32_vk_to_key[i] = Keyboard_Key_0 + (i - '0');
  }

  _win32_vk_to_key[VK_F1]  = Keyboard_Key_F1;
  _win32_vk_to_key[VK_F2]  = Keyboard_Key_F2;
  _win32_vk_to_key[VK_F3]  = Keyboard_Key_F3;
  _win32_vk_to_key[VK_F4]  = Keyboard_Key_F4;
  _win32_vk_to_key[VK_F5]  = Keyboard_Key_F5;
  _win32_vk_to_key[VK_F6]  = Keyboard_Key_F6;
  _win32_vk_to_key[VK_F7]  = Keyboard_Key_F7;
  _win32_vk_to_key[VK_F8]  = Keyboard_Key_F8;
  _win32_vk_to_key[VK_F9]  = Keyboard_Key_F9;
  _win32_vk_to_key[VK_F10] = Keyboard_Key_F10;
  _win32_vk_to_key[VK_F11] = Keyboard_Key_F11;
  _win32_vk_to_key[VK_F12] = Keyboard_Key_F12;

#ifdef VK_F13
  _win32_vk_to_key[VK_F13] = Keyboard_Key_F13;
  _win32_vk_to_key[VK_F14] = Keyboard_Key_F14;
  _win32_vk_to_key[VK_F15] = Keyboard_Key_F15;
  _win32_vk_to_key[VK_F16] = Keyboard_Key_F16;
  _win32_vk_to_key[VK_F17] = Keyboard_Key_F17;
  _win32_vk_to_key[VK_F18] = Keyboard_Key_F18;
  _win32_vk_to_key[VK_F19] = Keyboard_Key_F19;
  _win32_vk_to_key[VK_F20] = Keyboard_Key_F20;
  _win32_vk_to_key[VK_F21] = Keyboard_Key_F21;
  _win32_vk_to_key[VK_F22] = Keyboard_Key_F22;
  _win32_vk_to_key[VK_F23] = Keyboard_Key_F23;
  _win32_vk_to_key[VK_F24] = Keyboard_Key_F24;
#endif

  _win32_vk_to_key[VK_INSERT] = Keyboard_Key_INSERT;
  _win32_vk_to_key[VK_DELETE] = Keyboard_Key_DELETE;
  _win32_vk_to_key[VK_HOME]   = Keyboard_Key_HOME;
  _win32_vk_to_key[VK_END]    = Keyboard_Key_END;
  _win32_vk_to_key[VK_PRIOR]  = Keyboard_Key_Count; // @TODO(fz): Keyboard_Key_PAGE_UP
  _win32_vk_to_key[VK_NEXT]   = Keyboard_Key_Count; // @TODO(fz): Keyboard_Key_PAGE_DOWN

  _win32_vk_to_key[VK_LEFT]  = Keyboard_Key_ARROW_LEFT;
  _win32_vk_to_key[VK_RIGHT] = Keyboard_Key_ARROW_RIGHT;
  _win32_vk_to_key[VK_UP]    = Keyboard_Key_ARROW_UP;
  _win32_vk_to_key[VK_DOWN]  = Keyboard_Key_ARROW_DOWN;

  _win32_vk_to_key[VK_ESCAPE] = Keyboard_Key_ESCAPE;
  _win32_vk_to_key[VK_TAB]    = Keyboard_Key_TAB;
  _win32_vk_to_key[VK_RETURN] = Keyboard_Key_ENTER;
  _win32_vk_to_key[VK_SPACE]  = Keyboard_Key_SPACE;
  _win32_vk_to_key[VK_BACK]   = Keyboard_Key_BACKSPACE;

  _win32_vk_to_key[VK_SHIFT]   = Keyboard_Key_SHIFT;
  _win32_vk_to_key[VK_CONTROL] = Keyboard_Key_CONTROL;
  _win32_vk_to_key[VK_MENU]    = Keyboard_Key_MENU; // ALT

  _win32_vk_to_key[VK_CAPITAL] = Keyboard_Key_CAPS_LOCK;
  _win32_vk_to_key[VK_NUMLOCK] = Keyboard_Key_Count; // @TODO(fz): Keyboard_Key_NUM_LOCK;
  _win32_vk_to_key[VK_SCROLL]  = Keyboard_Key_Count; // @TODO(fz): Keyboard_Key_SCROLL_LOCK;

  _win32_vk_to_key[VK_NUMPAD0] = Keyboard_Key_NUMPAD0;
  _win32_vk_to_key[VK_NUMPAD1] = Keyboard_Key_NUMPAD1;
  _win32_vk_to_key[VK_NUMPAD2] = Keyboard_Key_NUMPAD2;
  _win32_vk_to_key[VK_NUMPAD3] = Keyboard_Key_NUMPAD3;
  _win32_vk_to_key[VK_NUMPAD4] = Keyboard_Key_NUMPAD4;
  _win32_vk_to_key[VK_NUMPAD5] = Keyboard_Key_NUMPAD5;
  _win32_vk_to_key[VK_NUMPAD6] = Keyboard_Key_NUMPAD6;
  _win32_vk_to_key[VK_NUMPAD7] = Keyboard_Key_NUMPAD7;
  _win32_vk_to_key[VK_NUMPAD8] = Keyboard_Key_NUMPAD8;
  _win32_vk_to_key[VK_NUMPAD9] = Keyboard_Key_NUMPAD9;

  _win32_vk_to_key[VK_MULTIPLY] = Keyboard_Key_NUMPAD_MULTIPLY;
  _win32_vk_to_key[VK_ADD]      = Keyboard_Key_NUMPAD_ADD;
  _win32_vk_to_key[VK_SUBTRACT] = Keyboard_Key_NUMPAD_SUBTRACT;
  _win32_vk_to_key[VK_DIVIDE]   = Keyboard_Key_NUMPAD_DIVIDE;
  _win32_vk_to_key[VK_DECIMAL]  = Keyboard_Key_NUMPAD_DECIMAL;

  _win32_vk_to_key[VK_OEM_1]      = Keyboard_Key_SEMICOLON;
  _win32_vk_to_key[VK_OEM_PLUS]   = Keyboard_Key_EQUAL;
  _win32_vk_to_key[VK_OEM_COMMA]  = Keyboard_Key_COMMA;
  _win32_vk_to_key[VK_OEM_MINUS]  = Keyboard_Key_MINUS;
  _win32_vk_to_key[VK_OEM_PERIOD] = Keyboard_Key_PERIOD;
  _win32_vk_to_key[VK_OEM_2]      = Keyboard_Key_SLASH;
  _win32_vk_to_key[VK_OEM_3]      = Keyboard_Key_GRAVE;

  _win32_vk_to_key[VK_OEM_4] = Keyboard_Key_LEFT_BRACKET;
  _win32_vk_to_key[VK_OEM_5] = Keyboard_Key_BACKSLASH;
  _win32_vk_to_key[VK_OEM_6] = Keyboard_Key_RIGHT_BRACKET;

}

function Keyboard_Key
_key_from_native_key(u32 native_key)
{
  Keyboard_Key result = Keyboard_Key_Count;
  if (native_key < 256)
  {
    result = _win32_vk_to_key[native_key];
  }
  return result;
}

function void
_input_init()
{
  memory_zero_struct(&GlobalWindow.input);
  
  GlobalWindow.input.mouse_previous.screen_space.x = -1;
  GlobalWindow.input.mouse_previous.screen_space.y = -1;
}

function void
_input_update()
{
  // Compute new deltas
  GlobalWindow.input.mouse_current.delta.x = GlobalWindow.input.mouse_current.screen_space.x - GlobalWindow.input.mouse_previous.screen_space.x; 
  GlobalWindow.input.mouse_current.delta.y = GlobalWindow.input.mouse_current.screen_space.y - GlobalWindow.input.mouse_previous.screen_space.y;
  GlobalWindow.input.mouse_current.wheel_delta = 0;

  memory_copy(&(GlobalWindow.input.keyboard_previous), &(GlobalWindow.input.keyboard_current), sizeof(Keyboard_State));
  memory_copy(&(GlobalWindow.input.mouse_previous),    &(GlobalWindow.input.mouse_current),    sizeof(Mouse_State));
}

function b8
is_key_up(Keyboard_Key key)
{
  b8 result = GlobalWindow.input.keyboard_current.keys[key] == false;
  return result;
}

function b8
is_key_down(Keyboard_Key key)
{
  b8 result = GlobalWindow.input.keyboard_current.keys[key] == true;
  return result;
}

function b8
was_key_up(Keyboard_Key key)
{
  b8 result = GlobalWindow.input.keyboard_previous.keys[key] == false;
  return result;
}

function b8
was_key_down(Keyboard_Key key)
{
  b8 result = GlobalWindow.input.keyboard_previous.keys[key] == true;
  return result;
}

function b8
is_key_clicked(Keyboard_Key key)
{
  return is_key_down(key) && was_key_up(key);
}

function void
_input_process_keyboard_key(Keyboard_Key key, b8 is_pressed)
{
  // if (GlobalWindow.input.keyboard_current.keys[key] != is_pressed)
  // {
  //   GlobalWindow.input.keyboard_current.keys[key] = is_pressed;
  // }
  GlobalWindow.input.keyboard_current.keys[key] = is_pressed;
}

function b8
is_button_up(Mouse_Button button)
{
  b8 result = GlobalWindow.input.mouse_current.buttons[button] == false;
  return result;
}

function b8
is_button_down(Mouse_Button button)
{
  b8 result = GlobalWindow.input.mouse_current.buttons[button] == true;
  return result;
}

function b8
was_button_up(Mouse_Button button)
{
  b8 result = GlobalWindow.input.mouse_previous.buttons[button] == false;
  return result;
}

function b8
was_button_down(Mouse_Button button)
{
  b8 result = GlobalWindow.input.mouse_previous.buttons[button] == true;
  return result;
}

function b8
is_button_clicked(Mouse_Button button)
{
  b8 result = is_button_down(button) && was_button_up(button);
  return result;
}

function void
_input_process_mouse_button(Mouse_Button button, b8 is_pressed)
{
  // if (GlobalWindow.input.mouse_current.buttons[button] != (b8)is_pressed)
  // {
  //   GlobalWindow.input.mouse_current.buttons[button] = (b8)is_pressed;
  // }
  GlobalWindow.input.mouse_current.buttons[button] = (b8)is_pressed;
}

function s32
get_mouse_x()
{
  return GlobalWindow.input.mouse_current.screen_space.x;
}

function s32
get_mouse_y()
{
  return GlobalWindow.input.mouse_current.screen_space.y;
}

function V2s32
get_mouse_position()
{
  return GlobalWindow.input.mouse_current.screen_space;
}

function s32
get_mouse_delta_x()
{
  return GlobalWindow.input.mouse_current.delta.x;
}

function s32
get_mouse_delta_y()
{
  return GlobalWindow.input.mouse_current.delta.y;
}

function V2s32
get_mouse_delta()
{
  return GlobalWindow.input.mouse_current.delta;
}

function s32
get_mouse_wheel_delta()
{
  return GlobalWindow.input.mouse_current.wheel_delta;
}

function u32
get_window_width()
{
  return GlobalWindow.width;
}

function u32
get_window_height()
{
  return GlobalWindow.height;
}

function V2u32
get_window_center(Window *window)
{
  V2u32 result = v2u32(get_window_width() / 2, get_window_height() / 2);
  return result;
}

function V2u32
get_window_dimensions()
{
  return v2u32(GlobalWindow.width, GlobalWindow.height);
}

function u32   
get_window_x()
{
  return GlobalWindow.x;
}

function u32   
get_window_y()
{
  return GlobalWindow.y;
}

function V2u32 
get_window_position()
{
  return v2u32(GlobalWindow.x, GlobalWindow.y);
}

function b32
is_window_focused()
{
  return GlobalWindow.is_focused;
}
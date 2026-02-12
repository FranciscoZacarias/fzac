#ifndef WINDOW_CREATION
#define WINDOW_CREATION

/*
  @Module: Handles window related functionality, such as window creation and input.
*/

// @Section: Window

typedef struct Window Window;

function Window* window_create(Window* parent, String title, u32 width, u32 height, u32 x, u32 y);
function void    window_swap_buffers(Window* window);
function void    window_destroy(Window* window);

// @Section: Input
typedef u32 Keyboard_Key;
enum META_ENUM_LINK(Keyboard_Key)
{
  Keyboard_Key_BACKSPACE,
  Keyboard_Key_ENTER,
  Keyboard_Key_TAB,
  Keyboard_Key_SHIFT,
  Keyboard_Key_CONTROL,
  Keyboard_Key_PAUSE,
  Keyboard_Key_CAPS_LOCK,
  Keyboard_Key_ESCAPE,
  Keyboard_Key_CONVERT,
  Keyboard_Key_NONCONVERT,
  Keyboard_Key_ACCEPT,
  Keyboard_Key_MODECHANGE,
  Keyboard_Key_SPACE,
  Keyboard_Key_PRIOR,
  Keyboard_Key_NEXT,
  Keyboard_Key_END,
  Keyboard_Key_HOME,
  Keyboard_Key_ARROW_LEFT,
  Keyboard_Key_ARROW_UP,
  Keyboard_Key_ARROW_RIGHT,
  Keyboard_Key_ARROW_DOWN,
  Keyboard_Key_SELECT,
  Keyboard_Key_PRINT,
  Keyboard_Key_EXECUTE,
  Keyboard_Key_SNAPSHOT,
  Keyboard_Key_INSERT,
  Keyboard_Key_DELETE,
  Keyboard_Key_HELP,
  Keyboard_Key_0,
  Keyboard_Key_1,
  Keyboard_Key_2,
  Keyboard_Key_3,
  Keyboard_Key_4,
  Keyboard_Key_5,
  Keyboard_Key_6,
  Keyboard_Key_7,
  Keyboard_Key_8,
  Keyboard_Key_9,
  Keyboard_Key_A,
  Keyboard_Key_B,
  Keyboard_Key_C,
  Keyboard_Key_D,
  Keyboard_Key_E,
  Keyboard_Key_F,
  Keyboard_Key_G,
  Keyboard_Key_H,
  Keyboard_Key_I,
  Keyboard_Key_J,
  Keyboard_Key_K,
  Keyboard_Key_L,
  Keyboard_Key_M,
  Keyboard_Key_N,
  Keyboard_Key_O,
  Keyboard_Key_P,
  Keyboard_Key_Q,
  Keyboard_Key_R,
  Keyboard_Key_S,
  Keyboard_Key_T,
  Keyboard_Key_U,
  Keyboard_Key_V,
  Keyboard_Key_W,
  Keyboard_Key_X,
  Keyboard_Key_Y,
  Keyboard_Key_Z,
  Keyboard_Key_LEFT_WIN,
  Keyboard_Key_RIGHT_WIN,
  Keyboard_Key_APPS,
  Keyboard_Key_SLEEP,
  Keyboard_Key_NUMPAD0,
  Keyboard_Key_NUMPAD1,
  Keyboard_Key_NUMPAD2,
  Keyboard_Key_NUMPAD3,
  Keyboard_Key_NUMPAD4,
  Keyboard_Key_NUMPAD5,
  Keyboard_Key_NUMPAD6,
  Keyboard_Key_NUMPAD7,
  Keyboard_Key_NUMPAD8,
  Keyboard_Key_NUMPAD9,
  Keyboard_Key_NUMPAD_MULTIPLY,
  Keyboard_Key_NUMPAD_ADD,
  Keyboard_Key_NUMPAD_SEPARATOR,
  Keyboard_Key_NUMPAD_SUBTRACT,
  Keyboard_Key_NUMPAD_DECIMAL,
  Keyboard_Key_NUMPAD_DIVIDE,
  Keyboard_Key_F1,
  Keyboard_Key_F2,
  Keyboard_Key_F3,
  Keyboard_Key_F4,
  Keyboard_Key_F5,
  Keyboard_Key_F6,
  Keyboard_Key_F7,
  Keyboard_Key_F8,
  Keyboard_Key_F9,
  Keyboard_Key_F10,
  Keyboard_Key_F11,
  Keyboard_Key_F12,
  Keyboard_Key_F13,
  Keyboard_Key_F14,
  Keyboard_Key_F15,
  Keyboard_Key_F16,
  Keyboard_Key_F17,
  Keyboard_Key_F18,
  Keyboard_Key_F19,
  Keyboard_Key_F20,
  Keyboard_Key_F21,
  Keyboard_Key_F22,
  Keyboard_Key_F23,
  Keyboard_Key_F24,
  Keyboard_Key_NUMLOCK,
  Keyboard_Key_SCROLL,
  Keyboard_Key_NUMPAD_EQUAL,
  Keyboard_Key_LEFT_SHIFT,
  Keyboard_Key_RIGHT_SHIFT,
  Keyboard_Key_LEFT_CONTROL,
  Keyboard_Key_RIGHT_CONTROL,
  Keyboard_Key_LEFT_MENU,
  Keyboard_Key_RIGHT_MENU,
  Keyboard_Key_SEMICOLON,
  Keyboard_Key_PLUS,
  Keyboard_Key_COMMA,
  Keyboard_Key_MINUS,
  Keyboard_Key_PERIOD,
  Keyboard_Key_SLASH,
  Keyboard_Key_GRAVE,
  Keyboard_Key_LEFT_BRACKET,
  Keyboard_Key_BACKSLASH,
  Keyboard_Key_RIGHT_BRACKET,
  Keyboard_Key_QUOTE,
  
  Keyboard_Key_Count,
};

function void         _init_win32_key_tables(); /* Initializes the key tables for quick lookup at runtime */
function u32          _native_key_from_key(Keyboard_Key key); /* Converts os key to native key */
function Keyboard_Key _key_from_native_key(u32 native_key); /* Converts native key to os key */


typedef u32 Mouse_Button;
enum META_ENUM_LINK(Mouse_Button)
{
  Mouse_Button_Left,
  Mouse_Button_Right,
  Mouse_Button_Middle,
  
  Mouse_Button_Count,
};

#define KEYBOARD_STATE_SIZE 256
typedef struct Keyboard_State Keyboard_State;
struct Keyboard_State
{
  b8 keys[KEYBOARD_STATE_SIZE];
};

typedef struct Mouse_State Mouse_State;
struct Mouse_State
{
  V2s32 screen_space;
  V2s32 delta;
  s32 wheel_delta; 
  b8 buttons[Mouse_Button_Count];
};

typedef struct Input_State Input_State;
struct Input_State
{
  Keyboard_State keyboard_current;
  Keyboard_State keyboard_previous;
  Mouse_State    mouse_current;
  Mouse_State    mouse_previous;
};

function void _input_init(); /* Initializes input state (clears previous + current states) */
function void _input_update(); /* Updates previous input state with current state (to track deltas and transitions) */

function b32 input_is_key_up(Keyboard_Key key); /* True if the given key is currently up */
function b32 input_is_key_pressed(Keyboard_Key key); /* True if the given key is currently down */
function b32 input_was_key_up(Keyboard_Key key); /* True if the given key was up on the previous frame */
function b32 input_was_key_pressed(Keyboard_Key key); /* True if the given key was down on the previous frame */
function b32 input_is_key_clicked(Keyboard_Key key); /* True if the given key is down this frame but was up last frame */

function b32 input_is_button_up(Mouse_Button button); /* True if the given mouse button is currently up */
function b32 input_is_button_pressed(Mouse_Button button); /* True if the given mouse button is currently down */
function b32 input_was_button_up(Mouse_Button button); /* True if the given mouse button was up on the previous frame */
function b32 input_was_button_pressed(Mouse_Button button); /* True if the given mouse button was down on the previous frame */
function b32 input_is_button_clicked(Mouse_Button button); /* True if the given mouse button is down this frame but was up last frame */

function void _input_process_keyboard_key(Keyboard_Key key, b8 is_pressed); /* Internal: Processes a key press/release and updates keyboard state */
function void _input_process_mouse_move(s32 x, s32 y);
function void _input_process_mouse_button(Mouse_Button button, b32 is_pressed); /* Internal: Processes mouse button press/release and updates mouse state */

// @Section: Events

typedef u32 Event_Kind;
enum META_ENUM_LINK(Event_Kind)
{
  Event_Error = 0,
  Event_Keyboard,
  Event_Text_Input,
  Event_Mouse_Wheel,
  Event_Window,
  Event_Quit,
  Event_Drag_And_Drop,
};

typedef struct Window_Event Window_Event;
struct Window_Event
{
  Event_Kind kind;
  union
  {
    struct
    {
      b32 shift_pressed : 1;
      b32 ctrl_pressed  : 1;
      b32 alt_pressed   : 1;
      b32 cmd_pressed   : 1; // MACOS
    };
    u32 packed;
  } modifiers;

  union
  {
    Keyboard_Key key;
    u32 codepoint; // UTF-32
  } payload;
};

typedef struct Event_Array Event_Array;
struct Event_Array
{
  Window_Event* data;
  u32 count;
  u32 capacity;
};

function Window_Event* _event_push(Event_Array* array);
function u32            get_total_events_this_frame();
function Window_Event*  get_event_this_frame(u32 index);

typedef struct Window_Context Window_Context;
struct Window_Context
{
  Arena* arena;
  Arena* frame_arena;
  Window* window_list;
  u32 total_windows;
  Window* focused_window;
  Event_Array events_this_frame;
  Input_State input;

  b32 any_window_pending_close;
};

thread_local global Window_Context WindowContext;

function void update_window_events(); /* Processes all window events this frame. Returns false if app should close */

struct Window
{
  Window *next;

  String title;

  u32 width;
  u32 height;

  u32 x;
  u32 y;

  b32 is_focused;
  b32 should_close;

#if OS_WINDOWS

  HWND  hwnd;
  HGLRC rc;
  HDC   dc;
};

  global Window* WindowListHead = NULL;
  global b32 WindowClassInited = 0;

  function void _init_window_class(); /* Only needs to be called one time per process. */

  #include "windowsx.h"
  #include "Window/Window_Win32.c"

#else
# error Operating System not supported
#endif

#endif // WINDOW_CREATION
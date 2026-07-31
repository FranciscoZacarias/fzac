#ifndef WINDOW_CREATION
#define WINDOW_CREATION

/*
  @Module: Handles window related functionality, such as window creation and input.
*/

// @Section: Window

typedef struct OS_Window OS_Window;

fz_internal OS_Window* window_create(String title, u32 width, u32 height, u32 x, u32 y);
fz_internal void       window_swap_buffers();
fz_internal void       window_destroy();

// @Section: Input
typedef u32 Keyboard_Key;
enum
{
  Keyboard_Key_BACKSPACE,
  Keyboard_Key_ENTER,
  Keyboard_Key_TAB,
  Keyboard_Key_SHIFT,
  Keyboard_Key_CONTROL,
  Keyboard_Key_MENU,
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
  Keyboard_Key_EQUAL,
  
  Keyboard_Key_Count,
};

static const char *const keyboard_key_to_string[] = {
  "Keyboard_Key_BACKSPACE", "Keyboard_Key_ENTER", "Keyboard_Key_TAB", "Keyboard_Key_SHIFT", "Keyboard_Key_CONTROL", "Keyboard_Key_MENU", "Keyboard_Key_PAUSE", "Keyboard_Key_CAPS_LOCK", "Keyboard_Key_ESCAPE", "Keyboard_Key_CONVERT", "Keyboard_Key_NONCONVERT", "Keyboard_Key_ACCEPT", "Keyboard_Key_MODECHANGE", "Keyboard_Key_SPACE", "Keyboard_Key_PRIOR", "Keyboard_Key_NEXT", "Keyboard_Key_END", "Keyboard_Key_HOME", "Keyboard_Key_ARROW_LEFT", "Keyboard_Key_ARROW_UP", "Keyboard_Key_ARROW_RIGHT", "Keyboard_Key_ARROW_DOWN", "Keyboard_Key_SELECT", "Keyboard_Key_PRINT", "Keyboard_Key_EXECUTE", "Keyboard_Key_SNAPSHOT", "Keyboard_Key_INSERT", "Keyboard_Key_DELETE", "Keyboard_Key_HELP", "Keyboard_Key_0", "Keyboard_Key_1", "Keyboard_Key_2", "Keyboard_Key_3", "Keyboard_Key_4", "Keyboard_Key_5", "Keyboard_Key_6", "Keyboard_Key_7", "Keyboard_Key_8", "Keyboard_Key_9", "Keyboard_Key_A", "Keyboard_Key_B", "Keyboard_Key_C", "Keyboard_Key_D", "Keyboard_Key_E", "Keyboard_Key_F", "Keyboard_Key_G", "Keyboard_Key_H", "Keyboard_Key_I", "Keyboard_Key_J", "Keyboard_Key_K", "Keyboard_Key_L", "Keyboard_Key_M", "Keyboard_Key_N", "Keyboard_Key_O", "Keyboard_Key_P", "Keyboard_Key_Q", "Keyboard_Key_R", "Keyboard_Key_S", "Keyboard_Key_T", "Keyboard_Key_U", "Keyboard_Key_V", "Keyboard_Key_W", "Keyboard_Key_X", "Keyboard_Key_Y", "Keyboard_Key_Z", "Keyboard_Key_LEFT_WIN", "Keyboard_Key_RIGHT_WIN", "Keyboard_Key_APPS", "Keyboard_Key_SLEEP", "Keyboard_Key_NUMPAD0", "Keyboard_Key_NUMPAD1", "Keyboard_Key_NUMPAD2", "Keyboard_Key_NUMPAD3", "Keyboard_Key_NUMPAD4", "Keyboard_Key_NUMPAD5", "Keyboard_Key_NUMPAD6", "Keyboard_Key_NUMPAD7", "Keyboard_Key_NUMPAD8", "Keyboard_Key_NUMPAD9", "Keyboard_Key_NUMPAD_MULTIPLY", "Keyboard_Key_NUMPAD_ADD", "Keyboard_Key_NUMPAD_SUBTRACT", "Keyboard_Key_NUMPAD_DECIMAL", "Keyboard_Key_NUMPAD_DIVIDE", "Keyboard_Key_F1", "Keyboard_Key_F2", "Keyboard_Key_F3", "Keyboard_Key_F4", "Keyboard_Key_F5", "Keyboard_Key_F6", "Keyboard_Key_F7", "Keyboard_Key_F8", "Keyboard_Key_F9", "Keyboard_Key_F10", "Keyboard_Key_F11", "Keyboard_Key_F12", "Keyboard_Key_F13", "Keyboard_Key_F14", "Keyboard_Key_F15", "Keyboard_Key_F16", "Keyboard_Key_F17", "Keyboard_Key_F18", "Keyboard_Key_F19", "Keyboard_Key_F20", "Keyboard_Key_F21", "Keyboard_Key_F22", "Keyboard_Key_F23", "Keyboard_Key_F24", "Keyboard_Key_NUMLOCK", "Keyboard_Key_SCROLL", "Keyboard_Key_NUMPAD_EQUAL", "Keyboard_Key_LEFT_SHIFT", "Keyboard_Key_RIGHT_SHIFT", "Keyboard_Key_LEFT_CONTROL", "Keyboard_Key_RIGHT_CONTROL", "Keyboard_Key_LEFT_MENU", "Keyboard_Key_RIGHT_MENU", "Keyboard_Key_SEMICOLON", "Keyboard_Key_PLUS", "Keyboard_Key_COMMA", "Keyboard_Key_MINUS", "Keyboard_Key_PERIOD", "Keyboard_Key_SLASH", "Keyboard_Key_GRAVE", "Keyboard_Key_LEFT_BRACKET", "Keyboard_Key_BACKSLASH", "Keyboard_Key_RIGHT_BRACKET", "Keyboard_Key_QUOTE", "Keyboard_Key_EQUAL",
  "Keyboard_Key_Count"
};

fz_internal void         _init_native_key_table(); /* Initializes the key tables for quick lookup at runtime */
fz_internal u32          _native_key_from_key(Keyboard_Key key); /* Converts os key to native key */
fz_internal Keyboard_Key _key_from_native_key(u32 native_key); /* Converts native key to os key */


typedef u32 Mouse_Button;
enum
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
  V2s32 raw_delta;
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
  b32 is_cursor_locked;
};

fz_internal void _input_init(); /* Initializes input state (clears previous + current states) */
fz_internal void _input_update(); /* Updates previous input state with current state (to track deltas and transitions) */
fz_internal void _input_process_keyboard_key(Keyboard_Key key, b8 is_pressed); /* Internal: Processes a key press/release and updates keyboard state */
fz_internal void _input_process_mouse_move(s32 x, s32 y);
fz_internal void _input_process_mouse_button(Mouse_Button button, b8 is_pressed); /* Internal: Processes mouse button press/release and updates mouse state */

fz_internal b8 is_key_up(Keyboard_Key key); /* True if the given key is currently up */
fz_internal b8 is_key_down(Keyboard_Key key); /* True if the given key is currently down */
fz_internal b8 was_key_up(Keyboard_Key key); /* True if the given key was up on the previous frame */
fz_internal b8 was_key_down(Keyboard_Key key); /* True if the given key was down on the previous frame */
fz_internal b8 is_key_clicked(Keyboard_Key key); /* True if the given key is down this frame but was up last frame */

fz_internal b8 is_button_up(Mouse_Button button); /* True if the given mouse button is currently up */
fz_internal b8 is_button_down(Mouse_Button button); /* True if the given mouse button is currently down */
fz_internal b8 was_button_up(Mouse_Button button); /* True if the given mouse button was up on the previous frame */
fz_internal b8 was_button_down(Mouse_Button button); /* True if the given mouse button was down on the previous frame */
fz_internal b8 is_button_clicked(Mouse_Button button); /* True if the given mouse button is down this frame but was up last frame */

fz_internal s32   get_mouse_x();
fz_internal s32   get_mouse_y();
fz_internal V2s32 get_mouse_position();
fz_internal s32   get_mouse_delta_x();
fz_internal s32   get_mouse_delta_y();
fz_internal V2s32 get_mouse_delta();
fz_internal s32   get_mouse_wheel_delta();

// @Section: Events

typedef u32 Event_Kind;
enum
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
      b8 shift_pressed : 1;
      b8 ctrl_pressed  : 1;
      b8 alt_pressed   : 1;
      b8 cmd_pressed   : 1; // MACOS
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

fz_internal Window_Event* _event_push(Event_Array* array);
fz_internal u32           get_total_events_this_frame();
fz_internal Window_Event*  get_event_this_frame(u32 index);

fz_internal void window_update_events(); /* Processes all window events this frame. Returns false if app should close */

#if OS_LINUX
// Opaque types so Window.h doesn't need to include <X11/Xlib.h> or <GL/glx.h> directly, avoiding name collisions
typedef struct _XDisplay Display;
typedef unsigned long    Window_X11;
typedef struct __GLXcontextRec* GLXContext;
typedef unsigned long    Atom;
#endif

struct OS_Window
{
  String title;

  s32 width;
  s32 height;

  s32 x;
  s32 y;

  b8 is_focused;
  b8 should_close;

  Input_State input;

  Event_Array events_this_frame;

  Arena *frame_arena;

#if OS_WINDOWS
  HINSTANCE hinstance;
  HWND      hwnd;
  HGLRC     rc;
  HDC       dc;
#elif OS_LINUX
  Display*   display;
  Window_X11 handle;
  GLXContext glx_context;
  Atom       wm_delete_window;
#else
# error Operating System not supported
#endif
};

fz_global OS_Window GlobalWindow;
fz_global b8 WindowClassInited = 0;

#if OS_WINDOWS
  fz_internal void _init_window_class(); /* Only needs to be called one time per process. */
  #include "windowsx.h"
  #include "Window/Window_Win32.c"
#elif OS_LINUX
  // Includes for the Linux implementation file (put X11 headers in Window_Linux.c)
  #include "Window/Window_Linux.c"
#endif

fz_internal s32   get_window_width();
fz_internal s32   get_window_height();
fz_internal V2s32 get_window_center();
fz_internal V2s32 get_window_dimensions();
fz_internal s32   get_window_x();
fz_internal s32   get_window_y();
fz_internal V2s32 get_window_position();
fz_internal b32   is_window_focused();

#endif // WINDOW_CREATION
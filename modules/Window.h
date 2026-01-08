#ifndef WINDOW_CREATION
#define WINDOW_CREATION

/*
  @Module: Handles window related functionality, such as window creation and input.
*/

// @Section: Window

typedef struct Window Window;

function Window* window_create(Window* parent, String title, u32 width, u32 height, u32 x, u32 y);

// @Section: Events and Input

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

typedef struct Event Event;
struct Event
{
  Event_Kind kind;
  union
  {
    struct
    {
      b32 shift_down : 1;
      b32 ctrl_down  : 1;
      b32 alt_down   : 1;
      b32 cmd_down   : 1; // MACOS
    };
    u32 packed;
  } modifiers;
};

typedef struct Event_Array Event_Array;
struct Event_Array
{
  Event* data;
  u32 count;
  u32 capacity;
};

function Event* _event_push(Event_Array* array);
function u32    get_total_events_this_frame();
function Event* get_event_this_frame(u32 index);

typedef struct Window_Context Window_Context;
struct Window_Context
{
  Arena* frame_arena;
  Event_Array events_this_frame;
};

global Window_Context WindowContext;

function void update_window_events(); /* Processes all window events this frame. Returns false if app should close */

#if OS_WINDOWS

  function void _init_window_class(); /* Only needs to be called one time per process. */
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

  #include "Window/Window_Win32.c"

#else
# error Operating System not supported
#endif

#endif // WINDOW_CREATION
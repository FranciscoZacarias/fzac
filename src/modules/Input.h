#ifndef INPUT_H
#define INPUT_H

typedef struct Event Event;

struct Event
{
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
  }
  modifiers;
};

global Event* EventsThisFrame;

function b32 update_window_events(); /* Processes all window events this frame. Returns false if app should close */

#if OS_WINDOWS

# include "Input/Input_Win32.c"

#else
# error Operating System not supported
#endif

#endif // INPUT_H
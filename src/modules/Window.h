#ifndef WINDOW_CREATION
#define WINDOW_CREATION

/*
  @Module: Handles window related functionality, such as window creation.
*/

typedef struct Window Window;

function Window* window_create(Window* parent, String title, u32 width, u32 height, u32 x, u32 y);
function void    console_attach();

#if OS_WINDOWS

# include "Window/Window_Win32.c"

#else
# error Operating System not supported
#endif

#endif // WINDOW_CREATION
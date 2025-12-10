#ifndef WINDOW_CREATION
#define WINDOW_CREATION

/*
  @Module: Handles window related functionality, such as window creation.
*/

typedef struct Window Window;

function Window* window_create(Window* parent, String title, u32 width, u32 height, u32 x, u32 y);

#if OS_WINDOWS

# pragma comment(lib, "user32.lib")
# pragma comment(lib, "gdi32.lib")
# pragma comment(lib, "kernel32.lib")

# define WIN32_LEAN_AND_MEAN
# include <windows.h>

# include "Window_Creation_Win32.c"

#else
# error Operating System not supported
#endif

#endif // WINDOW_CREATION
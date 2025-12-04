#ifndef WINDOW_CREATION
#define WINDOW_CREATION

/*
  @Module: Handles window related functionality, such as window creation.
*/

typedef struct Window Windoow;

function Window* window_create(String title, u32 width, u32 height);

#if OS_WINDOWS
#
# include "Window_Creation_WIN32.c"
#
#else
# error Operating System not supported
#endif

#endif // WINDOW_CREATION
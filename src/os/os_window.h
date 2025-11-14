#ifndef OS_WINDOW_H
#define OS_WINDOW_H

typedef struct OS_Window_Internal OS_Window_Internal; /* Forward declaration of internal implementation for the os window */

typedef struct OS_Window OS_Window;
struct OS_Window
{
  OS_Window* next;
  OS_Window* prev;

  String   title;
  Rangef32 rect;

  OS_Window_Internal* internal_window;
};

#endif // OS_WINDOW_H
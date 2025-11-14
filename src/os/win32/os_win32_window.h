#ifndef OS_WIN32_WINDOW_H
#define OS_WIN32_WINDOW_H

typedef struct OS_Window_Internal OS_Window_Internal; /* Win32 dependent implementation for os window */
struct OS_Window_Internal
{
  HWND  hwnd;
  HDC   hdc;
  HGLRC hglrc;
};

#endif // OS_WIN32_WINDOW_H
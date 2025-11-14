#ifndef WIN32_H
#define WIN32_H

// @Section: 16 bit 
typedef struct String_Win32 String_Win32;
struct String_Win32
{
  u64 size;
  WCHAR* str;
};

#include "os_win32_window.h"

#endif // WIN32_H
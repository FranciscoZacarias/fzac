#ifndef PLATFORM_H
#define PLATFORM_H

#include "Window.h"

// @Section: Console
fz_function void console_attach(); /* Opens application terminal */

// @Section: Clipboard
fz_function void   clipboard_write(String str);
fz_function String clipboard_read(Arena *arena);

// @Section: Cursor
typedef enum Cursor_Type
{
  Cursor_Arrow,
  Cursor_Hand,
  Cursor_Crosshair,
  Cursor_Ibeam,
  Cursor_Wait,
  Cursor_Size_All,
} Cursor_Type;

fz_function V2f32 cursor_get_position(Window *window);
fz_function void  cursor_lock(Window *window);
fz_function void  cursor_unlock();
fz_function b32   cursor_is_locked();
fz_function void  cursor_hide(b32 hide);
fz_function void  cursor_set_position(Window *window, s32 x, s32 y);
fz_function void  cursor_set_icon(Cursor_Type type);

// @Section: Errors
fz_function void message_box(String title, String content, String file, u32 line);

#if OS_WINDOWS
# include "Platform/Platform_Win32.c"
#else
# error Operating System not supported
#endif

#endif // PLATFORM_H
#ifndef FZAC_PLATFORM_H
#define FZAC_PLATFORM_H

#if FZ_RAYLIB
# error "Platform.h is replaced by raylib in Base_Raylib.h"
#endif

#include "Window.h"

// @Section: Clipboard
fz_internal void   clipboard_write(String str);
fz_internal String clipboard_read(Arena *arena);

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

fz_internal Vector2 cursor_get_position(OS_Window *window);
fz_internal void  cursor_lock(OS_Window *window);
fz_internal void  cursor_unlock();
fz_internal b32   cursor_is_locked();
fz_internal void  cursor_hide(b32 hide);
fz_internal void  cursor_set_position(OS_Window *window, s32 x, s32 y);
fz_internal void  cursor_set_icon(Cursor_Type type);

// @Section: Errors
fz_internal void message_box(String title, String content, String file, u32 line);

#if OS_WINDOWS
# include "Platform/Platform_Win32.c"
#else
# error Operating System not supported
#endif

#endif // FZAC_PLATFORM_H
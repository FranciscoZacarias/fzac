#ifndef PLATFORM_H
#define PLATFORM_H

// @Section: Console
function void console_attach(); /* Opens application terminal */

// @Section: Errors
function void error_box(String title, String content, String file, u32 line);

#if OS_WINDOWS
# include "Platform/Platform_Win32.c"
#else
# error Operating System not supported
#endif

#endif // PLATFORM_H
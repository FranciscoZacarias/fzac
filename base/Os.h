#ifndef OS_H
#define OS_H

fz_internal void message_box(String title, String content, String file, u32 line);
fz_internal void message_box_with_debugger(String title, String content, String file, u32 line);

#if OS_WINDOWS
# include "Os/Os_Win32.c"
#elif OS_LINUX
# include "Os/Os_Linux.c"
#else
# error Operating System not supported
#endif

#endif // OS_H
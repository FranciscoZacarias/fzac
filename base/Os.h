#ifndef FZAC_OS_H
#define FZAC_OS_H

fz_internal void message_box(String title, String content, String file, u32 line);
fz_internal void message_box_with_debugger(String title, String content, String file, u32 line);
fz_internal void console_attach(); /* Attaches to the parent process console, or opens a new one */
fz_internal b32  debugger_is_attached(); /* True if any debugger is attached to the process */

#if OS_WINDOWS
# include "Os/Os_Win32.c"
#elif OS_LINUX
# include "Os/Os_Linux.c"
#else
# error Operating System not supported
#endif

#endif // FZAC_OS_H
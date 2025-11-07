#ifndef OS_H
#define OS_H

#if OS_WINDOWS
#
# ifdef UNICODE
# undef UNICODE
# endif // UNICODE
#
# pragma warning(push)
# pragma warning(disable: 4042) // Avoids known warning from winnls.h
#
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#
# pragma warning(pop)
# include "win32/os_win32.h"
#
#elif OS_LINUX
#
# include "linux/os_linux.h"
#
#else
#
# error OS core layer not implemented for this operating system.
#
#endif

#include "input/os_input.h"
#include "os_memory.h"

#endif // OS_H
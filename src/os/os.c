#if OS_WINDOWS
#
# include "win32/os_win32.c"
#
#elif OS_LINUX
#
# include "linux/os_linux.c"
#
#else
#
# error "OS layer not implemented for this operating system."
#
#endif

#include "input/os_input.c"
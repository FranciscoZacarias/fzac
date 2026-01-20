#ifndef CONTEXT_CRACKING_H
#define CONTEXT_CRACKING_H

#if defined(__cplusplus)
# define C_LINKAGE extern "C"
# define C_LINKAGE_BEGIN extern "C" {
# define C_LINKAGE_END }
#else
# define C_LINKAGE
# define C_LINKAGE_BEGIN
# define C_LINKAGE_END
#endif

#define COMPILER_CLANG 0
#define COMPILER_MSVC  0
#define COMPILER_GCC   0

#if defined(__clang__)
# undef COMPILER_CLANG
# define COMPILER_CLANG 1
# define COMPILER_CLANG_MAJOR __clang_major__
# define COMPILER_CLANG_MINOR __clang_minor__
#elif defined(_MSC_VER)
# undef COMPILER_MSVC
# define COMPILER_MSVC 1
# define COMPILER_MSVC_VER _MSC_VER
#elif defined(__GNUC__)
# undef COMPILER_GCC
# define COMPILER_GCC 1
# define COMPILER_GCC_MAJOR __GNUC__
# define COMPILER_GCC_MINOR __GNUC_MINOR__
#else
# error Unsupported compiler
#endif

#define OS_WINDOWS 0
#define OS_LINUX   0
#define OS_MACOS   0
#define OS_IOS     0
#define OS_FREEBSD 0
#define OS_OPENBSD 0

#if defined(_WIN32)
# undef OS_WINDOWS
# define OS_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
# include <TargetConditionals.h>
# if TARGET_OS_IPHONE
#  undef OS_IOS
#  define OS_IOS 1
# else
#  undef OS_MACOS
#  define OS_MACOS 1
# endif
#elif defined(__linux__)
# undef OS_LINUX
# define OS_LINUX 1
#elif defined(__FreeBSD__)
# undef OS_FREEBSD
# define OS_FREEBSD 1
#elif defined(__OpenBSD__)
# undef OS_OPENBSD
# define OS_OPENBSD 1
#else
# error Unsupported OS
#endif

#define ARCH_X64   0
#define ARCH_X86   0
#define ARCH_ARM64 0
#define ARCH_ARM32 0
#define ARCH_PPC64 0
#define ARCH_PPC32 0

#if defined(__x86_64__) || defined(_M_AMD64)
# undef ARCH_X64
# define ARCH_X64 1
#elif defined(__i386__) || defined(_M_IX86)
# undef ARCH_X86
# define ARCH_X86 1
#elif defined(__aarch64__) || defined(_M_ARM64)
# undef ARCH_ARM64
# define ARCH_ARM64 1
#elif defined(__arm__) || defined(_M_ARM)
# undef ARCH_ARM32
# define ARCH_ARM32 1
#elif defined(__ppc64__)
# undef ARCH_PPC64
# define ARCH_PPC64 1
#elif defined(__ppc__)
# undef ARCH_PPC32
# define ARCH_PPC32 1
#else
# error Unsupported architecture
#endif

#if ARCH_X64 || ARCH_ARM64 || ARCH_PPC64
# define ARCH_ADDRSIZE 64
#else
# define ARCH_ADDRSIZE 32
#endif

#if ARCH_PPC32 || ARCH_PPC64
# define ARCH_BIG_ENDIAN 1
# define ARCH_LITTLE_ENDIAN 0
#else
# define ARCH_BIG_ENDIAN 0
# define ARCH_LITTLE_ENDIAN 1
#endif

#if COMPILER_MSVC
# define thread_static __declspec(thread)
#elif COMPILER_CLANG || COMPILER_GCC
# define thread_static __thread
#else
# define thread_static
#endif

#if OS_WINDOWS
# if COMPILER_MSVC
#  define shared_function C_LINKAGE __declspec(dllexport)
# else
#  define shared_function C_LINKAGE __attribute__((dllexport))
# endif
#else
# define shared_function C_LINKAGE
#endif

#if OS_WINDOWS
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# ifdef UNICODE
#  undef UNICODE
# endif
# if COMPILER_MSVC
#  pragma warning(push)
#  pragma warning(disable: 4042)
# endif
# include <windows.h>
# ifdef min
#  undef min
# endif
# ifdef max
#  undef max
# endif
# if COMPILER_MSVC
#  pragma warning(pop)
# endif
#endif

#endif

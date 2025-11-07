#ifndef CONTEXT_CRACKING_H
#define CONTEXT_CRACKING_H

/* === C/C++ Linkage === */
#if __cplusplus
#
# define C_LINKAGE extern "C"
# define C_LINKAGE_BEGIN extern "C" {
# define C_LINKAGE_END   }
#
#else
#
# define C_LINKAGE
# define C_LINKAGE_BEGIN
# define C_LINKAGE_END
#
#endif

/* === Compiler Detection === */
#if defined(__clang__)
#
# define COMPILER_CLANG 1
# define COMPILER_CLANG_MAJOR __clang_major__
# define COMPILER_CLANG_MINOR __clang_minor__
#
#elif defined(_MSC_VER)
#
# define COMPILER_MSVC 1
# define COMPILER_MSVC_VER _MSC_VER
#
#elif defined(__GNUC__) || defined(__GNUG__)
#
# define COMPILER_GCC 1
# define COMPILER_GCC_MAJOR __GNUC__
# define COMPILER_GCC_MINOR __GNUC_MINOR__
#
#else
#
# error "Unsupported compiler"
#
#endif

/* === Compiler to string === */
#if COMPILER_MSVC
#
# define COMPILER_NAME "MSVC"
# define COMPILER_VERSION_STRING _MSC_FULL_VER
#
#elif COMPILER_CLANG
#
# define COMPILER_NAME "Clang"
# define COMPILER_VERSION_STRING (__clang_major__ * 100 + __clang_minor__)
#
#elif COMPILER_GCC
#
# define COMPILER_NAME "GCC"
# define COMPILER_VERSION_STRING (__GNUC__ * 100 + __GNUC_MINOR__)
#
#endif

/* === Default Compiler Macros === */
#ifndef COMPILER_CLANG
# define COMPILER_CLANG 0
#endif
#ifndef COMPILER_MSVC
# define COMPILER_MSVC 0
#endif
#ifndef COMPILER_GCC
# define COMPILER_GCC 0
#endif

/* === OS Detection (Independent of Compiler) === */
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#
# define OS_WINDOWS 1
#
#elif defined(__APPLE__) && defined(__MACH__)
#
# define OS_MAC 1
# include <TargetConditionals.h>
# if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#  define OS_IOS 1
# elif TARGET_OS_MAC
#  define OS_MACOS 1
# endif
#
#elif defined(__linux__) || defined(__gnu_linux__)
#
# define OS_LINUX 1
#
#elif defined(__FreeBSD__)
#
# define OS_FREEBSD 1
#
#elif defined(__OpenBSD__)
#
# define OS_OPENBSD 1
#
#else
#
# error "Unknown OS"
#
#endif

/* Default OS macros */
#ifndef OS_WINDOWS
# define OS_WINDOWS 0
#endif
#ifndef OS_MAC
# define OS_MAC 0
#endif
#ifndef OS_LINUX
# define OS_LINUX 0
#endif
#ifndef OS_IOS
# define OS_IOS 0
#endif
#ifndef OS_MACOS
# define OS_MACOS 0
#endif

/* === Architecture Detection === */
#if defined(__x86_64__) || defined(_M_AMD64) || defined(__amd64__)
# define ARCH_X64 1
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
# define ARCH_X86 1
#elif defined(__aarch64__) || defined(_M_ARM64)
# define ARCH_ARM64 1
#elif defined(__arm__) || defined(_M_ARM)
# define ARCH_ARM32 1
#elif defined(__ppc64__) || defined(__powerpc64__)
# define ARCH_PPC64 1
#elif defined(__ppc__) || defined(__powerpc__)
# define ARCH_PPC32 1
#else
# error "Unsupported architecture"
#endif

/* Default arch macros */
#ifndef ARCH_X64
# define ARCH_X64 0
#endif
#ifndef ARCH_X86
# define ARCH_X86 0
#endif
#ifndef ARCH_ARM64
# define ARCH_ARM64 0
#endif
#ifndef ARCH_ARM32
# define ARCH_ARM32 0
#endif

/* === Address Size === */
#if ARCH_X64 || ARCH_ARM64 || ARCH_PPC64
# define ARCH_ADDRSIZE 64
#else
# define ARCH_ADDRSIZE 32
#endif

/* === Endianness === */
#if ARCH_X86 || ARCH_X64 || ARCH_ARM32 || ARCH_ARM64
#
# define ARCH_LITTLE_ENDIAN 1
# define ARCH_BIG_ENDIAN    0
#
#elif ARCH_PPC32 || ARCH_PPC64
#
# define ARCH_LITTLE_ENDIAN 0
# define ARCH_BIG_ENDIAN    1
#
#else
#
# define ARCH_LITTLE_ENDIAN 1  // assume LE
# define ARCH_BIG_ENDIAN    0
#
#endif

/* === Thread Local Storage === */
#if COMPILER_MSVC
#
# define thread_static __declspec(thread)
#
#elif COMPILER_CLANG || COMPILER_GCC
#
# define thread_static __thread
#
#else
#
# define thread_static /* not supported */
#
#endif

/* === DLL Export === */
#if OS_WINDOWS
#
# if COMPILER_MSVC
#  define shared_function __declspec(dllexport) C_LINKAGE
# else
#  define shared_function __attribute__((dllexport)) C_LINKAGE
# endif
#
#else
#
# define shared_function C_LINKAGE
#
#endif

#endif // CONTEXT_CRACKING_H
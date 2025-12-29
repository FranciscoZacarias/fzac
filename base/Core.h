#ifndef CORE_H
#define CORE_H

/* === Macro Helpers === */
#define statement(m) do{ m }while(0)
#define stringigy_(S) #S
#define stringigy(S) stringigy_(S)
#define glue_(A,B) A##B
#define glue(A,B) glue_(A,B)

/* === Static is Ambiguous === */
#define local_persist static
#define global        static
#define function      static

/* === breakpoints === */
#if COMPILER_MSVC
# define breakpoint() DebugBreak();
#elif COMPILER_CLANG || COMPILER_GCC
# define breakpoint() __builtin_trap();
#else
# error "Unknown trap intrinsic for this compiler."
#endif

/* === Readonly === */
#if OS_WINDOWS
# pragma section(".roglob", read)
# define read_only __declspec(allocate(".roglob"))
#else
# define read_only
#endif

/* === asserts === */
#if DEBUG
# define assert_break(condition) (*(volatile int*)0 = 0)
# define assert(condition) statement(if (!(condition)){ assert_break(condition); })
# define assert_no_reentry() statement(local_persist b32 __triggered__ = 0; assert(__triggered__ == 0); __triggered__ = 1;) 
# define assert_unreachable() assert_break(0)
# define static_assert(condition,label) typedef u8 glue(label,__LINE__) [(condition)?1:-1]
#else
# define assert_break(condition)
# define assert(condition)
# define assert_no_reentry()
# define assert_unreachable()
# define static_assert(condition,label)
#endif

/* === Code === */
#define array_count(a) (sizeof(a)/sizeof((a)[0]))

#define int_from_pointer(p) (u64)((u8*)p - (u8*)0)
#define pointer_from_int(i) (void*)((u8*)0 + (i))

#define member(T,m) (((T*)0)->m)
#define offset_of_member(T,m) int_from_pointer(&member(T,m))

/* === Numbers === */
#define kilobytes(n) ((u64)(n * 1024))
#define megabytes(n) ((u64)(n * 1024 * 1024))
#define gigabytes(n) ((u64)(n * 1024 * 1024 * 1024))
#define terabytes(n) ((u64)(n * 1024 * 1024 * 1024 * 1024))

#define thousand(n) ((n)*1000)
#define million(n)  ((n)*1000000llu)
#define billion(n)  ((n)*1000000000llu)
#define trillion(n) ((n)*1000000000000llu)

#define DEFAULT_ALIGNMENT sizeof(void*)

#define align_power_of_two(x,b)      (((x) + (b) - 1)&(~((b) - 1)))
#define align_down_power_of_two(x,b) ((x)&(~((b) - 1)))
#define is_power_of_two(x)           ((x)!=0 && ((x)&((x)-1))==0)
#define is_power_of_two_or_zero(x)   ((((x) - 1)&(x)) == 0)

/* === Memory === */
#define memory_copy(dst, src, size) memcpy((dst), (src), (size_t)(size))
#define memory_move(dst, src, size) memmove((dst), (src), (size_t)(size))
#define memory_set(dst, val, size)  memset((dst), (val), (size_t)(size))
#define memory_match(a,b,size)     (memcmp((a),(b),(size_t)(size)) == 0)

#define memory_copy_struct(dst, src) statement(memory_copy((dst), (src), sizeof(*(dst)));)
#define memory_copy_array(dst, src)  statement(memory_copy((dst), (src), sizeof(src));)

#define memory_zero(ptr, size) memory_set((ptr), 0, (size))
#define memory_zero_struct(ptr) memory_zero((ptr), sizeof(*(ptr)))
#define memory_zero_array(arr)  memory_zero((arr), sizeof(arr))

/* === Flags === */
#define set_flags(flags, flag)    ((flags) |= (flag))
#define has_flags(flags, check_flags) (((flags) & (check_flags)) == (check_flags))
#define toggle_flag(flags, flag) ((flags) ^= (flag))

/* === Control flow === */
#define defer_loop(begin, end) for(u32 _defer_loop_internal_index_ = ((begin), 0); !_defer_loop_internal_index_; _defer_loop_internal_index_ += 1, (end))

/* === Data types === */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef float f32;
typedef double f64;

typedef s8  b8;
typedef s32 b32;
typedef s64 b64;

#define true  1
#define false 0

/* === Data structures === */

#endif // CORE_H
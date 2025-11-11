#ifndef CORE_H
#define CORE_H

/* === macro Helpers === */
#define macro(m) do{ m }while(0)
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
# define assert(condition) macro(if (!(condition)){ assert_break(condition); })
# define assert_no_reentry() macro(local_persist b32 __triggered__ = 0; assert(__triggered__ == 0); __triggered__ = 1;) 
# define static_assert(condition,label) typedef u8 glue(label,__LINE__) [(condition)?1:-1]
#else
# define assert_break(condition)
# define assert(condition)
# define assert_no_reentry()
# define static_assert(condition,label)
#endif

/* === Code === */
#define array_count(a) (sizeof(a)/sizeof((a)[0]))

#define min(A,B) (((A)<(B))?(A):(B))
#define max(A,B) (((A)>(B))?(A):(B))
#define clamp_top(A,X) min(A,X)
#define clamp_bot(X,B) max(X,B)
#define clamp(val,min,max) (((val)<(min))?(min):((val)>(max))?(max):(val))

#define int_from_pointer(p) (u64)((u8*)p - (u8*)0)
#define pointer_from_int(i) (void*)((u8*)0 + (i))

#define member(T,m) (((T*)0)->m)
#define offset_of_member(T,m) int_from_pointer(&member(T,m))

#define ignore_unused(var) ((void)var)

/* === Numbers === */
#define kilobytes(n) ((u64)(n * 1024))
#define megabytes(n) ((u64)(n * 1024 * 1024))
#define gigabytes(n) ((u64)(n * 1024 * 1024 * 1024))
#define terabytes(n) ((u64)(n * 1024 * 1024 * 1024 * 1024))

#define thousand(n) ((n)*1000)
#define million(n)  ((n)*1000000llu)
#define billion(n)  ((n)*1000000000llu)
#define trillion(n) ((n)*1000000000000llu)

#define DefaultAlignment   sizeof(void*)
#define align_power_of_two(x,b)      (((x) + (b) - 1)&(~((b) - 1)))
#define align_down_power_of_two(x,b) ((x)&(~((b) - 1)))
#define is_power_of_two(x)           ((x)!=0 && ((x)&((x)-1))==0)
#define is_power_of_two_or_zero(x)   ((((x) - 1)&(x)) == 0)

/* === Memory === */
#define memory_copy(dst, src, size) memcpy((dst), (src), (size_t)(size))
#define memory_move(dst, src, size) memmove((dst), (src), (size_t)(size))
#define memory_set(dst, val, size)  memset((dst), (val), (size_t)(size))
#define memory_match(a,b,size)     (memcmp((a),(b),(size_t)(size)) == 0)

#define memory_copy_struct(dst, src) macro(memory_copy((dst), (src), sizeof(*(dst)));)
#define memory_copy_array(dst, src)  macro(memory_copy((dst), (src), sizeof(src));)

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
read_only global u8 U8Min = 0x00;
read_only global u8 U8Max = 0xFF;
typedef unsigned short u16;
read_only global u16 U16Min = 0x0000;
read_only global u16 U16Max = 0xFFFF;
typedef unsigned int u32;
read_only global u32 U32Min = 0x00000000;
read_only global u32 U32Max = 0xFFFFFFFF;
typedef unsigned long long u64;
read_only global u64 U64Min = 0x0000000000000000ULL;
read_only global u64 U64Max = 0xFFFFFFFFFFFFFFFFULL;
typedef signed char s8;
read_only global s8 S8Min = (-0x7F - 1);
read_only global s8 S8Max = 0x7F;
typedef signed short s16;
read_only global s16 S16Min = (-0x7FFF - 1);
read_only global s16 S16Max = 0x7FFF;
typedef signed int s32;
read_only global s32 S32Min = (-0x7FFFFFFF - 1);
read_only global s32 S32Max = 0x7FFFFFFF;
typedef signed long long s64;
read_only global s64 S64Min = (-0x7FFFFFFFFFFFFFFFLL - 1);
read_only global s64 S64Max = 0x7FFFFFFFFFFFFFFFLL;
typedef float f32;
read_only global f32 F32Min     = -3.402823466e+38f;
read_only global f32 F32Max     =  3.402823466e+38f;
read_only global f32 F32Epsilon =  1.192092896e-07f;
typedef double f64;
read_only global f64 F64Min     = -1.7976931348623158e+308;
read_only global f64 F64Max     =  1.7976931348623158e+308;
read_only global f64 F64Epsilon =  2.2204460492503131e-16;

typedef s8  b8;
typedef s32 b32;
typedef s64 b64;
#define true  1
#define false 0

/* === Data structures === */

#endif // CORE_H
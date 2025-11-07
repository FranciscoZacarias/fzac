#ifndef CORE_H
#define CORE_H

/* === Macro Helpers === */
#define Macro(m) do{ m }while(0)
#define Stringify_(S) #S
#define Stringify(S) Stringify_(S)
#define Glue_(A,B) A##B
#define Glue(A,B) Glue_(A,B)

/* === Static is Ambiguous === */
#define local_persist static
#define global        static
#define function      static

/* === Breakpoints === */
#if COMPILER_MSVC
# define Breakpoint() DebugBreak();
#elif COMPILER_CLANG || COMPILER_GCC
# define Breakpoint() __builtin_trap();
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

/* === Asserts === */
#if DEBUG
# define AssertBreak(condition) (*(volatile int*)0 = 0)
# define Assert(condition) Macro(if (!(condition)){ AssertBreak(condition); })
# define AssertNoReentry() Macro(local_persist b32 __triggered__ = 0; Assert(__triggered__ == 0); __triggered__ = 1;) 
# define StaticAssert(condition,label) typedef u8 Glue(label,__LINE__) [(condition)?1:-1]
#else
# define AssertBreak(condition)
# define Assert(condition)
# define AssertNoReentry()
# define StaticAssert(condition,label)
#endif

/* === Code === */
#define ArrayCount(a) (sizeof(a)/sizeof((a)[0]))

#define Min(A,B) (((A)<(B))?(A):(B))
#define Max(A,B) (((A)>(B))?(A):(B))
#define Clamp(val,min,Max) (((val)<(min))?(min):((val)>(Max))?(Max):(val))

#define IntFromPtr(p) (u64)((u8*)p - (u8*)0)
#define PtrFromInt(i) (void*)((u8*)0 + (i))

#define Member(T,m) (((T*)0)->m)
#define OffsetOfMember(T,m) IntFromPtr(&Member(T,m))

/* === Numbers === */
#define Kilobytes(n) ((u64)(n * 1024))
#define Megabytes(n) ((u64)(n * 1024 * 1024))
#define Gigabytes(n) ((u64)(n * 1024 * 1024 * 1024))
#define Terabytes(n) ((u64)(n * 1024 * 1024 * 1024 * 1024))

#define Thousand(n) ((n)*1000)
#define Million(n)  ((n)*1000000llu)
#define Billion(n)  ((n)*1000000000llu)
#define Trillion(n) ((n)*1000000000000llu)

#define DEFAULT_ALIGNMENT sizeof(void*)
#define AlignPow2(x,b)     (((x) + (b) - 1)&(~((b) - 1)))
#define AlignDownPow2(x,b) ((x)&(~((b) - 1)))
#define IsPow2(x)          ((x)!=0 && ((x)&((x)-1))==0)
#define IsPow2OrZero(x)    ((((x) - 1)&(x)) == 0)

/* === Memory === */
#define MemoryCopy(dst, src, size) memcpy((dst), (src), (size))
#define MemoryMove(dst, src, size) memmove((dst), (src), (size))
#define MemorySet(dst, val, size)  memset((dst), (val), (size))
#define MemoryMatch(a,b,size)     (memcmp((a),(b),(size)) == 0)

#define MemoryCopyStruct(dst, src) Macro(MemoryCopy((dst), (src), sizeof(*(dst)));)
#define MemoryCopyArray(dst, src)  Macro(MemoryCopy((dst), (src), sizeof(src));)

#define MemoryZero(ptr, size) MemorySet((ptr), 0, (size))
#define MemoryZeroStruct(ptr) MemoryZero((ptr), sizeof(*(ptr)))
#define MemoryZeroArray(arr)  MemoryZero((arr), sizeof(arr))

/* === Flags === */
#define SetFlags(flags, flag)    ((flags) |= (flag))
#define HasFlags(flags, check_flags) (((flags) & (check_flags)) == (check_flags))
#define ToggleFlag(flags, flag) ((flags) ^= (flag))

/* === Control flow === */
#define DeferLoop(begin, end) for(u32 _defer_loop_internal_index_ = ((begin), 0); !_defer_loop_internal_index_; _defer_loop_internal_index_ += 1, (end))

/* === Data types === */
typedef unsigned char u8;
read_only global u8 U8_MIN = 0x00;
read_only global u8 U8_MAX = 0xFF;
typedef unsigned short u16;
read_only global u16 U16_MIN = 0x0000;
read_only global u16 U16_MAX = 0xFFFF;
typedef unsigned int u32;
read_only global u32 U32_MIN = 0x00000000;
read_only global u32 U32_MAX = 0xFFFFFFFF;
typedef unsigned long long u64;
read_only global u64 U64_MIN = 0x0000000000000000ULL;
read_only global u64 U64_MAX = 0xFFFFFFFFFFFFFFFFULL;
typedef signed char s8;
read_only global s8 S8_MIN = (-0x7F - 1);
read_only global s8 S8_MAX = 0x7F;
typedef signed short s16;
read_only global s16 S16_MIN = (-0x7FFF - 1);
read_only global s16 S16_MAX = 0x7FFF;
typedef signed int s32;
read_only global s32 S32_MIN = (-0x7FFFFFFF - 1);
read_only global s32 S32_MAX = 0x7FFFFFFF;
typedef signed long long s64;
read_only global s64 S64_MIN = (-0x7FFFFFFFFFFFFFFFLL - 1);
read_only global s64 S64_MAX = 0x7FFFFFFFFFFFFFFFLL;
typedef float f32;
read_only global f32 F32_MIN     = -3.402823466e+38f;
read_only global f32 F32_MAX     =  3.402823466e+38f;
read_only global f32 F32_EPSILON =  1.192092896e-07f;
typedef double f64;
read_only global f64 F64_MIN     = -1.7976931348623158e+308;
read_only global f64 F64_MAX     =  1.7976931348623158e+308;
read_only global f64 F64_EPSILON =  2.2204460492503131e-16;

typedef s8  b8;
typedef s32 b32;
typedef s64 b64;
#define true  1
#define false 0

/* === Data structures === */

#endif // CORE_H
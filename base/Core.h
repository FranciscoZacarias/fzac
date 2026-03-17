
#ifndef CORE_H
#define CORE_H

/* === Macro Helpers === */
#define statement(m) do{ m }while(0)
#define stringify_(S) #S
#define stringify(S) stringify_(S)
#define glue_(A,B) A##B
#define glue(A,B) glue_(A,B)

/* === Static is Ambiguous === */
#define local_persist   static
#define global          static
#define function        static
#define function_inline static inline

/* === Readonly === */
#if OS_WINDOWS
# pragma section(".roglob", read)
# define read_only __declspec(allocate(".roglob"))
#else
# define read_only
#endif

/* === Thread local === */
#if COMPILER_MSVC
# define thread_local __declspec(thread)
#elif COMPILER_CLANG || COMPILER_GCC
# define thread_local __thread
#else
# define thread_local
# warning "thread_local not supported on this compiler"
#endif

/* === asserts === */
#if DEBUG
# define assert_break() (*(volatile int*)0 = 0)
# define assert(condition) statement(if (!(condition)){ assert_break(); })
# define assert_no_reentry() statement(local_persist b32 __triggered__ = 0; assert(__triggered__ == 0); __triggered__ = 1;) 
# define assert_unreachable() assert_break(0)
# define static_assert(condition,label) typedef u8 glue(label,__LINE__) [(condition)?1:-1]
# define assert_expr(condition) ((condition) ? 0 : (assert_break(), 0))

/* === breakpoints === */
# if COMPILER_MSVC
#  define breakpoint() DebugBreak();
# elif COMPILER_CLANG || COMPILER_GCC
#  define breakpoint() __builtin_trap();
# else
#  warning "Unknown trap intrinsic (breakpoint) for this compiler."
# endif

#else
# define assert_break()
# define assert(condition)
# define assert_no_reentry()
# define assert_unreachable()
# define static_assert(condition,label)
# define assert_expr(condition) (0)
# define breakpoint()
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
#define set_flags(flags, flag)        ((flags) |= (flag))
#define has_flags(flags, check_flags) (((flags) & (check_flags)) == (check_flags))
#define toggle_flag(flags, flag)      ((flags) ^= (flag))

/* === Control flow === */
#define DEFER_LOOP_ID2(a, b) a##b
#define DEFER_LOOP_ID(a, b) DEFER_LOOP_ID2(a, b)
#define defer_loop_impl(begin, end, id)                                  \
  for(u32 DEFER_LOOP_ID(_defer_loop_internal_index_, id) = ((begin), 0); \
      !DEFER_LOOP_ID(_defer_loop_internal_index_, id);                   \
      DEFER_LOOP_ID(_defer_loop_internal_index_, id) += 1, (end))

#define defer_loop(begin, end)                                           \
  defer_loop_impl(begin, end, __COUNTER__)


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

/* === Section: Utils === */
#undef min
#undef max
#undef abs
#define min(A,B) (((A) < (B)) ? (A) : (B))
#define max(A,B) (((A) > (B)) ? (A) : (B))
#define abs(val) (((val) < 0) ? -(val) : (val))

#define clamp_top(A,X) min(A,X)
#define clamp_bot(X,B) max(X,B)
#define clamp(val,lo,hi) (((val) < (lo)) ? (lo) : (((val) > (hi)) ? (hi) : (val)))

#define to_degrees(r) ((r) * (180.0f / PI32))
#define to_radians(d) ((d) * (PI32 / 180.0f))

/* === Constants === */
#define U8_MIN      (u8) 0x00
#define U8_MAX      (u8) 0xFF
#define U16_MIN     (u16) 0x0000
#define U16_MAX     (u16) 0xFFFF
#define U32_MIN     (u32) 0x00000000
#define U32_MAX     (u32) 0xFFFFFFFF
#define U64_MIN     (u64) 0x0000000000000000ULL
#define U64_MAX     (u64) 0xFFFFFFFFFFFFFFFFULL

#define S8_MIN      (s8) (-0x7F - 1)
#define S8_MAX      (s8) 0x7F
#define S16_MIN     (s16) (-0x7FFF - 1)
#define S16_MAX     (s16) 0x7FFF
#define S32_MIN     (s32) (-0x7FFFFFFF - 1)
#define S32_MAX     (s32) 0x7FFFFFFF
#define S64_MIN     (s64) (-0x7FFFFFFFFFFFFFFFLL - 1)
#define S64_MAX     (s64) 0x7FFFFFFFFFFFFFFFLL

#define F32_MIN     (f32) -3.402823466e+38f
#define F32_MAX     (f32)  3.402823466e+38f
#define F32_EPSILON (f32)  1.192092896e-07f

#define F64_MIN     (f64) -1.7976931348623158e+308
#define F64_MAX     (f64)  1.7976931348623158e+308
#define F64_EPSILON (f64)  2.2204460492503131e-16

#define TAU32 (f32) 6.283185
#define TAU64 (f64) 6.28318530717958648

#define PI32 (f32) 3.1415927
#define PI64 (f64) 3.141592653589793

#endif // CORE_H
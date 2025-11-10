#ifndef TEST_H
#define TEST_H

#include <stdio.h>

typedef struct Test_Result Test_Result;
struct Test_Result
{
  u64 total;
  u64 passed;
};

/* === Initialization === */
#define test_init() (Test_Result){0, 0}

/* === Basic equality === */
#define test_equal(test_result, a, b)                            \
  Macro(                                                         \
    (test_result).total += 1;                                    \
    if ((a) == (b))                                              \
    {                                                            \
      (test_result).passed += 1;                                 \
    }                                                            \
    else                                                         \
    {                                                            \
      fprintf(stderr, "FAIL: %s:%d  %s != %s  (%lld vs %lld)\n", \
              __FILE__, __LINE__, #a, #b, (s64)(a), (s64)(b));   \
    }                                                            \
  )

#define test_not_equal(test_result, a, b)                        \
  Macro(                                                         \
    (test_result).total += 1;                                    \
    if ((a) != (b))                                              \
    {                                                            \
      (test_result).passed += 1;                                 \
    }                                                            \
    else                                                         \
    {                                                            \
      fprintf(stderr, "FAIL: %s:%d  %s == %s\n",                 \
              __FILE__, __LINE__, #a, #b);                       \
    }                                                            \
  )

/* === Boolean checks === */
#define test_true(test_result, expr)  test_equal(test_result, (expr) ? 1 : 0, 1)
#define test_false(test_result, expr) test_equal(test_result, (expr) ? 1 : 0, 0)

/* === Memory comparison === */
#define test_memory_equal(test_result, a, b, size)               \
  Macro(                                                         \
    (test_result).total += 1;                                    \
    if (MemoryMatch((a), (b), (size)))                           \
    {                                                            \
      (test_result).passed += 1;                                 \
    }                                                            \
    else                                                         \
    {                                                            \
      fprintf(stderr, "FAIL: %s:%d  memory mismatch (%s vs %s, %llu bytes)\n", \
              __FILE__, __LINE__, #a, #b, (u64)(size));          \
    }                                                            \
  )

/* === Numbers === */
#define test_typed_equal(test_result, type, a, b)                \
  Macro(                                                         \
    (test_result).total += 1;                                    \
    type A_ = (a);                                               \
    type B_ = (b);                                               \
    if (A_ == B_)                                                \
    {                                                            \
      (test_result).passed += 1;                                 \
    }                                                            \
    else                                                         \
    {                                                            \
      fprintf(stderr, "FAIL: %s:%d  %s != %s  ("                 \
                      Stringify(type)                            \
                      ": %lld vs %lld)\n",                       \
              __FILE__, __LINE__, #a, #b, (s64)A_, (s64)B_);     \
    }                                                            \
  )

#define test_typed_equal_float(test_result, type, a, b, epsilon) \
  Macro(                                                         \
    (test_result).total += 1;                                    \
    f64 A_ = (f64)(a);                                           \
    f64 B_ = (f64)(b);                                           \
    f64 diff_ = fabs(A_ - B_);                                   \
    if (diff_ <= (epsilon))                                      \
    {                                                            \
      (test_result).passed += 1;                                 \
    }                                                            \
    else                                                         \
    {                                                            \
      fprintf(stderr, "FAIL: %s:%d  %s != %s  ("                 \
                      Stringify(type)                            \
                      ": %f vs %f, diff=%f)\n",                  \
              __FILE__, __LINE__, #a, #b, A_, B_, diff_);        \
    }                                                            \
  )

/* === Section and summary === */
#define test_section(name) printf("\n== %s ==\n", (name))

#define test_summary(test_result)                                \
  Macro(                                                         \
    printf("Tests: %llu/%llu passed\n",                          \
           (test_result).passed, (test_result).total);           \
  )

/* === Assert helper === */
#define test_assert(expr)                                        \
  Macro(                                                         \
    if (!(expr))                                                 \
    {                                                            \
      fprintf(stderr, "ASSERT FAIL: %s:%d  %s\n",                \
              __FILE__, __LINE__, #expr);                        \
      Breakpoint();                                              \
    }                                                            \
  )

#endif // TEST_H

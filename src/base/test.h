#ifndef TEST_H
#define TEST_H

#include <stdio.h>

typedef struct Test_Result Test_Result;
struct Test_Result
{
  u32 total;
  u32 passed;
};

/* === Initialization === */
#define test_init() (Test_Result){0, 0}

/* === Basic equality === */
#define test_equal(test_result, a, b)                            \
  macro(                                                         \
    (test_result)->total += 1;                                   \
    if ((a) == (b))                                              \
    {                                                            \
      (test_result)->passed += 1;                                \
    }                                                            \
    else                                                         \
    {                                                            \
      fprintf(stderr, "FAIL: %s:%d  %s!= %s  (%lld vs %lld)\n",  \
              __FILE__, __LINE__, stringigy(a), stringigy(b), (s64)(a), (s64)(b));   \
    }                                                            \
  )

#define test_not_equal(test_result, a, b)                        \
  macro(                                                         \
    (test_result)->total += 1;                                   \
    if ((a) != (b))                                              \
    {                                                            \
      (test_result)->passed += 1;                                \
    }                                                            \
    else                                                         \
    {                                                            \
      fprintf(stderr, "FAIL: %s:%d  %s == %s\n",                 \
              __FILE__, __LINE__, stringigy(a), stringigy(b));                       \
    }                                                            \
  )

/* === Boolean checks === */
#define test_true(test_result, expr)  test_equal(test_result, (expr) ? 1 : 0, 1)
#define test_false(test_result, expr) test_equal(test_result, (expr) ? 1 : 0, 0)

/* === Memory comparison === */
#define test_memory_equal(test_result, a, b, size)               \
  macro(                                                         \
    (test_result)->total += 1;                                   \
    if (memory_match((a), (b), (size)))                           \
    {                                                            \
      (test_result)->passed += 1;                                \
    }                                                            \
    else                                                         \
    {                                                            \
      fprintf(stderr, "FAIL: %s:%d  memory mismatch (%s vs %s, %u bytes)\n", \
              __FILE__, __LINE__, stringigy(a), stringigy(b), (u32)(size));          \
    }                                                            \
  )

/* === Numbers === */
#define test_typed_equal(test_result, type, a, b)                \
  macro(                                                         \
    (test_result)->total += 1;                                   \
    type A_ = (a);                                               \
    type B_ = (b);                                               \
    if (A_ == B_)                                                \
    {                                                            \
      (test_result)->passed += 1;                                \
    }                                                            \
    else                                                         \
    {                                                            \
      fprintf(stderr, "FAIL: %s:%d  %s != %s  (" stringigy(type) ": %lld vs %lld)\n", \
              __FILE__, __LINE__, stringigy(a), stringigy(b), (s64)A_, (s64)B_);     \
    }                                                            \
  )

#define test_typed_equal_float(test_result, type, a, b, epsilon) \
  macro(                                                         \
    (test_result)->total += 1;                                   \
    f64 A_ = (f64)(a);                                           \
    f64 B_ = (f64)(b);                                           \
    f64 diff_ = fabs(A_ - B_);                                   \
    if (diff_ <= (epsilon))                                      \
    {                                                            \
      (test_result)->passed += 1;                                \
    }                                                            \
    else                                                         \
    {                                                            \
      fprintf(stderr, "FAIL: %s:%d  %s != %s  (" stringigy(type) ": %f vs %f, diff=%f)\n", \
              __FILE__, __LINE__, stringigy(a), stringigy(b), A_, B_, diff_);        \
    }                                                            \
  )

/* === Section and summary === */
#define test_section(name) printf("\n== %s ==\n", (name))

#define test_summary(test_result)                                \
  macro(                                                         \
    printf("Tests: %u/%u passed\n",                              \
           (test_result)->passed, (test_result)->total);         \
  )

/* === assert helper === */
#define test_assert(expr)                                        \
  macro(                                                         \
    if (!(expr))                                                 \
    {                                                            \
      fprintf(stderr, "ASSERT FAIL: %s:%d  %s\n",                \
              __FILE__, __LINE__, #expr);                        \
      breakpoint();                                              \
    }                                                            \
  )

#endif // TEST_H

#ifndef TEST_H
#define TEST_H

#include <stdio.h>

typedef struct Test_Result Test_Result;
struct Test_Result
{
  u32 total;
  u32 passed;
};

#endif // TEST_H

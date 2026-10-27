#ifndef FZAC_MATH_H
#define FZAC_MATH_H

#define RAYMATH_STATIC_INLINE
#include "Extern/raylib/raymath.h"

typedef struct V2s32 V2s32;
struct V2s32 { s32 x, y; };
#define v2s32(x,y) (V2s32){(x),(y)}

fz_internal V2s32 v2s32_add(V2s32 a, V2s32 b);
fz_internal V2s32 v2s32_sub(V2s32 a, V2s32 b);

fz_internal V2s32
v2s32_add(V2s32 a, V2s32 b)
{
  V2s32 result = { a.x + b.x, a.y + b.y };
  return result;
}

fz_internal V2s32
v2s32_sub(V2s32 a, V2s32 b)
{
  V2s32 result = { a.x - b.x, a.y - b.y };
  return result;
}

#endif // FZAC_MATH_H

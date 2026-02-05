/* Generated code */

#ifndef MATH_GENERATED_H
#define MATH_GENERATED_H

// Vector 2 types
// --------------
typedef struct V2f32 V2f32;
struct V2f32 { f32 x, y; };
#define v2f32(x,y) (V2f32){(x),(y)}
#define v2f32_zero() v2f32(0,0)
function V2f32 v2f32_add(V2f32 a, V2f32 b);
function V2f32 v2f32_sub(V2f32 a, V2f32 b);
function V2f32 v2f32_mul(V2f32 a, V2f32 b);
function V2f32 v2f32_scale(V2f32 v, f32 s);
function V2f32 v2f32_min(V2f32 a, V2f32 b);
function V2f32 v2f32_max(V2f32 a, V2f32 b);
function V2f32 v2f32_clamp(V2f32 v, V2f32 min, V2f32 max);

typedef struct V2u32 V2u32;
struct V2u32 { u32 x, y; };
#define v2u32(x,y) (V2u32){(x),(y)}
#define v2u32_zero() v2u32(0,0)
function V2u32 v2u32_add(V2u32 a, V2u32 b);
function V2u32 v2u32_sub(V2u32 a, V2u32 b);
function V2u32 v2u32_mul(V2u32 a, V2u32 b);
function V2u32 v2u32_scale(V2u32 v, u32 s);
function V2u32 v2u32_min(V2u32 a, V2u32 b);
function V2u32 v2u32_max(V2u32 a, V2u32 b);
function V2u32 v2u32_clamp(V2u32 v, V2u32 min, V2u32 max);

// Vector 3 types
// --------------
typedef struct V3f32 V3f32;
struct V3f32 { f32 x, y, z; };
#define v3f32(x,y,z) (V3f32){(x),(y),(z)}
#define v3f32_zero() v3f32(0,0,0)
function V3f32 v3f32_add(V3f32 a, V3f32 b);
function V3f32 v3f32_sub(V3f32 a, V3f32 b);
function V3f32 v3f32_mul(V3f32 a, V3f32 b);
function V3f32 v3f32_scale(V3f32 v, f32 s);
function V3f32 v3f32_min(V3f32 a, V3f32 b);
function V3f32 v3f32_max(V3f32 a, V3f32 b);
function V3f32 v3f32_clamp(V3f32 v, V3f32 min, V3f32 max);

typedef struct V3u32 V3u32;
struct V3u32 { u32 x, y, z; };
#define v3u32(x,y,z) (V3u32){(x),(y),(z)}
#define v3u32_zero() v3u32(0,0,0)
function V3u32 v3u32_add(V3u32 a, V3u32 b);
function V3u32 v3u32_sub(V3u32 a, V3u32 b);
function V3u32 v3u32_mul(V3u32 a, V3u32 b);
function V3u32 v3u32_scale(V3u32 v, u32 s);
function V3u32 v3u32_min(V3u32 a, V3u32 b);
function V3u32 v3u32_max(V3u32 a, V3u32 b);
function V3u32 v3u32_clamp(V3u32 v, V3u32 min, V3u32 max);

// Vector4 types
// -------------

typedef struct V4f32 V4f32;
struct V4f32 { f32 x, y, z, w; };
#define v4f32(x,y,z,w) (V4f32){(x),(y),(z),(w)}
#define v4f32_zero() v4f32(0,0,0,0)
function V4f32 v4f32_add(V4f32 a, V4f32 b);
function V4f32 v4f32_sub(V4f32 a, V4f32 b);
function V4f32 v4f32_mul(V4f32 a, V4f32 b);
function V4f32 v4f32_scale(V4f32 v, f32 s);
function V4f32 v4f32_min(V4f32 a, V4f32 b);
function V4f32 v4f32_max(V4f32 a, V4f32 b);
function V4f32 v4f32_clamp(V4f32 v, V4f32 min, V4f32 max);

typedef struct V4u32 V4u32;
struct V4u32 { u32 x, y, z, w; };
#define v4u32(x,y,z,w) (V4u32){(x),(y),(z),(w)}
#define v4u32_zero() v4u32(0,0,0,0)
function V4u32 v4u32_add(V4u32 a, V4u32 b);
function V4u32 v4u32_sub(V4u32 a, V4u32 b);
function V4u32 v4u32_mul(V4u32 a, V4u32 b);
function V4u32 v4u32_scale(V4u32 v, u32 s);
function V4u32 v4u32_min(V4u32 a, V4u32 b);
function V4u32 v4u32_max(V4u32 a, V4u32 b);
function V4u32 v4u32_clamp(V4u32 v, V4u32 min, V4u32 max);
#endif // MATH_GENERATED_H
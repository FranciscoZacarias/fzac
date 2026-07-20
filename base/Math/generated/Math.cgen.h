/* Generated code */

#ifndef MATH_GENERATED_H
#define MATH_GENERATED_H


// Vector 2 types
// --------------
typedef struct V2f32 V2f32;
struct V2f32 { f32 x, y; };
#define v2f32(x,y) (V2f32){(x),(y)}
#define v2f32n(n)  (V2f32){(n),(n)}
#define v2f32_zero() v2f32(0,0)
fz_function V2f32 v2f32_add(V2f32 a, V2f32 b);
fz_function V2f32 v2f32_sub(V2f32 a, V2f32 b);
fz_function V2f32 v2f32_mul(V2f32 a, V2f32 b);
fz_function V2f32 v2f32_scale(V2f32 v, f32 s);
fz_function V2f32 v2f32_min(V2f32 a, V2f32 b);
fz_function V2f32 v2f32_max(V2f32 a, V2f32 b);
fz_function V2f32 v2f32_clamp(V2f32 v, V2f32 min, V2f32 max);
fz_function String    v2f32_to_string(Arena* arena, V2f32 v);
typedef struct V2u32 V2u32;
struct V2u32 { u32 x, y; };
#define v2u32(x,y) (V2u32){(x),(y)}
#define v2u32n(n)  (V2u32){(n),(n)}
#define v2u32_zero() v2u32(0,0)
fz_function V2u32 v2u32_add(V2u32 a, V2u32 b);
fz_function V2u32 v2u32_sub(V2u32 a, V2u32 b);
fz_function V2u32 v2u32_mul(V2u32 a, V2u32 b);
fz_function V2u32 v2u32_scale(V2u32 v, u32 s);
fz_function V2u32 v2u32_min(V2u32 a, V2u32 b);
fz_function V2u32 v2u32_max(V2u32 a, V2u32 b);
fz_function V2u32 v2u32_clamp(V2u32 v, V2u32 min, V2u32 max);
fz_function String    v2u32_to_string(Arena* arena, V2u32 v);
typedef struct V2s32 V2s32;
struct V2s32 { s32 x, y; };
#define v2s32(x,y) (V2s32){(x),(y)}
#define v2s32n(n)  (V2s32){(n),(n)}
#define v2s32_zero() v2s32(0,0)
fz_function V2s32 v2s32_add(V2s32 a, V2s32 b);
fz_function V2s32 v2s32_sub(V2s32 a, V2s32 b);
fz_function V2s32 v2s32_mul(V2s32 a, V2s32 b);
fz_function V2s32 v2s32_scale(V2s32 v, s32 s);
fz_function V2s32 v2s32_min(V2s32 a, V2s32 b);
fz_function V2s32 v2s32_max(V2s32 a, V2s32 b);
fz_function V2s32 v2s32_clamp(V2s32 v, V2s32 min, V2s32 max);
fz_function String    v2s32_to_string(Arena* arena, V2s32 v);

// Vector 3 types
// --------------
typedef struct V3f32 V3f32;
struct V3f32 { f32 x, y, z; };
#define v3f32(x,y,z) (V3f32){(x),(y),(z)}
#define v3f32n(n)  (V3f32){(n),(n),(n)}
#define v3f32_zero() v3f32(0,0,0)
fz_function V3f32 v3f32_add(V3f32 a, V3f32 b);
fz_function V3f32 v3f32_sub(V3f32 a, V3f32 b);
fz_function V3f32 v3f32_mul(V3f32 a, V3f32 b);
fz_function V3f32 v3f32_scale(V3f32 v, f32 s);
fz_function V3f32 v3f32_min(V3f32 a, V3f32 b);
fz_function V3f32 v3f32_max(V3f32 a, V3f32 b);
fz_function V3f32 v3f32_clamp(V3f32 v, V3f32 min, V3f32 max);
fz_function String    v3f32_to_string(Arena* arena, V3f32 v);
typedef struct V3u32 V3u32;
struct V3u32 { u32 x, y, z; };
#define v3u32(x,y,z) (V3u32){(x),(y),(z)}
#define v3u32n(n)  (V3u32){(n),(n),(n)}
#define v3u32_zero() v3u32(0,0,0)
fz_function V3u32 v3u32_add(V3u32 a, V3u32 b);
fz_function V3u32 v3u32_sub(V3u32 a, V3u32 b);
fz_function V3u32 v3u32_mul(V3u32 a, V3u32 b);
fz_function V3u32 v3u32_scale(V3u32 v, u32 s);
fz_function V3u32 v3u32_min(V3u32 a, V3u32 b);
fz_function V3u32 v3u32_max(V3u32 a, V3u32 b);
fz_function V3u32 v3u32_clamp(V3u32 v, V3u32 min, V3u32 max);
fz_function String    v3u32_to_string(Arena* arena, V3u32 v);
typedef struct V3s32 V3s32;
struct V3s32 { s32 x, y, z; };
#define v3s32(x,y,z) (V3s32){(x),(y),(z)}
#define v3s32n(n)  (V3s32){(n),(n),(n)}
#define v3s32_zero() v3s32(0,0,0)
fz_function V3s32 v3s32_add(V3s32 a, V3s32 b);
fz_function V3s32 v3s32_sub(V3s32 a, V3s32 b);
fz_function V3s32 v3s32_mul(V3s32 a, V3s32 b);
fz_function V3s32 v3s32_scale(V3s32 v, s32 s);
fz_function V3s32 v3s32_min(V3s32 a, V3s32 b);
fz_function V3s32 v3s32_max(V3s32 a, V3s32 b);
fz_function V3s32 v3s32_clamp(V3s32 v, V3s32 min, V3s32 max);
fz_function String    v3s32_to_string(Arena* arena, V3s32 v);

// Vector4 types
// -------------

typedef struct V4f32 V4f32;
struct V4f32 { f32 x, y, z, w; };
#define v4f32(x,y,z,w) (V4f32){(x),(y),(z),(w)}
#define v4f32n(n)  (V4f32){(n),(n),(n),(n)}
#define v4f32_zero() v4f32(0,0,0,0)
fz_function V4f32 v4f32_add(V4f32 a, V4f32 b);
fz_function V4f32 v4f32_sub(V4f32 a, V4f32 b);
fz_function V4f32 v4f32_mul(V4f32 a, V4f32 b);
fz_function V4f32 v4f32_scale(V4f32 v, f32 s);
fz_function V4f32 v4f32_min(V4f32 a, V4f32 b);
fz_function V4f32 v4f32_max(V4f32 a, V4f32 b);
fz_function V4f32 v4f32_clamp(V4f32 v, V4f32 min, V4f32 max);
fz_function String    v4f32_to_string(Arena* arena, V4f32 v);

typedef struct V4u32 V4u32;
struct V4u32 { u32 x, y, z, w; };
#define v4u32(x,y,z,w) (V4u32){(x),(y),(z),(w)}
#define v4u32n(n)  (V4u32){(n),(n),(n),(n)}
#define v4u32_zero() v4u32(0,0,0,0)
fz_function V4u32 v4u32_add(V4u32 a, V4u32 b);
fz_function V4u32 v4u32_sub(V4u32 a, V4u32 b);
fz_function V4u32 v4u32_mul(V4u32 a, V4u32 b);
fz_function V4u32 v4u32_scale(V4u32 v, u32 s);
fz_function V4u32 v4u32_min(V4u32 a, V4u32 b);
fz_function V4u32 v4u32_max(V4u32 a, V4u32 b);
fz_function V4u32 v4u32_clamp(V4u32 v, V4u32 min, V4u32 max);
fz_function String    v4u32_to_string(Arena* arena, V4u32 v);

typedef struct V4s32 V4s32;
struct V4s32 { s32 x, y, z, w; };
#define v4s32(x,y,z,w) (V4s32){(x),(y),(z),(w)}
#define v4s32n(n)  (V4s32){(n),(n),(n),(n)}
#define v4s32_zero() v4s32(0,0,0,0)
fz_function V4s32 v4s32_add(V4s32 a, V4s32 b);
fz_function V4s32 v4s32_sub(V4s32 a, V4s32 b);
fz_function V4s32 v4s32_mul(V4s32 a, V4s32 b);
fz_function V4s32 v4s32_scale(V4s32 v, s32 s);
fz_function V4s32 v4s32_min(V4s32 a, V4s32 b);
fz_function V4s32 v4s32_max(V4s32 a, V4s32 b);
fz_function V4s32 v4s32_clamp(V4s32 v, V4s32 min, V4s32 max);
fz_function String    v4s32_to_string(Arena* arena, V4s32 v);

// Rect types
// ----------

typedef struct Rectf32 Rectf32;
struct Rectf32 { V2f32 min; V2f32 max; };
#define rectf32(min,max) (Rectf32){(min),(max)}
fz_function Rectf32 rectf32_from_position_size(V2f32 pos, V2f32 size);
fz_function V2f32   rectf32_size(Rectf32 rect);
fz_function V2f32   rectf32_center(Rectf32 rect);
fz_function f32     rectf32_width(Rectf32 rect);
fz_function f32     rectf32_height(Rectf32 rect);
fz_function f32     rectf32_area(Rectf32 rect);
fz_function b32     rectf32_contains_point(Rectf32 rect, V2f32 point);
fz_function b32     rectf32_contains_rect(Rectf32 a, Rectf32 b);
fz_function b32     rectf32_overlaps(Rectf32 a, Rectf32 b);
fz_function Rectf32 rectf32_translate(Rectf32 rect, V2f32 offset);
fz_function Rectf32 rectf32_intersection(Rectf32 a, Rectf32 b);
fz_function Rectf32 rectf32_union(Rectf32 a, Rectf32 b);

typedef struct Rectu32 Rectu32;
struct Rectu32 { V2u32 min; V2u32 max; };
#define rectu32(min,max) (Rectu32){(min),(max)}
fz_function Rectu32 rectu32_from_position_size(V2u32 pos, V2u32 size);
fz_function V2u32   rectu32_size(Rectu32 rect);
fz_function V2u32   rectu32_center(Rectu32 rect);
fz_function u32     rectu32_width(Rectu32 rect);
fz_function u32     rectu32_height(Rectu32 rect);
fz_function u32     rectu32_area(Rectu32 rect);
fz_function b32     rectu32_contains_point(Rectu32 rect, V2u32 point);
fz_function b32     rectu32_contains_rect(Rectu32 a, Rectu32 b);
fz_function b32     rectu32_overlaps(Rectu32 a, Rectu32 b);
fz_function Rectu32 rectu32_translate(Rectu32 rect, V2u32 offset);
fz_function Rectu32 rectu32_intersection(Rectu32 a, Rectu32 b);
fz_function Rectu32 rectu32_union(Rectu32 a, Rectu32 b);

typedef struct Rects32 Rects32;
struct Rects32 { V2s32 min; V2s32 max; };
#define rects32(min,max) (Rects32){(min),(max)}
fz_function Rects32 rects32_from_position_size(V2s32 pos, V2s32 size);
fz_function V2s32   rects32_size(Rects32 rect);
fz_function V2s32   rects32_center(Rects32 rect);
fz_function s32     rects32_width(Rects32 rect);
fz_function s32     rects32_height(Rects32 rect);
fz_function s32     rects32_area(Rects32 rect);
fz_function b32     rects32_contains_point(Rects32 rect, V2s32 point);
fz_function b32     rects32_contains_rect(Rects32 a, Rects32 b);
fz_function b32     rects32_overlaps(Rects32 a, Rects32 b);
fz_function Rects32 rects32_translate(Rects32 rect, V2s32 offset);
fz_function Rects32 rects32_intersection(Rects32 a, Rects32 b);
fz_function Rects32 rects32_union(Rects32 a, Rects32 b);

#endif // MATH_GENERATED_H
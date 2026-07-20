/* Generated code */

// V2 Implementation
fz_function V2f32
v2f32_add(V2f32 a, V2f32 b)
{
  V2f32 result = { a.x + b.x, a.y + b.y };
  return result;
}

fz_function V2f32
v2f32_sub(V2f32 a, V2f32 b)
{
  V2f32 result = { a.x - b.x, a.y - b.y };
  return result;
}

fz_function V2f32
v2f32_mul(V2f32 a, V2f32 b)
{
  V2f32 result = { a.x * b.x, a.y * b.y };
  return result;
}

fz_function V2f32
v2f32_scale(V2f32 v, f32 s)
{
  V2f32 result = { v.x * s, v.y * s };
  return result;
}

fz_function V2f32
v2f32_min(V2f32 a, V2f32 b)
{
  V2f32 result =
  {
    a.x < b.x ? a.x : b.x,
    a.y < b.y ? a.y : b.y
  };
  return result;
}

fz_function V2f32
v2f32_max(V2f32 a, V2f32 b)
{
  V2f32 result =
  {
    a.x > b.x ? a.x : b.x,
    a.y > b.y ? a.y : b.y
  };
  return result;
}

fz_function V2f32
v2f32_clamp(V2f32 v, V2f32 min, V2f32 max)
{
  V2f32 result =
  {
    v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
    v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y)
  };
  return result;
}

fz_function String
v2f32_to_string(Arena *arena, V2f32 v)
{
  String result = Sf(arena, "(%.2f, %.2f)", v.x, v.y);
  return result;
}
  fz_function V2u32
v2u32_add(V2u32 a, V2u32 b)
{
  V2u32 result = { a.x + b.x, a.y + b.y };
  return result;
}

fz_function V2u32
v2u32_sub(V2u32 a, V2u32 b)
{
  V2u32 result = { a.x - b.x, a.y - b.y };
  return result;
}

fz_function V2u32
v2u32_mul(V2u32 a, V2u32 b)
{
  V2u32 result = { a.x * b.x, a.y * b.y };
  return result;
}

fz_function V2u32
v2u32_scale(V2u32 v, u32 s)
{
  V2u32 result = { v.x * s, v.y * s };
  return result;
}

fz_function V2u32
v2u32_min(V2u32 a, V2u32 b)
{
  V2u32 result =
  {
    a.x < b.x ? a.x : b.x,
    a.y < b.y ? a.y : b.y
  };
  return result;
}

fz_function V2u32
v2u32_max(V2u32 a, V2u32 b)
{
  V2u32 result =
  {
    a.x > b.x ? a.x : b.x,
    a.y > b.y ? a.y : b.y
  };
  return result;
}

fz_function V2u32
v2u32_clamp(V2u32 v, V2u32 min, V2u32 max)
{
  V2u32 result =
  {
    v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
    v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y)
  };
  return result;
}

fz_function String
v2u32_to_string(Arena *arena, V2u32 v)
{
  String result = Sf(arena, "(%u, %u)", v.x, v.y);
  return result;
}
  fz_function V2s32
v2s32_add(V2s32 a, V2s32 b)
{
  V2s32 result = { a.x + b.x, a.y + b.y };
  return result;
}

fz_function V2s32
v2s32_sub(V2s32 a, V2s32 b)
{
  V2s32 result = { a.x - b.x, a.y - b.y };
  return result;
}

fz_function V2s32
v2s32_mul(V2s32 a, V2s32 b)
{
  V2s32 result = { a.x * b.x, a.y * b.y };
  return result;
}

fz_function V2s32
v2s32_scale(V2s32 v, s32 s)
{
  V2s32 result = { v.x * s, v.y * s };
  return result;
}

fz_function V2s32
v2s32_min(V2s32 a, V2s32 b)
{
  V2s32 result =
  {
    a.x < b.x ? a.x : b.x,
    a.y < b.y ? a.y : b.y
  };
  return result;
}

fz_function V2s32
v2s32_max(V2s32 a, V2s32 b)
{
  V2s32 result =
  {
    a.x > b.x ? a.x : b.x,
    a.y > b.y ? a.y : b.y
  };
  return result;
}

fz_function V2s32
v2s32_clamp(V2s32 v, V2s32 min, V2s32 max)
{
  V2s32 result =
  {
    v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
    v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y)
  };
  return result;
}

fz_function String
v2s32_to_string(Arena *arena, V2s32 v)
{
  String result = Sf(arena, "(%d, %d)", v.x, v.y);
  return result;
}
  // V3 Implementation
fz_function V3f32
v3f32_add(V3f32 a, V3f32 b)
{
  V3f32 result = { a.x + b.x, a.y + b.y, a.z + b.z };
  return result;
}

fz_function V3f32
v3f32_sub(V3f32 a, V3f32 b)
{
  V3f32 result = { a.x - b.x, a.y - b.y, a.z - b.z };
  return result;
}

fz_function V3f32
v3f32_mul(V3f32 a, V3f32 b)
{
  V3f32 result = { a.x * b.x, a.y * b.y, a.z * b.z };
  return result;
}

fz_function V3f32
v3f32_scale(V3f32 v, f32 s)
{
  V3f32 result = { v.x * s, v.y * s, v.z * s };
  return result;
}

fz_function V3f32
v3f32_min(V3f32 a, V3f32 b)
{
  V3f32 result = { a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z };
  return result;
}

fz_function V3f32
v3f32_max(V3f32 a, V3f32 b)
{
  V3f32 result = { a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z };
  return result;
}

fz_function V3f32
v3f32_clamp(V3f32 v, V3f32 min, V3f32 max)
{
  V3f32 result = { v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
                       v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y),
                       v.z < min.z ? min.z : (v.z > max.z ? max.z : v.z) };
  return result;
}


fz_function String
v3f32_to_string(Arena *arena, V3f32 v)
{
  String result = Sf(arena, "(%.2f, %.2f, %.2f)", v.x, v.y, v.z);
  return result;
}

fz_function V3u32
v3u32_add(V3u32 a, V3u32 b)
{
  V3u32 result = { a.x + b.x, a.y + b.y, a.z + b.z };
  return result;
}

fz_function V3u32
v3u32_sub(V3u32 a, V3u32 b)
{
  V3u32 result = { a.x - b.x, a.y - b.y, a.z - b.z };
  return result;
}

fz_function V3u32
v3u32_mul(V3u32 a, V3u32 b)
{
  V3u32 result = { a.x * b.x, a.y * b.y, a.z * b.z };
  return result;
}

fz_function V3u32
v3u32_scale(V3u32 v, u32 s)
{
  V3u32 result = { v.x * s, v.y * s, v.z * s };
  return result;
}

fz_function V3u32
v3u32_min(V3u32 a, V3u32 b)
{
  V3u32 result = { a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z };
  return result;
}

fz_function V3u32
v3u32_max(V3u32 a, V3u32 b)
{
  V3u32 result = { a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z };
  return result;
}

fz_function V3u32
v3u32_clamp(V3u32 v, V3u32 min, V3u32 max)
{
  V3u32 result = { v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
                       v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y),
                       v.z < min.z ? min.z : (v.z > max.z ? max.z : v.z) };
  return result;
}


fz_function String
v3u32_to_string(Arena *arena, V3u32 v)
{
  String result = Sf(arena, "(%u, %u, %u)", v.x, v.y, v.z);
  return result;
}

fz_function V3s32
v3s32_add(V3s32 a, V3s32 b)
{
  V3s32 result = { a.x + b.x, a.y + b.y, a.z + b.z };
  return result;
}

fz_function V3s32
v3s32_sub(V3s32 a, V3s32 b)
{
  V3s32 result = { a.x - b.x, a.y - b.y, a.z - b.z };
  return result;
}

fz_function V3s32
v3s32_mul(V3s32 a, V3s32 b)
{
  V3s32 result = { a.x * b.x, a.y * b.y, a.z * b.z };
  return result;
}

fz_function V3s32
v3s32_scale(V3s32 v, s32 s)
{
  V3s32 result = { v.x * s, v.y * s, v.z * s };
  return result;
}

fz_function V3s32
v3s32_min(V3s32 a, V3s32 b)
{
  V3s32 result = { a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z };
  return result;
}

fz_function V3s32
v3s32_max(V3s32 a, V3s32 b)
{
  V3s32 result = { a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z };
  return result;
}

fz_function V3s32
v3s32_clamp(V3s32 v, V3s32 min, V3s32 max)
{
  V3s32 result = { v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
                       v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y),
                       v.z < min.z ? min.z : (v.z > max.z ? max.z : v.z) };
  return result;
}


fz_function String
v3s32_to_string(Arena *arena, V3s32 v)
{
  String result = Sf(arena, "(%d, %d, %d)", v.x, v.y, v.z);
  return result;
}

// V4 Implementation
fz_function V4f32
v4f32_add(V4f32 a, V4f32 b)
{
  V4f32 result = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
  return result;
}

fz_function V4f32
v4f32_sub(V4f32 a, V4f32 b)
{
  V4f32 result = { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
  return result;
}

fz_function V4f32
v4f32_mul(V4f32 a, V4f32 b)
{
  V4f32 result = { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
  return result;
}

fz_function V4f32
v4f32_scale(V4f32 v, f32 s)
{
  V4f32 result = { v.x * s, v.y * s, v.z * s, v.w * s };
  return result;
}

fz_function V4f32
v4f32_min(V4f32 a, V4f32 b)
{
  V4f32 result = { a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z,
                       a.w < b.w ? a.w : b.w };
  return result;
}

fz_function V4f32
v4f32_max(V4f32 a, V4f32 b)
{
  V4f32 result = { a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z,
                       a.w > b.w ? a.w : b.w };
  return result;
}

fz_function V4f32
v4f32_clamp(V4f32 v, V4f32 min, V4f32 max)
{
  V4f32 result = { v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
                       v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y),
                       v.z < min.z ? min.z : (v.z > max.z ? max.z : v.z),
                       v.w < min.w ? min.w : (v.w > max.w ? max.w : v.w) };
  return result;
}

fz_function String
v4f32_to_string(Arena *arena, V4f32 v)
{
  String result = Sf(arena, "(%.2f, %.2f, %.2f, %.2f)", v.x, v.y, v.z, v.w);
  return result;
}


fz_function V4u32
v4u32_add(V4u32 a, V4u32 b)
{
  V4u32 result = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
  return result;
}

fz_function V4u32
v4u32_sub(V4u32 a, V4u32 b)
{
  V4u32 result = { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
  return result;
}

fz_function V4u32
v4u32_mul(V4u32 a, V4u32 b)
{
  V4u32 result = { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
  return result;
}

fz_function V4u32
v4u32_scale(V4u32 v, u32 s)
{
  V4u32 result = { v.x * s, v.y * s, v.z * s, v.w * s };
  return result;
}

fz_function V4u32
v4u32_min(V4u32 a, V4u32 b)
{
  V4u32 result = { a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z,
                       a.w < b.w ? a.w : b.w };
  return result;
}

fz_function V4u32
v4u32_max(V4u32 a, V4u32 b)
{
  V4u32 result = { a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z,
                       a.w > b.w ? a.w : b.w };
  return result;
}

fz_function V4u32
v4u32_clamp(V4u32 v, V4u32 min, V4u32 max)
{
  V4u32 result = { v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
                       v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y),
                       v.z < min.z ? min.z : (v.z > max.z ? max.z : v.z),
                       v.w < min.w ? min.w : (v.w > max.w ? max.w : v.w) };
  return result;
}

fz_function String
v4u32_to_string(Arena *arena, V4u32 v)
{
  String result = Sf(arena, "(%u, %u, %u, %u)", v.x, v.y, v.z, v.w);
  return result;
}


fz_function V4s32
v4s32_add(V4s32 a, V4s32 b)
{
  V4s32 result = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
  return result;
}

fz_function V4s32
v4s32_sub(V4s32 a, V4s32 b)
{
  V4s32 result = { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
  return result;
}

fz_function V4s32
v4s32_mul(V4s32 a, V4s32 b)
{
  V4s32 result = { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
  return result;
}

fz_function V4s32
v4s32_scale(V4s32 v, s32 s)
{
  V4s32 result = { v.x * s, v.y * s, v.z * s, v.w * s };
  return result;
}

fz_function V4s32
v4s32_min(V4s32 a, V4s32 b)
{
  V4s32 result = { a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z,
                       a.w < b.w ? a.w : b.w };
  return result;
}

fz_function V4s32
v4s32_max(V4s32 a, V4s32 b)
{
  V4s32 result = { a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z,
                       a.w > b.w ? a.w : b.w };
  return result;
}

fz_function V4s32
v4s32_clamp(V4s32 v, V4s32 min, V4s32 max)
{
  V4s32 result = { v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
                       v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y),
                       v.z < min.z ? min.z : (v.z > max.z ? max.z : v.z),
                       v.w < min.w ? min.w : (v.w > max.w ? max.w : v.w) };
  return result;
}

fz_function String
v4s32_to_string(Arena *arena, V4s32 v)
{
  String result = Sf(arena, "(%d, %d, %d, %d)", v.x, v.y, v.z, v.w);
  return result;
}


// Rect Implementation

fz_function Rectf32
rectf32_from_position_size(V2f32 position, V2f32 size)
{
  Rectf32 result = rectf32(position, v2f32_add(position, size));
  return result;
}

fz_function V2f32
rectf32_size(Rectf32 rect)
{
  V2f32 result = v2f32_sub(rect.max, rect.min);
  return result;
}

fz_function V2f32
rectf32_center(Rectf32 rect)
{
  V2f32 size = rectf32_size(rect);
  V2f32 result = v2f32(rect.min.x + size.x / 2, rect.min.y + size.y / 2);
  return result;
}

fz_function f32
rectf32_width(Rectf32 rect)
{
  f32 result = rect.max.x - rect.min.x;
  return result;
}

fz_function f32
rectf32_height(Rectf32 rect)
{
  f32 result = rect.max.y - rect.min.y;
  return result;
}

fz_function f32
rectf32_area(Rectf32 rect)
{
  f32 result = rectf32_width(rect) * rectf32_height(rect);
  return result;
}

fz_function b32
rectf32_contains_point(Rectf32 rect, V2f32 point)
{
  b32 result = point.x >= rect.min.x &&
               point.x <  rect.max.x &&
               point.y >= rect.min.y &&
               point.y <  rect.max.y;
  return result;
}

fz_function b32
rectf32_contains_rect(Rectf32 a, Rectf32 b)
{
  b32 result = b.min.x >= a.min.x &&
               b.min.y >= a.min.y &&
               b.max.x <= a.max.x &&
               b.max.y <= a.max.y;
  return result;
}

fz_function b32
rectf32_overlaps(Rectf32 a, Rectf32 b)
{
  b32 result = a.min.x < b.max.x &&
               a.max.x > b.min.x &&
               a.min.y < b.max.y &&
               a.max.y > b.min.y;
  return result;
}

fz_function Rectf32
rectf32_translate(Rectf32 rect, V2f32 offset)
{
  Rectf32 result = rectf32(v2f32_add(rect.min, offset), v2f32_add(rect.max, offset));
  return result;
}

fz_function Rectf32
rectf32_intersection(Rectf32 a, Rectf32 b)
{
  Rectf32 result;
  result.min = v2f32_max(a.min, b.min);
  result.max = v2f32_min(a.max, b.max);
  if(result.min.x > result.max.x || result.min.y > result.max.y)
  {
    result.min = result.max;
  }
  return result;
}

fz_function Rectf32
rectf32_union(Rectf32 a, Rectf32 b)
{
  Rectf32 result;
  result.min = v2f32_min(a.min, b.min);
  result.max = v2f32_max(a.max, b.max);
  return result;
}

fz_function Rectu32
rectu32_from_position_size(V2u32 position, V2u32 size)
{
  Rectu32 result = rectu32(position, v2u32_add(position, size));
  return result;
}

fz_function V2u32
rectu32_size(Rectu32 rect)
{
  V2u32 result = v2u32_sub(rect.max, rect.min);
  return result;
}

fz_function V2u32
rectu32_center(Rectu32 rect)
{
  V2u32 size = rectu32_size(rect);
  V2u32 result = v2u32(rect.min.x + size.x / 2, rect.min.y + size.y / 2);
  return result;
}

fz_function u32
rectu32_width(Rectu32 rect)
{
  u32 result = rect.max.x - rect.min.x;
  return result;
}

fz_function u32
rectu32_height(Rectu32 rect)
{
  u32 result = rect.max.y - rect.min.y;
  return result;
}

fz_function u32
rectu32_area(Rectu32 rect)
{
  u32 result = rectu32_width(rect) * rectu32_height(rect);
  return result;
}

fz_function b32
rectu32_contains_point(Rectu32 rect, V2u32 point)
{
  b32 result = point.x >= rect.min.x &&
               point.x <  rect.max.x &&
               point.y >= rect.min.y &&
               point.y <  rect.max.y;
  return result;
}

fz_function b32
rectu32_contains_rect(Rectu32 a, Rectu32 b)
{
  b32 result = b.min.x >= a.min.x &&
               b.min.y >= a.min.y &&
               b.max.x <= a.max.x &&
               b.max.y <= a.max.y;
  return result;
}

fz_function b32
rectu32_overlaps(Rectu32 a, Rectu32 b)
{
  b32 result = a.min.x < b.max.x &&
               a.max.x > b.min.x &&
               a.min.y < b.max.y &&
               a.max.y > b.min.y;
  return result;
}

fz_function Rectu32
rectu32_translate(Rectu32 rect, V2u32 offset)
{
  Rectu32 result = rectu32(v2u32_add(rect.min, offset), v2u32_add(rect.max, offset));
  return result;
}

fz_function Rectu32
rectu32_intersection(Rectu32 a, Rectu32 b)
{
  Rectu32 result;
  result.min = v2u32_max(a.min, b.min);
  result.max = v2u32_min(a.max, b.max);
  if(result.min.x > result.max.x || result.min.y > result.max.y)
  {
    result.min = result.max;
  }
  return result;
}

fz_function Rectu32
rectu32_union(Rectu32 a, Rectu32 b)
{
  Rectu32 result;
  result.min = v2u32_min(a.min, b.min);
  result.max = v2u32_max(a.max, b.max);
  return result;
}

fz_function Rects32
rects32_from_position_size(V2s32 position, V2s32 size)
{
  Rects32 result = rects32(position, v2s32_add(position, size));
  return result;
}

fz_function V2s32
rects32_size(Rects32 rect)
{
  V2s32 result = v2s32_sub(rect.max, rect.min);
  return result;
}

fz_function V2s32
rects32_center(Rects32 rect)
{
  V2s32 size = rects32_size(rect);
  V2s32 result = v2s32(rect.min.x + size.x / 2, rect.min.y + size.y / 2);
  return result;
}

fz_function s32
rects32_width(Rects32 rect)
{
  s32 result = rect.max.x - rect.min.x;
  return result;
}

fz_function s32
rects32_height(Rects32 rect)
{
  s32 result = rect.max.y - rect.min.y;
  return result;
}

fz_function s32
rects32_area(Rects32 rect)
{
  s32 result = rects32_width(rect) * rects32_height(rect);
  return result;
}

fz_function b32
rects32_contains_point(Rects32 rect, V2s32 point)
{
  b32 result = point.x >= rect.min.x &&
               point.x <  rect.max.x &&
               point.y >= rect.min.y &&
               point.y <  rect.max.y;
  return result;
}

fz_function b32
rects32_contains_rect(Rects32 a, Rects32 b)
{
  b32 result = b.min.x >= a.min.x &&
               b.min.y >= a.min.y &&
               b.max.x <= a.max.x &&
               b.max.y <= a.max.y;
  return result;
}

fz_function b32
rects32_overlaps(Rects32 a, Rects32 b)
{
  b32 result = a.min.x < b.max.x &&
               a.max.x > b.min.x &&
               a.min.y < b.max.y &&
               a.max.y > b.min.y;
  return result;
}

fz_function Rects32
rects32_translate(Rects32 rect, V2s32 offset)
{
  Rects32 result = rects32(v2s32_add(rect.min, offset), v2s32_add(rect.max, offset));
  return result;
}

fz_function Rects32
rects32_intersection(Rects32 a, Rects32 b)
{
  Rects32 result;
  result.min = v2s32_max(a.min, b.min);
  result.max = v2s32_min(a.max, b.max);
  if(result.min.x > result.max.x || result.min.y > result.max.y)
  {
    result.min = result.max;
  }
  return result;
}

fz_function Rects32
rects32_union(Rects32 a, Rects32 b)
{
  Rects32 result;
  result.min = v2s32_min(a.min, b.min);
  result.max = v2s32_max(a.max, b.max);
  return result;
}

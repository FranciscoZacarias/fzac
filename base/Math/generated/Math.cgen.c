/* Generated code */

// V2 Implementation
function V2f32
v2f32_add(V2f32 a, V2f32 b)
{
  V2f32 result = { a.x + b.x, a.y + b.y };
  return result;
}

function V2f32
v2f32_sub(V2f32 a, V2f32 b)
{
  V2f32 result = { a.x - b.x, a.y - b.y };
  return result;
}

function V2f32
v2f32_mul(V2f32 a, V2f32 b)
{
  V2f32 result = { a.x * b.x, a.y * b.y };
  return result;
}

function V2f32
v2f32_scale(V2f32 v, f32 s)
{
  V2f32 result = { v.x * s, v.y * s };
  return result;
}

function V2f32
v2f32_min(V2f32 a, V2f32 b)
{
  V2f32 result =
  {
    a.x < b.x ? a.x : b.x,
    a.y < b.y ? a.y : b.y
  };
  return result;
}

function V2f32
v2f32_max(V2f32 a, V2f32 b)
{
  V2f32 result =
  {
    a.x > b.x ? a.x : b.x,
    a.y > b.y ? a.y : b.y
  };
  return result;
}

function V2f32
v2f32_clamp(V2f32 v, V2f32 min, V2f32 max)
{
  V2f32 result =
  {
    v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
    v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y)
  };
  return result;
}

function V2u32
v2u32_add(V2u32 a, V2u32 b)
{
  V2u32 result = { a.x + b.x, a.y + b.y };
  return result;
}

function V2u32
v2u32_sub(V2u32 a, V2u32 b)
{
  V2u32 result = { a.x - b.x, a.y - b.y };
  return result;
}

function V2u32
v2u32_mul(V2u32 a, V2u32 b)
{
  V2u32 result = { a.x * b.x, a.y * b.y };
  return result;
}

function V2u32
v2u32_scale(V2u32 v, u32 s)
{
  V2u32 result = { v.x * s, v.y * s };
  return result;
}

function V2u32
v2u32_min(V2u32 a, V2u32 b)
{
  V2u32 result =
  {
    a.x < b.x ? a.x : b.x,
    a.y < b.y ? a.y : b.y
  };
  return result;
}

function V2u32
v2u32_max(V2u32 a, V2u32 b)
{
  V2u32 result =
  {
    a.x > b.x ? a.x : b.x,
    a.y > b.y ? a.y : b.y
  };
  return result;
}

function V2u32
v2u32_clamp(V2u32 v, V2u32 min, V2u32 max)
{
  V2u32 result =
  {
    v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
    v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y)
  };
  return result;
}

function V2s32
v2s32_add(V2s32 a, V2s32 b)
{
  V2s32 result = { a.x + b.x, a.y + b.y };
  return result;
}

function V2s32
v2s32_sub(V2s32 a, V2s32 b)
{
  V2s32 result = { a.x - b.x, a.y - b.y };
  return result;
}

function V2s32
v2s32_mul(V2s32 a, V2s32 b)
{
  V2s32 result = { a.x * b.x, a.y * b.y };
  return result;
}

function V2s32
v2s32_scale(V2s32 v, s32 s)
{
  V2s32 result = { v.x * s, v.y * s };
  return result;
}

function V2s32
v2s32_min(V2s32 a, V2s32 b)
{
  V2s32 result =
  {
    a.x < b.x ? a.x : b.x,
    a.y < b.y ? a.y : b.y
  };
  return result;
}

function V2s32
v2s32_max(V2s32 a, V2s32 b)
{
  V2s32 result =
  {
    a.x > b.x ? a.x : b.x,
    a.y > b.y ? a.y : b.y
  };
  return result;
}

function V2s32
v2s32_clamp(V2s32 v, V2s32 min, V2s32 max)
{
  V2s32 result =
  {
    v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
    v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y)
  };
  return result;
}

// V3 Implementation
function V3f32
v3f32_add(V3f32 a, V3f32 b)
{
  V3f32 result = { a.x + b.x, a.y + b.y, a.z + b.z };
  return result;
}

function V3f32
v3f32_sub(V3f32 a, V3f32 b)
{
  V3f32 result = { a.x - b.x, a.y - b.y, a.z - b.z };
  return result;
}

function V3f32
v3f32_mul(V3f32 a, V3f32 b)
{
  V3f32 result = { a.x * b.x, a.y * b.y, a.z * b.z };
  return result;
}

function V3f32
v3f32_scale(V3f32 v, f32 s)
{
  V3f32 result = { v.x * s, v.y * s, v.z * s };
  return result;
}

function V3f32
v3f32_min(V3f32 a, V3f32 b)
{
  V3f32 result = { a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z };
  return result;
}

function V3f32
v3f32_max(V3f32 a, V3f32 b)
{
  V3f32 result = { a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z };
  return result;
}

function V3f32
v3f32_clamp(V3f32 v, V3f32 min, V3f32 max)
{
  V3f32 result = { v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
                       v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y),
                       v.z < min.z ? min.z : (v.z > max.z ? max.z : v.z) };
  return result;
}

function V3u32
v3u32_add(V3u32 a, V3u32 b)
{
  V3u32 result = { a.x + b.x, a.y + b.y, a.z + b.z };
  return result;
}

function V3u32
v3u32_sub(V3u32 a, V3u32 b)
{
  V3u32 result = { a.x - b.x, a.y - b.y, a.z - b.z };
  return result;
}

function V3u32
v3u32_mul(V3u32 a, V3u32 b)
{
  V3u32 result = { a.x * b.x, a.y * b.y, a.z * b.z };
  return result;
}

function V3u32
v3u32_scale(V3u32 v, u32 s)
{
  V3u32 result = { v.x * s, v.y * s, v.z * s };
  return result;
}

function V3u32
v3u32_min(V3u32 a, V3u32 b)
{
  V3u32 result = { a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z };
  return result;
}

function V3u32
v3u32_max(V3u32 a, V3u32 b)
{
  V3u32 result = { a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z };
  return result;
}

function V3u32
v3u32_clamp(V3u32 v, V3u32 min, V3u32 max)
{
  V3u32 result = { v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
                       v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y),
                       v.z < min.z ? min.z : (v.z > max.z ? max.z : v.z) };
  return result;
}

function V3s32
v3s32_add(V3s32 a, V3s32 b)
{
  V3s32 result = { a.x + b.x, a.y + b.y, a.z + b.z };
  return result;
}

function V3s32
v3s32_sub(V3s32 a, V3s32 b)
{
  V3s32 result = { a.x - b.x, a.y - b.y, a.z - b.z };
  return result;
}

function V3s32
v3s32_mul(V3s32 a, V3s32 b)
{
  V3s32 result = { a.x * b.x, a.y * b.y, a.z * b.z };
  return result;
}

function V3s32
v3s32_scale(V3s32 v, s32 s)
{
  V3s32 result = { v.x * s, v.y * s, v.z * s };
  return result;
}

function V3s32
v3s32_min(V3s32 a, V3s32 b)
{
  V3s32 result = { a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z };
  return result;
}

function V3s32
v3s32_max(V3s32 a, V3s32 b)
{
  V3s32 result = { a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z };
  return result;
}

function V3s32
v3s32_clamp(V3s32 v, V3s32 min, V3s32 max)
{
  V3s32 result = { v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
                       v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y),
                       v.z < min.z ? min.z : (v.z > max.z ? max.z : v.z) };
  return result;
}

// V4 Implementation
function V4f32
v4f32_add(V4f32 a, V4f32 b)
{
  V4f32 result = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
  return result;
}

function V4f32
v4f32_sub(V4f32 a, V4f32 b)
{
  V4f32 result = { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
  return result;
}

function V4f32
v4f32_mul(V4f32 a, V4f32 b)
{
  V4f32 result = { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
  return result;
}

function V4f32
v4f32_scale(V4f32 v, f32 s)
{
  V4f32 result = { v.x * s, v.y * s, v.z * s, v.w * s };
  return result;
}

function V4f32
v4f32_min(V4f32 a, V4f32 b)
{
  V4f32 result = { a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z,
                       a.w < b.w ? a.w : b.w };
  return result;
}

function V4f32
v4f32_max(V4f32 a, V4f32 b)
{
  V4f32 result = { a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z,
                       a.w > b.w ? a.w : b.w };
  return result;
}

function V4f32
v4f32_clamp(V4f32 v, V4f32 min, V4f32 max)
{
  V4f32 result = { v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
                       v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y),
                       v.z < min.z ? min.z : (v.z > max.z ? max.z : v.z),
                       v.w < min.w ? min.w : (v.w > max.w ? max.w : v.w) };
  return result;
}
function V4u32
v4u32_add(V4u32 a, V4u32 b)
{
  V4u32 result = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
  return result;
}

function V4u32
v4u32_sub(V4u32 a, V4u32 b)
{
  V4u32 result = { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
  return result;
}

function V4u32
v4u32_mul(V4u32 a, V4u32 b)
{
  V4u32 result = { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
  return result;
}

function V4u32
v4u32_scale(V4u32 v, u32 s)
{
  V4u32 result = { v.x * s, v.y * s, v.z * s, v.w * s };
  return result;
}

function V4u32
v4u32_min(V4u32 a, V4u32 b)
{
  V4u32 result = { a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z,
                       a.w < b.w ? a.w : b.w };
  return result;
}

function V4u32
v4u32_max(V4u32 a, V4u32 b)
{
  V4u32 result = { a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z,
                       a.w > b.w ? a.w : b.w };
  return result;
}

function V4u32
v4u32_clamp(V4u32 v, V4u32 min, V4u32 max)
{
  V4u32 result = { v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
                       v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y),
                       v.z < min.z ? min.z : (v.z > max.z ? max.z : v.z),
                       v.w < min.w ? min.w : (v.w > max.w ? max.w : v.w) };
  return result;
}
function V4s32
v4s32_add(V4s32 a, V4s32 b)
{
  V4s32 result = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
  return result;
}

function V4s32
v4s32_sub(V4s32 a, V4s32 b)
{
  V4s32 result = { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
  return result;
}

function V4s32
v4s32_mul(V4s32 a, V4s32 b)
{
  V4s32 result = { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
  return result;
}

function V4s32
v4s32_scale(V4s32 v, s32 s)
{
  V4s32 result = { v.x * s, v.y * s, v.z * s, v.w * s };
  return result;
}

function V4s32
v4s32_min(V4s32 a, V4s32 b)
{
  V4s32 result = { a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z,
                       a.w < b.w ? a.w : b.w };
  return result;
}

function V4s32
v4s32_max(V4s32 a, V4s32 b)
{
  V4s32 result = { a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z,
                       a.w > b.w ? a.w : b.w };
  return result;
}

function V4s32
v4s32_clamp(V4s32 v, V4s32 min, V4s32 max)
{
  V4s32 result = { v.x < min.x ? min.x : (v.x > max.x ? max.x : v.x),
                       v.y < min.y ? min.y : (v.y > max.y ? max.y : v.y),
                       v.z < min.z ? min.z : (v.z > max.z ? max.z : v.z),
                       v.w < min.w ? min.w : (v.w > max.w ? max.w : v.w) };
  return result;
}

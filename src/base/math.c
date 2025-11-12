/* ----- V2f32 functions ----- */
function V2f32
v2f32_add(V2f32 a, V2f32 b)
{
  V2f32 result = {a.x + b.x, a.y + b.y};
  return result;
}
function V2f32
v2f32_sub(V2f32 a, V2f32 b)
{
  V2f32 result = {a.x - b.x, a.y - b.y};
  return result;
}

function V2f32
v2f32_mul(V2f32 a, V2f32 b)
{
  V2f32 result = { a.x*b.x, a.y*b.y };
  return result;
}

function V2f32
v2f32_div(V2f32 a, V2f32 b)
{
  V2f32 result = { a.x/b.x, a.y/b.y };
  return result;
}

function f32
v2f32_dot(V2f32 a, V2f32 b)
{
  f32 result = (a.x * b.x + a.y * b.y);
  return result;
}

function V2f32
v2f32_normalize(V2f32 v)
{
  V2f32 result = { 0 };
  f32 length = sqrtf((v.x*v.x) + (v.y*v.y));
  if (length > 0)
  {
    f32 ilength = 1.0f / length;
    result.x = v.x * ilength;
    result.y = v.y * ilength;
  }
  return result;  
}

function V2f32
v2f32_transform(V2f32 v, M4f32 m)
{
  V2f32 result = { 0 };
  f32 x = v.x;
  f32 y = v.y;
  f32 z = 0;
  result.x = (m.m0 * x) + (m.m4 * y) + (m.m8 * z) + m.m12;
  result.y = (m.m1 * x) + (m.m5 * y) + (m.m9 * z) + m.m13;
  return result;
}

function f32
v2f32_len(V2f32 v)
{
  f32 result = sqrtf((v.x * v.x) + (v.y * v.y));
  return result;
}

function f32
v2f32_len_sqr(V2f32 v)
{
  f32 result = (v.x * v.x) + (v.y * v.y);
  return result;
}

function f32
v2f32_cross(V2f32 a, V2f32 b)
{
  f32 result = (a.x * b.y - a.y * b.x);
  return result;
}

function V2f32
v2f32_scale(V2f32 v, f32 scale)
{
  V2f32 result = {v.x * scale, v.y * scale};
  return result;
}

function V2f32
v2f32_lerp(V2f32 a, V2f32 b, f32 step)
{
  V2f32 result = { 0 };

  result.x = a.x + (step * (b.x - a.x));
  result.y = a.y + (step * (b.y - a.y));

  return result;
}

function f32
v2f32_dist(V2f32 a, V2f32 b)
{
  f32 result = sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  return result;
}

function f32
v2f32_dist_sqr(V2f32 a, V2f32 b)
{
  f32 result = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
  return result;
}

function f32
v2f32_angle(V2f32 a, V2f32 b)
{
  f32 dot   = (a.x * b.x + a.y * b.y);
  f32 cross = (a.x * b.y - a.y * b.x);
  f32 angle = atan2f(cross, dot);
  return angle;
}

function V2f32
v2f32_reflect(V2f32 v, V2f32 normal)
{
  V2f32 result = { 0 };
  f32 dot  = (v.x * normal.x) + (v.y * normal.y);
  result.x = v.x - (2.0f * normal.x) * dot;
  result.y = v.y - (2.0f * normal.y) * dot;
  return result;
}

function V2f32
v2f32_rotate(V2f32 v, f32 angle)
{
  V2f32 result = { 0 };
  f32 cosres = cosf(angle);
  f32 sinres = sinf(angle);
  result.x = (v.x * cosres) - (v.y * sinres);
  result.y = (v.x * sinres) + (v.y * cosres);
  return result;
}

function b32
v2f32_equals(V2f32 a, V2f32 b)
{
  b32 result = ((fabsf(a.x - b.x)) <= (Epsilon32*fmaxf(1.0f, fmaxf(fabsf(a.x), fabsf(b.x))))) &&
               ((fabsf(a.y - b.y)) <= (Epsilon32*fmaxf(1.0f, fmaxf(fabsf(a.y), fabsf(b.y)))));

  return result;
}

function V2f32
v2f32_refract(V2f32 incident, V2f32 surface_normal, f32 refraction_ratio)
{
  V2f32 result = { 0 };
  f32 dot = (incident.x * surface_normal.x) + (incident.y * surface_normal.y);
  f32 d = 1.0f - (refraction_ratio * refraction_ratio * (1.0f - dot * dot));
  if (d >= 0.0f)
  {
    d = sqrtf(d);
    incident.x = refraction_ratio * incident.x - (refraction_ratio * dot + d) * surface_normal.x;
    incident.y = refraction_ratio * incident.y - (refraction_ratio * dot + d) * surface_normal.y;
    result = incident;
  }
  return result;
}

/* ----- V3f32 functions ----- */
# pragma warning(push)
# pragma warning(disable: 4100) // Avoids known warning from winnls.h

function V3f32
v3f32_add(V3f32 a, V3f32 b)
{
  return v3f32_zero();
}

function V3f32
v3f32_sub(V3f32 a, V3f32 b)
{
  return v3f32_zero();
}

function V3f32
v3f32_scale(V3f32 v, f32 scalar)
{
  return v3f32_zero();
}

function V3f32
v3f32_mul(V3f32 a, V3f32 b)
{
  return v3f32_zero();
}

function V3f32
v3f32_cross(V3f32 a, V3f32 b)
{
  return v3f32_zero();
}

function V3f32
v3f32_perpendicular(V3f32 v)
{
  return v3f32_zero();
}

function f32
v3f32_length(V3f32 v)
{
  return 0;
}

function f32
v3f32_length_sqr(V3f32 v)
{
  return 0;
}

function f32
v3f32_dot(V3f32 a, V3f32 b)
{
  return 0;
}

function f32
v3f32_distance(V3f32 a, V3f32 b)
{
  return 0;
}

function f32
v3f32_distanceSqr(V3f32 a, V3f32 b)
{
  return 0;
}

function f32
v3f32_angle(V3f32 a, V3f32 b)
{
  return 0;
}

function V3f32
v3f32_div(V3f32 a, V3f32 b)
{
  return v3f32_zero();
}

function V3f32
v3f32_normalize(V3f32 v)
{
  return v3f32_zero();
}

function void
v3f32_ortho_normalize(V3f32* a, V3f32* b)
{

}

function V3f32
v3f32_transform(V3f32 v, M4f32 mat)
{
  return v3f32_zero();
}

function V3f32
v3f32_rotate_by_quaternion(V3f32 v, Qf32 q)
{
  return v3f32_zero();
}

function V3f32
v3f32_rotate_by_axis_angle(V3f32 v, V3f32 axis, f32 angle)
{
  return v3f32_zero();
}

function V3f32
v3f32_lerp(V3f32 a, V3f32 b, f32 amount)
{
  return v3f32_zero();
}

function V3f32
v3f32_reflect(V3f32 v, V3f32 normal)
{
  return v3f32_zero();
}

function V3f32
v3f32_barycenter(V3f32 p, V3f32 a, V3f32 b, V3f32 c)
{
  return v3f32_zero();
}

function V3f32
v3f32_unproject(V3f32 source, M4f32 projection, M4f32 view)
{
  return v3f32_zero();
}

function V3f32
v3f32_invert(V3f32 v)
{
  return v3f32_zero();
}

function b32
v3f32_equals(V3f32 p, V3f32 q)
{
  return false;
}

function V3f32
v3f32_refract(V3f32 v, V3f32 n, f32 r)
{
  return v3f32_zero();
}

# pragma warning(pop)

#ifndef MATH_H
#define MATH_H

// @Section: Constants
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

// @Section: Utils
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

// @Section: Math types

typedef struct Vector2    Vector2;    /* */
typedef struct Vector3    Vector3;    /* */
typedef struct Vector4    Vector4;    /* */
typedef struct Matrix4    Matrix4;    /* Matrix type (OpenGL style 4x4 - right handed, column major) */
typedef struct Quaternion Quaternion; /* */

// @Section: Vector2
struct Vector2 { f32 x, y; };
function Vector2 vector2_make(f32 x, f32 y); /* Creates a Vector 2 */
function Vector2 vector2_make_zero(f32 x, f32 y); /* Creates a zero initialized Vector 2 */
function Vector2 vector2_add(Vector2 v1, Vector2 v2); /* Adds two 2D vectors component-wise */
function Vector2 vector2_sub(Vector2 v1, Vector2 v2); /* Subtracts the second 2D vector from the first component-wise */
function f32     vector2_length(Vector2 v); /* Returns the Euclidean length (magnitude) of a 2D vector */
function f32     vector2_length_squared(Vector2 v); /* Returns the squared length of a 2D vector (avoids sqrt) */
function f32     vector2_dot(Vector2 v1, Vector2 v2); /* Computes the dot product of two 2D vectors */
function f32     vector2_cross(Vector2 v1, Vector2 v2); /* Computes the scalar 2D cross product (v1.x*v2.y - v1.y*v2.x) */
function f32     vector2_distance(Vector2 v1, Vector2 v2); /* Returns the distance between two 2D points */
function f32     vector2_distance_squared(Vector2 v1, Vector2 v2); /* Returns the squared distance between two 2D points */
function Vector2 vector2_scale(Vector2 v, f32 scale); /* Multiplies a 2D vector by a scalar */
function Vector2 vector2_mul(Vector2 v1, Vector2 v2); /* Multiplies two 2D vectors component-wise */
function Vector2 vector2_negate(Vector2 v); /* Negates a 2D vector */
function Vector2 vector2_div(Vector2 v1, Vector2 v2); /* Divides two 2D vectors component-wise */
function Vector2 vector2_normalize(Vector2 v); /* Returns a normalized (unit length) version of a 2D vector */
function Vector2 vector2_transform(Vector2 v, Matrix4 mat); /* Transforms a 2D vector by a 4x4 matrix */
function Vector2 vector2_lerp(Vector2 v1, Vector2 v2, f32 amount); /* Linearly interpolates between two 2D vectors */
function Vector2 vector2_reflect(Vector2 v, Vector2 normal); /* Reflects a 2D vector about a given normal */
function Vector2 vector2_rotate(Vector2 v, f32 angle); /* Rotates a 2D vector by an angle in radians */
function Vector2 vector2_move_towards(Vector2 v, Vector2 target, f32 max_distance); /* Moves a vector toward a target by a maximum distance */
function Vector2 vector2_invert(Vector2 v); /* Inverts each component of a 2D vector (1/x, 1/y) */
function Vector2 vector2_clamp(Vector2 v, Vector2 min, Vector2 max); /* Clamps each component of a 2D vector between min and max values */
function b32     vector2_equals(Vector2 p, Vector2 q); /* Returns true if two 2D vectors are equal component-wise */
function Vector2 vector2_refract(Vector2 v, Vector2 n, f32 r); /* Computes the direction of a refracted ray\n v: normalized direction of the incoming ray\n n: normalized normal vector of the interface of two optical media\n r: ratio of the refractive index of the medium from where the ray comes\n to the refractive index of the medium on the other side of the surface */

// @Section: Vector3
struct Vector3 { f32 x, y, z; };
function Vector3 vector3_make(f32 x, f32 y, f32 z); /* Creates a Vector 3 */
function Vector3 vector3_make_zero(f32 x, f32 y, f32 z); /* Creates a zero initialized Vector 3 */
function Vector3 vector3_add(Vector3 v1, Vector3 v2); /* Adds two 3D vectors component-wise */
function Vector3 vector3_sub(Vector3 v1, Vector3 v2); /* Subtracts the second 3D vector from the first component-wise */
function Vector3 vector3_scale(Vector3 v, f32 scalar); /* Multiplies a 3D vector by a scalar */
function Vector3 vector3_mul(Vector3 v1, Vector3 v2); /* Multiplies two 3D vectors component-wise */
function Vector3 vector3_cross(Vector3 v1, Vector3 v2); /* Computes the cross product of two 3D vectors */
function f32     vector3_length(Vector3 v); /* Returns the Euclidean length (magnitude) of a 3D vector */
function f32     vector3_length_squared(Vector3 v); /* Returns the squared length of a 3D vector (avoids sqrt) */
function f32     vector3_dot(Vector3 v1, Vector3 v2); /* Computes the dot product of two 3D vectors */
function f32     vector3_distance(Vector3 v1, Vector3 v2); /* Returns the distance between two 3D points */
function f32     vector3_distance_squared(Vector3 v1, Vector3 v2); /* Returns the squared distance between two 3D points */
function Vector3 vector3_negate(Vector3 v); /* Negates a 3D vector */
function Vector3 vector3_div(Vector3 v1, Vector3 v2); /* Divides two 3D vectors component-wise */
function Vector3 vector3_normalize(Vector3 v); /* Returns a normalized (unit length) version of a 3D vector */
function Vector3 vector3_project(Vector3 v1, Vector3 v2); /* Projects vector v1 onto vector v2 */
function Vector3 vector3_reject(Vector3 v1, Vector3 v2); /* Calculates the rejection of the vector v1 on to v2 */
function void    vector3_ortho_normalize(Vector3 *v1, Vector3 *v2); /* Orthonormalizes provided vectors.\n Makes vectors normalized and orthogonal to each other.\n Implements the Gram-Schmidt process */
function Vector3 vector3_transform(Vector3 v, Matrix4 mat); /* Transforms a 3D vector by a 4x4 matrix */
function Vector3 vector3_rotate_by_quaternion(Vector3 v, Quaternion q); /* Rotates a 3D vector using a quaternion */
function Vector3 vector3_rotate_by_axis_angle(Vector3 v, Vector3 axis, f32 angle); /* Rotates a 3D vector around an axis by an angle in radians */
function Vector3 vector3_move_towards(Vector3 v, Vector3 target, f32 max_distance); /* Moves a vector toward a target by a maximum distance */
function Vector3 vector3_lerp(Vector3 v1, Vector3 v2, f32 amount); /* Linearly interpolates between two 3D vectors */
function Vector3 vector3_cubic_hermite(Vector3 v1, Vector3 tangent1, Vector3 v2, Vector3 tangent2, f32 amount); /* Calculates cubic hermite interpolation between two vectors and their tangents,\n as described in the GLTF 2.0 specification:\n https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#interpolation-cubic */
function Vector3 vector3_reflect(Vector3 v, Vector3 normal); /* Reflects a 3D vector about a given normal */
function Vector3 vector3_barycenter(Vector3 p, Vector3 a, Vector3 b, Vector3 c); /* Computes barycenter coordinates (u, v, w) for point p with respect to triangle (a, b, c).\n Assumes p is on the plane of the triangle */
function Vector3 vector3_unproject(Vector3 source, Matrix4 projection, Matrix4 view); /* Projects a Vector3 from screen space into world space */
function Vector3 vector3_invert(Vector3 v); /* Inverts each component of a 3D vector (1/x, 1/y, 1/z) */
function Vector3 vector3_clamp(Vector3 v, Vector3 min, Vector3 max); /* Clamps each component of a 3D vector between min and max values */
function b32     vector3_equals(Vector3 p, Vector3 q); /* Returns true if two 3D vectors are equal component-wise */
function Vector3 vector3_refract(Vector3 v, Vector3 n, f32 r); /* Computes the direction of a refracted ray\n v: normalized direction of the incoming ray\n n: normalized normal vector of the interface of two optical media\n r: ratio of the refractive index of the medium from where the ray comes\n to the refractive index of the medium on the other side of the surface */

// @Section: Vector4
struct Vector4 { f32 x, y, z, w; };
function Vector4 vector4_make(f32 x, f32 y, f32 z, f32 w); /* Creates a Vector 4 */
function Vector4 vector4_make_zero(f32 x, f32 y, f32 z, f32 w); /* Creates a zero initialized Vector 4 */
function Vector4 vector4_add(Vector4 v1, Vector4 v2); /* Adds two 4D vectors component-wise */
function Vector4 vector4_sub(Vector4 v1, Vector4 v2); /* Subtracts the second 4D vector from the first component-wise */
function f32     vector4_length(Vector4 v); /* Returns the Euclidean length (magnitude) of a 4D vector */
function f32     vector4_length_squared(Vector4 v); /* Returns the squared length of a 4D vector (avoids sqrt) */
function f32     vector4_dot(Vector4 v1, Vector4 v2); /* Computes the dot product of two 4D vectors */
function f32     vector4_distance(Vector4 v1, Vector4 v2); /* Returns the distance between two 4D points */
function f32     vector4_distance_squared(Vector4 v1, Vector4 v2); /* Returns the squared distance between two 4D points */
function Vector4 vector4_scale(Vector4 v, f32 scale); /* Multiplies a 4D vector by a scalar */
function Vector4 vector4_mul(Vector4 v1, Vector4 v2); /* Multiplies two 4D vectors component-wise */
function Vector4 vector4_negate(Vector4 v); /* Negates a 4D vector */
function Vector4 vector4_div(Vector4 v1, Vector4 v2); /* Divides two 4D vectors component-wise */
function Vector4 vector4_normalize(Vector4 v); /* Returns a normalized (unit length) version of a 4D vector */
function Vector4 vector4_lerp(Vector4 v1, Vector4 v2, f32 amount); /* Linearly interpolates between two 4D vectors */
function Vector4 vector4_move_towards(Vector4 v, Vector4 target, f32 maxDistance); /* Moves a 4D vector toward a target by a maximum distance */
function Vector4 vector4_invert(Vector4 v); /* Inverts each component of a 4D vector (1/x, 1/y, 1/z, 1/w) */
function b32     vector4_equals(Vector4 p, Vector4 q); /* Returns true if two 4D vectors are equal component-wise */

// @Section: Matrix4
struct Matrix4
{
  f32 m0,  m4,  m8,  m12,
      m1,  m5,  m9,  m13,
      m2,  m6,  m10, m14,
      m3,  m7,  m11, m15;
};
#define matrix4(diagonal)          \
  (Matrix4){                       \
    (diagonal), 0.0f, 0.0f, 0.0f,  \
    0.0f, (diagonal), 0.0f, 0.0f,  \
    0.0f, 0.0f, (diagonal), 0.0f,  \
    0.0f, 0.0f, 0.0f, (diagonal)   }
#define matrix4_identity() matrix4(1.0f)

function f32     matrix4_determinant(Matrix4 mat); /* Computes the determinant of a 4x4 matrix */
function f32     matrix4_trace(Matrix4 mat); /* Computes the trace of a 4x4 matrix (sum of diagonal elements) */
function Matrix4 matrix4_transpose(Matrix4 mat); /* Returns the transpose of a 4x4 matrix */
function Matrix4 matrix4_invert(Matrix4 mat); /* Returns the inverse of a 4x4 matrix */
function Matrix4 matrix4_add(Matrix4 left, Matrix4 right); /* Adds two 4x4 matrices component-wise */
function Matrix4 matrix4_sub(Matrix4 left, Matrix4 right); /* Subtracts the right 4x4 matrix from the left component-wise */
function Matrix4 matrix4_multiply(Matrix4 left, Matrix4 right); /* Multiplies two 4x4 matrices */
function Matrix4 matrix4_translate(f32 x, f32 y, f32 z); /* Creates a translation matrix for the given x, y, z offsets */
function Matrix4 matrix4_rotate(Vector3 axis, f32 angle); /* Creates a rotation matrix around a given axis by an angle in radians */
function Matrix4 matrix4_rotate_X(f32 angle); /* Creates a rotation matrix around the X-axis by an angle in radians */
function Matrix4 matrix4_rotate_Y(f32 angle); /* Creates a rotation matrix around the Y-axis by an angle in radians */
function Matrix4 matrix4_rotate_Z(f32 angle); /* Creates a rotation matrix around the Z-axis by an angle in radians */
function Matrix4 matrix4_rotate_XYZ(Vector3 angle); /* Creates a combined rotation matrix around the X, Y, and Z axes in order XYZ (angles in radians) */
function Matrix4 matrix4_rotate_ZYX(Vector3 angle); /* Creates a combined rotation matrix around the Z, Y, and X axes in order ZYX (angles in radians) */
function Matrix4 matrix4_scale(f32 x, f32 y, f32 z); /* Creates a scaling matrix with scale factors for each axis */
function Matrix4 matrix4_frustum(f64 left, f64 right, f64 bottom, f64 top, f64 near_plane, f64 far_plane); /* Creates a perspective frustum projection matrix\n defined by the given left, right, bottom, top, near, and far planes */
function Matrix4 matrix4_perspective(f64 fovY, f64 aspect, f64 near_plane, f64 far_plane); /* Creates a perspective projection matrix using a vertical field of view in radians\n with the given aspect ratio, near, and far planes */
function Matrix4 matrix4_ortho(f64 left, f64 right, f64 bottom, f64 top, f64 nearPlane, f64 farPlane); /* Creates an orthographic projection matrix defined by the given planes */
function Matrix4 matrix4_look_at(Vector3 eye, Vector3 target, Vector3 up); /* Creates a view matrix that looks from 'eye' toward 'target' using the 'up' direction */

// @Section: Quaternion
struct Quaternion { f32 x, y, z, w; };
#define quaternion(x,y,z)     (Quaternion){(x),(y),(z),1.0f}
#define quaternion_w(x,y,z,w) (Quaternion){(x),(y),(z),(w)}
#define quaternion_identity() (Quaternion){0.0f,0.0f,0.0f,1.0f}

function Quaternion quaternion_add(Quaternion q1, Quaternion q2); /* Adds two quaternions component-wise */
function Quaternion quaternion_sub(Quaternion q1, Quaternion q2); /* Subtracts the second quaternion from the first component-wise */
function f32        quaternion_length(Quaternion q); /* Returns the length (magnitude) of a quaternion */
function Quaternion quaternion_normalize(Quaternion q); /* Returns a normalized (unit length) quaternion */
function Quaternion quaternion_invert(Quaternion q); /* Returns the inverse of a quaternion */
function Quaternion quaternion_mul(Quaternion q1, Quaternion q2); /* Multiplies two quaternions (combines rotations) */
function Quaternion quaternion_scale(Quaternion q, f32 mul); /* Scales each component of a quaternion by a scalar */
function Quaternion quaternion_div(Quaternion q1, Quaternion q2); /* Divides one quaternion by another (q1 * inverse(q2)) */
function Quaternion quaternion_lerp(Quaternion q1, Quaternion q2, f32 amount); /* Performs linear interpolation between two quaternions */
function Quaternion quaternion_nlerp(Quaternion q1, Quaternion q2, f32 amount); /* Calculates normalized linear interpolation (nlerp) between two quaternions.\n Provides a faster approximation of slerp */
function Quaternion quaternion_slerp(Quaternion q1, Quaternion q2, f32 amount); /* Calculates spherical linear interpolation (slerp) between two quaternions */
function Quaternion quaternion_cubic_hermite_spline(Quaternion q1, Quaternion out_tangent1, Quaternion q2, Quaternion in_tangent2, f32 t); /* Calculates quaternion cubic spline interpolation using the Cubic Hermite Spline algorithm,\n as described in the GLTF 2.0 specification:\n https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#interpolation-cubic */
function Quaternion quaternion_from_vector3_to_vector3(Vector3 from, Vector3 to); /* Creates a quaternion representing the rotation from one vector to another */
function Quaternion quaternion_from_matrix4(Matrix4 mat); /* Converts a rotation matrix to a quaternion */
function Matrix4    quaternion_to_matrix4(Quaternion q); /* Converts a quaternion to a rotation matrix */
function Quaternion quaternion_from_axis_angle(Vector3 axis, f32 angle); /* Creates a quaternion representing a rotation around an axis by an angle in radians */
function void       quaternion_to_axis_angle(Quaternion q, Vector3 *out_axis, f32 *out_angle); /* Extracts the rotation axis and angle (in radians) from a quaternion */
function Quaternion quaternion_from_euler(f32 pitch, f32 yaw, f32 roll); /* Creates a quaternion equivalent to Euler angles (Z-Y-X rotation order)\n Angles must be provided in radians */
function Vector3    quaternion_to_euler(Quaternion q); /* Converts a quaternion to Euler angles (roll, pitch, yaw)\n Returns angles in radians as a Vector3 struct */
function Quaternion quaternion_transform(Quaternion q, Matrix4 mat); /* Transforms a quaternion by a 4x4 matrix */
function b32        quaternion_equals(Quaternion p, Quaternion q); /* Returns true if two quaternions are equal component-wise */


// @Section: Implementation
function Vector2
vector2_add(Vector2 v1, Vector2 v2)
{
  Vector2 result = { v1.x + v2.x, v1.y + v2.y };
  return result;
}

function Vector2
vector2_sub(Vector2 v1, Vector2 v2)
{
  Vector2 result = { v1.x - v2.x, v1.y - v2.y };
  return result;
}

function f32
vector2_length(Vector2 v)
{
  f32 result = sqrtf((v.x*v.x) + (v.y*v.y));
  return result;
}

function f32
vector2_length_squared(Vector2 v)
{
  f32 result = (v.x*v.x) + (v.y*v.y);
  return result;
}

function f32
vector2_dot(Vector2 v1, Vector2 v2)
{
  f32 result = (v1.x*v2.x + v1.y*v2.y);
  return result;
}

function f32
vector2_cross(Vector2 v1, Vector2 v2)
{
  f32 result = (v1.x*v2.y - v1.y*v2.x);
  return result;
}

function f32
vector2_distance(Vector2 v1, Vector2 v2)
{
  f32 result = sqrtf((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
  return result;
}

function f32
vector2_distance_squared(Vector2 v1, Vector2 v2)
{
  f32 result = ((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
  return result;
}

function Vector2
vector2_scale(Vector2 v, f32 scale)
{
  Vector2 result = { v.x*scale, v.y*scale };
  return result;
}

function Vector2
vector2_mul(Vector2 v1, Vector2 v2)
{
  Vector2 result = { v1.x*v2.x, v1.y*v2.y };
  return result;
}

function Vector2
vector2_negate(Vector2 v)
{
  Vector2 result = { -v.x, -v.y };
  return result;
}

function Vector2
vector2_div(Vector2 v1, Vector2 v2)
{
  Vector2 result = { v1.x/v2.x, v1.y/v2.y };
  return result;
}

function Vector2
vector2_normalize(Vector2 v)
{
  Vector2 result = { 0 };
  f32 length = sqrtf((v.x*v.x) + (v.y*v.y));
  if (length > 0)
  {
    f32 ilength = 1.0f/length;
    result.x = v.x*ilength;
    result.y = v.y*ilength;
  }
  return result;
}

function Vector2
vector2_transform(Vector2 v, Matrix4 mat)
{
  Vector2 result = { 0 };
  f32 x = v.x;
  f32 y = v.y;
  f32 z = 0;
  result.x = mat.m0*x + mat.m4*y + mat.m8*z + mat.m12;
  result.y = mat.m1*x + mat.m5*y + mat.m9*z + mat.m13;
  return result;
}

function Vector2
vector2_lerp(Vector2 v1, Vector2 v2, f32 amount)
{
  Vector2 result = { 0 };
  result.x = v1.x + amount*(v2.x - v1.x);
  result.y = v1.y + amount*(v2.y - v1.y);
  return result;
}

function Vector2
vector2_reflect(Vector2 v, Vector2 normal)
{
  Vector2 result = { 0 };
  f32 dot = (v.x*normal.x + v.y*normal.y);
  result.x = v.x - (2.0f*normal.x)*dot;
  result.y = v.y - (2.0f*normal.y)*dot;
  return result;
}

function Vector2
vector2_rotate(Vector2 v, f32 angle)
{
  Vector2 result = { 0 };
  f32 cosres = cosf(angle);
  f32 sinres = sinf(angle);
  result.x = v.x*cosres - v.y*sinres;
  result.y = v.x*sinres + v.y*cosres;
  return result;
}

function Vector2
vector2_move_towards(Vector2 v, Vector2 target, f32 max_distance)
{
  Vector2 result = { 0 };
  f32 dx = target.x - v.x;
  f32 dy = target.y - v.y;
  f32 value = (dx*dx) + (dy*dy);
  if ((value == 0) || ((max_distance >= 0) && (value <= max_distance*max_distance)))
  {
    return target;
  }
  f32 dist = sqrtf(value);
  result.x = v.x + dx/dist*max_distance;
  result.y = v.y + dy/dist*max_distance;
  return result;
}

function Vector2
vector2_invert(Vector2 v)
{
  Vector2 result = { 1.0f/v.x, 1.0f/v.y };
  return result;
}

function Vector2
vector2_clamp(Vector2 v, Vector2 min, Vector2 max)
{
  Vector2 result = { 0 };
  result.x = fminf(max.x, fmaxf(min.x, v.x));
  result.y = fminf(max.y, fmaxf(min.y, v.y));
  return result;
}

function b32
vector2_equals(Vector2 p, Vector2 q)
{
  b32 result = ((fabsf(p.x - q.x)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
               ((fabsf(p.y - q.y)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y)))));
  return result;
}

function Vector2
vector2_refract(Vector2 v, Vector2 n, f32 r)
{
  Vector2 result = { 0 };
  f32 dot = v.x*n.x + v.y*n.y;
  f32 d = 1.0f - r*r*(1.0f - dot*dot);
  if (d >= 0.0f)
  {
    d = sqrtf(d);
    v.x = r*v.x - (r*dot + d)*n.x;
    v.y = r*v.y - (r*dot + d)*n.y;
    result = v;
  }
  return result;
}

function Vector3
vector3_add(Vector3 v1, Vector3 v2)
{
  Vector3 result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
  return result;
}

function Vector3
vector3_sub(Vector3 v1, Vector3 v2)
{
  Vector3 result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
  return result;
}

function Vector3
vector3_scale(Vector3 v, f32 scalar)
{
  Vector3 result = { v.x*scalar, v.y*scalar, v.z*scalar };
  return result;
}

function Vector3
vector3_mul(Vector3 v1, Vector3 v2)
{
  Vector3 result = { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z };
  return result;
}

function Vector3
vector3_cross(Vector3 v1, Vector3 v2)
{
  Vector3 result = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
  return result;
}

function f32
vector3_length(const Vector3 v)
{
  f32 result = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
  return result;
}

function f32
vector3_length_squared(const Vector3 v)
{
  f32 result = v.x*v.x + v.y*v.y + v.z*v.z;
  return result;
}

function f32
vector3_dot(Vector3 v1, Vector3 v2)
{
  f32 result = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
  return result;
}

function f32
vector3_distance(Vector3 v1, Vector3 v2)
{
  f32 result = 0.0f;
  f32 dx = v2.x - v1.x;
  f32 dy = v2.y - v1.y;
  f32 dz = v2.z - v1.z;
  result = sqrtf(dx*dx + dy*dy + dz*dz);
  return result;
}

function f32
vector3_distance_squared(Vector3 v1, Vector3 v2)
{
  f32 result = 0.0f;
  f32 dx = v2.x - v1.x;
  f32 dy = v2.y - v1.y;
  f32 dz = v2.z - v1.z;
  result = dx*dx + dy*dy + dz*dz;
  return result;
}

function Vector3
vector3_negate(Vector3 v)
{
  Vector3 result = { -v.x, -v.y, -v.z };
  return result;
}

function Vector3
vector3_div(Vector3 v1, Vector3 v2)
{
  Vector3 result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z };
  return result;
}

function Vector3
vector3_normalize(Vector3 v)
{
  Vector3 result = v;

  f32 length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
  if (length != 0.0f)
  {
    f32 ilength = 1.0f/length;

    result.x *= ilength;
    result.y *= ilength;
    result.z *= ilength;
  }

  return result;
}

function Vector3
vector3_project(Vector3 v1, Vector3 v2)
{
  Vector3 result = { 0 };
  f32 v1dv2 = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
  f32 v2dv2 = (v2.x*v2.x + v2.y*v2.y + v2.z*v2.z);
  f32 mag = v1dv2/v2dv2;
  result.x = v2.x*mag;
  result.y = v2.y*mag;
  result.z = v2.z*mag;
  return result;
}

function Vector3
vector3_reject(Vector3 v1, Vector3 v2)
{
  Vector3 result = { 0 };
  f32 v1dv2 = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
  f32 v2dv2 = (v2.x*v2.x + v2.y*v2.y + v2.z*v2.z);
  f32 mag = v1dv2/v2dv2;
  result.x = v1.x - (v2.x*mag);
  result.y = v1.y - (v2.y*mag);
  result.z = v1.z - (v2.z*mag);
  return result;
}

function void
vector3_ortho_normalize(Vector3 *v1, Vector3 *v2)
{
  f32 length = 0.0f;
  f32 ilength = 0.0f;

  // vector3_normalize(*v1);
  Vector3 v = *v1;
  length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
  if (length == 0.0f) length = 1.0f;
  ilength = 1.0f/length;
  v1->x *= ilength;
  v1->y *= ilength;
  v1->z *= ilength;

  // vector3_cross(*v1, *v2)
  Vector3 vn1 = { v1->y*v2->z - v1->z*v2->y, v1->z*v2->x - v1->x*v2->z, v1->x*v2->y - v1->y*v2->x };

  // vector3_normalize(vn1);
  v = vn1;
  length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
  if (length == 0.0f) length = 1.0f;
  ilength = 1.0f/length;
  vn1.x *= ilength;
  vn1.y *= ilength;
  vn1.z *= ilength;

  // vector3_cross(vn1, *v1)
  Vector3 vn2 = { vn1.y*v1->z - vn1.z*v1->y, vn1.z*v1->x - vn1.x*v1->z, vn1.x*v1->y - vn1.y*v1->x };

  *v2 = vn2;
}

function Vector3
vector3_transform(Vector3 v, Matrix4 mat)
{
  Vector3 result = { 0 };
  f32 x = v.x;
  f32 y = v.y;
  f32 z = v.z;
  result.x = mat.m0*x + mat.m4*y + mat.m8*z + mat.m12;
  result.y = mat.m1*x + mat.m5*y + mat.m9*z + mat.m13;
  result.z = mat.m2*x + mat.m6*y + mat.m10*z + mat.m14;
  return result;
}

function Vector3
vector3_rotate_by_quaternion(Vector3 v, Quaternion q)
{
  Vector3 result = { 0 };
  result.x = v.x*(q.x*q.x + q.w*q.w - q.y*q.y - q.z*q.z) + v.y*(2*q.x*q.y - 2*q.w*q.z) + v.z*(2*q.x*q.z + 2*q.w*q.y);
  result.y = v.x*(2*q.w*q.z + 2*q.x*q.y) + v.y*(q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z) + v.z*(-2*q.w*q.x + 2*q.y*q.z);
  result.z = v.x*(-2*q.w*q.y + 2*q.x*q.z) + v.y*(2*q.w*q.x + 2*q.y*q.z)+ v.z*(q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z);
  return result;
}

function Vector3
vector3_rotate_by_axis_angle(Vector3 v, Vector3 axis, f32 angle)
{
  // Using Euler-Rodrigues Formula
  // Ref.: https://en.wikipedia.org/w/index.php?title=Euler%E2%80%93Rodrigues_formula

  Vector3 result = v;

  // vector3_normalize(axis);
  f32 length = sqrtf(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
  if (length == 0.0f) length = 1.0f;
  f32 ilength = 1.0f/length;
  axis.x *= ilength;
  axis.y *= ilength;
  axis.z *= ilength;

  angle /= 2.0f;
  f32 a = sinf(angle);
  f32 b = axis.x*a;
  f32 c = axis.y*a;
  f32 d = axis.z*a;
  a = cosf(angle);
  Vector3 w = { b, c, d };

  // vector3_cross(w, v)
  Vector3 wv = { w.y*v.z - w.z*v.y, w.z*v.x - w.x*v.z, w.x*v.y - w.y*v.x };

  // vector3_cross(w, wv)
  Vector3 wwv = { w.y*wv.z - w.z*wv.y, w.z*wv.x - w.x*wv.z, w.x*wv.y - w.y*wv.x };

  // vector3_scale(wv, 2*a)
  a *= 2;
  wv.x *= a;
  wv.y *= a;
  wv.z *= a;

  // vector3_scale(wwv, 2)
  wwv.x *= 2;
  wwv.y *= 2;
  wwv.z *= 2;

  result.x += wv.x;
  result.y += wv.y;
  result.z += wv.z;

  result.x += wwv.x;
  result.y += wwv.y;
  result.z += wwv.z;

  return result;
}

function Vector3
vector3_move_towards(Vector3 v, Vector3 target, f32 max_distance)
{
  Vector3 result = { 0 };
  f32 dx = target.x - v.x;
  f32 dy = target.y - v.y;
  f32 dz = target.z - v.z;
  f32 value = (dx*dx) + (dy*dy) + (dz*dz);
  if ((value == 0) || ((max_distance >= 0) && (value <= max_distance*max_distance)))
  {
    return target;
  }
  f32 dist = sqrtf(value);
  result.x = v.x + dx/dist*max_distance;
  result.y = v.y + dy/dist*max_distance;
  result.z = v.z + dz/dist*max_distance;
  return result;
}

function Vector3
vector3_lerp(Vector3 v1, Vector3 v2, f32 amount)
{
  Vector3 result = { 0 };
  result.x = v1.x + amount*(v2.x - v1.x);
  result.y = v1.y + amount*(v2.y - v1.y);
  result.z = v1.z + amount*(v2.z - v1.z);
  return result;
}

function Vector3
vector3_cubic_hermite(Vector3 v1, Vector3 tangent1, Vector3 v2, Vector3 tangent2, f32 amount)
{
  Vector3 result = { 0 };
  f32 amountPow2 = amount*amount;
  f32 amountPow3 = amount*amount*amount;
  result.x = (2*amountPow3 - 3*amountPow2 + 1)*v1.x + (amountPow3 - 2*amountPow2 + amount)*tangent1.x + (-2*amountPow3 + 3*amountPow2)*v2.x + (amountPow3 - amountPow2)*tangent2.x;
  result.y = (2*amountPow3 - 3*amountPow2 + 1)*v1.y + (amountPow3 - 2*amountPow2 + amount)*tangent1.y + (-2*amountPow3 + 3*amountPow2)*v2.y + (amountPow3 - amountPow2)*tangent2.y;
  result.z = (2*amountPow3 - 3*amountPow2 + 1)*v1.z + (amountPow3 - 2*amountPow2 + amount)*tangent1.z + (-2*amountPow3 + 3*amountPow2)*v2.z + (amountPow3 - amountPow2)*tangent2.z;
  return result;
}

function Vector3
vector3_reflect(Vector3 v, Vector3 normal)
{
  Vector3 result = { 0 };

  // I is the original vector
  // N is the normal of the incident plane
  // R = I - (2*N*(DotProduct[I, N]))

  f32 dot = (v.x*normal.x + v.y*normal.y + v.z*normal.z);
  result.x = v.x - (2.0f*normal.x)*dot;
  result.y = v.y - (2.0f*normal.y)*dot;
  result.z = v.z - (2.0f*normal.z)*dot;
  return result;
}

function Vector3
vector3_barycenter(Vector3 p, Vector3 a, Vector3 b, Vector3 c)
{
  Vector3 result = { 0 };

  Vector3 v0 = { b.x - a.x, b.y - a.y, b.z - a.z }; // vector3_sub(b, a)
  Vector3 v1 = { c.x - a.x, c.y - a.y, c.z - a.z }; // vector3_sub(c, a)
  Vector3 v2 = { p.x - a.x, p.y - a.y, p.z - a.z }; // vector3_sub(p, a)
  f32 d00 = (v0.x*v0.x + v0.y*v0.y + v0.z*v0.z); // vector3_dot(v0, v0)
  f32 d01 = (v0.x*v1.x + v0.y*v1.y + v0.z*v1.z); // vector3_dot(v0, v1)
  f32 d11 = (v1.x*v1.x + v1.y*v1.y + v1.z*v1.z); // vector3_dot(v1, v1)
  f32 d20 = (v2.x*v0.x + v2.y*v0.y + v2.z*v0.z); // vector3_dot(v2, v0)
  f32 d21 = (v2.x*v1.x + v2.y*v1.y + v2.z*v1.z); // vector3_dot(v2, v1)

  f32 denom = d00*d11 - d01*d01;

  result.y = (d11*d20 - d01*d21)/denom;
  result.z = (d00*d21 - d01*d20)/denom;
  result.x = 1.0f - (result.z + result.y);

  return result;
}

function Vector3
vector3_unproject(Vector3 source, Matrix4 projection, Matrix4 view)
{
  Vector3 result = { 0 };

  // Calculate unprojected matrix (multiply view matrix by projection matrix) and invert it
  Matrix4 mat_view_proj = { // matrix4_mul(view, projection);
    view.m0*projection.m0 + view.m1*projection.m4 + view.m2*projection.m8 + view.m3*projection.m12,
    view.m0*projection.m1 + view.m1*projection.m5 + view.m2*projection.m9 + view.m3*projection.m13,
    view.m0*projection.m2 + view.m1*projection.m6 + view.m2*projection.m10 + view.m3*projection.m14,
    view.m0*projection.m3 + view.m1*projection.m7 + view.m2*projection.m11 + view.m3*projection.m15,
    view.m4*projection.m0 + view.m5*projection.m4 + view.m6*projection.m8 + view.m7*projection.m12,
    view.m4*projection.m1 + view.m5*projection.m5 + view.m6*projection.m9 + view.m7*projection.m13,
    view.m4*projection.m2 + view.m5*projection.m6 + view.m6*projection.m10 + view.m7*projection.m14,
    view.m4*projection.m3 + view.m5*projection.m7 + view.m6*projection.m11 + view.m7*projection.m15,
    view.m8*projection.m0 + view.m9*projection.m4 + view.m10*projection.m8 + view.m11*projection.m12,
    view.m8*projection.m1 + view.m9*projection.m5 + view.m10*projection.m9 + view.m11*projection.m13,
    view.m8*projection.m2 + view.m9*projection.m6 + view.m10*projection.m10 + view.m11*projection.m14,
    view.m8*projection.m3 + view.m9*projection.m7 + view.m10*projection.m11 + view.m11*projection.m15,
    view.m12*projection.m0 + view.m13*projection.m4 + view.m14*projection.m8 + view.m15*projection.m12,
    view.m12*projection.m1 + view.m13*projection.m5 + view.m14*projection.m9 + view.m15*projection.m13,
    view.m12*projection.m2 + view.m13*projection.m6 + view.m14*projection.m10 + view.m15*projection.m14,
    view.m12*projection.m3 + view.m13*projection.m7 + view.m14*projection.m11 + view.m15*projection.m15 };

  // Calculate inverted matrix -> Matrix4_invert(mat_view_proj);
  // Cache the matrix values (speed optimization)
  f32 a00 = mat_view_proj.m0, a01 = mat_view_proj.m1, a02 = mat_view_proj.m2, a03 = mat_view_proj.m3;
  f32 a10 = mat_view_proj.m4, a11 = mat_view_proj.m5, a12 = mat_view_proj.m6, a13 = mat_view_proj.m7;
  f32 a20 = mat_view_proj.m8, a21 = mat_view_proj.m9, a22 = mat_view_proj.m10, a23 = mat_view_proj.m11;
  f32 a30 = mat_view_proj.m12, a31 = mat_view_proj.m13, a32 = mat_view_proj.m14, a33 = mat_view_proj.m15;

  f32 b00 = a00*a11 - a01*a10;
  f32 b01 = a00*a12 - a02*a10;
  f32 b02 = a00*a13 - a03*a10;
  f32 b03 = a01*a12 - a02*a11;
  f32 b04 = a01*a13 - a03*a11;
  f32 b05 = a02*a13 - a03*a12;
  f32 b06 = a20*a31 - a21*a30;
  f32 b07 = a20*a32 - a22*a30;
  f32 b08 = a20*a33 - a23*a30;
  f32 b09 = a21*a32 - a22*a31;
  f32 b10 = a21*a33 - a23*a31;
  f32 b11 = a22*a33 - a23*a32;

  // Calculate the invert determinant (inlined to avoid double-caching)
  f32 invDet = 1.0f/(b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);

  Matrix4 mat_view_proj_inverse = {
    (a11*b11 - a12*b10 + a13*b09)*invDet,
    (-a01*b11 + a02*b10 - a03*b09)*invDet,
    (a31*b05 - a32*b04 + a33*b03)*invDet,
    (-a21*b05 + a22*b04 - a23*b03)*invDet,
    (-a10*b11 + a12*b08 - a13*b07)*invDet,
    (a00*b11 - a02*b08 + a03*b07)*invDet,
    (-a30*b05 + a32*b02 - a33*b01)*invDet,
    (a20*b05 - a22*b02 + a23*b01)*invDet,
    (a10*b10 - a11*b08 + a13*b06)*invDet,
    (-a00*b10 + a01*b08 - a03*b06)*invDet,
    (a30*b04 - a31*b02 + a33*b00)*invDet,
    (-a20*b04 + a21*b02 - a23*b00)*invDet,
    (-a10*b09 + a11*b07 - a12*b06)*invDet,
    (a00*b09 - a01*b07 + a02*b06)*invDet,
    (-a30*b03 + a31*b01 - a32*b00)*invDet,
    (a20*b03 - a21*b01 + a22*b00)*invDet };

  // Create quaternion from source point
  Quaternion quat = { source.x, source.y, source.z, 1.0f };

  // Multiply quat point by unprojecte matrix
  Quaternion qtransformed = { // Quaternion_transform(quat, mat_view_proj_inverse)
    mat_view_proj_inverse.m0*quat.x + mat_view_proj_inverse.m4*quat.y + mat_view_proj_inverse.m8*quat.z + mat_view_proj_inverse.m12*quat.w,
    mat_view_proj_inverse.m1*quat.x + mat_view_proj_inverse.m5*quat.y + mat_view_proj_inverse.m9*quat.z + mat_view_proj_inverse.m13*quat.w,
    mat_view_proj_inverse.m2*quat.x + mat_view_proj_inverse.m6*quat.y + mat_view_proj_inverse.m10*quat.z + mat_view_proj_inverse.m14*quat.w,
    mat_view_proj_inverse.m3*quat.x + mat_view_proj_inverse.m7*quat.y + mat_view_proj_inverse.m11*quat.z + mat_view_proj_inverse.m15*quat.w };

  // Normalized world points in vectors
  result.x = qtransformed.x/qtransformed.w;
  result.y = qtransformed.y/qtransformed.w;
  result.z = qtransformed.z/qtransformed.w;

  return result;
}

function Vector3
vector3_invert(Vector3 v)
{
  Vector3 result = { 1.0f/v.x, 1.0f/v.y, 1.0f/v.z };
  return result;
}

function Vector3
vector3_clamp(Vector3 v, Vector3 min, Vector3 max)
{
  Vector3 result = { 0 };

  result.x = fminf(max.x, fmaxf(min.x, v.x));
  result.y = fminf(max.y, fmaxf(min.y, v.y));
  result.z = fminf(max.z, fmaxf(min.z, v.z));

  return result;
}

function b32
vector3_equals(Vector3 p, Vector3 q)
{
  b32 result = ((fabsf(p.x - q.x)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
         ((fabsf(p.y - q.y)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) &&
         ((fabsf(p.z - q.z)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z)))));

  return result;
}

function Vector3
vector3_refract(Vector3 v, Vector3 n, f32 r)
{
  Vector3 result = { 0 };
  f32 dot = v.x*n.x + v.y*n.y + v.z*n.z;
  f32 d = 1.0f - r*r*(1.0f - dot*dot);
  if (d >= 0.0f)
  {
    d = sqrtf(d);
    v.x = r*v.x - (r*dot + d)*n.x;
    v.y = r*v.y - (r*dot + d)*n.y;
    v.z = r*v.z - (r*dot + d)*n.z;
    result = v;
  }
  return result;
}

function Vector4
vector4_add(Vector4 v1, Vector4 v2)
{
  Vector4 result = {
    v1.x + v2.x,
    v1.y + v2.y,
    v1.z + v2.z,
    v1.w + v2.w
  };
  return result;
}

function Vector4
vector4_sub(Vector4 v1, Vector4 v2)
{
  Vector4 result = {
    v1.x - v2.x,
    v1.y - v2.y,
    v1.z - v2.z,
    v1.w - v2.w
  };
  return result;
}

function f32
vector4_length(Vector4 v)
{
  f32 result = sqrtf((v.x*v.x) + (v.y*v.y) + (v.z*v.z) + (v.w*v.w));
  return result;
}

function f32
vector4_length_squared(Vector4 v)
{
  f32 result = (v.x*v.x) + (v.y*v.y) + (v.z*v.z) + (v.w*v.w);
  return result;
}

function f32
vector4_dot(Vector4 v1, Vector4 v2)
{
  f32 result = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w);
  return result;
}

function f32
vector4_distance(Vector4 v1, Vector4 v2)
{
  f32 result = sqrtf(
    (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y) +
    (v1.z - v2.z)*(v1.z - v2.z) + (v1.w - v2.w)*(v1.w - v2.w));
  return result;
}

function f32
vector4_distance_squared(Vector4 v1, Vector4 v2)
{
  f32 result =
    (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y) +
    (v1.z - v2.z)*(v1.z - v2.z) + (v1.w - v2.w)*(v1.w - v2.w);
  return result;
}

function Vector4
vector4_scale(Vector4 v, f32 scale)
{
  Vector4 result = { v.x*scale, v.y*scale, v.z*scale, v.w*scale };
  return result;
}

function Vector4
vector4_mul(Vector4 v1, Vector4 v2)
{
  Vector4 result = { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w };
  return result;
}

function Vector4
vector4_negate(Vector4 v)
{
  Vector4 result = { -v.x, -v.y, -v.z, -v.w };
  return result;
}

function Vector4
vector4_div(Vector4 v1, Vector4 v2)
{
  Vector4 result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w };
  return result;
}

function Vector4
vector4_normalize(Vector4 v)
{
  Vector4 result = { 0 };
  f32 length = sqrtf((v.x*v.x) + (v.y*v.y) + (v.z*v.z) + (v.w*v.w));
  if (length > 0)
  {
    f32 ilength = 1.0f/length;
    result.x = v.x*ilength;
    result.y = v.y*ilength;
    result.z = v.z*ilength;
    result.w = v.w*ilength;
  }
  return result;
}

function Vector4
vector4_lerp(Vector4 v1, Vector4 v2, f32 amount)
{
  Vector4 result = { 0 };
  result.x = v1.x + amount*(v2.x - v1.x);
  result.y = v1.y + amount*(v2.y - v1.y);
  result.z = v1.z + amount*(v2.z - v1.z);
  result.w = v1.w + amount*(v2.w - v1.w);
  return result;
}

function Vector4
vector4_move_towards(Vector4 v, Vector4 target, f32 maxDistance)
{
  Vector4 result = { 0 };
  f32 dx = target.x - v.x;
  f32 dy = target.y - v.y;
  f32 dz = target.z - v.z;
  f32 dw = target.w - v.w;
  f32 value = (dx*dx) + (dy*dy) + (dz*dz) + (dw*dw);
  if ((value == 0) || ((maxDistance >= 0) && (value <= maxDistance*maxDistance)))
  {
    return target;
  }
  f32 dist = sqrtf(value);
  result.x = v.x + dx/dist*maxDistance;
  result.y = v.y + dy/dist*maxDistance;
  result.z = v.z + dz/dist*maxDistance;
  result.w = v.w + dw/dist*maxDistance;
  return result;
}

function Vector4
vector4_invert(Vector4 v)
{
  Vector4 result = { 1.0f/v.x, 1.0f/v.y, 1.0f/v.z, 1.0f/v.w };
  return result;
}

function b32
vector4_equals(Vector4 p, Vector4 q)
{
  b32 result = ((fabsf(p.x - q.x)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
               ((fabsf(p.y - q.y)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) &&
               ((fabsf(p.z - q.z)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z))))) &&
               ((fabsf(p.w - q.w)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.w), fabsf(q.w)))));
  return result;
}

function f32
matrix4_determinant(Matrix4 mat)
{
  f32 result = 0.0f;

  // Cache the matrix values (speed optimization)
  f32 m0  = mat.m0,  m1  = mat.m1,  m2  = mat.m2,  m3  = mat.m3;
  f32 m4  = mat.m4,  m5  = mat.m5,  m6  = mat.m6,  m7  = mat.m7;
  f32 m8  = mat.m8,  m9  = mat.m9,  m10 = mat.m10, m11 = mat.m11;
  f32 m12 = mat.m12, m13 = mat.m13, m14 = mat.m14, m15 = mat.m15;

  result = (m0*((m5*(m10*m15 - m11*m14) - m9*(m6*m15 - m7*m14) + m13*(m6*m11 - m7*m10))) -
            m4*((m1*(m10*m15 - m11*m14) - m9*(m2*m15 - m3*m14) + m13*(m2*m11 - m3*m10))) +
            m8*((m1*(m6*m15 - m7*m14) - m5*(m2*m15 - m3*m14) + m13*(m2*m7 - m3*m6))) -
            m12*((m1*(m6*m11 - m7*m10) - m5*(m2*m11 - m3*m10) + m9*(m2*m7 - m3*m6))));

  return result;
}

function f32
matrix4_trace(Matrix4 mat)
{
  f32 result = (mat.m0 + mat.m5 + mat.m10 + mat.m15);
  return result;
}

function Matrix4
matrix4_transpose(Matrix4 mat)
{
  Matrix4 result = { 0 };

  result.m0 = mat.m0;
  result.m1 = mat.m4;
  result.m2 = mat.m8;
  result.m3 = mat.m12;
  result.m4 = mat.m1;
  result.m5 = mat.m5;
  result.m6 = mat.m9;
  result.m7 = mat.m13;
  result.m8 = mat.m2;
  result.m9 = mat.m6;
  result.m10 = mat.m10;
  result.m11 = mat.m14;
  result.m12 = mat.m3;
  result.m13 = mat.m7;
  result.m14 = mat.m11;
  result.m15 = mat.m15;

  return result;
}

// Invert provided matrix
function Matrix4
matrix4_invert(Matrix4 mat)
{
  Matrix4 result = { 0 };

  // Cache the matrix values (speed optimization)
  f32 a00 = mat.m0, a01 = mat.m1, a02 = mat.m2, a03 = mat.m3;
  f32 a10 = mat.m4, a11 = mat.m5, a12 = mat.m6, a13 = mat.m7;
  f32 a20 = mat.m8, a21 = mat.m9, a22 = mat.m10, a23 = mat.m11;
  f32 a30 = mat.m12, a31 = mat.m13, a32 = mat.m14, a33 = mat.m15;

  f32 b00 = a00*a11 - a01*a10;
  f32 b01 = a00*a12 - a02*a10;
  f32 b02 = a00*a13 - a03*a10;
  f32 b03 = a01*a12 - a02*a11;
  f32 b04 = a01*a13 - a03*a11;
  f32 b05 = a02*a13 - a03*a12;
  f32 b06 = a20*a31 - a21*a30;
  f32 b07 = a20*a32 - a22*a30;
  f32 b08 = a20*a33 - a23*a30;
  f32 b09 = a21*a32 - a22*a31;
  f32 b10 = a21*a33 - a23*a31;
  f32 b11 = a22*a33 - a23*a32;

  // Calculate the invert determinant (inlined to avoid double-caching)
  f32 invDet = 1.0f/(b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);

  result.m0 = (a11*b11 - a12*b10 + a13*b09)*invDet;
  result.m1 = (-a01*b11 + a02*b10 - a03*b09)*invDet;
  result.m2 = (a31*b05 - a32*b04 + a33*b03)*invDet;
  result.m3 = (-a21*b05 + a22*b04 - a23*b03)*invDet;
  result.m4 = (-a10*b11 + a12*b08 - a13*b07)*invDet;
  result.m5 = (a00*b11 - a02*b08 + a03*b07)*invDet;
  result.m6 = (-a30*b05 + a32*b02 - a33*b01)*invDet;
  result.m7 = (a20*b05 - a22*b02 + a23*b01)*invDet;
  result.m8 = (a10*b10 - a11*b08 + a13*b06)*invDet;
  result.m9 = (-a00*b10 + a01*b08 - a03*b06)*invDet;
  result.m10 = (a30*b04 - a31*b02 + a33*b00)*invDet;
  result.m11 = (-a20*b04 + a21*b02 - a23*b00)*invDet;
  result.m12 = (-a10*b09 + a11*b07 - a12*b06)*invDet;
  result.m13 = (a00*b09 - a01*b07 + a02*b06)*invDet;
  result.m14 = (-a30*b03 + a31*b01 - a32*b00)*invDet;
  result.m15 = (a20*b03 - a21*b01 + a22*b00)*invDet;

  return result;
}

function Matrix4
matrix4_add(Matrix4 left, Matrix4 right)
{
  Matrix4 result = { 0 };

  result.m0 = left.m0 + right.m0;
  result.m1 = left.m1 + right.m1;
  result.m2 = left.m2 + right.m2;
  result.m3 = left.m3 + right.m3;
  result.m4 = left.m4 + right.m4;
  result.m5 = left.m5 + right.m5;
  result.m6 = left.m6 + right.m6;
  result.m7 = left.m7 + right.m7;
  result.m8 = left.m8 + right.m8;
  result.m9 = left.m9 + right.m9;
  result.m10 = left.m10 + right.m10;
  result.m11 = left.m11 + right.m11;
  result.m12 = left.m12 + right.m12;
  result.m13 = left.m13 + right.m13;
  result.m14 = left.m14 + right.m14;
  result.m15 = left.m15 + right.m15;

  return result;
}

function Matrix4
matrix4_sub(Matrix4 left, Matrix4 right)
{
  Matrix4 result = { 0 };

  result.m0 = left.m0 - right.m0;
  result.m1 = left.m1 - right.m1;
  result.m2 = left.m2 - right.m2;
  result.m3 = left.m3 - right.m3;
  result.m4 = left.m4 - right.m4;
  result.m5 = left.m5 - right.m5;
  result.m6 = left.m6 - right.m6;
  result.m7 = left.m7 - right.m7;
  result.m8 = left.m8 - right.m8;
  result.m9 = left.m9 - right.m9;
  result.m10 = left.m10 - right.m10;
  result.m11 = left.m11 - right.m11;
  result.m12 = left.m12 - right.m12;
  result.m13 = left.m13 - right.m13;
  result.m14 = left.m14 - right.m14;
  result.m15 = left.m15 - right.m15;

  return result;
}

function Matrix4
matrix4_multiply(Matrix4 left, Matrix4 right)
{
  Matrix4 result = { 0 };

  result.m0 = left.m0*right.m0 + left.m1*right.m4 + left.m2*right.m8 + left.m3*right.m12;
  result.m1 = left.m0*right.m1 + left.m1*right.m5 + left.m2*right.m9 + left.m3*right.m13;
  result.m2 = left.m0*right.m2 + left.m1*right.m6 + left.m2*right.m10 + left.m3*right.m14;
  result.m3 = left.m0*right.m3 + left.m1*right.m7 + left.m2*right.m11 + left.m3*right.m15;
  result.m4 = left.m4*right.m0 + left.m5*right.m4 + left.m6*right.m8 + left.m7*right.m12;
  result.m5 = left.m4*right.m1 + left.m5*right.m5 + left.m6*right.m9 + left.m7*right.m13;
  result.m6 = left.m4*right.m2 + left.m5*right.m6 + left.m6*right.m10 + left.m7*right.m14;
  result.m7 = left.m4*right.m3 + left.m5*right.m7 + left.m6*right.m11 + left.m7*right.m15;
  result.m8 = left.m8*right.m0 + left.m9*right.m4 + left.m10*right.m8 + left.m11*right.m12;
  result.m9 = left.m8*right.m1 + left.m9*right.m5 + left.m10*right.m9 + left.m11*right.m13;
  result.m10 = left.m8*right.m2 + left.m9*right.m6 + left.m10*right.m10 + left.m11*right.m14;
  result.m11 = left.m8*right.m3 + left.m9*right.m7 + left.m10*right.m11 + left.m11*right.m15;
  result.m12 = left.m12*right.m0 + left.m13*right.m4 + left.m14*right.m8 + left.m15*right.m12;
  result.m13 = left.m12*right.m1 + left.m13*right.m5 + left.m14*right.m9 + left.m15*right.m13;
  result.m14 = left.m12*right.m2 + left.m13*right.m6 + left.m14*right.m10 + left.m15*right.m14;
  result.m15 = left.m12*right.m3 + left.m13*right.m7 + left.m14*right.m11 + left.m15*right.m15;

  return result;
}

function Matrix4
matrix4_translate(f32 x, f32 y, f32 z)
{
  Matrix4 result = { 1.0f, 0.0f, 0.0f, x,
                   0.0f, 1.0f, 0.0f, y,
                   0.0f, 0.0f, 1.0f, z,
                   0.0f, 0.0f, 0.0f, 1.0f };
  return result;
}

function Matrix4
matrix4_rotate(Vector3 axis, f32 angle) /* Angle in Radians */
{
  Matrix4 result = { 0 };

  f32 x = axis.x, y = axis.y, z = axis.z;

  f32 lengthSquared = x*x + y*y + z*z;

  if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f))
  {
    f32 ilength = 1.0f/sqrtf(lengthSquared);
    x *= ilength;
    y *= ilength;
    z *= ilength;
  }

  f32 sinres = sinf(angle);
  f32 cosres = cosf(angle);
  f32 t = 1.0f - cosres;

  result.m0 = x*x*t + cosres;
  result.m1 = y*x*t + z*sinres;
  result.m2 = z*x*t - y*sinres;
  result.m3 = 0.0f;

  result.m4 = x*y*t - z*sinres;
  result.m5 = y*y*t + cosres;
  result.m6 = z*y*t + x*sinres;
  result.m7 = 0.0f;

  result.m8 = x*z*t + y*sinres;
  result.m9 = y*z*t - x*sinres;
  result.m10 = z*z*t + cosres;
  result.m11 = 0.0f;

  result.m12 = 0.0f;
  result.m13 = 0.0f;
  result.m14 = 0.0f;
  result.m15 = 1.0f;

  return result;
}

function Matrix4
matrix4_rotate_X(f32 angle)
{
  Matrix4 result = matrix4_identity();
  f32 cosres = cosf(angle);
  f32 sinres = sinf(angle);
  result.m5 = cosres;
  result.m6 = sinres;
  result.m9 = -sinres;
  result.m10 = cosres;
  return result;
}

function Matrix4
matrix4_rotate_Y(f32 angle)
{
  Matrix4 result = matrix4_identity();
  f32 cosres = cosf(angle);
  f32 sinres = sinf(angle);
  result.m0 = cosres;
  result.m2 = -sinres;
  result.m8 = sinres;
  result.m10 = cosres;
  return result;
}

function Matrix4
matrix4_rotate_Z(f32 angle)
{
  Matrix4 result = matrix4_identity();
  f32 cosres = cosf(angle);
  f32 sinres = sinf(angle);
  result.m0 = cosres;
  result.m1 = sinres;
  result.m4 = -sinres;
  result.m5 = cosres;
  return result;
}

function Matrix4
matrix4_rotate_XYZ(Vector3 angle)
{
  Matrix4 result = matrix4_identity();
  f32 cosz = cosf(-angle.z);
  f32 sinz = sinf(-angle.z);
  f32 cosy = cosf(-angle.y);
  f32 siny = sinf(-angle.y);
  f32 cosx = cosf(-angle.x);
  f32 sinx = sinf(-angle.x);
  result.m0 = cosz*cosy;
  result.m1 = (cosz*siny*sinx) - (sinz*cosx);
  result.m2 = (cosz*siny*cosx) + (sinz*sinx);
  result.m4 = sinz*cosy;
  result.m5 = (sinz*siny*sinx) + (cosz*cosx);
  result.m6 = (sinz*siny*cosx) - (cosz*sinx);
  result.m8 = -siny;
  result.m9 = cosy*sinx;
  result.m10= cosy*cosx;
  return result;
}

function Matrix4
matrix4_rotate_ZYX(Vector3 angle)
{
  Matrix4 result = { 0 };
  f32 cz = cosf(angle.z);
  f32 sz = sinf(angle.z);
  f32 cy = cosf(angle.y);
  f32 sy = sinf(angle.y);
  f32 cx = cosf(angle.x);
  f32 sx = sinf(angle.x);
  result.m0 = cz*cy;
  result.m4 = cz*sy*sx - cx*sz;
  result.m8 = sz*sx + cz*cx*sy;
  result.m12 = 0;
  result.m1 = cy*sz;
  result.m5 = cz*cx + sz*sy*sx;
  result.m9 = cx*sz*sy - cz*sx;
  result.m13 = 0;
  result.m2 = -sy;
  result.m6 = cy*sx;
  result.m10 = cy*cx;
  result.m14 = 0;
  result.m3 = 0;
  result.m7 = 0;
  result.m11 = 0;
  result.m15 = 1;
  return result;
}

function Matrix4
matrix4_scale(f32 x, f32 y, f32 z)
{
  Matrix4 result = { x,    0.0f, 0.0f, 0.0f,
                   0.0f, y,    0.0f, 0.0f,
                   0.0f, 0.0f, z,    0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f };

  return result;
}

function Matrix4
matrix4_frustum(f64 left, f64 right, f64 bottom, f64 top, f64 near_plane, f64 far_plane)
{
  Matrix4 result = { 0 };
  f32 rl = (f32)(right - left);
  f32 tb = (f32)(top - bottom);
  f32 fn = (f32)(far_plane - near_plane);
  result.m0 = ((f32)near_plane*2.0f)/rl;
  result.m1 = 0.0f;
  result.m2 = 0.0f;
  result.m3 = 0.0f;
  result.m4 = 0.0f;
  result.m5 = ((f32)near_plane*2.0f)/tb;
  result.m6 = 0.0f;
  result.m7 = 0.0f;
  result.m8 = ((f32)right + (f32)left)/rl;
  result.m9 = ((f32)top + (f32)bottom)/tb;
  result.m10 = -((f32)far_plane + (f32)near_plane)/fn;
  result.m11 = -1.0f;
  result.m12 = 0.0f;
  result.m13 = 0.0f;
  result.m14 = -((f32)far_plane*(f32)near_plane*2.0f)/fn;
  result.m15 = 0.0f;
  return result;
}

function Matrix4
matrix4_perspective(f64 fovY, f64 aspect, f64 near_plane, f64 far_plane)
{
  Matrix4 result = { 0 };

  f64 top = near_plane*tan(fovY*0.5);
  f64 bottom = -top;
  f64 right = top*aspect;
  f64 left = -right;

  // matrix4_frustum(-right, right, -top, top, near, far);
  f32 rl = (f32)(right - left);
  f32 tb = (f32)(top - bottom);
  f32 fn = (f32)(far_plane - near_plane);

  result.m0 = ((f32)near_plane*2.0f)/rl;
  result.m5 = ((f32)near_plane*2.0f)/tb;
  result.m8 = ((f32)right + (f32)left)/rl;
  result.m9 = ((f32)top + (f32)bottom)/tb;
  result.m10 = -((f32)far_plane + (f32)near_plane)/fn;
  result.m11 = -1.0f;
  result.m14 = -((f32)far_plane*(f32)near_plane*2.0f)/fn;

  return result;
}

function Matrix4
matrix4_ortho(f64 left, f64 right, f64 bottom, f64 top, f64 nearPlane, f64 farPlane)
{
  Matrix4 result = { 0 };
  f32 rl = (f32)(right - left);
  f32 tb = (f32)(top - bottom);
  f32 fn = (f32)(farPlane - nearPlane);
  result.m0 = 2.0f/rl;
  result.m1 = 0.0f;
  result.m2 = 0.0f;
  result.m3 = 0.0f;
  result.m4 = 0.0f;
  result.m5 = 2.0f/tb;
  result.m6 = 0.0f;
  result.m7 = 0.0f;
  result.m8 = 0.0f;
  result.m9 = 0.0f;
  result.m10 = -2.0f/fn;
  result.m11 = 0.0f;
  result.m12 = -((f32)left + (f32)right)/rl;
  result.m13 = -((f32)top + (f32)bottom)/tb;
  result.m14 = -((f32)farPlane + (f32)nearPlane)/fn;
  result.m15 = 1.0f;
  return result;
}

function Matrix4
matrix4_look_at(Vector3 eye, Vector3 target, Vector3 up)
{
  Matrix4 result = { 0 };

  f32 length = 0.0f;
  f32 ilength = 0.0f;

  // vector3_sub(eye, target)
  Vector3 vz = { eye.x - target.x, eye.y - target.y, eye.z - target.z };

  // vector3_normalize(vz)
  Vector3 v = vz;
  length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
  if (length == 0.0f) length = 1.0f;
  ilength = 1.0f/length;
  vz.x *= ilength;
  vz.y *= ilength;
  vz.z *= ilength;

  // vector3_cross(up, vz)
  Vector3 vx = { up.y*vz.z - up.z*vz.y, up.z*vz.x - up.x*vz.z, up.x*vz.y - up.y*vz.x };

  // vector3_normalize(x)
  v = vx;
  length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
  if (length == 0.0f) length = 1.0f;
  ilength = 1.0f/length;
  vx.x *= ilength;
  vx.y *= ilength;
  vx.z *= ilength;

  // vector3_cross(vz, vx)
  Vector3 vy = { vz.y*vx.z - vz.z*vx.y, vz.z*vx.x - vz.x*vx.z, vz.x*vx.y - vz.y*vx.x };

  result.m0 = vx.x;
  result.m1 = vy.x;
  result.m2 = vz.x;
  result.m3 = 0.0f;
  result.m4 = vx.y;
  result.m5 = vy.y;
  result.m6 = vz.y;
  result.m7 = 0.0f;
  result.m8 = vx.z;
  result.m9 = vy.z;
  result.m10 = vz.z;
  result.m11 = 0.0f;
  result.m12 = -(vx.x*eye.x + vx.y*eye.y + vx.z*eye.z);   // vector3_dot(vx, eye)
  result.m13 = -(vy.x*eye.x + vy.y*eye.y + vy.z*eye.z);   // vector3_dot(vy, eye)
  result.m14 = -(vz.x*eye.x + vz.y*eye.y + vz.z*eye.z);   // vector3_dot(vz, eye)
  result.m15 = 1.0f;

  return result;
}

function Quaternion
quaternion_add(Quaternion q1, Quaternion q2)
{
  Quaternion result = {q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w};
  return result;
}

function Quaternion
quaternion_sub(Quaternion q1, Quaternion q2)
{
  Quaternion result = {q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w};
  return result;
}

function f32
quaternion_length(Quaternion q)
{
  f32 result = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
  return result;
}

function Quaternion
quaternion_normalize(Quaternion q)
{
  Quaternion result = { 0 };
  f32 length = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
  if (length == 0.0f)
  {
    length = 1.0f;
  }
  f32 ilength = 1.0f/length;
  result.x = q.x*ilength;
  result.y = q.y*ilength;
  result.z = q.z*ilength;
  result.w = q.w*ilength;
  return result;
}

function Quaternion
quaternion_invert(Quaternion q)
{
  Quaternion result = q;
  f32 lengthSq = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
  if (lengthSq != 0.0f)
  {
    f32 invLength = 1.0f/lengthSq;
    result.x *= -invLength;
    result.y *= -invLength;
    result.z *= -invLength;
    result.w *= invLength;
  }
  return result;
}

function Quaternion
quaternion_mul(Quaternion q1, Quaternion q2)
{
  Quaternion result = { 0 };
  f32 qax = q1.x, qay = q1.y, qaz = q1.z, qaw = q1.w;
  f32 qbx = q2.x, qby = q2.y, qbz = q2.z, qbw = q2.w;
  result.x = qax*qbw + qaw*qbx + qay*qbz - qaz*qby;
  result.y = qay*qbw + qaw*qby + qaz*qbx - qax*qbz;
  result.z = qaz*qbw + qaw*qbz + qax*qby - qay*qbx;
  result.w = qaw*qbw - qax*qbx - qay*qby - qaz*qbz;
  return result;
}

function Quaternion
quaternion_scale(Quaternion q, f32 mul)
{
  Quaternion result = { 0 };
  result.x = q.x*mul;
  result.y = q.y*mul;
  result.z = q.z*mul;
  result.w = q.w*mul;
  return result;
}

function Quaternion
quaternion_div(Quaternion q1, Quaternion q2)
{
  Quaternion result = { q1.x/q2.x, q1.y/q2.y, q1.z/q2.z, q1.w/q2.w };
  return result;
}

function Quaternion
quaternion_lerp(Quaternion q1, Quaternion q2, f32 amount)
{
  Quaternion result = { 0 };

  result.x = q1.x + amount*(q2.x - q1.x);
  result.y = q1.y + amount*(q2.y - q1.y);
  result.z = q1.z + amount*(q2.z - q1.z);
  result.w = q1.w + amount*(q2.w - q1.w);

  return result;
}

function Quaternion
quaternion_nlerp(Quaternion q1, Quaternion q2, f32 amount)
{
  Quaternion result = { 0 };

  // quaternion_lerp(q1, q2, amount)
  result.x = q1.x + amount*(q2.x - q1.x);
  result.y = q1.y + amount*(q2.y - q1.y);
  result.z = q1.z + amount*(q2.z - q1.z);
  result.w = q1.w + amount*(q2.w - q1.w);

  // quaternion_normalize(q);
  Quaternion q = result;
  f32 length = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
  if (length == 0.0f) length = 1.0f;
  f32 ilength = 1.0f/length;

  result.x = q.x*ilength;
  result.y = q.y*ilength;
  result.z = q.z*ilength;
  result.w = q.w*ilength;

  return result;
}

function Quaternion
quaternion_slerp(Quaternion q1, Quaternion q2, f32 amount)
{
  Quaternion result = { 0 };
  f32 cosHalfTheta = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
  if (cosHalfTheta < 0)
  {
    q2.x = -q2.x; q2.y = -q2.y; q2.z = -q2.z; q2.w = -q2.w;
    cosHalfTheta = -cosHalfTheta;
  }
  if (fabsf(cosHalfTheta) >= 1.0f)
  {
    result = q1;
  }
  else if (cosHalfTheta > 0.95f)
  {
    result = quaternion_nlerp(q1, q2, amount);
  }
  else
  {
    f32 halfTheta = acosf(cosHalfTheta);
    f32 sinHalfTheta = sqrtf(1.0f - cosHalfTheta*cosHalfTheta);

    if (fabsf(sinHalfTheta) < F32_EPSILON)
    {
      result.x = (q1.x*0.5f + q2.x*0.5f);
      result.y = (q1.y*0.5f + q2.y*0.5f);
      result.z = (q1.z*0.5f + q2.z*0.5f);
      result.w = (q1.w*0.5f + q2.w*0.5f);
    }
    else
    {
      f32 ratioA = sinf((1 - amount)*halfTheta)/sinHalfTheta;
      f32 ratioB = sinf(amount*halfTheta)/sinHalfTheta;

      result.x = (q1.x*ratioA + q2.x*ratioB);
      result.y = (q1.y*ratioA + q2.y*ratioB);
      result.z = (q1.z*ratioA + q2.z*ratioB);
      result.w = (q1.w*ratioA + q2.w*ratioB);
    }
  }

  return result;
}

function Quaternion
quaternion_cubic_hermite_spline(Quaternion q1, Quaternion out_tangent1, Quaternion q2, Quaternion in_tangent2, f32 t)
{
  f32 t2 = t*t;
  f32 t3 = t2*t;
  f32 h00 = 2*t3 - 3*t2 + 1;
  f32 h10 = t3 - 2*t2 + t;
  f32 h01 = -2*t3 + 3*t2;
  f32 h11 = t3 - t2;

  Quaternion p0 = quaternion_scale(q1, h00);
  Quaternion m0 = quaternion_scale(out_tangent1, h10);
  Quaternion p1 = quaternion_scale(q2, h01);
  Quaternion m1 = quaternion_scale(in_tangent2, h11);

  Quaternion result = { 0 };

  result = quaternion_add(p0, m0);
  result = quaternion_add(result, p1);
  result = quaternion_add(result, m1);
  result = quaternion_normalize(result);

  return result;
}

function Quaternion
quaternion_from_vector3_to_vector3(Vector3 from, Vector3 to)
{
  Quaternion result = { 0 };

  f32 cos2Theta = (from.x*to.x + from.y*to.y + from.z*to.z);  // Vector3DotProduct(from, to)
  Vector3 cross = { from.y*to.z - from.z*to.y, from.z*to.x - from.x*to.z, from.x*to.y - from.y*to.x }; // Vector3CrossProduct(from, to)

  result.x = cross.x;
  result.y = cross.y;
  result.z = cross.z;
  result.w = 1.0f + cos2Theta;

  // quaternion_normalize(q);
  // NOTE: Normalize to essentially nlerp the original and identity to 0.5
  Quaternion q = result;
  f32 length = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
  if (length == 0.0f) length = 1.0f;
  f32 ilength = 1.0f/length;

  result.x = q.x*ilength;
  result.y = q.y*ilength;
  result.z = q.z*ilength;
  result.w = q.w*ilength;

  return result;
}

function Quaternion
quaternion_from_matrix4(Matrix4 mat)
{
  Quaternion result = { 0 };

  f32 fourWSquaredMinus1 = mat.m0  + mat.m5 + mat.m10;
  f32 fourXSquaredMinus1 = mat.m0  - mat.m5 - mat.m10;
  f32 fourYSquaredMinus1 = mat.m5  - mat.m0 - mat.m10;
  f32 fourZSquaredMinus1 = mat.m10 - mat.m0 - mat.m5;

  int biggestIndex = 0;
  f32 fourBiggestSquaredMinus1 = fourWSquaredMinus1;
  if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
  {
    fourBiggestSquaredMinus1 = fourXSquaredMinus1;
    biggestIndex = 1;
  }

  if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
  {
    fourBiggestSquaredMinus1 = fourYSquaredMinus1;
    biggestIndex = 2;
  }

  if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
  {
    fourBiggestSquaredMinus1 = fourZSquaredMinus1;
    biggestIndex = 3;
  }

  f32 biggestVal = sqrtf(fourBiggestSquaredMinus1 + 1.0f)*0.5f;
  f32 mult = 0.25f/biggestVal;

  switch (biggestIndex)
  {
    case 0:
      result.w = biggestVal;
      result.x = (mat.m6 - mat.m9)*mult;
      result.y = (mat.m8 - mat.m2)*mult;
      result.z = (mat.m1 - mat.m4)*mult;
      break;
    case 1:
      result.x = biggestVal;
      result.w = (mat.m6 - mat.m9)*mult;
      result.y = (mat.m1 + mat.m4)*mult;
      result.z = (mat.m8 + mat.m2)*mult;
      break;
    case 2:
      result.y = biggestVal;
      result.w = (mat.m8 - mat.m2)*mult;
      result.x = (mat.m1 + mat.m4)*mult;
      result.z = (mat.m6 + mat.m9)*mult;
      break;
    case 3:
      result.z = biggestVal;
      result.w = (mat.m1 - mat.m4)*mult;
      result.x = (mat.m8 + mat.m2)*mult;
      result.y = (mat.m6 + mat.m9)*mult;
      break;
  }

  return result;
}

function Matrix4
quaternion_to_matrix4(Quaternion q)
{
  Matrix4 result = matrix4_identity();
  f32 a2 = q.x*q.x;
  f32 b2 = q.y*q.y;
  f32 c2 = q.z*q.z;
  f32 ac = q.x*q.z;
  f32 ab = q.x*q.y;
  f32 bc = q.y*q.z;
  f32 ad = q.w*q.x;
  f32 bd = q.w*q.y;
  f32 cd = q.w*q.z;
  result.m0 = 1 - 2*(b2 + c2);
  result.m1 = 2*(ab + cd);
  result.m2 = 2*(ac - bd);
  result.m4 = 2*(ab - cd);
  result.m5 = 1 - 2*(a2 + c2);
  result.m6 = 2*(bc + ad);
  result.m8 = 2*(ac + bd);
  result.m9 = 2*(bc - ad);
  result.m10 = 1 - 2*(a2 + b2);
  return result;
}

function Quaternion
quaternion_from_axis_angle(Vector3 axis, f32 angle)
{
  Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };

  f32 axisLength = sqrtf(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);

  if (axisLength != 0.0f)
  {
    angle *= 0.5f;

    f32 length = 0.0f;
    f32 ilength = 0.0f;

    // Vector3Normalize(axis)
    length = axisLength;
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f/length;
    axis.x *= ilength;
    axis.y *= ilength;
    axis.z *= ilength;

    f32 sinres = sinf(angle);
    f32 cosres = cosf(angle);

    result.x = axis.x*sinres;
    result.y = axis.y*sinres;
    result.z = axis.z*sinres;
    result.w = cosres;

    // quaternion_normalize(q);
    Quaternion q = result;
    length = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f/length;
    result.x = q.x*ilength;
    result.y = q.y*ilength;
    result.z = q.z*ilength;
    result.w = q.w*ilength;
  }

  return result;
}

function void
quaternion_to_axis_angle(Quaternion q, Vector3 *out_axis, f32 *out_angle)
{
  if (fabsf(q.w) > 1.0f)
  {
    // quaternion_normalize(q);
    f32 length = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    if (length == 0.0f)
    {
      length = 1.0f;
    }
    f32 ilength = 1.0f/length;

    q.x = q.x*ilength;
    q.y = q.y*ilength;
    q.z = q.z*ilength;
    q.w = q.w*ilength;
  }

  Vector3 resAxis = { 0.0f, 0.0f, 0.0f };
  f32 resAngle = 2.0f*acosf(q.w);
  f32 den = sqrtf(1.0f - q.w*q.w);

  if (den > F32_EPSILON)
  {
    resAxis.x = q.x/den;
    resAxis.y = q.y/den;
    resAxis.z = q.z/den;
  }
  else
  {
    // This occurs when the angle is zero.
    // Not a problem: just set an arbitrary normalized axis.
    resAxis.x = 1.0f;
  }

  *out_axis = resAxis;
  *out_angle = resAngle;
}

function Quaternion
quaternion_from_euler(f32 pitch, f32 yaw, f32 roll)
{
  Quaternion result = { 0 };
  f32 x0 = cosf(pitch*0.5f);
  f32 x1 = sinf(pitch*0.5f);
  f32 y0 = cosf(yaw*0.5f);
  f32 y1 = sinf(yaw*0.5f);
  f32 z0 = cosf(roll*0.5f);
  f32 z1 = sinf(roll*0.5f);
  result.x = x1*y0*z0 - x0*y1*z1;
  result.y = x0*y1*z0 + x1*y0*z1;
  result.z = x0*y0*z1 - x1*y1*z0;
  result.w = x0*y0*z0 + x1*y1*z1;
  return result;
}

function Vector3
quaternion_to_euler(Quaternion q)
{
  Vector3 result = { 0 };

  // Roll (x-axis rotation)
  f32 x0 = 2.0f*(q.w*q.x + q.y*q.z);
  f32 x1 = 1.0f - 2.0f*(q.x*q.x + q.y*q.y);
  result.x = atan2f(x0, x1);

  // Pitch (y-axis rotation)
  f32 y0 = 2.0f*(q.w*q.y - q.z*q.x);
  y0 = y0 > 1.0f ? 1.0f : y0;
  y0 = y0 < -1.0f ? -1.0f : y0;
  result.y = asinf(y0);

  // Yaw (z-axis rotation)
  f32 z0 = 2.0f*(q.w*q.z + q.x*q.y);
  f32 z1 = 1.0f - 2.0f*(q.y*q.y + q.z*q.z);
  result.z = atan2f(z0, z1);

  return result;
}

function Quaternion
quaternion_transform(Quaternion q, Matrix4 mat)
{
  Quaternion result = { 0 };

  result.x = mat.m0*q.x + mat.m4*q.y + mat.m8*q.z + mat.m12*q.w;
  result.y = mat.m1*q.x + mat.m5*q.y + mat.m9*q.z + mat.m13*q.w;
  result.z = mat.m2*q.x + mat.m6*q.y + mat.m10*q.z + mat.m14*q.w;
  result.w = mat.m3*q.x + mat.m7*q.y + mat.m11*q.z + mat.m15*q.w;
  return result;
}

function b32
quaternion_equals(Quaternion p, Quaternion q)
{
  b32 result = (((fabsf(p.x - q.x)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
                ((fabsf(p.y - q.y)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) &&
                ((fabsf(p.z - q.z)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z))))) &&
                ((fabsf(p.w - q.w)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.w), fabsf(q.w)))))) ||
               (((fabsf(p.x + q.x)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
                ((fabsf(p.y + q.y)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) &&
                ((fabsf(p.z + q.z)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z))))) &&
                ((fabsf(p.w + q.w)) <= (F32_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.w), fabsf(q.w))))));
  return result;
}


#endif // MATH_H
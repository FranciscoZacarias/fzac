#ifndef MATH_H
#define MATH_H

/* --------------------------------------------------------------------- */
/* Global qualifiers, constants & forward declarations                   */
/* --------------------------------------------------------------------- */

read_only global f32 Pi32      = 3.14159265358979323846f;
read_only global f32 Epsilon32 = 0.000001f;

#define Degrees(r) ((r) * (180.0f / PI))
#define Radians(d) ((d) * (PI / 180.0f))

typedef struct V2f32    V2f32;    /* */
typedef struct V3f32    V3f32;    /* */
typedef struct V4f32    V4f32;    /* */
typedef struct M4f32    M4f32;    /* Matrix type (OpenGL style 4x4 - right handed, column major) */
typedef struct Qf32     Qf32;     /* */
typedef struct Rangef32 Rangef32; /* */

/* --------------------------------------------------------------------- */
/* Float helpers                                                         */
/* --------------------------------------------------------------------- */

function b32 f32_equals(f32 x, f32 y);

/* --------------------------------------------------------------------- */
/* Vector 2 types                                                        */
/* --------------------------------------------------------------------- */
struct V2f32
{
  union
  {
    f32 v[2];
    struct
    {
      f32 x, y;
    };
  };
};

/* ----- V2f32 functions ----- */
#define v2f32(x,y)  (V2f32){(x),(y)}
function V2f32 v2f32_add(V2f32 v1, V2f32 v2); /* Adds two 2D vectors component-wise */
function V2f32 v2f32_sub(V2f32 v1, V2f32 v2); /* Subtracts the second 2D vector from the first component-wise */
function f32   v2f32_length(V2f32 v); /* Returns the Euclidean length (magnitude) of a 2D vector */
function f32   v2f32_length_squared(V2f32 v); /* Returns the squared length of a 2D vector (avoids sqrt) */
function f32   v2f32_dot(V2f32 v1, V2f32 v2); /* Computes the dot product of two 2D vectors */
function f32   v2f32_cross(V2f32 v1, V2f32 v2); /* Computes the scalar 2D cross product (v1.x*v2.y - v1.y*v2.x) */
function f32   v2f32_distance(V2f32 v1, V2f32 v2); /* Returns the distance between two 2D points */
function f32   v2f32_distance_squared(V2f32 v1, V2f32 v2); /* Returns the squared distance between two 2D points */
function V2f32 v2f32_scale(V2f32 v, f32 scale); /* Multiplies a 2D vector by a scalar */
function V2f32 v2f32_mul(V2f32 v1, V2f32 v2); /* Multiplies two 2D vectors component-wise */
function V2f32 v2f32_negate(V2f32 v); /* Negates a 2D vector */
function V2f32 v2f32_div(V2f32 v1, V2f32 v2); /* Divides two 2D vectors component-wise */
function V2f32 v2f32_normalize(V2f32 v); /* Returns a normalized (unit length) version of a 2D vector */
function V2f32 v2f32_transform(V2f32 v, M4f32 mat); /* Transforms a 2D vector by a 4x4 matrix */
function V2f32 v2f32_lerp(V2f32 v1, V2f32 v2, f32 amount); /* Linearly interpolates between two 2D vectors */
function V2f32 v2f32_reflect(V2f32 v, V2f32 normal); /* Reflects a 2D vector about a given normal */
function V2f32 v2f32_rotate(V2f32 v, f32 angle); /* Rotates a 2D vector by an angle in radians */
function V2f32 v2f32_move_towards(V2f32 v, V2f32 target, f32 max_distance); /* Moves a vector toward a target by a maximum distance */
function V2f32 v2f32_invert(V2f32 v); /* Inverts each component of a 2D vector (1/x, 1/y) */
function V2f32 v2f32_clamp(V2f32 v, V2f32 min, V2f32 max); /* Clamps each component of a 2D vector between min and max values */
function b32   v2f32_equals(V2f32 p, V2f32 q); /* Returns true if two 2D vectors are equal component-wise */
function V2f32 v2f32_refract(V2f32 v, V2f32 n, f32 r); /* Computes the direction of a refracted ray\n v: normalized direction of the incoming ray\n n: normalized normal vector of the interface of two optical media\n r: ratio of the refractive index of the medium from where the ray comes\n to the refractive index of the medium on the other side of the surface */

/* --------------------------------------------------------------------- */
/* Vector 3 types                                                        */
/* --------------------------------------------------------------------- */
struct V3f32
{
  union
  {
    f32 v[3];
    struct
    {
      f32 x, y, z;
    };
  };
};

/* ----- V3f32 functions ----- */
#define        v3f32(x,y,z) (V3f32){(x),(y),(z)}
#define        v3f32_zero() v3f32(0.0f,0.0f,0.0f)
function V3f32 v3f32_add(V3f32 v1, V3f32 v2); /* Adds two 3D vectors component-wise */
function V3f32 v3f32_sub(V3f32 v1, V3f32 v2); /* Subtracts the second 3D vector from the first component-wise */
function V3f32 v3f32_scale(V3f32 v, f32 scalar); /* Multiplies a 3D vector by a scalar */
function V3f32 v3f32_mul(V3f32 v1, V3f32 v2); /* Multiplies two 3D vectors component-wise */
function V3f32 v3f32_cross(V3f32 v1, V3f32 v2); /* Computes the cross product of two 3D vectors */
function f32   v3f32_length(V3f32 v); /* Returns the Euclidean length (magnitude) of a 3D vector */
function f32   v3f32_length_squared(V3f32 v); /* Returns the squared length of a 3D vector (avoids sqrt) */
function f32   v3f32_dot(V3f32 v1, V3f32 v2); /* Computes the dot product of two 3D vectors */
function f32   v3f32_distance(V3f32 v1, V3f32 v2); /* Returns the distance between two 3D points */
function f32   v3f32_distance_squared(V3f32 v1, V3f32 v2); /* Returns the squared distance between two 3D points */
function V3f32 v3f32_negate(V3f32 v); /* Negates a 3D vector */
function V3f32 v3f32_div(V3f32 v1, V3f32 v2); /* Divides two 3D vectors component-wise */
function V3f32 v3f32_normalize(V3f32 v); /* Returns a normalized (unit length) version of a 3D vector */
function V3f32 v3f32_project(V3f32 v1, V3f32 v2); /* Projects vector v1 onto vector v2 */
function V3f32 v3f32_reject(V3f32 v1, V3f32 v2); /* Calculates the rejection of the vector v1 on to v2 */
function void  v3f32_ortho_normalize(V3f32 *v1, V3f32 *v2); /* Orthonormalizes provided vectors.\n Makes vectors normalized and orthogonal to each other.\n Implements the Gram-Schmidt process */
function V3f32 v3f32_transform(V3f32 v, M4f32 mat); /* Transforms a 3D vector by a 4x4 matrix */
function V3f32 v3f32_rotate_by_qf32(V3f32 v, Qf32 q); /* Rotates a 3D vector using a quaternion */
function V3f32 v3f32_rotate_by_axis_angle(V3f32 v, V3f32 axis, f32 angle); /* Rotates a 3D vector around an axis by an angle in radians */
function V3f32 v3f32_move_towards(V3f32 v, V3f32 target, f32 max_distance); /* Moves a vector toward a target by a maximum distance */
function V3f32 v3f32_lerp(V3f32 v1, V3f32 v2, f32 amount); /* Linearly interpolates between two 3D vectors */
function V3f32 v3f32_cubic_hermite(V3f32 v1, V3f32 tangent1, V3f32 v2, V3f32 tangent2, f32 amount); /* Calculates cubic hermite interpolation between two vectors and their tangents,\n as described in the GLTF 2.0 specification:\n https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#interpolation-cubic */
function V3f32 v3f32_reflect(V3f32 v, V3f32 normal); /* Reflects a 3D vector about a given normal */
function V3f32 v3f32_barycenter(V3f32 p, V3f32 a, V3f32 b, V3f32 c); /* Computes barycenter coordinates (u, v, w) for point p with respect to triangle (a, b, c).\n Assumes p is on the plane of the triangle */
function V3f32 v3f32_unproject(V3f32 source, M4f32 projection, M4f32 view); /* Projects a V3f32 from screen space into world space */
function V3f32 v3f32_invert(V3f32 v); /* Inverts each component of a 3D vector (1/x, 1/y, 1/z) */
function V3f32 v3f32_clamp(V3f32 v, V3f32 min, V3f32 max); /* Clamps each component of a 3D vector between min and max values */
function b32   v3f32_equals(V3f32 p, V3f32 q); /* Returns true if two 3D vectors are equal component-wise */
function V3f32 v3f32_refract(V3f32 v, V3f32 n, f32 r); /* Computes the direction of a refracted ray\n v: normalized direction of the incoming ray\n n: normalized normal vector of the interface of two optical media\n r: ratio of the refractive index of the medium from where the ray comes\n to the refractive index of the medium on the other side of the surface */

/* --------------------------------------------------------------------- */
/* Vector 4 types                                                        */
/* --------------------------------------------------------------------- */
struct V4f32
{
  union
  {
    f32 v[4];
    struct
    {
      f32 x, y, z, w;
    };
  };
};

/* ----- V4f32 functions ----- */
#define        v4f32(x,y,z,w) (V4f32){(x),(y),(z),(w)}
#define        v4f32_zero()   v4f32(0.0f,0.0f,0.0f,0.0f)
function V4f32 v4f32_add(V4f32 v1, V4f32 v2); /* Adds two 4D vectors component-wise */
function V4f32 v4f32_sub(V4f32 v1, V4f32 v2); /* Subtracts the second 4D vector from the first component-wise */
function f32   v4f32_length(V4f32 v); /* Returns the Euclidean length (magnitude) of a 4D vector */
function f32   v4f32_length_squared(V4f32 v); /* Returns the squared length of a 4D vector (avoids sqrt) */
function f32   v4f32_dot(V4f32 v1, V4f32 v2); /* Computes the dot product of two 4D vectors */
function f32   v4f32_distance(V4f32 v1, V4f32 v2); /* Returns the distance between two 4D points */
function f32   v4f32_distance_squared(V4f32 v1, V4f32 v2); /* Returns the squared distance between two 4D points */
function V4f32 v4f32_scale(V4f32 v, f32 scale); /* Multiplies a 4D vector by a scalar */
function V4f32 v4f32_mul(V4f32 v1, V4f32 v2); /* Multiplies two 4D vectors component-wise */
function V4f32 v4f32_negate(V4f32 v); /* Negates a 4D vector */
function V4f32 v4f32_div(V4f32 v1, V4f32 v2); /* Divides two 4D vectors component-wise */
function V4f32 v4f32_normalize(V4f32 v); /* Returns a normalized (unit length) version of a 4D vector */
function V4f32 v4f32_lerp(V4f32 v1, V4f32 v2, f32 amount); /* Linearly interpolates between two 4D vectors */
function V4f32 v4f32_move_towards(V4f32 v, V4f32 target, f32 maxDistance); /* Moves a 4D vector toward a target by a maximum distance */
function V4f32 v4f32_invert(V4f32 v); /* Inverts each component of a 4D vector (1/x, 1/y, 1/z, 1/w) */
function b32   v4f32_equals(V4f32 p, V4f32 q); /* Returns true if two 4D vectors are equal component-wise */

/* --------------------------------------------------------------------- */
/* Matrix 4 types – Right-Handed, Column-Major                           */
/* --------------------------------------------------------------------- */
struct M4f32
{
  union
  {
    f32 v[16];
    f32 vv[4][4];
    struct
    {
      f32 m0,  m4,  m8,  m12;
      f32 m1,  m5,  m9,  m13;
      f32 m2,  m6,  m10, m14;
      f32 m3,  m7,  m11, m15;
    };
  };
};
#define m4f32(diagonal)            \
  (M4f32){ .v = {                  \
    (diagonal), 0.0f, 0.0f, 0.0f,  \
    0.0f, (diagonal), 0.0f, 0.0f,  \
    0.0f, 0.0f, (diagonal), 0.0f,  \
    0.0f, 0.0f, 0.0f, (diagonal) } }

/* ----- M4f32 functions ----- */
#define        m4f32_identity() m4f32(1.0f)
function f32   m4f32_determinant(M4f32 mat); /* Computes the determinant of a 4x4 matrix */
function f32   m4f32_trace(M4f32 mat); /* Computes the trace of a 4x4 matrix (sum of diagonal elements) */
function M4f32 m4f32_transpose(M4f32 mat); /* Returns the transpose of a 4x4 matrix */
function M4f32 m4f32_invert(M4f32 mat); /* Returns the inverse of a 4x4 matrix */
function M4f32 m4f32_add(M4f32 left, M4f32 right); /* Adds two 4x4 matrices component-wise */
function M4f32 m4f32_sub(M4f32 left, M4f32 right); /* Subtracts the right 4x4 matrix from the left component-wise */
function M4f32 m4f32_multiply(M4f32 left, M4f32 right); /* Multiplies two 4x4 matrices */
function M4f32 m4f32_translate(f32 x, f32 y, f32 z); /* Creates a translation matrix for the given x, y, z offsets */
function M4f32 m4f32_rotate(V3f32 axis, f32 angle); /* Creates a rotation matrix around a given axis by an angle in radians */
function M4f32 m4f32_rotate_X(f32 angle); /* Creates a rotation matrix around the X-axis by an angle in radians */
function M4f32 m4f32_rotate_Y(f32 angle); /* Creates a rotation matrix around the Y-axis by an angle in radians */
function M4f32 m4f32_rotate_Z(f32 angle); /* Creates a rotation matrix around the Z-axis by an angle in radians */
function M4f32 m4f32_rotate_XYZ(V3f32 angle); /* Creates a combined rotation matrix around the X, Y, and Z axes in order XYZ (angles in radians) */
function M4f32 m4f32_rotate_ZYX(V3f32 angle); /* Creates a combined rotation matrix around the Z, Y, and X axes in order ZYX (angles in radians) */
function M4f32 m4f32_scale(f32 x, f32 y, f32 z); /* Creates a scaling matrix with scale factors for each axis */
function M4f32 m4f32_frustum(f64 left, f64 right, f64 bottom, f64 top, f64 near_plane, f64 far_plane); /* Creates a perspective frustum projection matrix\n defined by the given left, right, bottom, top, near, and far planes */
function M4f32 m4f32_perspective(f64 fovY, f64 aspect, f64 near_plane, f64 far_plane); /* Creates a perspective projection matrix using a vertical field of view in radians\n with the given aspect ratio, near, and far planes */
function M4f32 m4f32_ortho(f64 left, f64 right, f64 bottom, f64 top, f64 nearPlane, f64 farPlane); /* Creates an orthographic projection matrix defined by the given planes */
function M4f32 m4f32_look_at(V3f32 eye, V3f32 target, V3f32 up); /* Creates a view matrix that looks from 'eye' toward 'target' using the 'up' direction */

/* --------------------------------------------------------------------- */
/* Quaternion types                                                      */
/* --------------------------------------------------------------------- */
struct Qf32
{
  union
  {
    f32 v[4];
    struct
    {
      f32 x, y, z, w;
    };
  };
};
#define qf32(x,y,z,w)  (Qf32){(x),(y),(z),(w)}
#define qf32_identity() qf32(0.0f,0.0f,0.0f,1.0f)

/* ----- Qf32 functions ----- */
function Qf32  qf32_add(Qf32 q1, Qf32 q2); /* Adds two quaternions component-wise */
function Qf32  qf32_sub(Qf32 q1, Qf32 q2); /* Subtracts the second quaternion from the first component-wise */
function f32   qf32_length(Qf32 q); /* Returns the length (magnitude) of a quaternion */
function Qf32  qf32_normalize(Qf32 q); /* Returns a normalized (unit length) quaternion */
function Qf32  qf32_invert(Qf32 q); /* Returns the inverse of a quaternion */
function Qf32  qf32_mul(Qf32 q1, Qf32 q2); /* Multiplies two quaternions (combines rotations) */
function Qf32  qf32_scale(Qf32 q, f32 mul); /* Scales each component of a quaternion by a scalar */
function Qf32  qf32_div(Qf32 q1, Qf32 q2); /* Divides one quaternion by another (q1 * inverse(q2)) */
function Qf32  qf32_lerp(Qf32 q1, Qf32 q2, f32 amount); /* Performs linear interpolation between two quaternions */
function Qf32  qf32_nlerp(Qf32 q1, Qf32 q2, f32 amount); /* Calculates normalized linear interpolation (nlerp) between two quaternions.\n Provides a faster approximation of slerp */
function Qf32  qf32_slerp(Qf32 q1, Qf32 q2, f32 amount); /* Calculates spherical linear interpolation (slerp) between two quaternions */
function Qf32  qf32_cubic_hermite_spline(Qf32 q1, Qf32 out_tangent1, Qf32 q2, Qf32 in_tangent2, f32 t); /* Calculates quaternion cubic spline interpolation using the Cubic Hermite Spline algorithm,\n as described in the GLTF 2.0 specification:\n https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#interpolation-cubic */
function Qf32  qf32_from_v3f32_to_v3f32(V3f32 from, V3f32 to); /* Creates a quaternion representing the rotation from one vector to another */
function Qf32  qf32_from_m4f32(M4f32 mat); /* Converts a rotation matrix to a quaternion */
function M4f32 qf32_to_m4f32(Qf32 q); /* Converts a quaternion to a rotation matrix */
function Qf32  qf32_from_axis_angle(V3f32 axis, f32 angle); /* Creates a quaternion representing a rotation around an axis by an angle in radians */
function void  qf32_to_axis_angle(Qf32 q, V3f32 *out_axis, f32 *out_angle); /* Extracts the rotation axis and angle (in radians) from a quaternion */
function Qf32  qf32_from_euler(f32 pitch, f32 yaw, f32 roll); /* Creates a quaternion equivalent to Euler angles (Z-Y-X rotation order)\n Angles must be provided in radians */
function V3f32 qf32_to_euler(Qf32 q); /* Converts a quaternion to Euler angles (roll, pitch, yaw)\n Returns angles in radians as a V3f32 struct */
function Qf32  qf32_transform(Qf32 q, M4f32 mat); /* Transforms a quaternion by a 4x4 matrix */
function b32   qf32_equals(Qf32 p, Qf32 q); /* Returns true if two quaternions are equal component-wise */

/* --------------------------------------------------------------------- */
/* Range types                                                           */
/* --------------------------------------------------------------------- */
struct Rangef32
{
  V2f32 top_left;
  V2f32 bot_right;
};

/* ----- Rangef32 functions ----- */
#define           rangef32(top_left,bot_right) (Rangef32){(top_left),(bot_right)}
function Rangef32 rangef32_intersection(Rangef32 a, Rangef32 b);
function b32      rangef32_contains_point(Rangef32 r, V2f32 p);
function b32      rangef32_contains_range(Rangef32 a, Rangef32 b); /* Is b completely inside a? */
function b32      rangef32_equal(Rangef32 a, Rangef32 b);

#endif /* MATH_H */
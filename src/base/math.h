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
typedef struct T2f32    T2f32;    /* */
typedef struct T3f32    T3f32;    /* */
typedef struct Rectf32  Rectf32;  /* */
typedef struct Rangef32 Rangef32; /* */

/* --------------------------------------------------------------------- */
/* Float helpers                                                         */
/* --------------------------------------------------------------------- */



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
#define v2f32(x,y)  (V2f32){(x),(y)}
#define v2f32_zero() v2f32(0.0f,0.0f)

/* ----- V2f32 functions ----- */
function V2f32 v2f32_add(V2f32 a, V2f32 b); /* Adds two 2D vectors component-wise */
function V2f32 v2f32_sub(V2f32 a, V2f32 b); /* Subtracts vector b from vector a */
function V2f32 v2f32_mul(V2f32 a, V2f32 b); /* Multiplies two 2D vectors component-wise */
function V2f32 v2f32_div(V2f32 a, V2f32 b); /* Divides two 2D vectors component-wise */
function f32   v2f32_dot(V2f32 a, V2f32 b); /* Returns the dot product of two 2D vectors */
function V2f32 v2f32_normalize(V2f32 v); /* Returns the normalized (unit length) version of a vector */
function V2f32 v2f32_transform(V2f32 v, M4f32 m); /* Transforms a 2D vector using a 4x4 matrix */
function f32   v2f32_len(V2f32 v); /* Returns the length (magnitude) of a vector */
function f32   v2f32_len_sqr(V2f32 v); /* Returns the squared length of a vector (avoids sqrt for efficiency) */
function f32   v2f32_cross(V2f32 a, V2f32 b); /* Returns the scalar 2D cross product (perpendicular magnitude) of two vectors */
function V2f32 v2f32_scale(V2f32 v, f32 scale); /* Scales a vector by a scalar value */
function V2f32 v2f32_lerp(V2f32 a, V2f32 b, f32 step); /* Linearly interpolates between vectors a and b by step (0–1) */
function f32   v2f32_dist(V2f32 a, V2f32 b); /* Returns the distance between two points/vectors */
function f32   v2f32_dist_sqr(V2f32 a, V2f32 b); /* Returns the squared distance between two points/vectors */
function f32   v2f32_angle(V2f32 a, V2f32 b); /* Signed angle from a to b, relative to (0,0). Coordinate system convention: Positive X: right, Positive Y: up, Positive Angle: CCW, Negative Angle: CW */
function V2f32 v2f32_reflect(V2f32 v, V2f32 normal); /* Reflects a vector around a surface normal */
function V2f32 v2f32_rotate(V2f32 v, f32 angle); /* Rotates a vector by a given angle (in radians) */
function b32   v2f32_equals(V2f32 a, V2f32 b); /* Returns true if two vectors are equal (component-wise comparison) */
function V2f32 v2f32_refract(V2f32 incident, V2f32 surface_normal, f32 refraction_ratio); /* Computes the refracted direction of an incident vector given a surface normal and refraction ratio (per Snell’s law) */

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
#define v3f32(x,y,z) (V3f32){(x),(y),(z)}
#define v3f32_zero() v3f32(0.0f,0.0f,0.0f)

/* ----- V3f32 functions ----- */
function V3f32 v3f32_add(V3f32 a, V3f32 b);
function V3f32 v3f32_sub(V3f32 a, V3f32 b);
function V3f32 v3f32_mul(V3f32 a, V3f32 b);
function V3f32 v3f32_div(V3f32 a, V3f32 b);
function V3f32 v3f32_scale(V3f32 v, f32 s);
function f32   v3f32_dot(V3f32 a, V3f32 b);
function V3f32 v3f32_cross(V3f32 a, V3f32 b);
function f32   v3f32_length_squared(V3f32 v);
function f32   v3f32_length(V3f32 v);
function V3f32 v3f32_normalize(V3f32 v);
function V3f32 v3f32_negate(V3f32 v);
function b32   v3f32_equal(V3f32 a, V3f32 b);

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
#define v4f32(x,y,z,w) (V4f32){(x),(y),(z),(w)}
#define v4f32_zero()   v4f32(0.0f,0.0f,0.0f,0.0f)

/* ----- V4f32 functions ----- */
function V4f32 v4f32_add(V4f32 a, V4f32 b);
function V4f32 v4f32_sub(V4f32 a, V4f32 b);
function V4f32 v4f32_mul(V4f32 a, V4f32 b);
function V4f32 v4f32_div(V4f32 a, V4f32 b);
function V4f32 v4f32_scale(V4f32 v, f32 s);
function f32   v4f32_dot(V4f32 a, V4f32 b);
function f32   v4f32_length_squared(V4f32 v);
function f32   v4f32_length(V4f32 v);
function V4f32 v4f32_normalize(V4f32 v);
function V4f32 v4f32_negate(V4f32 v);
function b32   v4f32_equal(V4f32 a, V4f32 b);

/* --------------------------------------------------------------------- */
/* Matrix 4 types – Column-Major                                         */
/* --------------------------------------------------------------------- */
struct M4f32
{
  union
  {
    f32 v[16];
    f32 vv[4][4];
    struct
    {
      f32 m0,  m4,  m8,  m12;  // Row 0: X-axis + translation X
      f32 m1,  m5,  m9,  m13;  // Row 1: Y-axis + translation Y
      f32 m2,  m6,  m10, m14;  // Row 2: Z-axis + translation Z
      f32 m3,  m7,  m11, m15;  // Row 3: perspective row
    };
  };
};
#define m4f32(diagonal)            \
  (M4f32){ .v = {                  \
    (diagonal), 0.0f, 0.0f, 0.0f,  \
    0.0f, (diagonal), 0.0f, 0.0f,  \
    0.0f, 0.0f, (diagonal), 0.0f,  \
    0.0f, 0.0f, 0.0f, (diagonal) } }
#define m4f32_identity()  m4f32(1.0f)

/* ----- M4f32 functions ----- */
function M4f32 m4f32_add(M4f32 a, M4f32 b);
function M4f32 m4f32_sub(M4f32 a, M4f32 b);
function M4f32 m4f32_mul(M4f32 a, M4f32 b);
function M4f32 m4f32_mul_vector(M4f32 m, V4f32 v);
function V4f32 m4f32_mul_vector_transpose(V4f32 v, M4f32 m);
function M4f32 m4f32_scale(M4f32 m, f32 s);
function M4f32 m4f32_transpose(M4f32 m);
function f32   m4f32_determinant(M4f32 m);
function M4f32 m4f32_inverse(M4f32 m);
function b32   m4f32_equal(M4f32 a, M4f32 b);

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
function Qf32  qf32_add(Qf32 a, Qf32 b);
function Qf32  qf32_sub(Qf32 a, Qf32 b);
function Qf32  qf32_mul(Qf32 a, Qf32 b);
function Qf32  qf32_scale(Qf32 q, f32 s);
function f32   qf32_dot(Qf32 a, Qf32 b);
function f32   qf32_length_squared(Qf32 q);
function f32   qf32_length(Qf32 q);
function Qf32  qf32_normalize(Qf32 q);
function Qf32  qf32_conjugate(Qf32 q);
function Qf32  qf32_inverse(Qf32 q);
function V3f32 qf32_rotate_vector(Qf32 q, V3f32 v);
function b32   qf32_equal(Qf32 a, Qf32 b);

/* --------------------------------------------------------------------- */
/* Transform 2 types                                                     */
/* --------------------------------------------------------------------- */

struct T2f32
{
  V2f32 translation;
  f32   rotation;
  V2f32 scale;
};
#define t2f32(t,r,s) (T2f32){(t),(r),(s)}
#define t2f32_zero() (T2f32){v2f32_zero(),0.0f,v2f32_zero()}

/* ----- T2f32 functions ----- */
function T2f32 t2f32_combine(T2f32 a, T2f32 b);
function V2f32 t2f32_apply(T2f32 t, V2f32 v);
function T2f32 t2f32_inverse(T2f32 t);
function b32   t2f32_equal(T2f32 a, T2f32 b);

/* --------------------------------------------------------------------- */
/* Transform 3 types                                                     */
/* --------------------------------------------------------------------- */
struct T3f32
{
  V3f32 translation;
  Qf32 rotation;
  V3f32 scale;
};
#define t3f32(t,r,s) (T3f32){(t),(r),(s)}
#define t3f32_zero() (T3f32){v3f32_zero(),qf32_identity(),v3f32_zero()}

/* ----- T3f32 functions ----- */
function T3f32 t3f32_combine(T3f32 a, T3f32 b);
function V3f32 t3f32_apply(T3f32 t, V3f32 v);
function T3f32 t3f32_inverse(T3f32 t);
function M4f32 t3f32_to_mat4(T3f32 t);
function b32   t3f32_equal(T3f32 a, T3f32 b);

function void t3f32_tests();

/* --------------------------------------------------------------------- */
/* Rect types                                                            */
/* --------------------------------------------------------------------- */
struct Rectf32
{
  V2f32 top_left;
  V2f32 size;
};
#define rectf32(top_left,size) (Rectf32){(top_left),(size)}

/* ----- Rectf32 functions ----- */
function Rectf32 rectf32_union(Rectf32 a, Rectf32 b);
function Rectf32 rectf32_intersection(Rectf32 a, Rectf32 b);
function b32     rectf32_contains_point(Rectf32 r, V2f32 p);
function b32     rectf32_contains_rect(Rectf32 a, Rectf32 b);
function b32     rectf32_equal(Rectf32 a, Rectf32 b);

/* --------------------------------------------------------------------- */
/* Range types                                                           */
/* --------------------------------------------------------------------- */
struct Rangef32
{
  V2f32 top_left;
  V2f32 bot_right;
};
#define rangef32(top_left,bot_right) (Rangef32){(top_left),(bot_right)}

/* ----- Rangef32 functions ----- */
function Rangef32 rangef32_union(Rangef32 a, Rangef32 b);
function Rangef32 rangef32_intersection(Rangef32 a, Rangef32 b);
function b32      rangef32_contains_point(Rangef32 r, V2f32 p);
function b32      rangef32_contains_range(Rangef32 a, Rangef32 b);
function b32      rangef32_equal(Rangef32 a, Rangef32 b);

#endif /* MATH_H */
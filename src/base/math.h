#ifndef MATH_H
#define MATH_H

/* --------------------------------------------------------------------- */
/* Global qualifiers & constants                                         */
/* --------------------------------------------------------------------- */

read_only global f32 PI       = 3.14159265358979323846f;
read_only global f32 EPSILON  = 0.000001f;

#define Degrees(r) ((r) * (180.0f / PI))
#define Radians(d) ((d) * (PI / 180.0f))

/* --------------------------------------------------------------------- */
/* Vector 2 types                                                        */
/* --------------------------------------------------------------------- */

typedef struct Vec2f32 Vec2f32;
struct Vec2f32
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
#define vec2f32(x,y)      (Vec2f32){(x),(y)}
#define vec2f32_zero()    vec2f32(0.0f,0.0f)

/* ----- Vec2f32 functions ----- */
function Vec2f32 vec2f32_add(Vec2f32 a, Vec2f32 b);
function Vec2f32 vec2f32_subtract(Vec2f32 a, Vec2f32 b);
function Vec2f32 vec2f32_multiply(Vec2f32 a, Vec2f32 b);
function Vec2f32 vec2f32_divide(Vec2f32 a, Vec2f32 b);
function Vec2f32 vec2f32_scale(Vec2f32 v, f32 s);
function f32     vec2f32_dot(Vec2f32 a, Vec2f32 b);
function f32     vec2f32_length_squared(Vec2f32 v);
function f32     vec2f32_length(Vec2f32 v);
function Vec2f32 vec2f32_normalize(Vec2f32 v);
function Vec2f32 vec2f32_negate(Vec2f32 v);
function b32     vec2f32_equal(Vec2f32 a, Vec2f32 b);

function void    vec2f32_tests();

/* --------------------------------------------------------------------- */
/* Vector 3 types                                                        */
/* --------------------------------------------------------------------- */
typedef struct Vec3f32 Vec3f32;
struct Vec3f32
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
#define vec3f32(x,y,z)    (Vec3f32){(x),(y),(z)}
#define vec3f32_zero()    vec3f32(0.0f,0.0f,0.0f)

/* ----- Vec3f32 functions ----- */
function Vec3f32 vec3f32_add(Vec3f32 a, Vec3f32 b);
function Vec3f32 vec3f32_subtract(Vec3f32 a, Vec3f32 b);
function Vec3f32 vec3f32_multiply(Vec3f32 a, Vec3f32 b);
function Vec3f32 vec3f32_divide(Vec3f32 a, Vec3f32 b);
function Vec3f32 vec3f32_scale(Vec3f32 v, f32 s);
function f32     vec3f32_dot(Vec3f32 a, Vec3f32 b);
function Vec3f32 vec3f32_cross(Vec3f32 a, Vec3f32 b);
function f32     vec3f32_length_squared(Vec3f32 v);
function f32     vec3f32_length(Vec3f32 v);
function Vec3f32 vec3f32_normalize(Vec3f32 v);
function Vec3f32 vec3f32_negate(Vec3f32 v);
function b32     vec3f32_equal(Vec3f32 a, Vec3f32 b);

function void    vec3f32_tests();

/* --------------------------------------------------------------------- */
/* Vector 4 types                                                        */
/* --------------------------------------------------------------------- */
typedef struct Vec4f32 Vec4f32;
struct Vec4f32
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
#define vec4f32(x,y,z,w)  (Vec4f32){(x),(y),(z),(w)}
#define vec4f32_zero()    vec4f32(0.0f,0.0f,0.0f,0.0f)

/* ----- Vec4f32 functions ----- */
function Vec4f32 vec4f32_add(Vec4f32 a, Vec4f32 b);
function Vec4f32 vec4f32_subtract(Vec4f32 a, Vec4f32 b);
function Vec4f32 vec4f32_multiply(Vec4f32 a, Vec4f32 b);
function Vec4f32 vec4f32_divide(Vec4f32 a, Vec4f32 b);
function Vec4f32 vec4f32_scale(Vec4f32 v, f32 s);
function f32     vec4f32_dot(Vec4f32 a, Vec4f32 b);
function f32     vec4f32_length_squared(Vec4f32 v);
function f32     vec4f32_length(Vec4f32 v);
function Vec4f32 vec4f32_normalize(Vec4f32 v);
function Vec4f32 vec4f32_negate(Vec4f32 v);
function b32     vec4f32_equal(Vec4f32 a, Vec4f32 b);

function void    vec4f32_tests();

/* --------------------------------------------------------------------- */
/* Matrix 3 types – Column-Major                                         */
/* --------------------------------------------------------------------- */
typedef struct Mat3f32 Mat3f32;
struct Mat3f32
{
  union
  {
    f32 v[9];
    f32 vv[3][3];
    struct
    {
      f32 m0, m1, m2,
          m3, m4, m5,
          m6, m7, m8;
    };
  };
};
#define mat3f32(diagonal)                                 \
  (Mat3f32){ .v = {                                       \
    (diagonal), 0.0f, 0.0f,                               \
    0.0f, (diagonal), 0.0f,                               \
    0.0f, 0.0f, (diagonal) } }
#define mat3f32_identity()  mat3f32(1.0f)

/* ----- Mat3f32 functions ----- */
function Mat3f32 mat3f32_add(Mat3f32 a, Mat3f32 b);
function Mat3f32 mat3f32_subtract(Mat3f32 a, Mat3f32 b);
function Mat3f32 mat3f32_multiply(Mat3f32 a, Mat3f32 b);
function Mat3f32 mat3f32_multiply_vector(Mat3f32 m, Vec3f32 v);
function Vec3f32 mat3f32_multiply_vector_transpose(Vec3f32 v, Mat3f32 m);
function Mat3f32 mat3f32_scale(Mat3f32 m, f32 s);
function Mat3f32 mat3f32_transpose(Mat3f32 m);
function f32     mat3f32_determinant(Mat3f32 m);
function Mat3f32 mat3f32_inverse(Mat3f32 m);
function b32     mat3f32_equal(Mat3f32 a, Mat3f32 b);

function void    mat3f32_tests();

/* --------------------------------------------------------------------- */
/* Matrix 4 types – Column-Major                                         */
/* --------------------------------------------------------------------- */
typedef struct Mat4f32 Mat4f32;
struct Mat4f32
{
  union
  {
    f32 v[16];
    f32 vv[4][4];
    struct
    {
      f32 m0, m1, m2, m3,
          m4, m5, m6, m7,
          m8, m9, m10, m11,
          m12, m13, m14, m15;
    };
  };
};
#define mat4f32(diagonal)                                 \
  (Mat4f32){ .v = {                                       \
    (diagonal), 0.0f, 0.0f, 0.0f,                         \
    0.0f, (diagonal), 0.0f, 0.0f,                         \
    0.0f, 0.0f, (diagonal), 0.0f,                         \
    0.0f, 0.0f, 0.0f, (diagonal) } }
#define mat4f32_identity()  mat4f32(1.0f)

/* ----- Mat4f32 functions ----- */
function Mat4f32 mat4f32_add(Mat4f32 a, Mat4f32 b);
function Mat4f32 mat4f32_subtract(Mat4f32 a, Mat4f32 b);
function Mat4f32 mat4f32_multiply(Mat4f32 a, Mat4f32 b);
function Mat4f32 mat4f32_multiply_vector(Mat4f32 m, Vec4f32 v);
function Vec4f32 mat4f32_multiply_vector_transpose(Vec4f32 v, Mat4f32 m);
function Mat4f32 mat4f32_scale(Mat4f32 m, f32 s);
function Mat4f32 mat4f32_transpose(Mat4f32 m);
function f32     mat4f32_determinant(Mat4f32 m);
function Mat4f32 mat4f32_inverse(Mat4f32 m);
function b32     mat4f32_equal(Mat4f32 a, Mat4f32 b);

function void    mat4f32_tests();

/* --------------------------------------------------------------------- */
/* Quaternion types                                                      */
/* --------------------------------------------------------------------- */
typedef struct Quatf32 Quatf32;
struct Quatf32
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
#define quatf32(x,y,z,w)  (Quatf32){(x),(y),(z),(w)}
#define quatf32_identity() quatf32(0.0f,0.0f,0.0f,1.0f)

/* ----- Quatf32 functions ----- */
function Quatf32 quatf32_add(Quatf32 a, Quatf32 b);
function Quatf32 quatf32_subtract(Quatf32 a, Quatf32 b);
function Quatf32 quatf32_multiply(Quatf32 a, Quatf32 b);
function Quatf32 quatf32_scale(Quatf32 q, f32 s);
function f32     quatf32_dot(Quatf32 a, Quatf32 b);
function f32     quatf32_length_squared(Quatf32 q);
function f32     quatf32_length(Quatf32 q);
function Quatf32 quatf32_normalize(Quatf32 q);
function Quatf32 quatf32_conjugate(Quatf32 q);
function Quatf32 quatf32_inverse(Quatf32 q);
function Vec3f32 quatf32_rotate_vector(Quatf32 q, Vec3f32 v);
function b32     quatf32_equal(Quatf32 a, Quatf32 b);

function void    quatf32_tests();

/* --------------------------------------------------------------------- */
/* Transform 2 types                                                     */
/* --------------------------------------------------------------------- */

typedef struct Transform2f32 Transform2f32;
struct Transform2f32
{
  Vec2f32 translation;
  f32     rotation;
  Vec2f32 scale;
};
#define transform2f32(t,r,s) (Transform2f32){(t),(r),(s)}

/* ----- Transform2f32 functions ----- */
function Transform2f32 transform2f32_combine(Transform2f32 a, Transform2f32 b);
function Vec2f32       transform2f32_apply(Transform2f32 t, Vec2f32 v);
function Transform2f32 transform2f32_inverse(Transform2f32 t);
function b32           transform2f32_equal(Transform2f32 a, Transform2f32 b);

function void          transform2f32_tests();

/* --------------------------------------------------------------------- */
/* Transform 3 types                                                     */
/* --------------------------------------------------------------------- */
typedef struct Transform3f32 Transform3f32;
struct Transform3f32
{
  Vec3f32 translation;
  Quatf32 rotation;
  Vec3f32 scale;
};
#define transform3f32(t,r,s) (Transform3f32){(t),(r),(s)}

/* ----- Transform3f32 functions ----- */
function Transform3f32 transform3f32_combine(Transform3f32 a, Transform3f32 b);
function Vec3f32       transform3f32_apply(Transform3f32 t, Vec3f32 v);
function Transform3f32 transform3f32_inverse(Transform3f32 t);
function Mat4f32       transform3f32_to_mat4(Transform3f32 t);
function b32           transform3f32_equal(Transform3f32 a, Transform3f32 b);

function void          transform3f32_tests();

/* --------------------------------------------------------------------- */
/* Rect types                                                            */
/* --------------------------------------------------------------------- */
typedef struct Rectf32 Rectf32;
struct Rectf32
{
  Vec2f32 top_left;
  Vec2f32 size;
};
#define rectf32(top_left,size) (Rectf32){(top_left),(size)}

/* ----- Rectf32 functions ----- */
function Rectf32 rectf32_union(Rectf32 a, Rectf32 b);
function Rectf32 rectf32_intersection(Rectf32 a, Rectf32 b);
function b32     rectf32_contains_point(Rectf32 r, Vec2f32 p);
function b32     rectf32_contains_rect(Rectf32 a, Rectf32 b);
function b32     rectf32_equal(Rectf32 a, Rectf32 b);

function void    rectf32_tests();

/* --------------------------------------------------------------------- */
/* Range types                                                           */
/* --------------------------------------------------------------------- */
typedef struct Rangef32 Rangef32;
struct Rangef32
{
  Vec2f32 top_left;
  Vec2f32 bot_right;
};
#define rangef32(top_left,bot_right) (Rangef32){(top_left),(bot_right)}

/* ----- Rangef32 functions ----- */
function Rangef32 rangef32_union(Rangef32 a, Rangef32 b);
function Rangef32 rangef32_intersection(Rangef32 a, Rangef32 b);
function b32      rangef32_contains_point(Rangef32 r, Vec2f32 p);
function b32      rangef32_contains_range(Rangef32 a, Rangef32 b);
function b32      rangef32_equal(Rangef32 a, Rangef32 b);

function void     rangef32_tests();

#endif /* MATH_H */
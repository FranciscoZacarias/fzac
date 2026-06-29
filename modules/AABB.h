#ifndef FZ_AABB_H
#define FZ_AABB_H

typedef struct AABB AABB;
struct AABB
{
  V3f32 min; // smallest coordinate along each axis
  V3f32 max; // largest coordinate along each axis
};
#define aabb(min,max) (AABB){ min, max }
#define aabb_const(min,max) { min, max }

function b8
aabb_is_point_inside(AABB *box, V3f32 point)
{
  b8 result = point.x >= box->min.x && point.x <= box->max.x &&
              point.y >= box->min.y && point.y <= box->max.y &&
              point.z >= box->min.z && point.z <= box->max.z;
  return result;
}

function b8
aabb_intersect(AABB *box_a, AABB box_b)
{
  b8 result = box_a.min.x <= box_b.max.x && box_a.max.x >= box_b.min.x &&
              box_a.min.y <= box_b.max.y && box_a.max.y >= box_b.min.y &&
              box_a.min.z <= box_b.max.z && box_a.max.z >= box_b.min.z;
  return result;
}

#endif // FZ_AABB_H

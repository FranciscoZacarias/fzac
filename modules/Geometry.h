#ifndef FZ_GEOMETRY_H
#define FZ_GEOMETRY_H

typedef struct Ray Ray;
struct Ray
{
  V3f32 begin;
  V3f32 end;
};

typedef struct Box Box;
struct Box
{
  V3f32 min; // smallest coordinate along each axis
  V3f32 max; // largest coordinate along each axis
};
#define box(min,max) (Box){ min, max }
#define box_const(min,max) { min, max }

function V3f32 box_get_dimensions(Box *box);
function b8    box_is_point_inside(Box *box, V3f32 point);
function b8    box_intersect(Box *box_a, Box box_b);

// @Section: Implementation

function V3f32
box_get_dimensions(Box *box)
{
  V3f32 result;
  result.x = box->max.x - box->min.x;
  result.y = box->max.y - box->min.y;
  result.z = box->max.z - box->min.z;
  return result;
}

function b8
box_is_point_inside(Box *box, V3f32 point)
{
  b8 result = point.x >= box->min.x && point.x <= box->max.x &&
              point.y >= box->min.y && point.y <= box->max.y &&
              point.z >= box->min.z && point.z <= box->max.z;
  return result;
}

function b8
box_intersect(Box *box_a, Box box_b)
{
  b8 result = box_a->min.x <= box_b.max.x && box_a->max.x >= box_b.min.x &&
              box_a->min.y <= box_b.max.y && box_a->max.y >= box_b.min.y &&
              box_a->min.z <= box_b.max.z && box_a->max.z >= box_b.min.z;
  return result;
}

#endif // FZ_GEOMETRY_H

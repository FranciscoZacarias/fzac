#ifndef FZAC_GEOMETRY_H
#define FZAC_GEOMETRY_H

#if FZ_RAYLIB
# error "Geometry.h is not supported with Base_Raylib.h yet"
#endif

typedef struct Line_Segment Line_Segment;
struct Line_Segment
{
  Vector3 begin;
  Vector3 end;
};

typedef struct Box Box;
struct Box
{
  Vector3 min; // smallest coordinate along each axis
  Vector3 max; // largest coordinate along each axis
};
#define box(min,max) (Box){ min, max }
#define box_const(min,max) { min, max }

fz_internal Vector3 box_get_dimensions(Box *box);
fz_internal b8    box_is_point_inside(Box *box, Vector3 point);
fz_internal b8    box_intersect(Box *box_a, Box box_b);

// @Section: Implementation

fz_internal Vector3
box_get_dimensions(Box *box)
{
  Vector3 result;
  result.x = box->max.x - box->min.x;
  result.y = box->max.y - box->min.y;
  result.z = box->max.z - box->min.z;
  return result;
}

fz_internal b8
box_is_point_inside(Box *box, Vector3 point)
{
  b8 result = point.x >= box->min.x && point.x <= box->max.x &&
              point.y >= box->min.y && point.y <= box->max.y &&
              point.z >= box->min.z && point.z <= box->max.z;
  return result;
}

fz_internal b8
box_intersect(Box *box_a, Box box_b)
{
  b8 result = box_a->min.x <= box_b.max.x && box_a->max.x >= box_b.min.x &&
              box_a->min.y <= box_b.max.y && box_a->max.y >= box_b.min.y &&
              box_a->min.z <= box_b.max.z && box_a->max.z >= box_b.min.z;
  return result;
}

#endif // FZAC_GEOMETRY_H

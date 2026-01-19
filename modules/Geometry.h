#ifndef GEOMETRY_H
#define GEOMETRY_H

// @Section: 2D Geometry

typedef struct Rect Rect;
struct Rect
{
  Vector2 tl; /* Top left */
  Vector2 br; /* Bottom right */
};

typedef struct Rect_Center Rect_Center;
struct Rect_Center
{
  Vector2 center;
  Vector2 half;
};

typedef struct Circle Circle;
struct Circle
{
  Vector2 center;
  f32 radius;
};

#endif // GEOMETRY_H
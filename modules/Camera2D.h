#ifndef CAMERA2D_H
#define CAMERA2D_H

#if FZ_RAYLIB
# error "Camera2D.h is replaced by raylib in Base_Raylib.h"
#endif

typedef struct Camera2D Camera2D;
struct Camera2D
{
  Vector2 position;
  f32 zoom;
  Matrix  view;
  Matrix  projection;
  f32 near_plane;
  f32 far_plane;
};

fz_internal Camera2D camera2d_init(u32 window_width, u32 window_height);
fz_internal void    _camera2d_update(Camera2D *camera2d, u32 window_width, u32 window_height);
fz_internal void     camera2d_zoom(Camera2D *camera2d, f32 delta, u32 window_width, u32 window_height);
fz_internal void     camera2d_move(Camera2D *camera2d, Vector2 delta, u32 window_width, u32 window_height);
fz_internal void     camera2d_reset(Camera2D *camera2d); /* Resets camera zoom and position */

#endif // CAMERA2D_H

fz_internal Camera2D
camera2d_init(u32 window_width, u32 window_height)
{
  Camera2D camera2d;
  memory_zero_struct(&camera2d);
  camera2d.position   = (Vector2){0, 0};
  camera2d.zoom       = 1.0f;
  _camera2d_update(&camera2d, window_width, window_height);
  return camera2d;
}

fz_internal void
_camera2d_update(Camera2D *camera2d, u32 window_width, u32 window_height)
{
  f32 half_width  = (window_width  * 0.5f) / camera2d->zoom;
  f32 half_height = (window_height * 0.5f) / camera2d->zoom;
  camera2d->projection = MatrixOrtho(-half_width, half_width, half_height, -half_height, -1.0f, 1.0f);
  camera2d->view       = MatrixTranslate(-camera2d->position.x, -camera2d->position.y, 0.0f);
}

fz_internal void
camera2d_move(Camera2D *camera2d, Vector2 delta, u32 window_width, u32 window_height)
{
  camera2d->position = Vector2Add(camera2d->position, delta);
  _camera2d_update(camera2d, window_width, window_height);
}

fz_internal void
camera2d_zoom(Camera2D *camera2d, f32 delta, u32 window_width, u32 window_height)
{
  f32 new_zoom = camera2d->zoom + delta;
  if (new_zoom > 0.01f)
  {
    camera2d->zoom = new_zoom;
  }
  _camera2d_update(camera2d, window_width, window_height);
}

fz_internal void
camera2d_reset(Camera2D *camera2d)
{
  camera2d->zoom = 1.0f;
  camera2d->position = (Vector2){0,0};
}
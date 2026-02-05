#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera2D Camera2D;
struct Camera2D
{
  V2f32 position;
  f32 zoom;
  Matrix3 view;
  Matrix3 projection;
  f32 near_plane;
  f32 far_plane;
};

function Camera2D camera2d_init(u32 window_width, u32 window_height);
function void  _camera2d_update(Camera2D* camera2d, u32 window_width, u32 window_height);
function void   camera2d_zoom(Camera2D* camera2d, f32 delta, u32 window_width, u32 window_height);
function void   camera2d_move(Camera2D* camera2d, V2f32 delta, u32 window_width, u32 window_height);

#endif // CAMERA_H

function Camera2D
camera2d_init(u32 window_width, u32 window_height)
{
  Camera2D camera2d;
  memory_zero_struct(&camera2d);
  camera2d.position   = v2f32(0, 0);
  camera2d.zoom       = 1.0f;
  _camera2d_update(&camera2d, window_width, window_height);
  return camera2d;
}

function void
_camera2d_update(Camera2D* camera2d, u32 window_width, u32 window_height)
{
  f32 half_width  = (window_width  * 0.5f) / camera2d->zoom;
  f32 half_height = (window_height * 0.5f) / camera2d->zoom;
  camera2d->projection = matrix3_ortho(-half_width, half_width, half_height, -half_height);
  camera2d->view       = matrix3_translate(v2f32(-camera2d->position.x, -camera2d->position.y));
}

function void
camera2d_move(Camera2D* camera2d, V2f32 delta, u32 window_width, u32 window_height)
{
  camera2d->position = v2f32_add(camera2d->position, delta);
  _camera2d_update(camera2d, window_width, window_height);
}

function void
camera2d_zoom(Camera2D* camera2d, f32 delta, u32 window_width, u32 window_height)
{
  f32 new_zoom = camera2d->zoom + delta;
  if (new_zoom > 0.01f)
  {
    camera2d->zoom = new_zoom;
  }
  _camera2d_update(camera2d, window_width, window_height);
}

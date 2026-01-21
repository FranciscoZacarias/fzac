#ifndef CAMERA2D_H
#define CAMERA2D_H

typedef struct {
  Vector2 position;
  f32 zoom;
  Matrix4 projection;
  Matrix4 view;
  f32 near_plane;
  f32 far_plane;
} Camera2D;

function Camera2D camera2d_init(f32 window_width, f32 window_height);
function void     camera2d_zoom(Camera2D *camera, f32 delta, f32 window_width, f32 window_height);
function void     camera2d_move(Camera2D *camera, Vector2 delta, f32 window_width, f32 window_height);
function void    _camera2d_update(Camera2D *camera, f32 window_width, f32 window_height);

#endif // CAMERA2D_H

function Camera2D
camera2d_init(f32 window_width, f32 window_height)
{
  Camera2D camera   = {0};
  camera.position   = vector2(0.0f, 0.0f);
  camera.zoom       = 2.0f;
  camera.near_plane = -1.0f;
  camera.far_plane  = 1.0f;
  _camera2d_update(&camera, window_width, window_height);
  return camera;
}

function void
camera2d_zoom(Camera2D *camera, f32 delta, f32 window_width, f32 window_height)
{
  f32 new_zoom = camera->zoom + delta;
  if (!(new_zoom < 0.1 || new_zoom > camera->far_plane))
  {
    camera->zoom = new_zoom;
  }
  _camera2d_update(camera, window_width, window_height);
}

function void 
camera2d_move(Camera2D *camera, Vector2 delta, f32 window_width, f32 window_height)
{
  camera->position = vector2_add(camera->position, delta);
  _camera2d_update(camera, window_width, window_height);
}

function void
_camera2d_update(Camera2D *camera, f32 window_width, f32 window_height)
{
  f32 half_width  = (window_width / 2.0f) / camera->zoom;
  f32 half_height = (window_height / 2.0f) / camera->zoom;

  camera->projection = matrix4_ortho(-half_width, half_width, -half_height, half_height, camera->near_plane, camera->far_plane);
  camera->view = matrix4_translate(-camera->position.x, -camera->position.y, 0.0f);
}

#ifndef CAMERA2D_H
#define CAMERA2D_H

typedef struct Camera2D Camera2D;
struct Camera2D
{
  Vector2 position;
  f32 zoom;
  Matrix4 view;
  Matrix4 projection;
  f32 near_plane;
  f32 far_plane;
};

function Camera2D camera_init();
function void    _camera_update(Camera2D *camera, f32 window_width, f32 window_height);
function void     camera_move(Camera2D *camera, Vector2 delta, f32 window_width, f32 window_height);
function void     camera_zoom(Camera2D *camera, f32 delta, f32 window_width, f32 window_height);

#endif // CAMERA2D_H

function Camera2D
camera_init(f32 window_width, f32 window_height)
{
  Camera2D camera;
  memory_zero_struct(&camera);
  
  camera.position    = vector2(0, 0);
  camera.zoom        = 1.0f;
  camera.near_plane = -1.f;
  camera.far_plane  =  1.f;

  _camera_update(&camera, window_width, window_height);
  return camera;
}

function void
_camera_update(Camera2D *camera, f32 window_width, f32 window_height)
{
  f32 half_width  = (window_width  * 0.5f) / camera->zoom;
  f32 half_height = (window_height * 0.5f) / camera->zoom;

  camera->projection = matrix4_ortho(-half_width, half_width, half_height, -half_height, camera->near_plane, camera->far_plane);
  camera->view       = matrix4_translate(-camera->position.x, -camera->position.y, 0);
}

function void
camera_move(Camera2D *camera, Vector2 delta, f32 window_width, f32 window_height)
{
  camera->position = vector2_add(camera->position, delta);
  _camera_update(camera, window_width, window_height);
}

function void
camera_zoom(Camera2D *camera, f32 delta, f32 window_width, f32 window_height)
{
  f32 new_zoom = camera->zoom + delta;
  if (new_zoom > 0.01f)
  {
    camera->zoom = new_zoom;
  }
  _camera_update(camera, window_width, window_height);
}

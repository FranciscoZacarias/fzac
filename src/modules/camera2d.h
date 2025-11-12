#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera Camera;
struct Camera
{
  V2f32 position;
  f32 zoom;
  M4f32 view;
  M4f32 projection;
  f32 near_plane;
  f32 far_plane;
};

function Camera camera_init(f32 width, f32 height);
function void  _camera_update(Camera* camera, f32 width, f32 height);
function void   camera_zoom(Camera* camera, f32 delta, f32 width, f32 height);
function void   camera_move(Camera* camera, Vec2f32 delta, f32 width, f32 height);

function Camera
camera_init(f32 width, f32 height)
{
  Camera camera;
  memory_zero_struct(&camera);
  camera.position   = vec2f32(0, 0);
  camera.zoom       = 1.0f;
  _camera_update(&camera, width, height);
  return camera;
}

function void
_camera_update(Camera* camera, f32 width, f32 height)
{
  f32 half_width  = (width * 0.5f) / camera->zoom;
  f32 half_height = (height * 0.5f) / camera->zoom;
  camera->projection = m4f32_orthographic(-half_width, half_width, half_height, -half_height);
  camera->view = m4f32_translate(v2f32(-camera->position.x, -camera->position.y));
}

function void
camera_move(Camera* camera, V2f32 delta, f32 width, f32 height)
{
  camera->position = v2f32_add(camera->position, delta);
  _camera_update(camera, width, height);
}

function void
camera_zoom(Camera* camera, f32 delta, f32 width, f32 height)
{
  f32 new_zoom = camera->zoom + delta;
  if (new_zoom > 0.01f)
  {
    camera->zoom = new_zoom;
  }
  _camera_update(camera, width, height);
}

#endif // CAMERA_H
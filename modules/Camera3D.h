#ifndef CAMERA3D_H
#define CAMERA3D_H

#define WORLD_UP      v3f32(0.0f, 1.0f,  0.0f)
#define WORLD_RIGHT   v3f32(1.0f, 0.0f,  0.0f)
#define WORLD_FORWARD v3f32(0.0f, 0.0f, -1.0f)

typedef enum
{
  CameraMovement_Front = 0,
  CameraMovement_Back,
  CameraMovement_Right,
  CameraMovement_Left,
  CameraMovement_Up,
  CameraMovement_Down
} Camera_Movement;

typedef struct Camera3D Camera3D;
struct Camera3D
{
  V3f32 position;
  f32   fov;
  f32   speed;
  f32   sensitivity;
  f32   pitch;
  f32   yaw;
};

function void    camera3d_init(Camera3D *camera, u32 speed);
function void    camera3d_update(Camera3D *camera, V2s32 mouse_delta, f32 delta_time);
function V3f32   camera3d_get_forward(Camera3D *camera);
function V3f32   camera3d_get_right(Camera3D *camera);
function V3f32   camera3d_get_up(Camera3D *camera);
function Matrix4 camera3d_projection(Camera3D *camera, f32 width, f32 height, f32 near_plane, f32 far_plane);
function Matrix4 camera3d_view(Camera3D *camera);
function void    camera3d_look_at(Camera3D *camera, V3f32 target);
function void    camera3d_set_euler(Camera3D *camera, f32 pitch, f32 yaw, f32 roll);
function void    camera3d_set_speed(Camera3D *camera, u32 speed);

#endif // CAMERA3D_H

function void
camera3d_init(Camera3D *camera, u32 speed)
{
  memory_zero_struct(camera);
  camera->position    = v3f32(0.0f, 0.0f, 5.0f);
  camera->fov         = 60.0f;
  camera->speed       = (f32)speed;
  camera->sensitivity = 0.1f;
  camera->pitch       = 0.0f;
  camera->yaw         = 0.0f;
}

function void
camera3d_update(Camera3D *camera, V2s32 delta, f32 delta_time)
{
  f32 dx = delta.x * camera->sensitivity;
  f32 dy = delta.y * camera->sensitivity;

  camera->yaw   -= dx;
  camera->pitch -= dy;

  if (camera->pitch > 89.0f)  { camera->pitch = 89.0f; }
  if (camera->pitch < -89.0f) { camera->pitch = -89.0f; }

  V3f32 forward = camera3d_get_forward(camera);
  V3f32 right   = camera3d_get_right(camera);

  f32 move_speed = camera->speed * delta_time;

  if (is_key_down(Keyboard_Key_W)) { camera->position = v3f32_add(camera->position, v3f32_scale(forward, move_speed)); }
  if (is_key_down(Keyboard_Key_S)) { camera->position = v3f32_add(camera->position, v3f32_scale(forward, -move_speed)); }
  if (is_key_down(Keyboard_Key_D)) { camera->position = v3f32_add(camera->position, v3f32_scale(right, move_speed)); }
  if (is_key_down(Keyboard_Key_A)) { camera->position = v3f32_add(camera->position, v3f32_scale(right, -move_speed)); }
  if (is_key_down(Keyboard_Key_E)) { camera->position = v3f32_add(camera->position, v3f32_scale(WORLD_UP, move_speed)); }
  if (is_key_down(Keyboard_Key_Q)) { camera->position = v3f32_add(camera->position, v3f32_scale(WORLD_UP, -move_speed)); }
}

function V3f32
camera3d_get_forward(Camera3D *camera)
{
  f32 pitch_rad = radians_from_degrees(camera->pitch);
  f32 yaw_rad   = radians_from_degrees(camera->yaw);
  V3f32 forward;
  forward.x = -sinf(yaw_rad) * cosf(pitch_rad);
  forward.y =  sinf(pitch_rad);
  forward.z = -cosf(yaw_rad) * cosf(pitch_rad);
  return forward;
}

function V3f32
camera3d_get_right(Camera3D *camera)
{
  V3f32 forward = camera3d_get_forward(camera);
  return v3f32_normalize(v3f32_cross(forward, WORLD_UP));
}

function V3f32
camera3d_get_up(Camera3D *camera)
{
  V3f32 forward = camera3d_get_forward(camera);
  V3f32 right   = v3f32_normalize(v3f32_cross(forward, WORLD_UP));
  return v3f32_cross(right, forward);
}

function Matrix4
camera3d_projection(Camera3D *camera, f32 width, f32 height, f32 near_plane, f32 far_plane)
{
  Matrix4 result = matrix4_perspective(camera->fov, get_window_width(), get_window_height(), near_plane, far_plane);
  return result;
}

function Matrix4
camera3d_view(Camera3D *camera)
{
  V3f32 forward = camera3d_get_forward(camera);
  V3f32 up      = camera3d_get_up(camera);
  V3f32 target  = v3f32_add(camera->position, forward);
  return matrix4_look_at(camera->position, target, up);
}

function void
camera3d_look_at(Camera3D *camera, V3f32 target)
{
  V3f32 direction = v3f32_normalize(v3f32_sub(target, camera->position));
  camera->pitch   = degrees_from_radians(asinf(direction.y));
  camera->yaw     = degrees_from_radians(atan2f(-direction.x, -direction.z));
}

function void
camera3d_set_euler(Camera3D *camera, f32 pitch, f32 yaw, f32 roll)
{
  camera->pitch = pitch;
  camera->yaw   = yaw;
}

function void
camera3d_set_speed(Camera3D *camera, u32 speed)
{
  camera->speed = (f32)speed;
}
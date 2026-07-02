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

typedef struct Camera3D_Input_Config Camera3D_Input_Config;
struct Camera3D_Input_Config
{
  Keyboard_Key up;
  Keyboard_Key down;
  Keyboard_Key left;
  Keyboard_Key right;
  Keyboard_Key forward;
  Keyboard_Key backwards;
  Mouse_Button enable_move_camera_around; // If calling camera3d_update_mouse, use this mouse button to move the camera around
};

typedef struct Camera3D Camera3D;
struct Camera3D
{
  V3f32 position;
  f32   fov;
  f32   speed;
  f32   sensitivity;
  f32   pitch;
  f32   yaw;

  Matrix4 view;
  Matrix4 projection;

  Camera3D_Input_Config input_config;
};

function Camera3D* camera3d_init(Arena *arena, f32 speed);
function void      camera3d_default_update(Window *window, Camera3D* camera, f32 delta_time); /* This would be the default behaviour attached to the camera. User can implement it's own update functions and not call these. */
function V2s32     camera3d_update_mouse(Window *window, Camera3D* camera); /* Optional behaviour to hide mouse when pressing a mouse button */
function void      camera3d_update_move(Camera3D *camera, V2s32 mouse_delta, f32 delta_time);
function V3f32     camera3d_get_forward(Camera3D *camera);
function V3f32     camera3d_get_right(Camera3D *camera);
function V3f32     camera3d_get_up(Camera3D *camera);
function Matrix4   camera3d_projection(Camera3D *camera, f32 width, f32 height, f32 near_plane, f32 far_plane);
function Matrix4   camera3d_view(Camera3D *camera);
function void      camera3d_look_at(Camera3D *camera, V3f32 target);
function void      camera3d_set_euler(Camera3D *camera, f32 pitch, f32 yaw, f32 roll);
function void      camera3d_set_speed(Camera3D *camera, u32 speed);

#endif // CAMERA3D_H

function Camera3D*
camera3d_init(Arena *arena, f32 speed)
{
  Camera3D *result = push_array(arena, Camera3D, 1);

  memory_zero_struct(result);
  result->position    = v3f32(0.0f, 0.0f, 5.0f);
  result->fov         = 60.0f;
  result->speed       = speed;
  result->sensitivity = 0.1f;
  result->pitch       = 0.0f;
  result->yaw         = 0.0f;

  result->input_config = (Camera3D_Input_Config) {
    .up        = Keyboard_Key_E,
    .down      = Keyboard_Key_Q,
    .left      = Keyboard_Key_A,
    .right     = Keyboard_Key_D,
    .forward   = Keyboard_Key_W,
    .backwards = Keyboard_Key_S,
    .enable_move_camera_around = Mouse_Button_Right,
  };

  return result;
}

function void
camera3d_default_update(Window *window, Camera3D* camera, f32 delta_time)
{
  V2s32 mouse_delta = camera3d_update_mouse(window, camera);
  camera3d_update_move(camera, mouse_delta, delta_time);
}

function V2s32
camera3d_update_mouse(Window *window, Camera3D* camera)
{
  local_persist b32 was_right_mouse_button_down = false;
  local_persist b32 skip_first_delta = false;

  V2s32 mouse_delta = {0};
  local_persist V2s32 mouse_last_position = {0};
  if (is_button_down(camera->input_config.enable_move_camera_around))
  {
    if (!was_right_mouse_button_down)
    {
      mouse_last_position = get_mouse_position();

      cursor_lock(window);
      cursor_hide(true);

      was_right_mouse_button_down = true;
      skip_first_delta = true;
    }

    if (!skip_first_delta)
    {
      mouse_delta = get_mouse_delta();
    }
    else
    {
      skip_first_delta = false;
    }
  }
  else
  {
    if (was_right_mouse_button_down)
    {
      cursor_unlock();
      cursor_hide(false);

      was_right_mouse_button_down = false;

      cursor_set_position(window, mouse_last_position.x, mouse_last_position.y);
    }
  }
  return mouse_delta;
}

function void
camera3d_update_move(Camera3D *camera, V2s32 mouse_delta, f32 delta_time)
{
  f32 dx = mouse_delta.x * camera->sensitivity;
  f32 dy = mouse_delta.y * camera->sensitivity;

  camera->yaw   -= dx;
  camera->pitch -= dy;

  if (camera->pitch > 89.0f)  { camera->pitch = 89.0f; }
  if (camera->pitch < -89.0f) { camera->pitch = -89.0f; }

  V3f32 forward = camera3d_get_forward(camera);
  V3f32 right   = camera3d_get_right(camera);
  f32 move_speed = camera->speed * delta_time;
  if (is_key_down(camera->input_config.forward))   { camera->position = v3f32_add(camera->position, v3f32_scale(forward, move_speed));   }
  if (is_key_down(camera->input_config.backwards)) { camera->position = v3f32_add(camera->position, v3f32_scale(forward, -move_speed));  }
  if (is_key_down(camera->input_config.left))      { camera->position = v3f32_add(camera->position, v3f32_scale(right, -move_speed));    }
  if (is_key_down(camera->input_config.right))     { camera->position = v3f32_add(camera->position, v3f32_scale(right, move_speed));     }
  if (is_key_down(camera->input_config.up))        { camera->position = v3f32_add(camera->position, v3f32_scale(WORLD_UP, move_speed));  }
  if (is_key_down(camera->input_config.down))      { camera->position = v3f32_add(camera->position, v3f32_scale(WORLD_UP, -move_speed)); }

  camera->view       = camera3d_view(camera);
  camera->projection = camera3d_projection(camera, get_window_width(), get_window_height(), 0.1f, 100.0f);
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
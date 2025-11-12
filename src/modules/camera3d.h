#ifndef FZ_CAMERA_H
#define FZ_CAMERA_H

#define WORLD_UP      vec3f32(0.0f, 1.0f,  0.0f)
#define WORLD_RIGHT   vec3f32(1.0f, 0.0f,  0.0f)
#define WORLD_FORWARD vec3f32(0.0f, 0.0f, -1.0f)

typedef enum {
  CameraMode_Select,
  CameraMode_Fly,
  CameraMode_Disabled
} Camera_Mode;

typedef enum {
  CameraMovement_Front,
  CameraMovement_Back,
  CameraMovement_Right,
  CameraMovement_Left,
  CameraMovement_Up,
  CameraMovement_Down
} Camera_Movement;

typedef struct
{
  Vec3f32 position;
  Quatf32 orientation;
  f32 fov;
  f32 speed;
  f32 sensitivity;
  f32 pitch;
  f32 yaw;
  Camera_Mode mode;
} Camera3D;

function void    camera3d_init(Camera3D* camera, f32 speed);
function void    camera3d_update(Camera3D* camera, Input_Context* input, f32 delta_time);
function Vec3f32 camera3d_get_forward(Camera3D* camera);
function Vec3f32 camera3d_get_right(Camera3D* camera);
function Vec3f32 camera3d_get_up(Camera3D* camera);
function Mat4f32 camera3d_get_view_matrix(Camera3D* camera);
function void    camera3d_look_at(Camera3D* camera, Vec3f32 target);
function void    camera3d_set_euler(Camera3D* camera, f32 pitch, f32 yaw, f32 roll);
function void    camera3d_set_speed(Camera3D* camera, f32 speed);

function void
camera3d_init(Camera3D* camera, f32 speed)
{
  assert_no_reentry();
  memory_zero_struct(camera);
  camera->position    = vec3f32(0.0f, 0.0f, 5.0f);
  camera->orientation = quatf32_identity();
  camera->fov         = 90.0f;
  camera->speed       = speed;
  camera->sensitivity = 0.1f;
  camera->pitch       = 0.0f;
  camera->yaw         = 0.0f;
  camera->mode        = CameraMode_Select;
}

function void
camera3d_update(Camera3D* camera, Input_Context* input, f32 delta_time)
{
  local_persist b32 was_right_mouse_button_down = 0;

  if (input_is_button_down(input, Mouse_Button_Right))
  {
    if (!was_right_mouse_button_down)
    {
      input->mouse_previous.screen_space.x = input->mouse_current.screen_space.x;
      input->mouse_previous.screen_space.y = input->mouse_current.screen_space.y;
      was_right_mouse_button_down = 1;
      os_cursor_lock(input, true);
      os_cursor_hide(true);
    }

    camera->mode = CameraMode_Fly;

    f32 dx = input->mouse_current.delta.x;
    f32 dy = input->mouse_current.delta.y;

    camera->yaw   += -dx * camera->sensitivity * delta_time * 180.0f / PI;
    camera->pitch += -dy * camera->sensitivity * delta_time * 180.0f / PI;

    if (camera->pitch > 89.0f)
    {
      camera->pitch = 89.0f;
    }
    if (camera->pitch < -89.0f)
    {
      camera->pitch = -89.0f;
    }

    Quatf32 yaw_q   = quatf32_from_axis_angle((Vec3f32){0.0f, 1.0f, 0.0f}, Radians(camera->yaw));
    Quatf32 pitch_q = quatf32_from_axis_angle((Vec3f32){1.0f, 0.0f, 0.0f}, Radians(camera->pitch));

    camera->orientation = quatf32_multiply(yaw_q, pitch_q);
    camera->orientation = quatf32_normalize(camera->orientation);

    Vec3f32 forward = camera3d_get_forward(camera);
    Vec3f32 right   = camera3d_get_right(camera);
    Vec3f32 up      = camera3d_get_up(camera);

    f32 speed = camera->speed * delta_time;

    if (input_is_key_down(input, Keyboard_Key_W))
    {
      camera->position = vec3f32_add(camera->position, vec3f32_scale(forward, speed));
    }
    if (input_is_key_down(input, Keyboard_Key_S))
    {
      camera->position = vec3f32_subtract(camera->position, vec3f32_scale(forward, speed));
    }
    if (input_is_key_down(input, Keyboard_Key_D))
    {
      camera->position = vec3f32_add(camera->position, vec3f32_scale(right, speed));
    }
    if (input_is_key_down(input, Keyboard_Key_A))
    {
      camera->position = vec3f32_subtract(camera->position, vec3f32_scale(right, speed));
    }
    if (input_is_key_down(input, Keyboard_Key_E))
    {
      camera->position = vec3f32_add(camera->position, vec3f32_scale(WORLD_UP, speed));
    }
    if (input_is_key_down(input, Keyboard_Key_Q))
    {
      camera->position = vec3f32_subtract(camera->position, vec3f32_scale(WORLD_UP, speed));
    }

    Vec2f32 dimensions = os_window_get_client_dimensions();
    Vec2f32 center = vec2s32(dimensions.x / 2, dimensions.y / 2);
    Vec2f32 center_screen = os_window_client_to_screen(center);
    os_cursor_set_position(center_screen.x, center_screen.y);

    input->mouse_current.screen_space.x = (f32)(dimensions.x / 2);
    input->mouse_current.screen_space.y = (f32)(dimensions.y / 2);
  }
  else
  {
    camera->mode = CameraMode_Select;
    was_right_mouse_button_down = 0;
    os_cursor_lock(input, false);
    os_cursor_hide(false);
  }
}

function Vec3f32
camera3d_get_forward(Camera3D* camera)
{
  Mat4f32 rot     = mat4f32_from_quatf32(camera->orientation);
  Vec3f32 forward = {0.0f, 0.0f, -1.0f};
  return vec3f32_normalize(mat4f32_transform_vec3f32(rot, forward));
}

function Vec3f32
camera3d_get_right(Camera3D* camera)
{
  Mat4f32 rot   = mat4f32_from_quatf32(camera->orientation);
  Vec3f32 right = {1.0f, 0.0f, 0.0f};
  return vec3f32_normalize(mat4f32_transform_vec3f32(rot, right));
}

function Vec3f32
camera3d_get_up(Camera3D* camera)
{
  Mat4f32 rot = mat4f32_from_quatf32(camera->orientation);
  Vec3f32 up  = {0.0f, 1.0f, 0.0f};
  return vec3f32_normalize(mat4f32_transform_vec3f32(rot, up));
}

function Mat4f32
camera3d_get_view_matrix(Camera3D* camera)
{
  Vec3f32 forward = camera3d_get_forward(camera);
  Vec3f32 up      = camera3d_get_up(camera);
  Vec3f32 target  = vec3f32_add(camera->position, forward);
  Mat4f32 result  = mat4f32_look_at(camera->position, target, up);
  return result;
}

function void
camera3d_look_at(Camera3D* camera, Vec3f32 target)
{
  Vec3f32 direction = vec3f32_normalize(vec3f32_subtract(target, camera->position));
  
  // Calculate pitch and yaw from direction vector
  camera->pitch = asinf(direction.y) * 180.0f / PI;
  camera->yaw = atan2f(-direction.x, -direction.z) * 180.0f / PI;
  
  // Set quaternion from calculated angles
  Quatf32 yaw_q   = quatf32_from_axis_angle((Vec3f32){0.0f, 1.0f, 0.0f}, Radians(camera->yaw));
  Quatf32 pitch_q = quatf32_from_axis_angle((Vec3f32){1.0f, 0.0f, 0.0f}, Radians(camera->pitch));
  camera->orientation = quatf32_multiply(yaw_q, pitch_q);
  camera->orientation = quatf32_normalize(camera->orientation);
}

function void
camera3d_set_euler(Camera3D* camera, f32 pitch, f32 yaw, f32 roll)
{
  camera->orientation = quatf32_from_euler(pitch, yaw, roll);
}

function void
camera3d_set_speed(Camera3D* camera, f32 speed)
{
  camera->speed = speed;
}

#endif // FZ_CAMERA_H
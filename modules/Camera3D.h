#ifndef FZAC_CAMERA3D_H
#define FZAC_CAMERA3D_H

#if FZ_RAYLIB
# error "Camera3D.h is replaced by raylib in Base_Raylib.h"
#endif

#define WORLD_UP      (Vector3){0.0f, 1.0f,  0.0f}
#define WORLD_RIGHT   (Vector3){1.0f, 0.0f,  0.0f}
#define WORLD_FORWARD (Vector3){0.0f, 0.0f, -1.0f}

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
  Mouse_Button enable_move_camera_around;
};

typedef struct Camera3D Camera3D;
struct Camera3D
{
  Vector3 position;
  f32   fov;
  f32   speed;
  f32   sensitivity;
  f32   pitch;
  f32   yaw;

  f32 near_plane;
  f32 far_plane;

  Matrix view;
  Matrix projection;

  Camera3D_Input_Config input_config;
};

fz_internal Camera3D* camera3d_init(Arena *arena, Window *window, f32 speed, f32 near_plane, f32 far_plane);
fz_internal void      camera3d_default_update(Window *window, Camera3D* camera, f32 delta_time); /* This would be the default behaviour attached to the camera. User can implement it's own update functions and not call these. */
fz_internal void      camera3d_update_move(Camera3D *camera, V2s32 mouse_delta, f32 delta_time);
fz_internal Vector3   camera3d_get_forward(Camera3D *camera);
fz_internal Vector3   camera3d_get_right(Camera3D *camera);
fz_internal Vector3   camera3d_get_up(Camera3D *camera);
fz_internal Matrix    camera3d_projection(Camera3D *camera, f32 width, f32 height, f32 near_plane, f32 far_plane);
fz_internal Matrix    camera3d_view(Camera3D *camera);
fz_internal void      camera3d_look_at(Camera3D *camera, Vector3 target);
fz_internal void      camera3d_set_euler(Camera3D *camera, f32 pitch, f32 yaw, f32 roll);
fz_internal void      camera3d_set_speed(Camera3D *camera, u32 speed);

#endif // FZAC_CAMERA3D_H

fz_internal Camera3D*
camera3d_init(Arena *arena, Window *window, f32 speed, f32 near_plane, f32 far_plane)
{
  Camera3D *result = push_array(arena, Camera3D, 1);

  memory_zero_struct(result);
  result->position    = (Vector3){0.0f, 0.0f, 5.0f};
  result->fov         = 60.0f;
  result->speed       = speed;
  result->sensitivity = 0.1f;
  result->pitch       = 0.0f;
  result->yaw         = 0.0f;

  result->near_plane  = near_plane;
  result->far_plane   = far_plane;

  result->input_config = (Camera3D_Input_Config) {
    .up        = Keyboard_Key_E,
    .down      = Keyboard_Key_Q,
    .left      = Keyboard_Key_A,
    .right     = Keyboard_Key_D,
    .forward   = Keyboard_Key_W,
    .backwards = Keyboard_Key_S,
    .enable_move_camera_around = Mouse_Button_Right,
  };

  camera3d_default_update(window, result, 0.1f);
  camera3d_update_move(result, v2s32(0,0), 0.1f);

  return result;
}

fz_internal void
camera3d_default_update(Window *window, Camera3D* camera, f32 delta_time)
{
  fz_local_persist b32   was_right_mouse_button_down = false;
  fz_local_persist b32   skip_first_delta = false;
  fz_local_persist V2s32 mouse_last_position = {0};

  if (is_button_down(camera->input_config.enable_move_camera_around))
  {
    V2s32 mouse_delta = {0};
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
      camera3d_update_move(camera, mouse_delta, delta_time);
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
}

fz_internal void
camera3d_update_move(Camera3D *camera, V2s32 mouse_delta, f32 delta_time)
{
  f32 dx = mouse_delta.x * camera->sensitivity;
  f32 dy = mouse_delta.y * camera->sensitivity;

  camera->yaw   -= dx;
  camera->pitch -= dy;

  if (camera->pitch > 89.0f)  { camera->pitch = 89.0f; }
  if (camera->pitch < -89.0f) { camera->pitch = -89.0f; }

  Vector3 forward = camera3d_get_forward(camera);
  Vector3 right   = camera3d_get_right(camera);
  f32 move_speed = camera->speed * delta_time;
  if (is_key_down(camera->input_config.forward))   { camera->position = Vector3Add(camera->position, Vector3Scale(forward, move_speed));   }
  if (is_key_down(camera->input_config.backwards)) { camera->position = Vector3Add(camera->position, Vector3Scale(forward, -move_speed));  }
  if (is_key_down(camera->input_config.left))      { camera->position = Vector3Add(camera->position, Vector3Scale(right, -move_speed));    }
  if (is_key_down(camera->input_config.right))     { camera->position = Vector3Add(camera->position, Vector3Scale(right, move_speed));     }
  if (is_key_down(camera->input_config.up))        { camera->position = Vector3Add(camera->position, Vector3Scale(WORLD_UP, move_speed));  }
  if (is_key_down(camera->input_config.down))      { camera->position = Vector3Add(camera->position, Vector3Scale(WORLD_UP, -move_speed)); }

  camera->view       = camera3d_view(camera);
  camera->projection = camera3d_projection(camera, get_window_width(), get_window_height(), camera->near_plane, camera->far_plane);
}

fz_internal Vector3
camera3d_get_forward(Camera3D *camera)
{
  f32 pitch_rad = camera->pitch * DEG2RAD;
  f32 yaw_rad   = camera->yaw * DEG2RAD;
  Vector3 forward;
  forward.x = -sinf(yaw_rad) * cosf(pitch_rad);
  forward.y =  sinf(pitch_rad);
  forward.z = -cosf(yaw_rad) * cosf(pitch_rad);
  return forward;
}

fz_internal Vector3
camera3d_get_right(Camera3D *camera)
{
  Vector3 forward = camera3d_get_forward(camera);
  return Vector3Normalize(Vector3CrossProduct(forward, WORLD_UP));
}

fz_internal Vector3
camera3d_get_up(Camera3D *camera)
{
  Vector3 forward = camera3d_get_forward(camera);
  Vector3 right   = Vector3Normalize(Vector3CrossProduct(forward, WORLD_UP));
  return Vector3CrossProduct(right, forward);
}

fz_internal Matrix
camera3d_projection(Camera3D *camera, f32 width, f32 height, f32 near_plane, f32 far_plane)
{
  Matrix result = MatrixPerspective(camera->fov * DEG2RAD, (f64)get_window_width() / (f64)get_window_height(), near_plane, far_plane);
  return result;
}

fz_internal Matrix
camera3d_view(Camera3D *camera)
{
  Vector3 forward = camera3d_get_forward(camera);
  Vector3 up      = camera3d_get_up(camera);
  Vector3 target  = Vector3Add(camera->position, forward);
  return MatrixLookAt(camera->position, target, up);
}

fz_internal void
camera3d_look_at(Camera3D *camera, Vector3 target)
{
  Vector3 direction = Vector3Normalize(Vector3Subtract(target, camera->position));
  camera->pitch   = asinf(direction.y) * RAD2DEG;
  camera->yaw     = atan2f(-direction.x, -direction.z) * RAD2DEG;
}

fz_internal void
camera3d_set_euler(Camera3D *camera, f32 pitch, f32 yaw, f32 roll)
{
  camera->pitch = pitch;
  camera->yaw   = yaw;
}

fz_internal void
camera3d_set_speed(Camera3D *camera, u32 speed)
{
  camera->speed = (f32)speed;
}
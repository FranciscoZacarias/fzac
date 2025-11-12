#ifndef OS_INPUT_H
#define OS_INPUT_H

#include "os_input.hephaestus.h"

typedef enum
{
  Mouse_Button_Left,
  Mouse_Button_Right,
  Mouse_Button_Middle,
  
  Mouse_Button_Count,
} Mouse_Button;

#define KEYBOARD_STATE_SIZE 256
typedef struct Keyboard_State Keyboard_State;
struct Keyboard_State
{
  b8 keys[KEYBOARD_STATE_SIZE];
};

typedef struct Mouse_State Mouse_State;
struct Mouse_State
{
  V2f32 screen_space;
  V2f32 delta;
  b8 buttons[Mouse_Button_Count];
};

typedef struct Input_Context Input_Context;
struct Input_Context
{
  Keyboard_State keyboard_current;
  Keyboard_State keyboard_previous;
  Mouse_State    mouse_current;
  Mouse_State    mouse_previous;
  b32 _g_ignore_next_mouse_move;
  b32 _g_is_cursor_locked;
};

function void _input_init(Input_Context* input, f32 window_width, f32 window_height); /* Initializes input state (clears previous + current states) */
function void _input_update(Input_Context* input); /* Updates previous input state with current state (to track deltas and transitions) */

function b32 input_is_key_up(Input_Context* input, Keyboard_Key key); /* True if the given key is currently up */
function b32 input_is_key_down(Input_Context* input, Keyboard_Key key); /* True if the given key is currently down */
function b32 input_was_key_up(Input_Context* input, Keyboard_Key key); /* True if the given key was up on the previous frame */
function b32 input_was_key_down(Input_Context* input, Keyboard_Key key); /* True if the given key was down on the previous frame */
function b32 input_is_key_clicked(Input_Context* input, Keyboard_Key key); /* True if the given key is down this frame but was up last frame */

function b32 input_is_button_up(Input_Context* input, Mouse_Button button); /* True if the given mouse button is currently up */
function b32 input_is_button_down(Input_Context* input, Mouse_Button button); /* True if the given mouse button is currently down */
function b32 input_was_button_up(Input_Context* input, Mouse_Button button); /* True if the given mouse button was up on the previous frame */
function b32 input_was_button_down(Input_Context* input, Mouse_Button button); /* True if the given mouse button was down on the previous frame */
function b32 input_is_button_clicked(Input_Context* input, Mouse_Button button); /* True if the given mouse button is down this frame but was up last frame */

function void _input_process_keyboard_key(Input_Context* input, Keyboard_Key key, b8 is_pressed); /* Internal: Processes a key press/release and updates keyboard state */
function void _input_process_mouse_button(Input_Context* input, Mouse_Button button, b32 is_pressed); /* Internal: Processes mouse button press/release and updates mouse state */
function void _input_process_mouse_cursor(Input_Context* input, f32 x, f32 y); /* Internal: Updates current mouse cursor position in screen space */

function u32          _native_key_from_os_key(Keyboard_Key key); /* Converts os key to native key */
function Keyboard_Key _os_key_from_native_key(u32 native_key); /* Converts native key to os key */

#endif // OS_INPUT_H
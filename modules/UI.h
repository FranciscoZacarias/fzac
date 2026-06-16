#ifndef UI_H
#define UI_H

/*
  // @TODO(fz): Text input line
  // @TODO(fz): Text input multi line
  // @TODO(fz): Scroll view 
  // @TODO(fz): Drop down box
  // @TODO(fz): Latest interacted window should come to front
  // @TODO(fz): Window should dock on each other

  // @BUG(fz): Window with 100 width and 200 height seems to be blocked when resizing horizotnally

  How to:

  - Implement somewhere:
    function UI_Text_Metrics
    ui_measure_text(String text)
    {
      R_Text_Metrics r_metrics = r_measure_text(R_RenderContext.font, text);
      UI_Text_Metrics metrics = {0};
      metrics.width           = r_metrics.width;
      metrics.height          = r_metrics.height;
      metrics.line_count      = r_metrics.line_count;
      metrics.last_line_width = r_metrics.last_line_width;
      return metrics;
    }

  - Frame setup:

  while (!should_quit)
  {
    ...
  
    UI_Font ui_font = {0};
    ui_font.height = R_RenderContext.font->height;
    ui_init(ui_font);

    ...

    ui_end();
    UI_Command *commands = ui_get_commands();
    for (u32 i = 0; i < ui_get_commands_count(); i += 1)
    {
      UI_Command *command = &commands[i];
      switch (command->kind)
      {
        case UI_Command_Kind_Draw_Rect:
        {
          r_draw_quad_color_ext(R_Pipeline_Screen, command->top_left, command->size, command->color, command->rect.rotation, command->rect.roundness, command->rect.smoothness, command->rect.border_thickness, command->rect.border_color);
        } break;
        case UI_Command_Kind_Draw_Rect_With_Texture:
        {
          r_draw_quad_atlas_ext(R_Pipeline_Screen, command->rect.texture_index, command->rect.uv_min, command->rect.uv_max, command->top_left, command->size, command->color, command->rect.rotation, command->rect.roundness, command->rect.smoothness, command->rect.border_thickness, command->rect.border_color);
        } break;
        case UI_Command_Kind_Draw_Text:
        {
          R_Text_Params params = r_text_params_default(R_Pipeline_Screen);
          params.text          = string_copy(get_temporary_storage(), command->text.text);
          params.top_left      = command->top_left;
          params.max_width     = command->text.max_width;
          params.max_height    = command->text.max_height;
          params.color         = command->color;
          r_draw_text_params(params);
        } break;
      }
    }
    
    ...
  }
*/

#define UI_MAX_COMMANDS             thousand(10)
#define UI_MAX_CACHED_NODES         256
#define UI_MAX_PREVIOUS_FRAME_RECTS 16

#define UI_MAX_NODE_IDLE_LIFETIME_MS 2000

#define UI_WINDOW_SNAP_KEY Keyboard_Key_SHIFT
#define UI_FONT_HEIGHT_ADD Keyboard_Key_RIGHT_BRACKET
#define UI_FONT_HEIGHT_SUB Keyboard_Key_LEFT_BRACKET

#include "UI/generated/ui.cgen.h"

// Stacks
#define ui_stack(type, name, size) struct { type data[size]; u32 top_index; type  bottom_val; } name##_stack
#define ui_stack_init(name, bot_val) statement((UIContext.name##_stack).top_index = 0; (UIContext.name##_stack).bottom_val = (bot_val);)
#define ui_stack_push(name, val) ((UIContext.name##_stack).data[((UIContext.name##_stack).top_index < sizeof((UIContext.name##_stack).data)/sizeof((UIContext.name##_stack).data[0])) ? (UIContext.name##_stack).top_index++ : (UIContext.name##_stack).top_index] = (val))
#define ui_stack_pop(name) (((UIContext.name##_stack).top_index > 0) ? (UIContext.name##_stack).data[--(UIContext.name##_stack).top_index] : (UIContext.name##_stack).bottom_val)
#define ui_stack_top(name) (((UIContext.name##_stack).top_index > 0) ? (UIContext.name##_stack).data[(UIContext.name##_stack).top_index-1] : (UIContext.name##_stack).bottom_val)
#define ui_stack_assert_top_at(name, at) if((UIContext.name##_stack).top_index != (at)) ui_error(Sf(UIContext.arena, "UI: %s not at expected top value: '%u'. Actual value: '%u'", stringify((UIContext.name##_stack)), (at), (UIContext.name##_stack).top_index))
#define ui_stack_is_at_bottom(name) ((UIContext.name##_stack).top_index == 0)
#define ui_stack_defer(name, val) defer_loop(ui_stack_push(name, val), ui_stack_pop(name))

// Signal
#define ui_hovered(signal)        has_flags(signal.flags, UI_Signal_Flags_Mouse_Hovered)

#define ui_clicked(signal)        has_flags(signal.flags, UI_Signal_Flags_Left_Clicked)
#define ui_middle_clicked(signal) has_flags(signal.flags, UI_Signal_Flags_Middle_Clicked)
#define ui_right_clicked(signal)  has_flags(signal.flags, UI_Signal_Flags_Right_Clicked)

#define ui_pressed(signal)        has_flags(signal.flags, UI_Signal_Flags_Left_Pressed)
#define ui_middle_pressed(signal) has_flags(signal.flags, UI_Signal_Flags_Middle_Pressed)
#define ui_right_pressed(signal)  has_flags(signal.flags, UI_Signal_Flags_Right_Pressed)

#define ui_close(signal)          has_flags(signal.flags, UI_Signal_Flags_Close)

// UI
#define UI_EPSILON 0.01f

typedef struct UI_Rect UI_Rect;
struct UI_Rect
{
  V2s32 top_left;
  V2s32 size;
};

typedef struct UI_Text_Metrics UI_Text_Metrics;
struct UI_Text_Metrics
{
  f32 width;
  f32 height;
  u64 line_count;
  f32 last_line_width;
};

typedef struct UI_Font UI_Font;
struct UI_Font
{
  f32 height;
};

typedef struct UI_Texture UI_Texture;
struct UI_Texture
{
  s32 handle;
};

function UI_Text_Metrics ui_measure_text(String text); // @NOTE(fz): Define in user space!!

typedef struct UI_Color_Theme UI_Color_Theme;
struct UI_Color_Theme
{
  V4f32 light_border_color;
  V4f32 light_border_hover_color;
  V4f32 light_border_active_color;
  
  V4f32 dark_border_color;
  V4f32 dark_border_hover_color;
  V4f32 dark_border_active_color;

  V4f32 light_background_color;
  V4f32 light_background_hover_color;
  V4f32 light_background_active_color;

  V4f32 dark_background_color;
  V4f32 dark_background_hover_color;
  V4f32 dark_background_active_color;

  V4f32 interactable_border_color;
  V4f32 interactable_border_hover_color;
  V4f32 interactable_border_active_color;

  V4f32 interactable_background_color;
  V4f32 interactable_background_hover_color;
  V4f32 interactable_background_active_color;

  V4f32 text_color;
  V4f32 text_hover_color;
  V4f32 text_active_color;
};

typedef struct UI_Node_Color_Theme UI_Node_Color_Theme;
struct UI_Node_Color_Theme
{
  V4f32 border_color;
  V4f32 border_hover_color;
  V4f32 border_active_color;

  V4f32 background_color;
  V4f32 background_hover_color;
  V4f32 background_active_color;

  V4f32 text_color;
  V4f32 text_hover_color;
  V4f32 text_active_color;

  s32 border_thickness_px;
  s32 corner_roundness_px;
};

typedef struct UI_Node UI_Node;
struct UI_Node
{
  u64 hash;

  // Node tree
  UI_Node *first; /* First child */
  UI_Node *last;  /* Last child */
  UI_Node *next;  /* Next child, after current node */
  UI_Node *previous; /* Previous child, before current node */
  UI_Node *parent;   /* Parent of current node */

  UI_Rect bounds; /* Top left is relative to parent's cursor. Size is absolute */
  UI_Rect clip;   /* Top left is relative to bounds. Size is absolute */
  V2s32 cursor;   /* This is relative to clip */

  u32 child_spacing; /* Spacing between each child */
  UI_Alignment_Kind alignment; /* Alignment for child nodes to be position to */

  b32 should_render; /* Should this node be rendered? Some nodes are used purely for helping layouts */
  f32 depth;
  UI_Node_Flags flags; /* Behaviour for this node */

  // String
  String string;
  String string_clean; /* String with everything after ## stripped */
  V2f32 clean_string_dimensions; /* String dimensions */

  // Style
  UI_Node_Color_Theme color_theme;
  V4f32 target_border_color;
  V4f32 target_background_color;
  V4f32 target_text_color;

  // Texture
  UI_Texture texture;
  V2f32 uv_min;
  V2f32 uv_max;
};

typedef struct UI_Node_Cache UI_Node_Cache;
struct UI_Node_Cache
{
  u64 hash;
  u64 last_time_touched;

  V2s32 accumulated_drag_offset; /* How much it has been offseted in total */
  V2s32 accumulated_resize_offset; /* How much it as been resized in total */

  f32 hover_t;
  f32 active_t;

  /* Widget specific */
  struct
  {
    b8 is_slider_dragging;
  } slider;
};

typedef struct UI_Signal UI_Signal;
struct UI_Signal
{
  UI_Node* node;
  UI_Signal_Flags flags;
};

typedef enum
{
  UI_Command_Kind_Draw_Rect, // Just with color
  UI_Command_Kind_Draw_Rect_With_Texture,
  UI_Command_Kind_Draw_Text
} UI_Command_Kind;

typedef struct UI_Command UI_Command;
struct UI_Command
{
  UI_Command_Kind kind;
  V2f32 top_left;
  V2f32 size;
  V4f32 color;

  union
  {
    struct
    {
      f32   rotation;
      f32   roundness;
      f32   smoothness;
      f32   border_thickness;
      V4f32 border_color;

      s32 texture_index;
      V2f32 uv_min;
      V2f32 uv_max;
    } rect;

    struct
    {
      f32 max_width;
      f32 max_height;
      String text;
    } text;
  };
};

typedef struct UI_Context UI_Context;
struct UI_Context
{
  Arena* arena;
  Arena* frame_arena;

  UI_Node *root;

  u32 window_width;
  u32 window_height;

  s32 mouse_x;
  s32 mouse_y;
  s32 mouse_delta_x;
  s32 mouse_delta_y;

  f32 delta_time;
  u64 frame_count;

  UI_Node_Cache *cached_nodes;
  u32 cached_nodes_capacity;
  u32 cached_nodes_count;
  u32 cached_nodes_available;
  u64 cached_nodes_max_idle_lifetime_ms; /* How long until a node that wasn't searched should be removed from cache */

  u64 hash_active;
  u64 hash_active_previous;
  f32 hash_active_depth;
  u64 hash_hot;
  f32 hash_hot_depth;

  u64 hash_resizing; /* Node that is resizing */
  f32 hash_resizing_depth;
  
  s32 resize_area_side; /* The resize area is a quad. This is the side. */

  // Command Buffer
  UI_Command *commands;
  u32 commands_count;
  u32 commands_capacity;

  // Widget settings
  s32 window_min_width;
  s32 window_min_height;

  // Style
  UI_Color_Theme color_theme;
  f32 animation_speed;

  UI_Font font;
  u32 node_bounds_min_width;
  u32 node_bounds_min_height;

  struct
  {
    s32 corner_roundness;
    s32 border_thickness;
  } style;

  struct
  {
    Arena *rects_arena;
    UI_Rect* rects;
    u32 rects_count;
    u32 rects_capacity;
  } previous_frame;

  struct
  {
    b8 draw_debug_window;
    b8 draw_color_theme_window;

    b8 draw_clip;
    b8 draw_cursor;
    b8 draw_resize_picking_area;
    b8 draw_stuff_that_shouldnt_render;
    b8 disable_window_snap;
    b8 disable_node_cache_clear;
  } debug;

  ui_config_stacks;
};

global UI_Context UIContext;
global UI_Color_Theme UIColorThemeDark;
read_only global UI_Node UINodeNilSentinel =
{
  .first    = &UINodeNilSentinel,
  .last     = &UINodeNilSentinel,
  .next     = &UINodeNilSentinel,
  .previous = &UINodeNilSentinel,
  .parent   = &UINodeNilSentinel,
};

function void            ui_init(UI_Font font);
function void            ui_begin(u32 window_width, u32 window_height, u32 mouse_x, u32 mouse_y, u32 mouse_delta_x, u32 mouse_delta_y, f32 delta_time, u64 frame_count);
function void            ui_end();
function b32             ui_is_mouse_in_ui();
function void            ui_arena_window(String text, Arena *arena, s32 x, s32 y);
function void            ui_toggle_color_theme_window();
function void            ui_draw_color_theme_window();
function u32             ui_get_commands_count();
function UI_Command*     ui_get_commands();
function UI_Node*       _ui_node_from_string(String unique, String string, UI_Node_Flags flags);
function void           _ui_update_tree_nodes(UI_Node* node);
function V2s32          _ui_get_node_absolute_top_left(UI_Node *node);
function u32            _ui_get_node_depth(UI_Node *node);
function void           _ui_render_ui_tree(UI_Node *node);
function void           _ui_add_node_child(UI_Node *parent, UI_Node *child);
function UI_Node_Cache* _ui_get_cached_node(u64 hash);
function b32            _ui_is_mouse_in_node(UI_Node* node);
function b32            _ui_is_mouse_in_rect(UI_Rect rect);
function void           _ui_fill_signals_from_node(UI_Signal* signal);
function String         _ui_clean_string(Arena* arena, String string);
function void           _ui_purge_old_cached_nodes();
function UI_Node *      _ui_tree_next(UI_Node *node);
function void           _ui_push_draw_rect_command(V2f32 top_left, V2f32 size, V4f32 color, f32 rotation, f32 roundness, f32 smoothness, f32 border_thickness, V4f32 border_color);
function void           _ui_push_draw_rect_with_texture_command(s32 texture_index, V2f32 uv_min, V2f32 uv_max, V2f32 top_left, V2f32 size, V4f32 color, f32 rotation, f32 roundness, f32 smoothness, f32 border_thickness, V4f32 border_color);
function void           _ui_push_draw_text_command(V2f32 top_left, f32 max_width, f32 max_height, V4f32 color, String text);
function void inline    _ui_error(String message, String file, u32 line);

#if DEBUG
// Error
# define ui_error(msg) _ui_error((msg), S(__FILE__), __LINE__)
function void inline _ui_error(String message, String file, u32 line);
#else 
# define ui_error(msg)
#endif

#include "UI/UI.c"
#include "UI/UI_Widgets.c"

#endif // UI_H
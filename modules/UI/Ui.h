#ifndef UI_H
#define UI_H

#define UI_MAX_COMMANDS      thousand(1)
#define UI_MAX_CACHED_NODES  256
#define UI_MAX_PREVIOUS_FRAME_RECTS 16

#define UI_MAX_NODE_IDLE_LIFETIME_MS 2000

// Stacks
#define ui_stack(type, name, size) struct { type data[size]; u32 top_index; type bottom_val; } name##_stack
#define ui_stack_init(name, bot_val)   statement((UIContext.name##_stack).top_index = 0; (UIContext.name##_stack).bottom_val = (bot_val);)
#define ui_stack_push(name, val)       ((UIContext.name##_stack).data[((UIContext.name##_stack).top_index < sizeof((UIContext.name##_stack).data)/sizeof((UIContext.name##_stack).data[0])) ? (UIContext.name##_stack).top_index++ : (UIContext.name##_stack).top_index] = (val))
#define ui_stack_pop(name)             (((UIContext.name##_stack).top_index > 0) ? (UIContext.name##_stack).data[--(UIContext.name##_stack).top_index] : (UIContext.name##_stack).bottom_val)
#define ui_stack_top(name)             (((UIContext.name##_stack).top_index > 0) ? (UIContext.name##_stack).data[(UIContext.name##_stack).top_index-1] : (UIContext.name##_stack).bottom_val)
#define ui_stack_is_at_bottom(name)    ((UIContext.name##_stack).top_index == 0)
#define ui_stack_defer(name, val)      defer_loop(ui_stack_push(name, val), ui_stack_pop(name))

// Signal helpers
#define ui_hovered(signal)      has_flags(signal.flags, UI_Signal_Flags_Mouse_Hovered)
#define ui_clicked(signal)      has_flags(signal.flags, UI_Signal_Flags_Left_Clicked)
#define ui_right_clicked(signal) has_flags(signal.flags, UI_Signal_Flags_Right_Clicked)
#define ui_pressed(signal)      has_flags(signal.flags, UI_Signal_Flags_Left_Pressed)
#define ui_close(signal)        has_flags(signal.flags, UI_Signal_Flags_Close)

#define UI_EPSILON 0.01f

// -----------------------------------------------------------------------
// Enums
// -----------------------------------------------------------------------

typedef enum 
{
  UI_Node_Flags_None              = 0,
  UI_Node_Flags_Mouse_Clickable   = (1 << 0), /* Emits signal when clicked */
  UI_Node_Flags_Draggable_Panel   = (1 << 1), /* This node (and children) can be dragged */
  UI_Node_Flags_Draggable         = (1 << 2), /* Emits signal when dragged */
  UI_Node_Flags_Hoverable         = (1 << 3), /* Emits signal when hovered */
  UI_Node_Flags_Text_Display      = (1 << 4), /* Draws string on node */
  UI_Node_Flags_Text_Center_X     = (1 << 5),
  UI_Node_Flags_Text_Center_Y     = (1 << 6),
  UI_Node_Flags_Resizable         = (1 << 7), /* Node can be resized */
  UI_Node_Flags_Float_On_Parent   = (1 << 8), /* Ignores parent alignment, floats at top_left */
} UI_Node_Flags;

typedef enum 
{
  UI_Signal_Flags_None            = 0,
  UI_Signal_Flags_Left_Pressed    = (1 << 0),
  UI_Signal_Flags_Right_Pressed   = (1 << 1),
  UI_Signal_Flags_Left_Clicked    = (1 << 2),
  UI_Signal_Flags_Right_Clicked   = (1 << 3),
  UI_Signal_Flags_Mouse_Hovered   = (1 << 4),
  UI_Signal_Flags_Close           = (1 << 15),
} UI_Signal_Flags;

typedef enum 
{
  UI_Size_Kind_None     = 0,
  UI_Size_Kind_Fixed,    /* Absolute pixel size within parent clip */
  UI_Size_Kind_Relative, /* Fraction of parent's remaining space */
  UI_Size_Kind_Copy_X,   /* Copy computed X into Y */
  UI_Size_Kind_Copy_Y,   /* Copy computed Y into X */
} UI_Size_Kind;

typedef enum 
{
  UI_Alignment_Kind_Y,     /* Cursor advances downward after placing node */
  UI_Alignment_Kind_X,     /* Cursor advances rightward after placing node */
  UI_Alignment_Kind_Float, /* Cursor does not advance */
} UI_Alignment_Kind;

// -----------------------------------------------------------------------
// Structs
// -----------------------------------------------------------------------

typedef struct UI_Rect UI_Rect;
struct UI_Rect
{
  V2s32 top_left;
  V2s32 size;
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

  s32 border_thickness;
  f32 corner_roundness;
};

typedef struct UI_Node UI_Node;
struct UI_Node
{
  u64 hash;

  UI_Node *first;
  UI_Node *last;
  UI_Node *next;
  UI_Node *previous;
  UI_Node *parent;

  UI_Rect bounds;    /* top_left relative to parent cursor, size absolute */
  UI_Rect clip;      /* top_left relative to bounds, size absolute */
  V2s32   cursor;    /* relative to clip */

  u32               child_spacing;
  UI_Alignment_Kind alignment;

  b32           should_render;
  f32           depth;
  UI_Node_Flags flags;

  String string;
  String string_clean;
  V2f32  clean_string_dimensions;

  UI_Node_Color_Theme color_theme;
  V4f32 target_border_color;
  V4f32 target_background_color;
  V4f32 target_text_color;
};

typedef struct UI_Node_Cache UI_Node_Cache;
struct UI_Node_Cache
{
  u64 hash;
  u64 last_time_touched;

  V2s32 accumulated_drag_offset;
  V2s32 accumulated_resize_offset;

  f32 hover_t;
  f32 active_t;
};

typedef struct UI_Signal UI_Signal;
struct UI_Signal
{
  UI_Node        *node;
  UI_Signal_Flags flags;
};

typedef enum
{
  UI_Command_Kind_Draw_Rect,
  UI_Command_Kind_Draw_Rect_With_Texture,
  UI_Command_Kind_Draw_Text,
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

      s32   texture_index;
      V2f32 uv_min;
      V2f32 uv_max;
    } rect;

    struct
    {
      f32    max_width;
      f32    max_height;
      String text;
    } text;
  };
};

typedef struct UI_Context UI_Context;
struct UI_Context
{
  Arena *arena;
  Arena *frame_arena;

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
  u64 cached_nodes_max_idle_lifetime_ms;

  u64 hash_active;
  u64 hash_active_previous;
  f32 hash_active_depth;
  u64 hash_hot;
  f32 hash_hot_depth;

  u64 hash_resizing;
  f32 hash_resizing_depth;
  s32 resize_area_side;

  UI_Command *commands;
  u32 commands_count;
  u32 commands_capacity;

  s32 window_min_width;
  s32 window_min_height;

  R_Font *font;

  f32 animation_speed;

  struct
  {
    f32 corner_roundness;
    s32 border_thickness;
  } style;

  struct
  {
    Arena   *rects_arena;
    UI_Rect *rects;
    u32      rects_count;
    u32      rects_capacity;
  } previous_frame;

  // Per-node stacks (push before building a node, auto-consumed)
  struct { V2s32   data[32]; u32 top_index; V2s32   bottom_val; } top_left_stack;
  struct { s32     data[32]; u32 top_index; s32     bottom_val; } size_fixed_x_stack;
  struct { s32     data[32]; u32 top_index; s32     bottom_val; } size_fixed_y_stack;
  struct { f32     data[32]; u32 top_index; f32     bottom_val; } size_relative_x_stack;
  struct { f32     data[32]; u32 top_index; f32     bottom_val; } size_relative_y_stack;
  struct { UI_Size_Kind data[32]; u32 top_index; UI_Size_Kind bottom_val; } size_kind_x_stack;
  struct { UI_Size_Kind data[32]; u32 top_index; UI_Size_Kind bottom_val; } size_kind_y_stack;
  struct { s32     data[32]; u32 top_index; s32     bottom_val; } padding_fixed_stack;
  struct { s32     data[32]; u32 top_index; s32     bottom_val; } node_spacing_stack;
  struct { s32     data[32]; u32 top_index; s32     bottom_val; } border_thickness_stack;
  struct { f32     data[32]; u32 top_index; f32     bottom_val; } corner_roundness_factor_stack;
  struct { UI_Alignment_Kind data[32]; u32 top_index; UI_Alignment_Kind bottom_val; } alignment_kind_stack;
  struct { UI_Node *data[64]; u32 top_index; UI_Node *bottom_val; } parent_stack;
};

// -----------------------------------------------------------------------
// Globals
// -----------------------------------------------------------------------

global UI_Context UIContext;

read_only global UI_Node UINodeNilSentinel =
{
  .first    = &UINodeNilSentinel,
  .last     = &UINodeNilSentinel,
  .next     = &UINodeNilSentinel,
  .previous = &UINodeNilSentinel,
  .parent   = &UINodeNilSentinel,
};

// -----------------------------------------------------------------------
// Stack convenience macros (map to the inline stacks above)
// -----------------------------------------------------------------------

#define ui_stack_parent_push(val)             ui_stack_push(parent, val)
#define ui_stack_parent_pop()                 ui_stack_pop(parent)
#define ui_stack_parent_top()                 ui_stack_top(parent)

#define ui_stack_top_left_top()               ui_stack_top(top_left)
#define ui_stack_size_kind_x_top()            ui_stack_top(size_kind_x)
#define ui_stack_size_kind_y_top()            ui_stack_top(size_kind_y)
#define ui_stack_size_fixed_x_top()           ui_stack_top(size_fixed_x)
#define ui_stack_size_fixed_y_top()           ui_stack_top(size_fixed_y)
#define ui_stack_size_relative_x_top()        ui_stack_top(size_relative_x)
#define ui_stack_size_relative_y_top()        ui_stack_top(size_relative_y)
#define ui_stack_padding_fixed_top()          ui_stack_top(padding_fixed)
#define ui_stack_node_spacing_top()           ui_stack_top(node_spacing)
#define ui_stack_border_thickness_top()       ui_stack_top(border_thickness)
#define ui_stack_corner_roundness_factor_top() ui_stack_top(corner_roundness_factor)
#define ui_stack_alignment_kind_top()         ui_stack_top(alignment_kind)

// Single-use push macros (push a value that is consumed by the next _ui_node_from_string call)
#define ui_top_left(val)                  ui_stack_push(top_left, val)
#define ui_size_kind_x(val)               ui_stack_push(size_kind_x, val)
#define ui_size_kind_y(val)               ui_stack_push(size_kind_y, val)
#define ui_size_fixed_x(val)              ui_stack_push(size_fixed_x, val)
#define ui_size_fixed_y(val)              ui_stack_push(size_fixed_y, val)
#define ui_size_relative_x(val)           ui_stack_push(size_relative_x, val)
#define ui_size_relative_y(val)           ui_stack_push(size_relative_y, val)
#define ui_padding_fixed(val)             ui_stack_push(padding_fixed, val)
#define ui_node_spacing(val)              ui_stack_push(node_spacing, val)
#define ui_border_thickness(val)          ui_stack_push(border_thickness, val)
#define ui_corner_roundness_factor(val)   ui_stack_push(corner_roundness_factor, val)
#define ui_alignment_kind(val)            ui_stack_push(alignment_kind, val)

// -----------------------------------------------------------------------
// Window defer helper
// -----------------------------------------------------------------------

struct UI_Defer_Window_State
{
  u32        index;
  UI_Signal  signal;
};
#define defer_window(begin, end) \
  for (UI_Defer_Window_State _dw = {0, (begin)}; !_dw.index; _dw.index = 1, (end))
#define ui_window_wants_to_close() ui_close((_dw.signal))
#define ui_window_node()           (_dw.signal.node)
#define ui_window(text, x, y, w, h) \
  defer_window(ui_window_begin((text), (x), (y), (w), (h)), ui_window_end())

// -----------------------------------------------------------------------
// Error (debug only)
// -----------------------------------------------------------------------

#if DEBUG
# define ui_error(msg) _ui_error((msg), S(__FILE__), __LINE__)
function void inline _ui_error(String message, String file, u32 line);
#else
# define ui_error(msg)
#endif

// -----------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------

function void      ui_init(R_Font *font);
function void      ui_begin(u32 window_width, u32 window_height,
                            s32 mouse_x, s32 mouse_y,
                            s32 mouse_delta_x, s32 mouse_delta_y,
                            f32 delta_time, u64 frame_count);
function void      ui_end(void);
function b32       ui_is_mouse_in_ui(void);

function UI_Signal ui_window_begin(String text, s32 x, s32 y, s32 width, s32 height);
function void      ui_window_end(void);

function u32       ui_get_commands_count(void);
function UI_Command *ui_get_commands(void);

// Internal
function UI_Node        *_ui_node_from_string(String unique, String display, UI_Node_Flags flags);
function void            _ui_add_node_child(UI_Node *parent, UI_Node *child);
function void            _ui_update_tree_nodes(UI_Node *node);
function void            _ui_render_ui_tree(UI_Node *node);
function UI_Node_Cache  *_ui_get_cached_node(u64 hash);
function void            _ui_purge_old_cached_nodes(void);
function void            _ui_fill_signals_from_node(UI_Signal *signal);
function b32             _ui_is_mouse_in_node(UI_Node *node);
function b32             _ui_is_mouse_in_rect(UI_Rect rect);
function V2s32           _ui_get_node_absolute_top_left(UI_Node *node);
function String          _ui_clean_string(Arena *arena, String string);
function void            _ui_push_draw_rect_command(V2f32 top_left, V2f32 size, V4f32 color,
                                                    f32 rotation, f32 roundness, f32 smoothness,
                                                    f32 border_thickness, V4f32 border_color);
function void            _ui_push_draw_rect_with_texture_command(s32 texture_index,
                                                                 V2f32 uv_min, V2f32 uv_max,
                                                                 V2f32 top_left, V2f32 size,
                                                                 V4f32 color, f32 rotation,
                                                                 f32 roundness, f32 smoothness,
                                                                 f32 border_thickness, V4f32 border_color);
function void            _ui_push_draw_text_command(V2f32 top_left, f32 max_width, f32 max_height,
                                                    V4f32 color, String text);

#endif // UI_H
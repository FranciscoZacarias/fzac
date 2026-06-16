/* Generated code */

#ifndef UI_GENERATED_H
#define UI_GENERATED_H

typedef enum
{
  UI_Node_Flags_None = 0,
  UI_Node_Flags_Mouse_Clickable = (1 << 0), /* Makes this node emit a signal when clicked */
  UI_Node_Flags_Draggable_Panel = (1 << 1), /* Enables this node to be dragged (and all of their children) by any children or itself, that are draggbale. */
  UI_Node_Flags_Draggable = (1 << 2), /* Makes this node emit a signal when dragged */
  UI_Node_Flags_Hoverable = (1 << 3), /* Makes this node emit a signal when hovered */
  UI_Node_Flags_Text_Display = (1 << 4), /* The UI_Node will display the text on the cursor's position and skip the cursor relative to UI_Alignment_Kind */
  UI_Node_Flags_Text_Center_X = (1 << 5), /* If text is displayed, centers the text horizontally in the node  */
  UI_Node_Flags_Text_Center_Y = (1 << 6), /* If text is displayed, centers the text vertically in the node  */
  UI_Node_Flags_Text_Ignore_Clip = (1 << 7), /* Drawing text will simply be aligned by the sizes of it's node  */
  UI_Node_Flags_Resizable = (1 << 8), /* Node can be resized */
  UI_Node_Flags_Float_On_Parent = (1 << 9), /* Overrides parent's alignemnt rules to float on top of parent. Use top left to set the position within the parent */
} UI_Node_Flags;

typedef enum
{
  UI_Signal_Flags_None = 0,
  UI_Signal_Flags_Left_Pressed = (1 << 0), /* Emits signal if LMB is down */
  UI_Signal_Flags_Middle_Pressed = (1 << 1), /* Emits build signal if MMB is down */
  UI_Signal_Flags_Right_Pressed = (1 << 2), /* Emits signal if RMB is down */
  UI_Signal_Flags_Left_Clicked = (1 << 3), /* Emits signal if LMB is clicked */
  UI_Signal_Flags_Middle_Clicked = (1 << 4), /* Emits signal if MMB is clicked */
  UI_Signal_Flags_Right_Clicked = (1 << 5), /* Emits signal if RMB is clicked */
  UI_Signal_Flags_Mouse_Hovered = (1 << 6), /* Emits signal if cursor is hovering */
  UI_Signal_Flags_Close = (1 << 7), /* Emits a signal that the node wants to close */
} UI_Signal_Flags;

typedef enum
{
  UI_Size_Kind_None = 0,
  UI_Size_Kind_Relative, /* Sets the side of the node to be done relative to the parent's available clip space. I.e. If parent has 100 of height and it already had a node put in with 50 height, and we set the next child node to be 50relative, this child node's height will be 25, as per the 50 height available.  */
  UI_Size_Kind_Fixed, /* Sets the side of the node to be fixed (within parent's clip space)  */
  UI_Size_Kind_Copy_X, /* Copies the value of X, mutually exclusive with UI_Size_Kind_Copy_Y */
  UI_Size_Kind_Copy_Y, /* Copies the value of Y, mutually exclusive with UI_Size_Kind_Copy_X */
} UI_Size_Kind;
const char *ui_size_kind_to_string[] = {"UI_Size_Kind_Relative","UI_Size_Kind_Fixed","UI_Size_Kind_Copy_X","UI_Size_Kind_Copy_Y",};
#define ui_size_kind_not_handled(arena, kind) ui_error(Sf(arena, "Unhandled UI_Size_Kind kind: %s", ui_size_kind_to_string[kind]));

typedef enum
{
  UI_Alignment_Kind_None = 0,
  UI_Alignment_Kind_Y, /* After a node is placed, the cursor is moved vertically by the height of the node */
  UI_Alignment_Kind_X, /* After a node is placed, the cursor is moved horizontally by the width of the node */
  UI_Alignment_Kind_Float, /* AFter a node is placed, the cursor is not moved */
} UI_Alignment_Kind;
const char *ui_alignment_kind_to_string[] = {"UI_Alignment_Kind_Y","UI_Alignment_Kind_X","UI_Alignment_Kind_Float",};
#define ui_alignment_kind_not_handled(arena, kind) ui_error(Sf(arena, "Unhandled UI_Alignment_Kind kind: %s", ui_alignment_kind_to_string[kind]));

typedef enum
{
  UI_Layout_Kind_None = 0,
  UI_Layout_Kind_Row, /* Layout with fixed height and 100relative width */
  UI_Layout_Kind_Row_Fixed, /* Row layout with fixed width */
  UI_Layout_Kind_Column, /* Layout with fixed width and 100relative height */
  UI_Layout_Kind_Column_Fixed, /* Column layout with fixed height */
} UI_Layout_Kind;
const char *ui_layout_kind_to_string[] = {"UI_Layout_Kind_Row","UI_Layout_Kind_Row_Fixed","UI_Layout_Kind_Column","UI_Layout_Kind_Column_Fixed",};
#define ui_layout_kind_not_handled(arena, kind) ui_error(Sf(arena, "Unhandled UI_Layout_Kind kind: %s", ui_layout_kind_to_string[kind]));

typedef enum
{
  UI_Color_Theme_Light = 0,
  UI_Color_Theme_No_Background, /*  */
  UI_Color_Theme_White, /*  */
  UI_Color_Theme_Dark, /*  */
  UI_Color_Theme_Interactable, /* Borders show interactiveness */
  UI_Color_Theme_Interactable_Solid, /* Backgrounda and border show interactiveness */
} UI_Color_Theme_Style;

// Initializes all stack structures
#define ui_init_stacks(ctx) \
  ctx.parent_stack.top_index = 0; ctx.parent_stack.bottom_val = &UINodeNilSentinel; \
  ctx.top_left_stack.top_index = 0; ctx.top_left_stack.bottom_val = v2s32(0, 0); \
  ctx.padding_fixed_stack.top_index = 0; ctx.padding_fixed_stack.bottom_val = 0; \
  ctx.size_kind_x_stack.top_index = 0; ctx.size_kind_x_stack.bottom_val = UI_Size_Kind_Fixed; \
  ctx.size_kind_y_stack.top_index = 0; ctx.size_kind_y_stack.bottom_val = UI_Size_Kind_Fixed; \
  ctx.size_fixed_x_stack.top_index = 0; ctx.size_fixed_x_stack.bottom_val = 64; \
  ctx.size_fixed_y_stack.top_index = 0; ctx.size_fixed_y_stack.bottom_val = 64; \
  ctx.size_relative_x_stack.top_index = 0; ctx.size_relative_x_stack.bottom_val = 1.0f; \
  ctx.size_relative_y_stack.top_index = 0; ctx.size_relative_y_stack.bottom_val = 1.0f; \
  ctx.node_center_x_stack.top_index = 0; ctx.node_center_x_stack.bottom_val = false; \
  ctx.node_center_y_stack.top_index = 0; ctx.node_center_y_stack.bottom_val = false; \
  ctx.alignment_kind_stack.top_index = 0; ctx.alignment_kind_stack.bottom_val = UI_Alignment_Kind_Y; \
  ctx.node_spacing_stack.top_index = 0; ctx.node_spacing_stack.bottom_val = 0; \
  ctx.minimum_width_stack.top_index = 0; ctx.minimum_width_stack.bottom_val = 0; \
  ctx.minimum_height_stack.top_index = 0; ctx.minimum_height_stack.bottom_val = 0; \
  ctx.color_theme_style_stack.top_index = 0; ctx.color_theme_style_stack.bottom_val = UI_Color_Theme_Light; \
  ctx.texture_stack.top_index = 0; ctx.texture_stack.bottom_val = (UI_Texture){-1}; \
  ctx.text_color_stack.top_index = 0; ctx.text_color_stack.bottom_val = COLOR_CLEAR; \
  ctx.background_color_stack.top_index = 0; ctx.background_color_stack.bottom_val = COLOR_CLEAR; \
  ctx.background_transparency_stack.top_index = 0; ctx.background_transparency_stack.bottom_val = 1.0f; \
  ctx.border_color_stack.top_index = 0; ctx.border_color_stack.bottom_val = COLOR_CLEAR; \
  ctx.border_transparency_stack.top_index = 0; ctx.border_transparency_stack.bottom_val = 1.0f; \
  ctx.border_thickness_stack.top_index = 0; ctx.border_thickness_stack.bottom_val = 1; \
  ctx.corner_roundness_px_stack.top_index = 0; ctx.corner_roundness_px_stack.bottom_val = 0.0f; \

// Creates struct with all UI stacks
#define ui_config_stacks \
  struct \
  { \
    struct UI_Parent_stack { UI_Node* data[8]; u32 top_index; UI_Node* bottom_val; } parent_stack; /* Parent node of current node */ \
    struct UI_Top_left_stack { V2s32 data[8]; u32 top_index; V2s32 bottom_val; } top_left_stack; /* Top left position relative to parent's cursor */ \
    struct UI_Padding_fixed_stack { s32 data[8]; u32 top_index; s32 bottom_val; } padding_fixed_stack; /* Padding in all directions specified the border between node's bounds space and clip space */ \
    struct UI_Size_kind_x_stack { UI_Size_Kind data[8]; u32 top_index; UI_Size_Kind bottom_val; } size_kind_x_stack; /* How is width size calculated in this node */ \
    struct UI_Size_kind_y_stack { UI_Size_Kind data[8]; u32 top_index; UI_Size_Kind bottom_val; } size_kind_y_stack; /* How is height size calculated in this node */ \
    struct UI_Size_fixed_x_stack { s32 data[8]; u32 top_index; s32 bottom_val; } size_fixed_x_stack; /* Width size in pixels */ \
    struct UI_Size_fixed_y_stack { s32 data[8]; u32 top_index; s32 bottom_val; } size_fixed_y_stack; /* Height size in pixels */ \
    struct UI_Size_relative_x_stack { f32 data[8]; u32 top_index; f32 bottom_val; } size_relative_x_stack; /* Width size relative to AVAILABLE space in parent's node. Clamped between 0.0f, and 1.0f */ \
    struct UI_Size_relative_y_stack { f32 data[8]; u32 top_index; f32 bottom_val; } size_relative_y_stack; /* Height size relative to AVAILABLE space in parent's node. Clamped between 0.0f, and 1.0f */ \
    struct UI_Node_center_x_stack { b8 data[8]; u32 top_index; b8 bottom_val; } node_center_x_stack; /* Should this node be centered in the parent's available width? */ \
    struct UI_Node_center_y_stack { b8 data[8]; u32 top_index; b8 bottom_val; } node_center_y_stack; /* Should this node be centered in the parent's available height? */ \
    struct UI_Alignment_kind_stack { UI_Alignment_Kind data[8]; u32 top_index; UI_Alignment_Kind bottom_val; } alignment_kind_stack; /* What kind of alignment do the child nodes follow? */ \
    struct UI_Node_spacing_stack { s32 data[8]; u32 top_index; s32 bottom_val; } node_spacing_stack; /* How much spacing is set between the child nodes. */ \
    struct UI_Minimum_width_stack { s32 data[8]; u32 top_index; s32 bottom_val; } minimum_width_stack; /* Minimum width this node can have */ \
    struct UI_Minimum_height_stack { s32 data[8]; u32 top_index; s32 bottom_val; } minimum_height_stack; /* Minimum height this node can have */ \
    struct UI_Color_theme_style_stack { UI_Color_Theme_Style data[8]; u32 top_index; UI_Color_Theme_Style bottom_val; } color_theme_style_stack; /* Node's colors from UI_Node_COlor_Theme */ \
    struct UI_Texture_stack { UI_Texture data[8]; u32 top_index; UI_Texture bottom_val; } texture_stack; /* Display this image on the node */ \
    struct UI_Text_color_stack { V4f32 data[8]; u32 top_index; V4f32 bottom_val; } text_color_stack; /*  */ \
    struct UI_Background_color_stack { V4f32 data[8]; u32 top_index; V4f32 bottom_val; } background_color_stack; /*  */ \
    struct UI_Background_transparency_stack { f32 data[8]; u32 top_index; f32 bottom_val; } background_transparency_stack; /*  */ \
    struct UI_Border_color_stack { V4f32 data[8]; u32 top_index; V4f32 bottom_val; } border_color_stack; /*  */ \
    struct UI_Border_transparency_stack { f32 data[8]; u32 top_index; f32 bottom_val; } border_transparency_stack; /*  */ \
    struct UI_Border_thickness_stack { s32 data[8]; u32 top_index; s32 bottom_val; } border_thickness_stack; /* Border thickness */ \
    struct UI_Corner_roundness_px_stack { s32 data[8]; u32 top_index; s32 bottom_val; } corner_roundness_px_stack; /* How many pixels will the border be rounded around. */ \
  }

// Debug: Serves to check if all stacks are at the bottom by the end of the frame
#define ui_debug_stacks_at_bottom \
  ui_stack_assert_top_at(parent,1); \
  ui_stack_assert_top_at(top_left,0); \
  ui_stack_assert_top_at(padding_fixed,0); \
  ui_stack_assert_top_at(size_kind_x,0); \
  ui_stack_assert_top_at(size_kind_y,0); \
  ui_stack_assert_top_at(size_fixed_x,0); \
  ui_stack_assert_top_at(size_fixed_y,0); \
  ui_stack_assert_top_at(size_relative_x,0); \
  ui_stack_assert_top_at(size_relative_y,0); \
  ui_stack_assert_top_at(node_center_x,0); \
  ui_stack_assert_top_at(node_center_y,0); \
  ui_stack_assert_top_at(alignment_kind,0); \
  ui_stack_assert_top_at(node_spacing,0); \
  ui_stack_assert_top_at(minimum_width,0); \
  ui_stack_assert_top_at(minimum_height,0); \
  ui_stack_assert_top_at(color_theme_style,0); \
  ui_stack_assert_top_at(texture,0); \
  ui_stack_assert_top_at(text_color,0); \
  ui_stack_assert_top_at(background_color,0); \
  ui_stack_assert_top_at(background_transparency,0); \
  ui_stack_assert_top_at(border_color,0); \
  ui_stack_assert_top_at(border_transparency,0); \
  ui_stack_assert_top_at(border_thickness,0); \
  ui_stack_assert_top_at(corner_roundness_px,0); \


#endif // UI_GENERATED_H
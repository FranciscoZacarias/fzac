/* Generated code */

#ifndef UI_GENERATED_H
#define UI_GENERATED_H


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
  ctx.display_image_index_stack.top_index = 0; ctx.display_image_index_stack.bottom_val = -1; \
  ctx.text_color_stack.top_index = 0; ctx.text_color_stack.bottom_val = COLOR_CLEAR; \
  ctx.background_color_stack.top_index = 0; ctx.background_color_stack.bottom_val = COLOR_CLEAR; \
  ctx.background_transparency_stack.top_index = 0; ctx.background_transparency_stack.bottom_val = 1.0f; \
  ctx.border_color_stack.top_index = 0; ctx.border_color_stack.bottom_val = COLOR_CLEAR; \
  ctx.border_transparency_stack.top_index = 0; ctx.border_transparency_stack.bottom_val = 1.0f; \
  ctx.border_thickness_stack.top_index = 0; ctx.border_thickness_stack.bottom_val = 1; \
  ctx.corner_roundness_factor_stack.top_index = 0; ctx.corner_roundness_factor_stack.bottom_val = 1.0f; \

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
    struct UI_Display_image_index_stack { s32 data[8]; u32 top_index; s32 bottom_val; } display_image_index_stack; /* Display this image on the node */ \
    struct UI_Text_color_stack { V4f32 data[8]; u32 top_index; V4f32 bottom_val; } text_color_stack; /*  */ \
    struct UI_Background_color_stack { V4f32 data[8]; u32 top_index; V4f32 bottom_val; } background_color_stack; /*  */ \
    struct UI_Background_transparency_stack { f32 data[8]; u32 top_index; f32 bottom_val; } background_transparency_stack; /*  */ \
    struct UI_Border_color_stack { V4f32 data[8]; u32 top_index; V4f32 bottom_val; } border_color_stack; /*  */ \
    struct UI_Border_transparency_stack { f32 data[8]; u32 top_index; f32 bottom_val; } border_transparency_stack; /*  */ \
    struct UI_Border_thickness_stack { s32 data[8]; u32 top_index; s32 bottom_val; } border_thickness_stack; /* Border thickness */ \
    struct UI_Corner_roundness_factor_stack { f32 data[8]; u32 top_index; f32 bottom_val; } corner_roundness_factor_stack; /* How much of this color theme's border roundness we're gonna apply to this node. */ \
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
  ui_stack_assert_top_at(display_image_index,0); \
  ui_stack_assert_top_at(text_color,0); \
  ui_stack_assert_top_at(background_color,0); \
  ui_stack_assert_top_at(background_transparency,0); \
  ui_stack_assert_top_at(border_color,0); \
  ui_stack_assert_top_at(border_transparency,0); \
  ui_stack_assert_top_at(border_thickness,0); \
  ui_stack_assert_top_at(corner_roundness_factor,0); \


#endif // UI_GENERATED_H
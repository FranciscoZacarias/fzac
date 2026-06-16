/* Generated code */

function void ui_stack_parent_init(UI_Node* bot_val) { ui_stack_init(parent, bot_val); }
function void ui_stack_parent_push(UI_Node* val) { ui_stack_push(parent, val); }
function UI_Node* ui_stack_parent_pop() { return ui_stack_pop(parent); }
function UI_Node* ui_stack_parent_top() { return ui_stack_top(parent); }
function b32  ui_stack_parent_is_at_bottom() { return ui_stack_is_at_bottom(parent); }
function void ui_stack_parent_assert_top_at(u32 at) { ui_stack_assert_top_at(parent, at); }
#define ui_parent(val) defer_loop(ui_stack_parent_push(val), ui_stack_parent_pop())

function void ui_stack_top_left_init(V2s32 bot_val) { ui_stack_init(top_left, bot_val); }
function void ui_stack_top_left_push(V2s32 val) { ui_stack_push(top_left, val); }
function V2s32 ui_stack_top_left_pop() { return ui_stack_pop(top_left); }
function V2s32 ui_stack_top_left_top() { return ui_stack_top(top_left); }
function b32  ui_stack_top_left_is_at_bottom() { return ui_stack_is_at_bottom(top_left); }
function void ui_stack_top_left_assert_top_at(u32 at) { ui_stack_assert_top_at(top_left, at); }
#define ui_top_left(val) defer_loop(ui_stack_top_left_push(val), ui_stack_top_left_pop())

function void ui_stack_padding_fixed_init(s32 bot_val) { ui_stack_init(padding_fixed, bot_val); }
function void ui_stack_padding_fixed_push(s32 val) { ui_stack_push(padding_fixed, val); }
function s32 ui_stack_padding_fixed_pop() { return ui_stack_pop(padding_fixed); }
function s32 ui_stack_padding_fixed_top() { return ui_stack_top(padding_fixed); }
function b32  ui_stack_padding_fixed_is_at_bottom() { return ui_stack_is_at_bottom(padding_fixed); }
function void ui_stack_padding_fixed_assert_top_at(u32 at) { ui_stack_assert_top_at(padding_fixed, at); }
#define ui_padding_fixed(val) defer_loop(ui_stack_padding_fixed_push(val), ui_stack_padding_fixed_pop())

function void ui_stack_size_kind_x_init(UI_Size_Kind bot_val) { ui_stack_init(size_kind_x, bot_val); }
function void ui_stack_size_kind_x_push(UI_Size_Kind val) { ui_stack_push(size_kind_x, val); }
function UI_Size_Kind ui_stack_size_kind_x_pop() { return ui_stack_pop(size_kind_x); }
function UI_Size_Kind ui_stack_size_kind_x_top() { return ui_stack_top(size_kind_x); }
function b32  ui_stack_size_kind_x_is_at_bottom() { return ui_stack_is_at_bottom(size_kind_x); }
function void ui_stack_size_kind_x_assert_top_at(u32 at) { ui_stack_assert_top_at(size_kind_x, at); }
#define ui_size_kind_x(val) defer_loop(ui_stack_size_kind_x_push(val), ui_stack_size_kind_x_pop())

function void ui_stack_size_kind_y_init(UI_Size_Kind bot_val) { ui_stack_init(size_kind_y, bot_val); }
function void ui_stack_size_kind_y_push(UI_Size_Kind val) { ui_stack_push(size_kind_y, val); }
function UI_Size_Kind ui_stack_size_kind_y_pop() { return ui_stack_pop(size_kind_y); }
function UI_Size_Kind ui_stack_size_kind_y_top() { return ui_stack_top(size_kind_y); }
function b32  ui_stack_size_kind_y_is_at_bottom() { return ui_stack_is_at_bottom(size_kind_y); }
function void ui_stack_size_kind_y_assert_top_at(u32 at) { ui_stack_assert_top_at(size_kind_y, at); }
#define ui_size_kind_y(val) defer_loop(ui_stack_size_kind_y_push(val), ui_stack_size_kind_y_pop())

function void ui_stack_size_fixed_x_init(s32 bot_val) { ui_stack_init(size_fixed_x, bot_val); }
function void ui_stack_size_fixed_x_push(s32 val) { ui_stack_push(size_fixed_x, val); }
function s32 ui_stack_size_fixed_x_pop() { return ui_stack_pop(size_fixed_x); }
function s32 ui_stack_size_fixed_x_top() { return ui_stack_top(size_fixed_x); }
function b32  ui_stack_size_fixed_x_is_at_bottom() { return ui_stack_is_at_bottom(size_fixed_x); }
function void ui_stack_size_fixed_x_assert_top_at(u32 at) { ui_stack_assert_top_at(size_fixed_x, at); }
#define ui_size_fixed_x(val) defer_loop(ui_stack_size_fixed_x_push(val), ui_stack_size_fixed_x_pop())

function void ui_stack_size_fixed_y_init(s32 bot_val) { ui_stack_init(size_fixed_y, bot_val); }
function void ui_stack_size_fixed_y_push(s32 val) { ui_stack_push(size_fixed_y, val); }
function s32 ui_stack_size_fixed_y_pop() { return ui_stack_pop(size_fixed_y); }
function s32 ui_stack_size_fixed_y_top() { return ui_stack_top(size_fixed_y); }
function b32  ui_stack_size_fixed_y_is_at_bottom() { return ui_stack_is_at_bottom(size_fixed_y); }
function void ui_stack_size_fixed_y_assert_top_at(u32 at) { ui_stack_assert_top_at(size_fixed_y, at); }
#define ui_size_fixed_y(val) defer_loop(ui_stack_size_fixed_y_push(val), ui_stack_size_fixed_y_pop())

function void ui_stack_size_relative_x_init(f32 bot_val) { ui_stack_init(size_relative_x, bot_val); }
function void ui_stack_size_relative_x_push(f32 val) { ui_stack_push(size_relative_x, val); }
function f32 ui_stack_size_relative_x_pop() { return ui_stack_pop(size_relative_x); }
function f32 ui_stack_size_relative_x_top() { return ui_stack_top(size_relative_x); }
function b32  ui_stack_size_relative_x_is_at_bottom() { return ui_stack_is_at_bottom(size_relative_x); }
function void ui_stack_size_relative_x_assert_top_at(u32 at) { ui_stack_assert_top_at(size_relative_x, at); }
#define ui_size_relative_x(val) defer_loop(ui_stack_size_relative_x_push(val), ui_stack_size_relative_x_pop())

function void ui_stack_size_relative_y_init(f32 bot_val) { ui_stack_init(size_relative_y, bot_val); }
function void ui_stack_size_relative_y_push(f32 val) { ui_stack_push(size_relative_y, val); }
function f32 ui_stack_size_relative_y_pop() { return ui_stack_pop(size_relative_y); }
function f32 ui_stack_size_relative_y_top() { return ui_stack_top(size_relative_y); }
function b32  ui_stack_size_relative_y_is_at_bottom() { return ui_stack_is_at_bottom(size_relative_y); }
function void ui_stack_size_relative_y_assert_top_at(u32 at) { ui_stack_assert_top_at(size_relative_y, at); }
#define ui_size_relative_y(val) defer_loop(ui_stack_size_relative_y_push(val), ui_stack_size_relative_y_pop())

function void ui_stack_node_center_x_init(b8 bot_val) { ui_stack_init(node_center_x, bot_val); }
function void ui_stack_node_center_x_push(b8 val) { ui_stack_push(node_center_x, val); }
function b8 ui_stack_node_center_x_pop() { return ui_stack_pop(node_center_x); }
function b8 ui_stack_node_center_x_top() { return ui_stack_top(node_center_x); }
function b32  ui_stack_node_center_x_is_at_bottom() { return ui_stack_is_at_bottom(node_center_x); }
function void ui_stack_node_center_x_assert_top_at(u32 at) { ui_stack_assert_top_at(node_center_x, at); }
#define ui_node_center_x(val) defer_loop(ui_stack_node_center_x_push(val), ui_stack_node_center_x_pop())

function void ui_stack_node_center_y_init(b8 bot_val) { ui_stack_init(node_center_y, bot_val); }
function void ui_stack_node_center_y_push(b8 val) { ui_stack_push(node_center_y, val); }
function b8 ui_stack_node_center_y_pop() { return ui_stack_pop(node_center_y); }
function b8 ui_stack_node_center_y_top() { return ui_stack_top(node_center_y); }
function b32  ui_stack_node_center_y_is_at_bottom() { return ui_stack_is_at_bottom(node_center_y); }
function void ui_stack_node_center_y_assert_top_at(u32 at) { ui_stack_assert_top_at(node_center_y, at); }
#define ui_node_center_y(val) defer_loop(ui_stack_node_center_y_push(val), ui_stack_node_center_y_pop())

function void ui_stack_alignment_kind_init(UI_Alignment_Kind bot_val) { ui_stack_init(alignment_kind, bot_val); }
function void ui_stack_alignment_kind_push(UI_Alignment_Kind val) { ui_stack_push(alignment_kind, val); }
function UI_Alignment_Kind ui_stack_alignment_kind_pop() { return ui_stack_pop(alignment_kind); }
function UI_Alignment_Kind ui_stack_alignment_kind_top() { return ui_stack_top(alignment_kind); }
function b32  ui_stack_alignment_kind_is_at_bottom() { return ui_stack_is_at_bottom(alignment_kind); }
function void ui_stack_alignment_kind_assert_top_at(u32 at) { ui_stack_assert_top_at(alignment_kind, at); }
#define ui_alignment_kind(val) defer_loop(ui_stack_alignment_kind_push(val), ui_stack_alignment_kind_pop())

function void ui_stack_node_spacing_init(s32 bot_val) { ui_stack_init(node_spacing, bot_val); }
function void ui_stack_node_spacing_push(s32 val) { ui_stack_push(node_spacing, val); }
function s32 ui_stack_node_spacing_pop() { return ui_stack_pop(node_spacing); }
function s32 ui_stack_node_spacing_top() { return ui_stack_top(node_spacing); }
function b32  ui_stack_node_spacing_is_at_bottom() { return ui_stack_is_at_bottom(node_spacing); }
function void ui_stack_node_spacing_assert_top_at(u32 at) { ui_stack_assert_top_at(node_spacing, at); }
#define ui_node_spacing(val) defer_loop(ui_stack_node_spacing_push(val), ui_stack_node_spacing_pop())

function void ui_stack_minimum_width_init(s32 bot_val) { ui_stack_init(minimum_width, bot_val); }
function void ui_stack_minimum_width_push(s32 val) { ui_stack_push(minimum_width, val); }
function s32 ui_stack_minimum_width_pop() { return ui_stack_pop(minimum_width); }
function s32 ui_stack_minimum_width_top() { return ui_stack_top(minimum_width); }
function b32  ui_stack_minimum_width_is_at_bottom() { return ui_stack_is_at_bottom(minimum_width); }
function void ui_stack_minimum_width_assert_top_at(u32 at) { ui_stack_assert_top_at(minimum_width, at); }
#define ui_minimum_width(val) defer_loop(ui_stack_minimum_width_push(val), ui_stack_minimum_width_pop())

function void ui_stack_minimum_height_init(s32 bot_val) { ui_stack_init(minimum_height, bot_val); }
function void ui_stack_minimum_height_push(s32 val) { ui_stack_push(minimum_height, val); }
function s32 ui_stack_minimum_height_pop() { return ui_stack_pop(minimum_height); }
function s32 ui_stack_minimum_height_top() { return ui_stack_top(minimum_height); }
function b32  ui_stack_minimum_height_is_at_bottom() { return ui_stack_is_at_bottom(minimum_height); }
function void ui_stack_minimum_height_assert_top_at(u32 at) { ui_stack_assert_top_at(minimum_height, at); }
#define ui_minimum_height(val) defer_loop(ui_stack_minimum_height_push(val), ui_stack_minimum_height_pop())

function void ui_stack_color_theme_style_init(UI_Color_Theme_Style bot_val) { ui_stack_init(color_theme_style, bot_val); }
function void ui_stack_color_theme_style_push(UI_Color_Theme_Style val) { ui_stack_push(color_theme_style, val); }
function UI_Color_Theme_Style ui_stack_color_theme_style_pop() { return ui_stack_pop(color_theme_style); }
function UI_Color_Theme_Style ui_stack_color_theme_style_top() { return ui_stack_top(color_theme_style); }
function b32  ui_stack_color_theme_style_is_at_bottom() { return ui_stack_is_at_bottom(color_theme_style); }
function void ui_stack_color_theme_style_assert_top_at(u32 at) { ui_stack_assert_top_at(color_theme_style, at); }
#define ui_color_theme_style(val) defer_loop(ui_stack_color_theme_style_push(val), ui_stack_color_theme_style_pop())

function void ui_stack_texture_init(UI_Texture bot_val) { ui_stack_init(texture, bot_val); }
function void ui_stack_texture_push(UI_Texture val) { ui_stack_push(texture, val); }
function UI_Texture ui_stack_texture_pop() { return ui_stack_pop(texture); }
function UI_Texture ui_stack_texture_top() { return ui_stack_top(texture); }
function b32  ui_stack_texture_is_at_bottom() { return ui_stack_is_at_bottom(texture); }
function void ui_stack_texture_assert_top_at(u32 at) { ui_stack_assert_top_at(texture, at); }
#define ui_texture(val) defer_loop(ui_stack_texture_push(val), ui_stack_texture_pop())

function void ui_stack_text_color_init(V4f32 bot_val) { ui_stack_init(text_color, bot_val); }
function void ui_stack_text_color_push(V4f32 val) { ui_stack_push(text_color, val); }
function V4f32 ui_stack_text_color_pop() { return ui_stack_pop(text_color); }
function V4f32 ui_stack_text_color_top() { return ui_stack_top(text_color); }
function b32  ui_stack_text_color_is_at_bottom() { return ui_stack_is_at_bottom(text_color); }
function void ui_stack_text_color_assert_top_at(u32 at) { ui_stack_assert_top_at(text_color, at); }
#define ui_text_color(val) defer_loop(ui_stack_text_color_push(val), ui_stack_text_color_pop())

function void ui_stack_background_color_init(V4f32 bot_val) { ui_stack_init(background_color, bot_val); }
function void ui_stack_background_color_push(V4f32 val) { ui_stack_push(background_color, val); }
function V4f32 ui_stack_background_color_pop() { return ui_stack_pop(background_color); }
function V4f32 ui_stack_background_color_top() { return ui_stack_top(background_color); }
function b32  ui_stack_background_color_is_at_bottom() { return ui_stack_is_at_bottom(background_color); }
function void ui_stack_background_color_assert_top_at(u32 at) { ui_stack_assert_top_at(background_color, at); }
#define ui_background_color(val) defer_loop(ui_stack_background_color_push(val), ui_stack_background_color_pop())

function void ui_stack_background_transparency_init(f32 bot_val) { ui_stack_init(background_transparency, bot_val); }
function void ui_stack_background_transparency_push(f32 val) { ui_stack_push(background_transparency, val); }
function f32 ui_stack_background_transparency_pop() { return ui_stack_pop(background_transparency); }
function f32 ui_stack_background_transparency_top() { return ui_stack_top(background_transparency); }
function b32  ui_stack_background_transparency_is_at_bottom() { return ui_stack_is_at_bottom(background_transparency); }
function void ui_stack_background_transparency_assert_top_at(u32 at) { ui_stack_assert_top_at(background_transparency, at); }
#define ui_background_transparency(val) defer_loop(ui_stack_background_transparency_push(val), ui_stack_background_transparency_pop())

function void ui_stack_border_color_init(V4f32 bot_val) { ui_stack_init(border_color, bot_val); }
function void ui_stack_border_color_push(V4f32 val) { ui_stack_push(border_color, val); }
function V4f32 ui_stack_border_color_pop() { return ui_stack_pop(border_color); }
function V4f32 ui_stack_border_color_top() { return ui_stack_top(border_color); }
function b32  ui_stack_border_color_is_at_bottom() { return ui_stack_is_at_bottom(border_color); }
function void ui_stack_border_color_assert_top_at(u32 at) { ui_stack_assert_top_at(border_color, at); }
#define ui_border_color(val) defer_loop(ui_stack_border_color_push(val), ui_stack_border_color_pop())

function void ui_stack_border_transparency_init(f32 bot_val) { ui_stack_init(border_transparency, bot_val); }
function void ui_stack_border_transparency_push(f32 val) { ui_stack_push(border_transparency, val); }
function f32 ui_stack_border_transparency_pop() { return ui_stack_pop(border_transparency); }
function f32 ui_stack_border_transparency_top() { return ui_stack_top(border_transparency); }
function b32  ui_stack_border_transparency_is_at_bottom() { return ui_stack_is_at_bottom(border_transparency); }
function void ui_stack_border_transparency_assert_top_at(u32 at) { ui_stack_assert_top_at(border_transparency, at); }
#define ui_border_transparency(val) defer_loop(ui_stack_border_transparency_push(val), ui_stack_border_transparency_pop())

function void ui_stack_border_thickness_init(s32 bot_val) { ui_stack_init(border_thickness, bot_val); }
function void ui_stack_border_thickness_push(s32 val) { ui_stack_push(border_thickness, val); }
function s32 ui_stack_border_thickness_pop() { return ui_stack_pop(border_thickness); }
function s32 ui_stack_border_thickness_top() { return ui_stack_top(border_thickness); }
function b32  ui_stack_border_thickness_is_at_bottom() { return ui_stack_is_at_bottom(border_thickness); }
function void ui_stack_border_thickness_assert_top_at(u32 at) { ui_stack_assert_top_at(border_thickness, at); }
#define ui_border_thickness(val) defer_loop(ui_stack_border_thickness_push(val), ui_stack_border_thickness_pop())

function void ui_stack_corner_roundness_px_init(s32 bot_val) { ui_stack_init(corner_roundness_px, bot_val); }
function void ui_stack_corner_roundness_px_push(s32 val) { ui_stack_push(corner_roundness_px, val); }
function s32 ui_stack_corner_roundness_px_pop() { return ui_stack_pop(corner_roundness_px); }
function s32 ui_stack_corner_roundness_px_top() { return ui_stack_top(corner_roundness_px); }
function b32  ui_stack_corner_roundness_px_is_at_bottom() { return ui_stack_is_at_bottom(corner_roundness_px); }
function void ui_stack_corner_roundness_px_assert_top_at(u32 at) { ui_stack_assert_top_at(corner_roundness_px, at); }
#define ui_corner_roundness_px(val) defer_loop(ui_stack_corner_roundness_px_push(val), ui_stack_corner_roundness_px_pop())


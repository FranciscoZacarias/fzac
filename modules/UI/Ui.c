#include "generated/ui.cgen.c"

function void
ui_init(UI_Font font)
{
  UIColorThemeDark = (UI_Color_Theme)
  {
    .light_border_color        = v4f32(0.72f, 0.72f, 0.72f, 1.0f),
    .light_border_hover_color  = v4f32(0.93f, 0.93f, 0.93f, 1.0f),
    .light_border_active_color = v4f32(0.96f, 0.96f, 0.96f, 1.0f),

    .light_background_color        = v4f32(0.12f, 0.09f, 0.06f, 1.0f),
    .light_background_hover_color  = v4f32(0.16f, 0.12f, 0.08f, 1.0f),
    .light_background_active_color = v4f32(0.20f, 0.15f, 0.10f, 1.0f),

    .dark_border_color        = v4f32(0.22f, 0.14f, 0.08f, 1.0f),
    .dark_border_hover_color  = v4f32(0.32f, 0.20f, 0.12f, 1.0f),
    .dark_border_active_color = v4f32(0.42f, 0.26f, 0.16f, 1.0f),

    .dark_background_color         = v4f32(0.07f, 0.06f, 0.05f, 1.0f),
    .dark_background_hover_color   = v4f32(0.05f, 0.05f, 0.05f, 1.0f),
    .dark_background_active_color  = v4f32(0.10f, 0.08f, 0.06f, 1.0f),

    .interactable_border_color        = v4f32(0.68f, 0.39f, 0.00f, 1.0f),
    .interactable_border_hover_color  = v4f32(0.93f, 0.93f, 0.93f, 1.0f),
    .interactable_border_active_color = v4f32(0.77f, 0.59f, 0.1f, 1.0f),

    .interactable_background_color        = v4f32(0.68f, 0.39f, 0.00f, 1.0f),
    .interactable_background_hover_color  = v4f32(0.80f, 0.40f, 0.00f, 1.0f),
    .interactable_background_active_color = v4f32(0.77f, 0.59f, 0.1f, 1.0f),

    .text_color        = v4f32(0.79f, 0.76f, 0.71f, 1.0f),
    .text_hover_color  = v4f32(1.0f, 1.0f, 1.0f, 1.0f),
    .text_active_color = v4f32(0.92f, 0.92f, 0.92f, 1.0f),
  };

  memory_zero_struct(&UIContext);
  UIContext.arena       = arena_alloc();
  UIContext.frame_arena = arena_alloc();

  UIContext.animation_speed   = 10.0f;
  UIContext.color_theme       = UIColorThemeDark;
  UIContext.hash_hot_depth    = 1.0f;
  UIContext.hash_active_depth = 1.0f;
  UIContext.font              = font;
  UIContext.node_bounds_min_width  = UIContext.font.height * 3;
  UIContext.node_bounds_min_height = UIContext.font.height * 2;
  UIContext.resize_area_side  = 20;
  UIContext.window_min_width  = 150;
  UIContext.window_min_height = 50;

  array_init_with_arena(UIContext.arena, UIContext.commands, UI_Command, UI_MAX_COMMANDS);

  UIContext.style.corner_roundness = 0.2f;
  UIContext.style.border_thickness = 1.0f;

  array_init_with_arena(UIContext.arena, UIContext.cached_nodes, UI_Node_Cache, UI_MAX_CACHED_NODES);
  UIContext.cached_nodes_available = UIContext.cached_nodes_capacity;
  UIContext.cached_nodes_max_idle_lifetime_ms = UI_MAX_NODE_IDLE_LIFETIME_MS;

  UIContext.previous_frame.rects_arena = arena_alloc();
  array_init_with_arena(UIContext.previous_frame.rects_arena, UIContext.previous_frame.rects, UI_Rect, UI_MAX_PREVIOUS_FRAME_RECTS);

  ui_init_stacks(UIContext);

  // For quick iteration
  UIContext.debug.draw_debug_window               = false;
  UIContext.debug.draw_color_theme_window         = false;
  UIContext.debug.draw_clip                       = false;
  UIContext.debug.draw_cursor                     = false;
  UIContext.debug.draw_resize_picking_area        = false;
  UIContext.debug.draw_stuff_that_shouldnt_render = false;
  UIContext.debug.disable_window_snap             = false;
  UIContext.debug.disable_node_cache_clear        = false;
}

function void
ui_begin(u32 window_width, u32 window_height, u32 mouse_x, u32 mouse_y, u32 mouse_delta_x, u32 mouse_delta_y, f32 delta_time, u64 frame_count)
{
  UIContext.window_width  = window_width;
  UIContext.window_height = window_height;
  UIContext.mouse_x       = mouse_x;
  UIContext.mouse_y       = mouse_y;
  UIContext.mouse_delta_x = mouse_delta_x;
  UIContext.mouse_delta_y = mouse_delta_y;
  UIContext.delta_time    = delta_time;
  UIContext.frame_count   = frame_count;

  UIContext.commands_count = 0;

  // Root node, which is the window
  {
    UIContext.root = push_array(UIContext.frame_arena, UI_Node, 1);
    UI_Node* root_node = NULL;
    {
      String name = S("Root_Window");
      root_node = _ui_node_from_string(name, name, UI_Node_Flags_None);
      UIContext.root = root_node;
      UIContext.root->depth = 1.0f;
      ui_stack_parent_push(UIContext.root);
    }
  }
}

function void
ui_end()
{
  if (UIContext.debug.draw_debug_window)
  {
    ui_draw_debug_window();
  }
  if (UIContext.debug.draw_color_theme_window)
  {
    ui_draw_color_theme_window();
  }

#if DEBUG
  ui_debug_stacks_at_bottom;
  if (UIContext.root->depth != 1)
  {
    ui_error(Sf(UIContext.arena, "UI: UIContext.root->depth is expected to be 1. It was: %.10f\n", UIContext.root->depth));
  }
#endif // DEBUG

  _ui_render_ui_tree(UIContext.root);

  if (!UIContext.debug.disable_node_cache_clear)
  {
    _ui_purge_old_cached_nodes();
  }

  UIContext.previous_frame.rects_count    = 0;
  // Save UI rects to tell user space if UI is selected
  for (UI_Node *child = UIContext.root->first; child; child = child->next)
  {
    UI_Rect *rect;
    array_add(rect, UIContext.previous_frame.rects);
    *rect = child->bounds;
  }

  UIContext.hash_active_previous = UIContext.hash_active;

  UIContext.hash_hot = 0;
  UIContext.hash_hot_depth = 1.0f;

  UIContext.root->first    = NULL;
  UIContext.root->last     = NULL;
  UIContext.root->next     = NULL;
  UIContext.root->previous = NULL;
  UIContext.root->parent   = NULL;

  ui_stack_parent_pop(); // Pop root (os window) because it's regenerated every frame
  arena_clear(UIContext.frame_arena);
}

function b32
ui_is_mouse_in_ui()
{
  b32 result = false;
  for (u32 i = 0; i < UIContext.previous_frame.rects_count; i += 1)
  {
    if (_ui_is_mouse_in_rect(UIContext.previous_frame.rects[i]))
    {
      result = true;
      break;
    }
  }
  return result;
}

function UI_Signal
ui_window_begin(String text, s32 x, s32 y, s32 width, s32 height)
{
  UI_Signal signal = {0};

  // Window layout
  UI_Node *window_node = NULL;
  {
    ui_top_left(v2s32(x,y))
    ui_size_fixed_x(width)
    ui_size_fixed_y(height)
    ui_minimum_width(UIContext.font.height*5)
    ui_minimum_height(UIContext.font.height*2)
    ui_corner_roundness_factor(0.0f)
    {
      UI_Node_Flags window_flags = UI_Node_Flags_Draggable_Panel|UI_Node_Flags_Resizable;
      String window_text  = Sf(UIContext.frame_arena, S_FMT"##_window_", S_ARG(text));
      window_node         = _ui_node_from_string(window_text, window_text, window_flags);
      window_node->should_render = false;
      ui_stack_parent_push(window_node);
      signal.node = window_node;
    }
  }

  UI_Signal title_bar_layout_signal = {0};
  {
    ui_color_theme_style(UI_Color_Theme_Light)
    ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(1.0f)
    ui_size_kind_y(UI_Size_Kind_Fixed) ui_size_fixed_y(UIContext.font.height*2)
    ui_alignment_kind(UI_Alignment_Kind_X)
    ui_padding_fixed(1)
    ui_corner_roundness_factor(0.0f)
    {
      UI_Node_Flags title_bar_layout_flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Draggable|UI_Node_Flags_Mouse_Clickable;
      
      String title_bar_layout_text = string_zero();
      title_bar_layout_text = Sf(UIContext.frame_arena, S_FMT"##_title_bar_layout_", S_ARG(text));
      title_bar_layout_signal.node = _ui_node_from_string(title_bar_layout_text, title_bar_layout_text, title_bar_layout_flags);
      ui_stack_parent_push(title_bar_layout_signal.node);
    }
  }

  f32 title_bar_width  = title_bar_layout_signal.node->bounds.size.x;
  f32 title_bar_height = title_bar_layout_signal.node->bounds.size.y;
  f32 title_node_width = title_bar_width - title_bar_height;

  // Title bar node
  UI_Signal title_bar_signal = {0};
  {
    ui_color_theme_style(UI_Color_Theme_Dark)
    ui_size_kind_x(UI_Size_Kind_Fixed) ui_size_fixed_x(title_node_width)
    ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
    ui_border_thickness(0)
    ui_padding_fixed(5)
    ui_corner_roundness_factor(0.0f)
    {
      UI_Node_Flags title_bar_flags = UI_Node_Flags_Text_Display|UI_Node_Flags_Text_Center_Y|UI_Node_Flags_Hoverable|UI_Node_Flags_Draggable|UI_Node_Flags_Mouse_Clickable;
      String window_title_bar_text = Sf(UIContext.frame_arena, ""S_FMT"##_title_bar_", S_ARG(text));
      title_bar_signal.node = _ui_node_from_string(window_title_bar_text, window_title_bar_text, title_bar_flags);
      _ui_fill_signals_from_node(&title_bar_signal);
      set_flags(signal.flags, title_bar_signal.flags);
    }
  }

  // Close button layout
  {
    ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(1.0f)
    ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
    ui_padding_fixed(1)
    ui_corner_roundness_factor(0.0f)
    {
      UI_Node_Flags close_button_flags = 0;
      UI_Node *close_button_node = _ui_node_from_string(Sf(UIContext.frame_arena, ""S_FMT"##_close_button_layout_", S_ARG(text)), S(" "), close_button_flags);
      close_button_node->should_render = false;
      ui_stack_parent_push(close_button_node);
    }
  }

  UI_Signal close_button_signal = {0};
  {
    ui_color_theme_style(UI_Color_Theme_Interactable)
    ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(1.0f)
    ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
    {
      UI_Node_Flags close_button_flags = UI_Node_Flags_Text_Display|UI_Node_Flags_Text_Center_Y|UI_Node_Flags_Hoverable
                                        |UI_Node_Flags_Mouse_Clickable|UI_Node_Flags_Text_Center_X|UI_Node_Flags_Text_Center_Y;
      String close_button_text = Sf(UIContext.frame_arena, ""S_FMT"##_close_button_", S_ARG(text));
      close_button_signal.node = _ui_node_from_string(close_button_text, S("X"), close_button_flags);
      _ui_fill_signals_from_node(&close_button_signal);
      if (ui_clicked(close_button_signal ))
      {
        set_flags(signal.flags, UI_Signal_Flags_Close);
      }
    }
  }

  ui_stack_parent_pop(); // close_button_node
  ui_stack_parent_pop(); // title_bar_layout_signal.node

  // Body node
  {
    ui_color_theme_style(UI_Color_Theme_Light)
    ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(1.0f)
    ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
    ui_alignment_kind(UI_Alignment_Kind_Y)
    ui_padding_fixed(5)
    ui_node_spacing(5)
    ui_corner_roundness_factor(0.0f)
    {
      UI_Node_Flags window_body_flags = 0;
      String window_window_body_text = Sf(UIContext.frame_arena, ""S_FMT"##_window_body_", S_ARG(text));
      UI_Node *node = _ui_node_from_string(window_window_body_text, window_window_body_text, window_body_flags);
      ui_stack_parent_push(node);
    }
  }

  if (is_key_down(UI_WINDOW_SNAP_KEY))
  {
    if (UIContext.hash_active_previous == title_bar_signal.node->hash && !UIContext.debug.disable_window_snap)
    {
      // @NOTE(fz): I think it's okay here to just draw_quads directly since they are not really UI nodes. 

      s32 distance    = UIContext.window_width*0.1;
      s32 half_screen_height = UIContext.window_height/2;
      s32 snap_width  = 50;

      s32 middle_quad_height = UIContext.window_height*0.1;
      s32 small_quads_height = middle_quad_height/2;
      s32 quads_padding = 3;
      s32 total_height = small_quads_height + quads_padding + middle_quad_height + quads_padding + small_quads_height;

      // Middle
      {

        // Middle quad
        {
          V2f32 window_center = v2f32(UIContext.window_width/2, UIContext.window_height/2);
          V2f32 top_left = v2f32(window_center.x - (middle_quad_height/2), window_center.y - (middle_quad_height/2));
          V2f32 size = v2f32(middle_quad_height, middle_quad_height);

          UI_Rect quad;
          quad.top_left = v2s32(top_left.x, top_left.y);
          quad.size     = v2s32(size.x, size.y);

          V4f32 background_color = UIContext.color_theme.light_background_color;
          background_color.w = 0.5f;
          V4f32 border_color = UIContext.color_theme.interactable_background_color;
          border_color.w = 0.5f;

          if (_ui_is_mouse_in_rect(quad))
          {
            background_color.w = 0.6f;
            border_color.w     = 1.0f;
            
            _ui_push_draw_rect_command(v2f32(0,0), v2f32(UIContext.window_width, UIContext.window_height), background_color, 0.0f, 0.0f, 0.0f, 1.0f, border_color);

            if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
            {
              f32 target_w = UIContext.window_width;
              f32 target_h = UIContext.window_height;

              UI_Node_Cache *window_node_cache = _ui_get_cached_node(window_node->hash);

              s32 delta_w = (s32)target_w - width;
              s32 delta_h = (s32)target_h - height;
              
              window_node_cache->accumulated_drag_offset = v2s32(-x, -y);
              window_node_cache->accumulated_resize_offset.x = delta_w;
              window_node_cache->accumulated_resize_offset.y = delta_h;
            }
          }
          _ui_push_draw_rect_command(top_left, size, background_color, 0.0f, UIContext.style.corner_roundness*0.2f, 0.0f, 1.0f, border_color);
        }

        // Middle top
        {
          V2f32 window_center = v2f32(UIContext.window_width/2, UIContext.window_height/2);
          V2f32 size          = v2f32(middle_quad_height, small_quads_height);
          V2f32 top_left      = v2f32(window_center.x - size.x/2, window_center.y - (middle_quad_height/2) - size.y - quads_padding);

          UI_Rect quad;
          quad.top_left = v2s32(top_left.x, top_left.y);
          quad.size     = v2s32(size.x, size.y);

          V4f32 background_color = UIContext.color_theme.light_background_color;
          background_color.w = 0.5f;
          V4f32 border_color = UIContext.color_theme.interactable_background_color;
          border_color.w = 0.5f;

          if (_ui_is_mouse_in_rect(quad))
          {
            background_color.w = 0.6f;
            border_color.w     = 1.0f;
            _ui_push_draw_rect_command(v2f32(0,0), v2f32(UIContext.window_width, UIContext.window_height/2), background_color,0.0f,0.0f,0.0f, 1.0f, border_color);
            if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
            {
              f32 target_w = UIContext.window_width;
              f32 target_h = UIContext.window_height/2;

              UI_Node_Cache *window_node_cache = _ui_get_cached_node(window_node->hash);

              s32 delta_w = (s32)target_w - width;
              s32 delta_h = (s32)target_h - height;
              
              window_node_cache->accumulated_drag_offset = v2s32(-x, -y);
              window_node_cache->accumulated_resize_offset.x = delta_w;
              window_node_cache->accumulated_resize_offset.y = delta_h;
            }
          }
          _ui_push_draw_rect_command(top_left, size, background_color, 0.0f, UIContext.style.corner_roundness*0.5f,0.0f, 1.0f, border_color);
        }

        // Middle left
        {
          V2f32 window_center = v2f32(UIContext.window_width/2, UIContext.window_height/2);
          V2f32 size          = v2f32(small_quads_height, middle_quad_height);
          V2f32 top_left      = v2f32(window_center.x - (middle_quad_height/2) - size.x - quads_padding, window_center.y - size.y/2);

          UI_Rect quad;
          quad.top_left = v2s32(top_left.x, top_left.y);
          quad.size     = v2s32(size.x, size.y);

          V4f32 background_color = UIContext.color_theme.light_background_color;
          background_color.w = 0.5f;
          V4f32 border_color = UIContext.color_theme.interactable_background_color;
          border_color.w = 0.5f;

          if (_ui_is_mouse_in_rect(quad))
          {
            background_color.w = 0.6f;
            border_color.w     = 1.0f;
            _ui_push_draw_rect_command(v2f32(0,0), v2f32(UIContext.window_width/2, UIContext.window_height), background_color, 0.0f,0.0f,0.0f, 1.0f, border_color);
            if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
            {
              f32 target_w = UIContext.window_width/2;
              f32 target_h = UIContext.window_height;

              UI_Node_Cache *window_node_cache = _ui_get_cached_node(window_node->hash);

              s32 delta_w = (s32)target_w - width;
              s32 delta_h = (s32)target_h - height;
              
              window_node_cache->accumulated_drag_offset = v2s32(-x, -y);
              window_node_cache->accumulated_resize_offset.x = delta_w;
              window_node_cache->accumulated_resize_offset.y = delta_h;
            }
          }
          _ui_push_draw_rect_command(top_left, size, background_color, 0.0f, UIContext.style.corner_roundness*0.5f,0.0f, 1.0f, border_color);
        }

        // Middle right
        {
          V2f32 window_center = v2f32(UIContext.window_width/2, UIContext.window_height/2);
          V2f32 size          = v2f32(small_quads_height, middle_quad_height);
          V2f32 top_left      = v2f32(window_center.x + (middle_quad_height/2) + quads_padding, window_center.y - size.y/2);

          UI_Rect quad;
          quad.top_left = v2s32(top_left.x, top_left.y);
          quad.size     = v2s32(size.x, size.y);

          V4f32 background_color = UIContext.color_theme.light_background_color;
          background_color.w = 0.5f;
          V4f32 border_color = UIContext.color_theme.interactable_background_color;
          border_color.w = 0.5f;

          if (_ui_is_mouse_in_rect(quad))
          {
            background_color.w = 0.6f;
            border_color.w     = 1.0f;
            _ui_push_draw_rect_command(v2f32(UIContext.window_width/2,0), v2f32(UIContext.window_width/2, UIContext.window_height), background_color, 0.0f,0.0f,0.0f, 1.0f, border_color);
            if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
            {
              f32 target_w = UIContext.window_width/2;
              f32 target_h = UIContext.window_height;

              UI_Node_Cache *window_node_cache = _ui_get_cached_node(window_node->hash);

              s32 delta_w = (s32)target_w - width;
              s32 delta_h = (s32)target_h - height;
              
              window_node_cache->accumulated_drag_offset = v2s32((UIContext.window_width - target_w) - x, -y);
              window_node_cache->accumulated_resize_offset.x = delta_w;
              window_node_cache->accumulated_resize_offset.y = delta_h;
            }
          }
          _ui_push_draw_rect_command(top_left, size, background_color, 0.0f, UIContext.style.corner_roundness*0.5f,0.0f, 1.0f, border_color);
        }

        // Middle bottom
        {
          V2f32 window_center = v2f32(UIContext.window_width/2, UIContext.window_height/2);
          V2f32 size          = v2f32(middle_quad_height, small_quads_height);
          V2f32 top_left      = v2f32(window_center.x - size.x/2, window_center.y + (middle_quad_height/2) + quads_padding);

          UI_Rect quad;
          quad.top_left = v2s32(top_left.x, top_left.y);
          quad.size     = v2s32(size.x, size.y);

          V4f32 background_color = UIContext.color_theme.light_background_color;
          background_color.w = 0.5f;
          V4f32 border_color = UIContext.color_theme.interactable_background_color;
          border_color.w = 0.5f;

          if (_ui_is_mouse_in_rect(quad))
          {
            background_color.w = 0.6f;
            border_color.w     = 1.0f;
            _ui_push_draw_rect_command(v2f32(0,UIContext.window_height/2), v2f32(UIContext.window_width, UIContext.window_height/2), background_color, 0.0f,0.0f,0.0f, 1.0f, border_color);
            if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
            {
              f32 target_w = UIContext.window_width;
              f32 target_h = UIContext.window_height/2;

              UI_Node_Cache *window_node_cache = _ui_get_cached_node(window_node->hash);

              s32 delta_w = (s32)target_w - width;
              s32 delta_h = (s32)target_h - height;
              
              window_node_cache->accumulated_drag_offset = v2s32(-x, UIContext.window_height - target_h -y);
              window_node_cache->accumulated_resize_offset.x = delta_w;
              window_node_cache->accumulated_resize_offset.y = delta_h;
            }
          }
          _ui_push_draw_rect_command(top_left, size, background_color,  0.0f, UIContext.style.corner_roundness*0.5f, 0.0f, 1.0f, border_color);
        }
      }

      // Left side
      {
        V2f32 top_left_base;
        top_left_base.x = distance/2;
        top_left_base.y = half_screen_height - (total_height/2);

        // Top
        {
          V2f32 top_left = top_left_base;
          V2f32 size     = v2f32(snap_width, small_quads_height);

          UI_Rect quad;
          quad.top_left = v2s32(top_left.x, top_left.y);
          quad.size     = v2s32(size.x, size.y);

          V4f32 background_color = UIContext.color_theme.light_background_color;
          background_color.w = 0.5f;
          V4f32 border_color = UIContext.color_theme.interactable_background_color;
          border_color.w = 0.5f;

          if (_ui_is_mouse_in_rect(quad))
          {
            background_color.w = 0.6f;
            border_color.w     = 1.0f;
            _ui_push_draw_rect_command(v2f32(0,0), v2f32(UIContext.window_width*0.2f, UIContext.window_height/2), background_color, 0.0f,0.0f,0.0f, 1.0f, border_color);
            if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
            {
              f32 target_w = UIContext.window_width * 0.2f;
              f32 target_h = UIContext.window_height * 0.5f;

              UI_Node_Cache *window_node_cache = _ui_get_cached_node(window_node->hash);

              s32 delta_w = (s32)target_w - width;
              s32 delta_h = (s32)target_h - height;
              
              window_node_cache->accumulated_drag_offset = v2s32(-x, -y);
              window_node_cache->accumulated_resize_offset.x = delta_w;
              window_node_cache->accumulated_resize_offset.y = delta_h;
            }
          }
          _ui_push_draw_rect_command(top_left, size, background_color, 0.0f, UIContext.style.corner_roundness*0.5f, 0.0f, 1.0f, border_color);
        }

        // Middle
        {
          V2f32 top_left;
          top_left.x = top_left_base.x;
          top_left.y = top_left_base.y + small_quads_height + quads_padding;

          V2f32 size = v2f32(snap_width, middle_quad_height);

          UI_Rect quad;
          quad.top_left = v2s32(top_left.x, top_left.y);
          quad.size     = v2s32(size.x, size.y);

          V4f32 background_color = UIContext.color_theme.light_background_color;
          background_color.w = 0.5f;
          V4f32 border_color = UIContext.color_theme.interactable_background_color;
          border_color.w = 0.5f;

          if (_ui_is_mouse_in_rect(quad))
          {
            background_color.w = 0.6f;
            border_color.w     = 1.0f;

            _ui_push_draw_rect_command(v2f32(0,0), v2f32(UIContext.window_width*0.2f, UIContext.window_height), background_color, 0.0f,0.0f,0.0f, 1.0f, border_color);
            if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
            {
              f32 target_w = UIContext.window_width * 0.2f;
              f32 target_h = UIContext.window_height;

              UI_Node_Cache *window_node_cache = _ui_get_cached_node(window_node->hash);

              s32 delta_w = (s32)target_w - width;
              s32 delta_h = (s32)target_h - height;

              window_node_cache->accumulated_drag_offset = v2s32(-x, -y);
              window_node_cache->accumulated_resize_offset.x = delta_w;
              window_node_cache->accumulated_resize_offset.y = delta_h;
            }
          }
          _ui_push_draw_rect_command(top_left,size, background_color, 0.0f, UIContext.style.corner_roundness*0.5f, 0.0f, 1.0f, border_color);
        }

        // Bottom
        {
          V2f32 top_left;
          top_left.x = top_left_base.x;
          top_left.y = top_left_base.y + small_quads_height + quads_padding + middle_quad_height + quads_padding;

          V2f32 size = v2f32(snap_width, small_quads_height);

          UI_Rect quad;
          quad.top_left = v2s32(top_left.x, top_left.y);
          quad.size     = v2s32(size.x, size.y);

          V4f32 background_color = UIContext.color_theme.light_background_color;
          background_color.w = 0.5f;
          V4f32 border_color = UIContext.color_theme.interactable_background_color;
          border_color.w = 0.5f;

          if (_ui_is_mouse_in_rect(quad))
          {
            background_color.w = 0.6f;
            border_color.w     = 1.0f;

            _ui_push_draw_rect_command(v2f32(0,UIContext.window_height/2), v2f32(UIContext.window_width*0.2f, UIContext.window_height/2), background_color, 0.0f,0.0f,0.0f, 1.0f, border_color);
            if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
            {
              f32 target_w = UIContext.window_width * 0.2f;
              f32 target_h = UIContext.window_height * 0.5f;

              UI_Node_Cache *window_node_cache = _ui_get_cached_node(window_node->hash);

              s32 delta_w = (s32)target_w - width;
              s32 delta_h = (s32)target_h - height;

              window_node_cache->accumulated_drag_offset = v2s32(-x, (UIContext.window_height/2) - y);
              window_node_cache->accumulated_resize_offset.x = delta_w;
              window_node_cache->accumulated_resize_offset.y = delta_h;
            }
          }
          _ui_push_draw_rect_command(top_left,size, background_color, 0.0f, UIContext.style.corner_roundness*0.5f, 0.0f, 1.0f, border_color);
        }
      }

      // Right side
      {
        V2f32 top_left_base;
        top_left_base.x = UIContext.window_width - (distance/2) - snap_width;
        top_left_base.y = half_screen_height - (total_height/2);

        // Top
        {
          V2f32 top_left = top_left_base;
          V2f32 size     = v2f32(snap_width, small_quads_height);

          UI_Rect quad;
          quad.top_left = v2s32(top_left.x, top_left.y);
          quad.size     = v2s32(size.x, size.y);

          V4f32 background_color = UIContext.color_theme.light_background_color;
          background_color.w = 0.5f;
          V4f32 border_color = UIContext.color_theme.interactable_background_color;
          border_color.w = 0.5f;

          if (_ui_is_mouse_in_rect(quad))
          {
            background_color.w = 0.6f;
            border_color.w     = 1.0f;

            _ui_push_draw_rect_command(v2f32(UIContext.window_width - UIContext.window_width*0.2f,0), v2f32(UIContext.window_width*0.2f, UIContext.window_height/2), background_color, 0.0f,0.0f,0.0f, 1.0f, border_color);
            if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
            {
              f32 target_w = UIContext.window_width * 0.2f;
              f32 target_h = UIContext.window_height * 0.5f;

              UI_Node_Cache *window_node_cache = _ui_get_cached_node(window_node->hash);

              s32 delta_w = (s32)target_w - width;
              s32 delta_h = (s32)target_h - height;

              window_node_cache->accumulated_drag_offset = v2s32((UIContext.window_width - target_w) - x, -y);
              window_node_cache->accumulated_resize_offset.x = delta_w;
              window_node_cache->accumulated_resize_offset.y = delta_h;
            }
          }
          _ui_push_draw_rect_command(top_left,size, background_color, 0.0f, UIContext.style.corner_roundness*0.5f, 0.0f, 1.0f, border_color);
        }

        // Middle
        {
          V2f32 top_left;
          top_left.x = top_left_base.x;
          top_left.y = top_left_base.y + small_quads_height + quads_padding;

          V2f32 size = v2f32(snap_width, middle_quad_height);

          UI_Rect quad;
          quad.top_left = v2s32(top_left.x, top_left.y);
          quad.size     = v2s32(size.x, size.y);

          V4f32 background_color = UIContext.color_theme.light_background_color;
          background_color.w = 0.5f;
          V4f32 border_color = UIContext.color_theme.interactable_background_color;
          border_color.w = 0.5f;

          if (_ui_is_mouse_in_rect(quad))
          {
            background_color.w = 0.6f;
            border_color.w     = 1.0f;

            _ui_push_draw_rect_command(v2f32(UIContext.window_width - UIContext.window_width*0.2f,0), v2f32(UIContext.window_width*0.2f, UIContext.window_height), background_color, 0.0f,0.0f,0.0f, 1.0f, border_color);
            if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
            {
              f32 target_w = UIContext.window_width * 0.2f;
              f32 target_h = UIContext.window_height;

              UI_Node_Cache *window_node_cache = _ui_get_cached_node(window_node->hash);

              s32 delta_w = (s32)target_w - width;
              s32 delta_h = (s32)target_h - height;

              window_node_cache->accumulated_drag_offset = v2s32((UIContext.window_width - target_w) - x, -y);
              window_node_cache->accumulated_resize_offset.x = delta_w;
              window_node_cache->accumulated_resize_offset.y = delta_h;
            }
          }
          _ui_push_draw_rect_command(top_left,size, background_color, 0.0f, UIContext.style.corner_roundness*0.5f, 0.0f, 1.0f, border_color);
        }

        // Bottom
        {
          V2f32 top_left;
          top_left.x = top_left_base.x;
          top_left.y = top_left_base.y +
                      small_quads_height + quads_padding +
                      middle_quad_height + quads_padding;

          V2f32 size = v2f32(snap_width, small_quads_height);

          UI_Rect quad;
          quad.top_left = v2s32(top_left.x, top_left.y);
          quad.size     = v2s32(size.x, size.y);

          V4f32 background_color = UIContext.color_theme.light_background_color;
          background_color.w = 0.5f;
          V4f32 border_color = UIContext.color_theme.interactable_background_color;
          border_color.w = 0.5f;

          if (_ui_is_mouse_in_rect(quad))
          {
            background_color.w = 0.6f;
            border_color.w     = 1.0f;

            _ui_push_draw_rect_command(v2f32(UIContext.window_width - UIContext.window_width*0.2f, UIContext.window_height/2), v2f32(UIContext.window_width*0.2f, UIContext.window_height/2), background_color, 0.0f,0.0f,0.0f, 1.0f, border_color);
            if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
            {
              f32 target_w = UIContext.window_width * 0.2f;
              f32 target_h = UIContext.window_height * 0.5f;

              UI_Node_Cache *window_node_cache = _ui_get_cached_node(window_node->hash);

              s32 delta_w = (s32)target_w - width;
              s32 delta_h = (s32)target_h - height;

              window_node_cache->accumulated_drag_offset = v2s32((UIContext.window_width - target_w) - x, (UIContext.window_height/2) - y);
              window_node_cache->accumulated_resize_offset.x = delta_w;
              window_node_cache->accumulated_resize_offset.y = delta_h;
            }
          }
          _ui_push_draw_rect_command(top_left,size, background_color, 0.0f, UIContext.style.corner_roundness*0.5f, 0.0f, 1.0f, border_color);
        }
      }
    }
  }

  return signal;
}

function void
ui_window_end()
{
  UI_Node *body_node = ui_stack_parent_pop();
  UI_Node *top_node  = ui_stack_parent_top(); // Should be window node
  assert(string_contains(top_node->string, S("##_window_")));

  UI_Node_Cache *top_node_cache = _ui_get_cached_node(top_node->hash);
  V2s32 resized_clip_size = v2s32_add(top_node->clip.size, top_node_cache->accumulated_resize_offset);

  // We're adding the resize quad
  UI_Signal resize_quad = {0};
  {
    ui_color_theme_style(UI_Color_Theme_Dark)
    ui_top_left(v2s32(top_node->clip.top_left.x + resized_clip_size.x - UIContext.resize_area_side - 2,
                      top_node->clip.top_left.y + resized_clip_size.y - UIContext.resize_area_side - 2))
    ui_size_kind_x(UI_Size_Kind_Fixed) ui_size_fixed_x(UIContext.resize_area_side)
    ui_size_kind_y(UI_Size_Kind_Fixed) ui_size_fixed_y(UIContext.resize_area_side)
    ui_padding_fixed(1)
    {
      UI_Node_Flags title_bar_layout_flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable|UI_Node_Flags_Float_On_Parent;
      String resize_quad_str = Sf(UIContext.frame_arena, S_FMT"##_window_resize_quad", S_ARG(top_node  ->string));
      resize_quad.node = _ui_node_from_string(resize_quad_str, resize_quad_str, title_bar_layout_flags);
    }
  }

  UI_Node *window_node = ui_stack_parent_pop();
  _ui_update_tree_nodes(window_node);
}

function UI_Signal
ui_button(String text)
{
  UI_Text_Metrics text_metrics = ui_measure_text(text);
  UI_Signal signal = (UI_Signal){0};
  {
    ui_color_theme_style(UI_Color_Theme_Interactable)
    ui_size_kind_x(UI_Size_Kind_Fixed) ui_size_fixed_x((u32)text_metrics.width  + (UIContext.font.height))
    ui_size_kind_y(UI_Size_Kind_Fixed) ui_size_fixed_y((u32)text_metrics.height + (UIContext.font.height/2))
    {
      UI_Node_Flags label_flags = UI_Node_Flags_Text_Display|UI_Node_Flags_Text_Center_X|UI_Node_Flags_Text_Center_Y|
                                  UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable;
      signal.node  = _ui_node_from_string(text, text, label_flags);
      _ui_fill_signals_from_node(&signal);
    }
  }
  if (ui_stack_parent_top() == UIContext.root)
  {
    _ui_update_tree_nodes(signal.node);
  }
  return signal;
}

function UI_Signal
ui_text(String text)
{
  return ui_textf(text);
}

function UI_Signal
ui_textf(String fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  String text = string_from_format_va(UIContext.frame_arena, (const char*)fmt.cstring, args);
  va_end(args);

  // Measure using the display string
  UI_Text_Metrics metrics = ui_measure_text(text);
  UI_Signal signal = {0};
  {
    ui_color_theme_style(UI_Color_Theme_No_Background)
    ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(1.0f)
    ui_size_kind_y(UI_Size_Kind_Fixed)    ui_size_fixed_y((u32)metrics.height)
    {
      UI_Node_Flags flags = UI_Node_Flags_Text_Display;
      signal.node = _ui_node_from_string(fmt, text, flags);
      _ui_fill_signals_from_node(&signal);
    }
  }

  if (ui_stack_parent_top() == UIContext.root)
  {
    _ui_update_tree_nodes(signal.node);
  }

  return signal;
}

function UI_Signal
ui_text_colored(String text, V4f32 color)
{
  return ui_textf_colored(color, text);
}

function UI_Signal
ui_textf_colored(V4f32 color, String fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  String text = string_from_format_va(UIContext.frame_arena, (const char*)fmt.cstring, args);
  va_end(args);

  // Measure using the display string
  UI_Text_Metrics metrics = ui_measure_text(text);
  UI_Signal signal = {0};
  {
    ui_color_theme_style(UI_Color_Theme_No_Background)
    ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(1.0f)
    ui_size_kind_y(UI_Size_Kind_Fixed)    ui_size_fixed_y((u32)metrics.height)
    ui_text_color(color)
    {
      UI_Node_Flags flags = UI_Node_Flags_Text_Display;
      signal.node = _ui_node_from_string(fmt, text, flags);
      _ui_fill_signals_from_node(&signal);
    }
  }

  if (ui_stack_parent_top() == UIContext.root)
  {
    _ui_update_tree_nodes(signal.node);
  }

  return signal;
}

function UI_Signal
ui_checkbox(String text, b8 *checked)
{
  UI_Text_Metrics text_metrics = ui_measure_text(text);

  u32 layout_node_height = (u32)(UIContext.font.height*1.3) ;
  u32 spacing = 5;
  u32 checkbox_and_label_width = (u32)text_metrics.width + layout_node_height + spacing;

  // Layout node
  UI_Signal layout_signal = {0};
  {
    ui_size_kind_x(UI_Size_Kind_Fixed) ui_size_fixed_x(checkbox_and_label_width)
    ui_size_kind_y(UI_Size_Kind_Fixed) ui_size_fixed_y(layout_node_height)
    ui_alignment_kind(UI_Alignment_Kind_X)
    ui_node_spacing(spacing)
    ui_border_thickness(0)
    {
      UI_Node_Flags title_bar_flags = 0;
      layout_signal.node = _ui_node_from_string(text, text, title_bar_flags);
      layout_signal.node->should_render = false;
      ui_stack_parent_push(layout_signal.node);
    }
  }

  // Checkbox
  UI_Signal checkbox_signal = {0};
  UI_Signal checked_signal  = {0};
  {
    if (layout_signal.node->parent->bounds.size.x >= layout_signal.node->bounds.size.y + (s32)spacing)
    {
      ui_color_theme_style(UI_Color_Theme_Interactable)
      ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
      ui_size_kind_x(UI_Size_Kind_Copy_Y)
      ui_alignment_kind(UI_Alignment_Kind_X)
      {
        UI_Node_Flags title_bar_flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable;
        checkbox_signal.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_empty", text.cstring), S(" "), title_bar_flags);
        _ui_fill_signals_from_node(&checkbox_signal);
      }
    }
    else
    {
      ui_color_theme_style(UI_Color_Theme_Interactable)
      ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
      ui_size_kind_x(UI_Size_Kind_Fixed)    ui_size_fixed_x(layout_signal.node->parent->bounds.size.x)
      ui_alignment_kind(UI_Alignment_Kind_X)
      {
        UI_Node_Flags title_bar_flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable;
        checkbox_signal.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_empty", text.cstring), S(" "), title_bar_flags);
        _ui_fill_signals_from_node(&checkbox_signal);
      }
    }

    if (*checked)
    {
      ui_stack_parent_push(checkbox_signal.node);

      ui_color_theme_style(UI_Color_Theme_Interactable_Solid)
      ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(0.6f)
      ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(0.6f)
      ui_node_center_x(true) ui_node_center_y(true)
      {
        UI_Node_Flags checked_flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable;
        checked_signal.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_checked", text.cstring), S(" "), checked_flags);
        _ui_fill_signals_from_node(&checked_signal);
      }

      ui_stack_parent_pop();
    }
  }

  // Layout node for text
  UI_Text_Metrics metrics = ui_measure_text(text);
  {
    ui_color_theme_style(UI_Color_Theme_Light)
    ui_size_kind_x(UI_Size_Kind_Fixed)    ui_size_fixed_x((u32)metrics.width)
    ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
    ui_alignment_kind(UI_Alignment_Kind_X)
    {
      UI_Node_Flags layout_node_for_text = 0;
      UI_Node *node = _ui_node_from_string(text, Sf(UIContext.frame_arena, "%s_label", text.cstring), layout_node_for_text);
      node->should_render = false;
      ui_stack_parent_push(node);
    }
  }

  // Text
  UI_Signal checkbox_text_signal = {0};
  {
    ui_color_theme_style(UI_Color_Theme_Light)
    ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(1.0f)
    ui_size_kind_y(UI_Size_Kind_Fixed)    ui_size_fixed_y(layout_node_height)
    ui_border_thickness(0)
    {
      UI_Node_Flags flags = UI_Node_Flags_Text_Display|UI_Node_Flags_Text_Center_Y|UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable;
      checkbox_text_signal.node = _ui_node_from_string(text, text, flags);
      _ui_fill_signals_from_node(&checkbox_text_signal);
    }
  }

  if (ui_clicked(checkbox_signal) || ui_clicked(checkbox_text_signal))
  {
    *checked = (*checked) ? 0 : 1;
  }

  ui_stack_parent_pop(); // Text node
  ui_stack_parent_pop(); // Layout node

  UI_Signal result_signal = layout_signal;
  set_flags(result_signal.flags, checkbox_signal.flags);
  set_flags(result_signal.flags, checked_signal.flags);
  set_flags(result_signal.flags, checkbox_text_signal.flags);

  if (ui_stack_parent_top() == UIContext.root)
  {
    _ui_update_tree_nodes(result_signal.node);
  }

  return result_signal;
}

function UI_Signal
ui_color(String text, V4f32 *color, b8 show_alpha, b8 editable, b8 show_sliders)
{
  UI_Signal color_widget_signal = (UI_Signal){0};

  UI_Text_Metrics text_metrics = ui_measure_text(text);

  // @TODO(fz): Can be done offline, but has to be regenerated if font is regenerated
  f32 color_block_size = ui_measure_text(S("9.99")).width + 10;

  u32 layout_node_height = (u32)(UIContext.font.height*1.3);
  u32 spacing = 5;
  u32 checkbox_and_label_width = (u32)text_metrics.width + layout_node_height + spacing;

  // Layout node
  UI_Signal layout_signal = {0};
  if (!show_sliders)
  {
    ui_color_theme_style(UI_Color_Theme_Light)
    ui_size_kind_x(UI_Size_Kind_Fixed) ui_size_fixed_x(20)
    ui_size_kind_y(UI_Size_Kind_Fixed) ui_size_fixed_y(layout_node_height)
    ui_alignment_kind(UI_Alignment_Kind_X)
    ui_node_spacing(spacing)
    {
      UI_Node_Flags layout_flags = 0;
      layout_signal.node = _ui_node_from_string(text, text, layout_flags);
      layout_signal.node->should_render = false;
      ui_stack_parent_push(layout_signal.node);
    }
  }
  else
  {
    ui_color_theme_style(UI_Color_Theme_Light)
    ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(1.0f)
    ui_size_kind_y(UI_Size_Kind_Fixed) ui_size_fixed_y(layout_node_height)
    ui_alignment_kind(UI_Alignment_Kind_X)
    ui_node_spacing(spacing)
    {
      UI_Node_Flags layout_flags = 0;
      layout_signal.node = _ui_node_from_string(text, text, layout_flags);
      layout_signal.node->should_render = false;
      ui_stack_parent_push(layout_signal.node);
    }
  }

  // Checkbox
  UI_Signal color_box = {0};
  {
    if (layout_signal.node->parent->bounds.size.x >= layout_signal.node->bounds.size.y + (s32)spacing)
    {
      ui_color_theme_style(UI_Color_Theme_Light)
      ui_background_color(*color)
      ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
      ui_size_kind_x(UI_Size_Kind_Copy_Y)
      ui_alignment_kind(UI_Alignment_Kind_X)
      ui_corner_roundness_factor(3.0f)
      {
        UI_Node_Flags color_value_flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable;
        color_box.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_color_box", text.cstring), S(" "), color_value_flags);
        _ui_fill_signals_from_node(&color_box);
      }
    }
    else
    {
      ui_color_theme_style(UI_Color_Theme_Light)
      ui_background_color(*color)
      ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
      ui_size_kind_x(UI_Size_Kind_Fixed)    ui_size_fixed_x(layout_signal.node->parent->bounds.size.x)
      ui_alignment_kind(UI_Alignment_Kind_X)
      ui_corner_roundness_factor(3.0f)
      {
        UI_Node_Flags color_value_flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable;
        color_box.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_color_box", text.cstring), S(" "), color_value_flags);
        _ui_fill_signals_from_node(&color_box);
      }
    }
  }

  if (show_sliders)
  {
    if (editable)
    {
      ui_slider_f32(S("r_color_slider"), S(""), &color->x, 0.0f, 1.0f, 35.0f);
      ui_slider_f32(S("g_color_slider"), S(""), &color->y, 0.0f, 1.0f, 35.0f);
      ui_slider_f32(S("b_color_slider"), S(""), &color->z, 0.0f, 1.0f, 35.0f);
      if (show_alpha) 
      {
        ui_slider_f32(S("w_color_slider"), S(""), &color->w, 0.0f, 1.0f, 35.0f);
      }
    }
    else
    {
      UI_Signal x_box_signal = {0};
      {
        ui_color_theme_style(UI_Color_Theme_Dark)
        ui_size_kind_x(UI_Size_Kind_Fixed)    ui_size_fixed_x(color_block_size)
        ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
        ui_alignment_kind(UI_Alignment_Kind_X)
        {
          UI_Node_Flags color_value_flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable|UI_Node_Flags_Text_Display|UI_Node_Flags_Text_Center_X|UI_Node_Flags_Text_Center_Y;
          x_box_signal.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_color_x", text.cstring), Sf(UIContext.frame_arena, "%.2f", color->x), color_value_flags);
          _ui_fill_signals_from_node(&x_box_signal);
        }
      }
      UI_Signal y_box_signal = {0};
      {
        ui_color_theme_style(UI_Color_Theme_Dark)
        ui_size_kind_x(UI_Size_Kind_Fixed)    ui_size_fixed_x(color_block_size)
        ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
        ui_alignment_kind(UI_Alignment_Kind_X)
        {
          UI_Node_Flags color_value_flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable|UI_Node_Flags_Text_Display|UI_Node_Flags_Text_Center_X|UI_Node_Flags_Text_Center_Y;
          y_box_signal.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_empty_y", text.cstring), Sf(UIContext.frame_arena, "%.2f", color->y), color_value_flags);
          _ui_fill_signals_from_node(&y_box_signal);
        }
      }
      UI_Signal z_box_signal = {0};
      {
        ui_color_theme_style(UI_Color_Theme_Dark)
        ui_size_kind_x(UI_Size_Kind_Fixed)    ui_size_fixed_x(color_block_size)
        ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
        ui_alignment_kind(UI_Alignment_Kind_X)
        {
          UI_Node_Flags color_value_flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable|UI_Node_Flags_Text_Display|UI_Node_Flags_Text_Center_X|UI_Node_Flags_Text_Center_Y;
          z_box_signal.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_empty_z", text.cstring), Sf(UIContext.frame_arena, "%.2f", color->z), color_value_flags);
          _ui_fill_signals_from_node(&z_box_signal);
        }
      }
      if (show_alpha) 
      {
        UI_Signal w_box_signal = {0};
        ui_color_theme_style(UI_Color_Theme_Dark)
        ui_size_kind_x(UI_Size_Kind_Fixed)    ui_size_fixed_x(color_block_size)
        ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
        ui_alignment_kind(UI_Alignment_Kind_X)
        {
          UI_Node_Flags color_value_flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable|UI_Node_Flags_Text_Display|UI_Node_Flags_Text_Center_X|UI_Node_Flags_Text_Center_Y;
          w_box_signal.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_empty_w", text.cstring), Sf(UIContext.frame_arena, "%.2f", color->w), color_value_flags);
          _ui_fill_signals_from_node(&w_box_signal);
        }
      }
    }
  }

  // Text
  UI_Signal color_label = {0};
  {
    ui_color_theme_style(UI_Color_Theme_Light)
    ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(1.0f)
    ui_size_kind_y(UI_Size_Kind_Fixed)    ui_size_fixed_y(layout_node_height)
    ui_border_thickness(0)
    {
      UI_Node_Flags flags = UI_Node_Flags_Text_Display|UI_Node_Flags_Text_Center_Y;
      color_label.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_color_label", text.cstring), text, flags);
      _ui_fill_signals_from_node(&color_label);
    }
  }

  ui_stack_parent_pop(); // layout_signal.node

  return color_widget_signal;
}

function UI_Signal
ui_image(String unique, s32 texture_index, V2f32 uv_min, V2f32 uv_max)
{
  UI_Signal image_signal = {0};
  {
    ui_color_theme_style(UI_Color_Theme_White)
    ui_size_kind_x(UI_Size_Kind_Copy_Y)
    ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
    ui_display_image_index(texture_index)
    ui_corner_roundness_factor(0.2f)
    ui_padding_fixed(0)
    {
      UI_Node_Flags flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable;
      image_signal.node = _ui_node_from_string(
        unique,
        Sf(UIContext.frame_arena, "##_image_" S_FMT, S_ARG(unique)),
        flags);

      image_signal.node->texture_index = texture_index;
      image_signal.node->uv_min        = uv_min;
      image_signal.node->uv_max        = uv_max;

      _ui_fill_signals_from_node(&image_signal);
    }
  }

  return image_signal;
}

function UI_Signal
ui_slider_f32(String unique, String text, f32 *val, f32 min, f32 max, f32 width)
{
  *val = clamp(*val, min, max);

  String complete_text = Sf(UIContext.frame_arena, "%s%.2f", text.cstring, *val);
  UI_Text_Metrics text_metrics = ui_measure_text(complete_text);

  UI_Signal slider_signal = {0};
  {
    ui_color_theme_style(UI_Color_Theme_Interactable)
    ui_size_kind_x(UI_Size_Kind_Fixed) ui_size_fixed_x((u32)width)
    ui_size_kind_y(UI_Size_Kind_Fixed) ui_size_fixed_y((u32)text_metrics.height + (UIContext.font.height/2))
    {
      UI_Node_Flags flags = UI_Node_Flags_Text_Display|UI_Node_Flags_Text_Center_X|UI_Node_Flags_Text_Center_Y|UI_Node_Flags_Text_Ignore_Clip|UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable;
      slider_signal.node = _ui_node_from_string(unique, complete_text, flags);
      _ui_fill_signals_from_node(&slider_signal);
      ui_stack_parent_push(slider_signal.node);
    }
  }

  UI_Node_Cache *slider_node_cached = _ui_get_cached_node(slider_signal.node->hash);

  if (is_button_clicked(Mouse_Button_Left) && ui_hovered(slider_signal))
  {
    slider_node_cached->slider.is_slider_dragging = true;
  }
  if (is_button_up(Mouse_Button_Left))
  {
    slider_node_cached->slider.is_slider_dragging = false;
  }

  V2s32 absolute_top_left = _ui_get_node_absolute_top_left(slider_signal.node);

  // Add drag offsets from parent nodes, same as _ui_is_mouse_in_node does
  for (UI_Node *n = slider_signal.node; n != NULL; n = n->parent)
  {
    if (has_flags(n->flags, UI_Node_Flags_Draggable_Panel))
    {
      UI_Node_Cache *cache = _ui_get_cached_node(n->hash);
      absolute_top_left = v2s32_add(absolute_top_left, cache->accumulated_drag_offset);
    }
  }

  u32 start_x    = absolute_top_left.x;
  u32 full_width = slider_signal.node->bounds.size.x;
  f32 percentage = (*val - min) / (max - min);
  f32 new_size   = percentage * full_width;

  if (slider_node_cached->slider.is_slider_dragging && is_button_down(Mouse_Button_Left))
  {
    f32 relative_x = (f32)((s32)UIContext.mouse_x - (s32)start_x);
    f32 new_val    = min + (relative_x / (f32)full_width) * (max - min);
    *val     = clamp(new_val, min, max);
    new_size = clamp(relative_x, 0.0f, (f32)full_width);
  }

  UI_Signal filler_signal = {0};
  {
    ui_color_theme_style(UI_Color_Theme_Interactable_Solid)
    ui_size_kind_x(UI_Size_Kind_Fixed) ui_size_fixed_x(new_size)
    ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
    ui_background_transparency(0.5f)
    {
      UI_Node_Flags flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable;
      filler_signal.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_filler", unique.cstring), S(" "), flags);
      _ui_fill_signals_from_node(&filler_signal);
    }
  }

  ui_stack_parent_pop(); // slider_signal.node

  return slider_signal;
}

function void
ui_arena_window(String text, Arena *arena, s32 x, s32 y)
{
  ui_window(text, x, y, 400, 200)
  {
    ui_textf(S("Reserved: %llu\nCommitted: %llu (Commit Size: %llu, Align: %llu)\nPosition: %llu"), 
                arena->reserved, arena->committed, arena->commit_size, arena->align, arena->position);

    f32 filled = (arena->position * 100.0f) / arena->committed;
    String position_text = Sf(UIContext.frame_arena, "Pos: %llu (%.2f%%%%)", arena->position, filled);
    UI_Text_Metrics text_metrics = ui_measure_text(position_text);

    UI_Signal arena_box_signal = {0};
    {
      ui_color_theme_style(UI_Color_Theme_Interactable)
      ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(1.0f)
      ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
      ui_alignment_kind(UI_Alignment_Kind_X)
      ui_corner_roundness_factor(0.2f)
      ui_padding_fixed(1)
      ui_node_spacing(3)
      {
        UI_Node_Flags flags = UI_Node_Flags_Hoverable;
        arena_box_signal.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_arena_window", text.cstring), S(""), flags);
        _ui_fill_signals_from_node(&arena_box_signal);
      }
      ui_parent(arena_box_signal.node)
      {
        ui_color_theme_style(UI_Color_Theme_Interactable_Solid)
        ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(filled*0.01f)
        ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
        ui_background_transparency(0.5f)
        ui_corner_roundness_factor(0.2f)
        ui_padding_fixed(5.0f)
        {
          UI_Node_Flags flags = UI_Node_Flags_Hoverable;
          arena_box_signal.node = _ui_node_from_string(Sf(UIContext.frame_arena, "%s_arena_filled", text.cstring), S(""), flags);
          _ui_fill_signals_from_node(&arena_box_signal);
        }
        if (arena_box_signal.node->clip.size.x < text_metrics.width)
        {
          ui_text(position_text);
        }
        else
        {
          ui_parent(arena_box_signal.node)
          {
            ui_text(position_text);          
          }
        }
      }
    }
  }
}

function void
ui_toggle_color_theme_window()
{
  UIContext.debug.draw_color_theme_window = !UIContext.debug.draw_color_theme_window;
}

function void
ui_draw_color_theme_window()
{
  ui_window(S("Color Theme"), UIContext.window_width - 800, 5, 350, 500)
  {
    ui_text(S("Border:"));
    ui_color(S("Light##Border"),        &UIContext.color_theme.light_border_color, false, true, true);
    ui_color(S("Light Hover##Border"),  &UIContext.color_theme.light_border_hover_color, false, true, true);
    ui_color(S("Light Active##Border"), &UIContext.color_theme.light_border_active_color, false, true, true);
    ui_color(S("Dark##Border"),         &UIContext.color_theme.dark_border_color, false, true, true);
    ui_color(S("Dark Hover##Border"),   &UIContext.color_theme.dark_border_hover_color, false, true, true);
    ui_color(S("Dark Active##Border"),  &UIContext.color_theme.dark_border_active_color, false, true, true);

    ui_text(S("Background:"));
    ui_color(S("Light##Background"),        &UIContext.color_theme.light_background_color, false, true, true);
    ui_color(S("Light Hover##Background"),  &UIContext.color_theme.light_background_hover_color, false, true, true);
    ui_color(S("Light Active##Background"), &UIContext.color_theme.light_background_active_color, false, true, true);
    ui_color(S("Dark##Background"),         &UIContext.color_theme.dark_background_color, false, true, true);
    ui_color(S("Dark Hover##Background"),   &UIContext.color_theme.dark_background_hover_color, false, true, true);
    ui_color(S("Dark Active##Background"),  &UIContext.color_theme.dark_background_active_color, false, true, true);

    ui_text(S("Text:"));
    ui_color(S("Color##Text"),       &UIContext.color_theme.text_color, false, true, true);
    ui_color(S("Hover Color##Text"), &UIContext.color_theme.text_hover_color, false, true, true);
    ui_color(S("Active##Text"),      &UIContext.color_theme.text_active_color, false, true, true);

    ui_text(S("Style:"));
    ui_slider_f32(S("Roundness"), S("Roundness: "), &UIContext.style.corner_roundness, 0.0f, 0.4f, 150.0f);
  }
}

function void
ui_toggle_debug_window()
{
  UIContext.debug.draw_debug_window = !UIContext.debug.draw_debug_window;
}

function void
ui_draw_debug_window()
{
  UI_Node *active = NULL;
  UI_Node *hot = NULL;
  UI_Node *resizing = NULL;

  for(UI_Node *it = UIContext.root; it != NULL; it = (it->first ? it->first : (it->next ? it->next : _ui_tree_next(it->parent))))
  {
    if (it->hash == UIContext.hash_active) 
    {
      active = it;
      continue;
    }
    if (it->hash == UIContext.hash_hot)
    {
      hot = it;
      continue;
    }
    if (it->hash == UIContext.hash_resizing)
    {
      resizing = it;
      continue;
    }
  }

  ui_window(S("UI Debug"), UIContext.window_width-255, 5, 250, 400)
  {
    if (ui_window_wants_to_close())
    {
      UIContext.debug.draw_debug_window = false;
    }

    ui_text(S("Debug --- "));
    ui_checkbox(S("Draw clip"), &UIContext.debug.draw_clip);
    ui_checkbox(S("Draw cursor"), &UIContext.debug.draw_cursor);
    ui_checkbox(S("Resize picking area"), &UIContext.debug.draw_resize_picking_area);
    ui_checkbox(S("Draw undrawable"), &UIContext.debug.draw_stuff_that_shouldnt_render);
    ui_checkbox(S("Disable window snap"), &UIContext.debug.disable_window_snap);
    ui_checkbox(S("Disable cache clear"), &UIContext.debug.disable_node_cache_clear);

    ui_text(S("UI Context:"));
    ui_textf(S(" Cached Nodes: %u/%u"), UIContext.cached_nodes_capacity - UIContext.cached_nodes_available, UIContext.cached_nodes_capacity);
    if (active)
    {
      ui_textf(S(" Active: \"%s\", %.8f"), active->string_clean.cstring, UIContext.hash_active_depth);
    }
    else
    {
      ui_text(S(" Active: None"));
    }
    if (hot)
    {
      ui_textf(S(" Hot: \"%s\", %.8f"), hot->string_clean.cstring, UIContext.hash_hot_depth);
    }
    else
    {
      ui_text(S(" Hot: None"));
    }

    if (resizing)
    {
      UI_Node_Cache *cache = _ui_get_cached_node(resizing->hash);
      ui_textf(S(" Resize: \"%s\", %.8f\n   Offset: %d,%d\n"), resizing->string_clean.cstring, UIContext.hash_resizing_depth, cache->accumulated_resize_offset.x, cache->accumulated_resize_offset.y);
    }
    else
    {
      ui_text(S(" Resize: None"));
    }
  }
}

function u32
ui_get_commands_count()
{
  return UIContext.commands_count;
}

function UI_Command*
ui_get_commands()
{
  return UIContext.commands;
}

function UI_Node*
_ui_node_from_string(String unique, String string, UI_Node_Flags flags)
{
  UI_Node* parent = ui_stack_parent_top();
  UI_Node* node = push_array(UIContext.frame_arena, UI_Node, 1);

  if (parent == &UINodeNilSentinel)
  {
    // If we're the UIContext.root (screen)
    // We just hardcode everything since we know what the screen node must look like.
    // @TODO(fz): This can be done just on window resize.
    node->string          = S("__root_window__");
    node->hash            = string_hash(node->string);
    node->bounds.top_left = v2s32(0,0);
    node->bounds.size     = v2s32(UIContext.window_width, UIContext.window_height);
    node->clip            = node->bounds;
    node->cursor          = node->bounds.top_left;
    node->depth           = 1.0f;
    node->should_render   = false;
    node->alignment       = UI_Alignment_Kind_Float;

    UIContext.root = node;
    return node;
  }

  node->hash              = parent->hash ^ string_hash(unique);
  node->flags             = flags;
  node->should_render     = true;
  node->child_spacing     = ui_stack_node_spacing_top();
  node->alignment         = ui_stack_alignment_kind_top();
  node->texture_index     = ui_stack_display_image_index_top();

  node->string            = string_copy(UIContext.frame_arena, string);
  node->string_clean      = _ui_clean_string(UIContext.frame_arena, string);
  UI_Text_Metrics metrics = ui_measure_text(node->string_clean);
  node->clean_string_dimensions = v2f32(metrics.width, metrics.height);

  UI_Color_Theme_Style color_style = ui_stack_color_theme_style_top();

  if (color_style == UI_Color_Theme_No_Background)
  {
    node->color_theme.border_color        = COLOR_CLEAR;
    node->color_theme.border_hover_color  = COLOR_CLEAR;
    node->color_theme.border_active_color = COLOR_CLEAR;

    node->color_theme.background_color        = COLOR_CLEAR;
    node->color_theme.background_hover_color  = COLOR_CLEAR;
    node->color_theme.background_active_color = COLOR_CLEAR;
  }
  else
  {
    switch (color_style)
    {
      case UI_Color_Theme_White:
      {
        node->color_theme.border_color        = UIContext.color_theme.interactable_border_color;
        node->color_theme.border_hover_color  = UIContext.color_theme.interactable_border_hover_color;
        node->color_theme.border_active_color = UIContext.color_theme.interactable_border_active_color;

        node->color_theme.background_color        = COLOR_WHITE(1);
        node->color_theme.background_hover_color  = COLOR_WHITE(1);
        node->color_theme.background_active_color = COLOR_WHITE(1);
      }
      break;
      case UI_Color_Theme_Light:
      {
        node->color_theme.border_color        = UIContext.color_theme.light_border_color;
        node->color_theme.border_hover_color  = UIContext.color_theme.light_border_hover_color;    
        node->color_theme.border_active_color = UIContext.color_theme.light_border_active_color;

        node->color_theme.background_color        = UIContext.color_theme.light_background_color;
        node->color_theme.background_hover_color  = UIContext.color_theme.light_background_hover_color;
        node->color_theme.background_active_color = UIContext.color_theme.light_background_active_color;
      }
      break;
      case UI_Color_Theme_Dark: 
      {
        node->color_theme.border_color        = UIContext.color_theme.dark_border_color;
        node->color_theme.border_hover_color  = UIContext.color_theme.dark_border_hover_color;    
        node->color_theme.border_active_color = UIContext.color_theme.dark_border_active_color;

        node->color_theme.background_color        = UIContext.color_theme.dark_background_color;
        node->color_theme.background_hover_color  = UIContext.color_theme.dark_background_hover_color;
        node->color_theme.background_active_color = UIContext.color_theme.dark_background_active_color;
      }
      break;
      case UI_Color_Theme_Interactable: 
      {
        node->color_theme.border_color        = UIContext.color_theme.interactable_border_color;
        node->color_theme.border_hover_color  = UIContext.color_theme.interactable_border_hover_color;    
        node->color_theme.border_active_color = UIContext.color_theme.interactable_border_active_color;

        node->color_theme.background_color        = UIContext.color_theme.light_background_color;
        node->color_theme.background_hover_color  = UIContext.color_theme.light_background_hover_color;
        node->color_theme.background_active_color = UIContext.color_theme.light_background_active_color;
      }
      break;
      case UI_Color_Theme_Interactable_Solid: 
      {
        node->color_theme.border_color        = UIContext.color_theme.interactable_border_color;
        node->color_theme.border_hover_color  = UIContext.color_theme.interactable_border_hover_color;
        node->color_theme.border_active_color = UIContext.color_theme.interactable_border_active_color;

        node->color_theme.background_color        = UIContext.color_theme.interactable_background_color;
        node->color_theme.background_hover_color  = UIContext.color_theme.interactable_background_hover_color;
        node->color_theme.background_active_color = UIContext.color_theme.interactable_background_active_color;
      }
      break;
    }

    f32 border_transparency     = ui_stack_border_transparency_top();
    f32 background_transparency = ui_stack_background_transparency_top();

    node->color_theme.border_color.w        = border_transparency;
    node->color_theme.border_hover_color.w  = border_transparency;
    node->color_theme.border_active_color.w = border_transparency;

    node->color_theme.background_color.w        = background_transparency;
    node->color_theme.background_hover_color.w  = background_transparency;
    node->color_theme.background_active_color.w = background_transparency;
  }

  node->color_theme.text_color = UIContext.color_theme.text_color;
  if (!ui_stack_text_color_is_at_bottom())
  {
    node->color_theme.text_color = ui_stack_text_color_top();
  }

  node->color_theme.text_hover_color  = UIContext.color_theme.text_hover_color;
  node->color_theme.text_active_color = UIContext.color_theme.text_active_color;

  node->color_theme.border_thickness  = ui_stack_border_thickness_top();
  if (!ui_stack_border_color_is_at_bottom())
  {
    node->color_theme.border_color = ui_stack_border_color_top();
  }

  node->color_theme.corner_roundness  = UIContext.style.corner_roundness * ui_stack_corner_roundness_factor_top();
  node->color_theme.corner_roundness  = clamp(node->color_theme.corner_roundness, 0.0f, 0.5f);

  if (!ui_stack_background_color_is_at_bottom())
  {
    V4f32 color = ui_stack_background_color_top();
    node->color_theme.background_color        = color;
    node->color_theme.background_hover_color  = color;
    node->color_theme.background_active_color = color;
  }

  UI_Node_Cache* parent_cached_node = _ui_get_cached_node(parent->hash);
  UI_Node_Cache* cached_node = _ui_get_cached_node(node->hash);
  _ui_add_node_child(parent, node);

  // Get total depth, by subbing from all parents
  assert(node->parent != NULL);
  node->depth = node->parent->depth - UI_EPSILON;

  // Stacks
  s32 padding = ui_stack_padding_fixed_top();

  // Bounds ---------------
  V2s32 parent_bounds_size = v2s32_add(parent->bounds.size, parent_cached_node->accumulated_resize_offset);
  V2s32 parent_clip_size   = v2s32_add(parent->clip.size, parent_cached_node->accumulated_resize_offset);;

  s32 available_width  = parent_clip_size.x - (node->parent->cursor.x - parent->clip.top_left.x);
  s32 available_height = parent_clip_size.y - (node->parent->cursor.y - parent->clip.top_left.y);
  if (has_flags(node->flags, UI_Node_Flags_Float_On_Parent))
  {
    // If node if floating on parent, we dont care about parent cursor.
    available_width  = parent->bounds.size.x;
    available_height = parent->bounds.size.y;
  }

  UI_Size_Kind size_x_kind = ui_stack_size_kind_x_top();
  switch (size_x_kind)
  {
    case UI_Size_Kind_Fixed:
    {
      s32 fixed_x = ui_stack_size_fixed_x_top();
      s32 minimum_width = ui_stack_minimum_width_top();
      node->bounds.size.x = clamp(fixed_x, minimum_width, available_width);
    }
    break;
    case UI_Size_Kind_Relative:
    {
      f32 relative_factor = clamp(ui_stack_size_relative_x_top(), 0.0f, 1.0f);
      node->bounds.size.x = (u32)(relative_factor * available_width);
      node->bounds.size.x = max(node->bounds.size.x, ui_stack_minimum_width_top());
    }
    break;
    case UI_Size_Kind_Copy_Y:
    {
      /* Handled later */
    }
    break;
    default: { ui_size_kind_not_handled(UIContext.frame_arena, size_x_kind); }
  }

  UI_Size_Kind size_y_kind = ui_stack_size_kind_y_top();
  switch (size_y_kind)
  {
    case UI_Size_Kind_Fixed:
    {
      s32 fixed_y = ui_stack_size_fixed_y_top();
      s32 minimum_height = ui_stack_minimum_height_top();
      node->bounds.size.y = clamp(fixed_y, minimum_height, available_height);
    }
    break;
    case UI_Size_Kind_Relative:
    {
      f32 relative_factor  = clamp(ui_stack_size_relative_y_top(), 0.0f, 1.0f);
      node->bounds.size.y  = (u32)(relative_factor * available_height);
      node->bounds.size.y  = max(node->bounds.size.y, ui_stack_minimum_height_top());
    }
    break;
    case UI_Size_Kind_Copy_X:
    {
      /* Handled later */
    }
    break;
    default: { ui_size_kind_not_handled(UIContext.frame_arena, size_x_kind); }
  }

  // Handle Size copy, in case it's copying from either
  {
    if (size_x_kind == UI_Size_Kind_Copy_X) { ui_error(S("size_x_kind cannot be UI_Size_Kind_Copy_X.")); }
    if (size_y_kind == UI_Size_Kind_Copy_Y) { ui_error(S("size_y_kind cannot be UI_Size_Kind_Copy_Y.")); }
    if (size_y_kind == UI_Size_Kind_Copy_X && size_x_kind == UI_Size_Kind_Copy_Y) { ui_error(S("size_y_kind and size_x_kind cannot both copy each other")); }
    if (size_y_kind == UI_Size_Kind_Copy_X)
    {
      node->bounds.size.y = node->bounds.size.x;
    }
    else if (size_x_kind == UI_Size_Kind_Copy_Y)
    {
      node->bounds.size.x = node->bounds.size.y;
    }
  }

  V2s32 pop_top_left = ui_stack_top_left_top();
  pop_top_left.x = max(pop_top_left.x, 0);
  pop_top_left.y = max(pop_top_left.y, 0);

  if (has_flags(node->flags, UI_Node_Flags_Float_On_Parent))
  {
    node->bounds.top_left = pop_top_left;
  }
  else
  {
    node->bounds.top_left = v2s32_add(parent->cursor, pop_top_left);
  }

  if (ui_stack_node_center_x_top()) { node->bounds.top_left.x = node->bounds.top_left.x + (available_width  - node->bounds.size.x) / 2; }
  if (ui_stack_node_center_y_top()) { node->bounds.top_left.y = node->bounds.top_left.y + (available_height - node->bounds.size.y) / 2; }

  if (has_flags(node->flags, UI_Node_Flags_Float_On_Parent))
  {
  }
  else switch (parent->alignment)
  {
    case UI_Alignment_Kind_Y:
    {
      parent->cursor.y += (node->bounds.size.y + parent->child_spacing);
      parent->cursor.y -= node->color_theme.border_thickness;
      s32 max_cursor_y = parent->clip.top_left.y + parent_clip_size.y;
      parent->cursor.y = clamp(parent->cursor.y, parent->clip.top_left.y, max_cursor_y);
    }
    break;
    case UI_Alignment_Kind_X:
    {
      parent->cursor.x += (node->bounds.size.x + parent->child_spacing);
      parent->cursor.x -= node->color_theme.border_thickness;
      s32 max_cursor_x = parent->clip.top_left.x + parent_clip_size.x;
      parent->cursor.x = clamp(parent->cursor.x, parent->clip.top_left.x, max_cursor_x);
    }
    break;
    case UI_Alignment_Kind_Float:
    {
      /* We don't offset parent cursor at all */
    }
    break;
    default:
    {
      ui_alignment_kind_not_handled(UIContext.frame_arena, parent->alignment);
    }
    break;
  }

  // Clip -------
  padding = clamp_top(padding, node->bounds.size.y/3);
  s32 double_padding = padding * 2;

  node->clip.top_left = v2s32(padding,padding);
  node->clip.size.x   = clamp_bot(node->bounds.size.x - double_padding, 2);
  node->clip.size.y   = clamp_bot(node->bounds.size.y - double_padding, 2);
  node->cursor        = node->clip.top_left;

  // Interaction ------------
  if (_ui_is_mouse_in_node(node))
  {
    if (node->depth < UIContext.hash_hot_depth)
    {
      // Only set hash_hot for hoverable nodes
      if (has_flags(node->flags, UI_Node_Flags_Hoverable))
      {
        UIContext.hash_hot       = node->hash;
        UIContext.hash_hot_depth = node->depth;
      }
    
      // Only set hash_active for clickable nodes
      if (has_flags(node->flags, UI_Node_Flags_Mouse_Clickable))
      {
        if (is_button_clicked(Mouse_Button_Left))
        {
          UIContext.hash_active       = node->hash;
          UIContext.hash_active_depth = node->depth;
        }
      }
    }
  }

  // Resize
  if (has_flags(node->flags, UI_Node_Flags_Resizable))
  {
    V2s32 absolute_top_left  = _ui_get_node_absolute_top_left(node);
    V2s32 total_drag_offset = v2s32(0, 0);
    for (UI_Node *n = node; n != NULL; n = n->parent)
    {
      if (has_flags(n->flags, UI_Node_Flags_Draggable_Panel))
      {
        UI_Node_Cache *c = _ui_get_cached_node(n->hash);
        total_drag_offset = v2s32_add(total_drag_offset, c->accumulated_drag_offset);
      }
    }
    absolute_top_left = v2s32_add(absolute_top_left, total_drag_offset);

    V2s32 size = v2s32_add(node->bounds.size, cached_node->accumulated_resize_offset);
    V2s32 absolute_bot_right = v2s32_add(absolute_top_left, size);
    UI_Rect resizing_area_rect = {0};
    resizing_area_rect.top_left.x = absolute_bot_right.x - (UIContext.resize_area_side);
    resizing_area_rect.top_left.y = absolute_bot_right.y - (UIContext.resize_area_side);
    resizing_area_rect.size = v2s32(UIContext.resize_area_side, UIContext.resize_area_side);
    if (_ui_is_mouse_in_rect(resizing_area_rect))
    {
      if (is_button_clicked(Mouse_Button_Left))
      {
        UIContext.hash_resizing = node->hash;
        UIContext.hash_resizing_depth = node->depth;
      }
    }
  }

  if (UIContext.hash_resizing == node->hash)
  {
    UI_Node_Cache *cache = _ui_get_cached_node(node->hash);
    if (cache->accumulated_resize_offset.x + node->bounds.size.x + UIContext.mouse_delta_x > UIContext.window_min_width)
    {
      cache->accumulated_resize_offset.x += UIContext.mouse_delta_x;
    }
    if (cache->accumulated_resize_offset.y + node->bounds.size.y + UIContext.mouse_delta_y > UIContext.window_min_height)
    {
      cache->accumulated_resize_offset.y += UIContext.mouse_delta_y;
    }
  }

  // Dragging -----
  if (UIContext.hash_active == node->hash)
  {
    if (has_flags(node->flags, UI_Node_Flags_Draggable))
    {
      // Find parent
      UI_Node* window_node = node;
      while (window_node && !has_flags(window_node->flags, UI_Node_Flags_Draggable_Panel))
      {
        window_node = window_node->parent;
      }
    
      if (window_node)
      {
        UI_Node_Cache* window_cache = _ui_get_cached_node(window_node->hash);
        window_cache->accumulated_drag_offset.x += UIContext.mouse_delta_x;
        window_cache->accumulated_drag_offset.y += UIContext.mouse_delta_y;
      }
    }
  }

  // Reset intractables
  if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
  {
    UIContext.hash_active = 0;
    UIContext.hash_active_depth = 1.0f;

    UIContext.hash_resizing = 0;
    UIContext.hash_resizing_depth = 1.0f;
  }

  return node;
}

function void
_ui_update_tree_nodes(UI_Node* node)
{
  if (!node)
  {
    return;
  }

  if (node != UIContext.root)
  {
    UI_Node_Cache* cached_node = _ui_get_cached_node(node->hash);

    // Bounds
    node->bounds.top_left = v2s32_add(node->bounds.top_left, cached_node->accumulated_drag_offset);

    if (has_flags(node->flags, UI_Node_Flags_Resizable))
    {
      node->bounds.size = v2s32_add(node->bounds.size, cached_node->accumulated_resize_offset);
      node->clip.size   = v2s32_add(node->clip.size, cached_node->accumulated_resize_offset);
    }

    // Interaction
    // ----------- 
    {
      V4f32 border_color            = node->color_theme.border_color;
      V4f32 border_hover_color      = node->color_theme.border_hover_color;
      V4f32 border_active_color     = node->color_theme.border_active_color;
      V4f32 background_color        = node->color_theme.background_color;
      V4f32 background_hover_color  = node->color_theme.background_hover_color;
      V4f32 background_active_color = node->color_theme.background_active_color;
      V4f32 text_color              = node->color_theme.text_color;
      V4f32 text_hover_color        = node->color_theme.text_hover_color;
      V4f32 text_active_color       = node->color_theme.text_active_color;

      // Start with base colors
      node->target_border_color     = border_color;
      node->target_background_color = background_color;
      node->target_text_color       = text_color;

      if (has_flags(node->flags, UI_Node_Flags_Hoverable))
      {
        if (UIContext.hash_hot == node->hash)
        {
          cached_node->hover_t = clamp(cached_node->hover_t + UIContext.delta_time * UIContext.animation_speed, 0, 1);
        }
        else
        {
          cached_node->hover_t = clamp(cached_node->hover_t - UIContext.delta_time * UIContext.animation_speed, 0, 1);
        }

        node->target_border_color     = v4f32_lerp(node->target_border_color,     border_hover_color,     cached_node->hover_t);
        node->target_background_color = v4f32_lerp(node->target_background_color, background_hover_color, cached_node->hover_t);
        node->target_text_color       = v4f32_lerp(node->target_text_color,       text_hover_color,       cached_node->hover_t);
      }
  
      if (has_flags(node->flags, UI_Node_Flags_Mouse_Clickable))
      {
        if (UIContext.hash_active == node->hash)
        {
          cached_node->active_t = clamp(cached_node->active_t + UIContext.delta_time * UIContext.animation_speed, 0, 1);
        }
        else
        {
          cached_node->active_t = clamp(cached_node->active_t - UIContext.delta_time * UIContext.animation_speed, 0, 1);
        }

        node->target_border_color     = v4f32_lerp(node->target_border_color,     border_active_color,     cached_node->active_t);
        node->target_background_color = v4f32_lerp(node->target_background_color, background_active_color, cached_node->active_t);
        node->target_text_color       = v4f32_lerp(node->target_text_color,       text_active_color,       cached_node->active_t);
      }
    }
  }

  for (UI_Node* child = node->first; child; child = child->next)
  {
    _ui_update_tree_nodes(child);
  }
}

function V2s32
_ui_get_node_absolute_top_left(UI_Node *node)
{
  V2s32 top_left = v2s32(0, 0);
  
  for (UI_Node *n = node; n != NULL; n = n->parent)
  {
    // Just add bounds.top_left - it's already relative to parent's cursor position
    // The cursor position already accounts for the parent's clip.top_left
    top_left = v2s32_add(top_left, n->bounds.top_left);
  }
  
  return top_left;
}

function u32
_ui_get_node_depth(UI_Node *node)
{
  u32 depth = 0;

  UI_Node *cursor = node;
  while (cursor)
  {
    depth += 1;
    cursor = cursor->parent;
  }

  return depth;
}

function void
_ui_render_ui_tree(UI_Node *node)
{
  V2s32 absolute_top_left = _ui_get_node_absolute_top_left(node);

  if (UIContext.debug.draw_resize_picking_area)
  {
    if (has_flags(node->flags, UI_Node_Flags_Resizable))
    {
      V2s32 absolute_bot_right = v2s32_add(absolute_top_left, node->bounds.size);
      _ui_push_draw_rect_command(v2f32(absolute_bot_right.x - (UIContext.resize_area_side), absolute_bot_right.y - (UIContext.resize_area_side)), v2f32(UIContext.resize_area_side, UIContext.resize_area_side), COLOR_CLEAR, 0.0f, 0.0f, 0.0f, 1.0f, COLOR_YELLOW(1));
    }
  }

  if (node->should_render || UIContext.debug.draw_stuff_that_shouldnt_render)
  {
    if (node->texture_index >= 0)
    {
      _ui_push_draw_rect_with_texture_command(node->texture_index, node->uv_min, node->uv_max, v2f32((f32)absolute_top_left.x, (f32)absolute_top_left.y), v2f32((f32)node->bounds.size.x, (f32)node->bounds.size.y), node->target_background_color, 0, node->color_theme.corner_roundness, 0.0f, (f32)node->color_theme.border_thickness, node->target_border_color);
    }
    else
    {
      _ui_push_draw_rect_command(v2f32((f32)absolute_top_left.x, (f32)absolute_top_left.y), v2f32((f32)node->bounds.size.x, (f32)node->bounds.size.y), node->target_background_color, 0, node->color_theme.corner_roundness, 0.0f, (f32)node->color_theme.border_thickness, node->target_border_color);
    }

    if (UIContext.debug.draw_cursor)
    {
      f32 size = 3;
      V2s32 cursor_top_left = v2s32_add(absolute_top_left, node->cursor);
      V2s32 center = v2s32((u32)(cursor_top_left.x-(size/2)), (u32)(cursor_top_left.y-(size/2)));
      _ui_push_draw_rect_command(v2f32((f32)center.x,(f32)center.y), v2f32(size,size), COLOR_GREEN(0.8f), 0, 0, 0, 0, COLOR_WHITE(1));
    }
    if (UIContext.debug.draw_clip)
    {
      V4f32 background_color = COLOR_CLEAR;
#if DEBUG
      if (!node->should_render && UIContext.debug.draw_stuff_that_shouldnt_render && node->hash != UIContext.root->hash)
      {
        background_color = COLOR_YELLOW(0.2f);
      }
#endif
      _ui_push_draw_rect_command(v2f32_add(v2f32((f32)absolute_top_left.x, (f32)absolute_top_left.y), v2f32((f32)node->clip.top_left.x, (f32)node->clip.top_left.y)), v2f32((f32)node->clip.size.x,(f32)node->clip.size.y), background_color, 0, node->color_theme.corner_roundness, 0.0f, (f32)node->color_theme.border_thickness, COLOR_YELLOW(0.6f));
    }

    // Draw text
    if (has_flags(node->flags, UI_Node_Flags_Text_Display))
    {
      V2s32 text_position = v2s32_add(absolute_top_left, node->cursor);

      if (has_flags(node->flags, UI_Node_Flags_Text_Ignore_Clip))
      {
        text_position = absolute_top_left;
      }

      if (has_flags(node->flags, UI_Node_Flags_Text_Center_Y))
      {
        if (node->clean_string_dimensions.y < node->clip.size.y)
        {
          s32 text_height_half  = (s32)node->clean_string_dimensions.y/2;
          s32 clip_height_half  = (s32)node->clip.size.y/2;
          text_position.y += abs(text_height_half - clip_height_half);
        }
      }
      if (has_flags(node->flags, UI_Node_Flags_Text_Center_X))
      {
        if (node->clean_string_dimensions.x < node->clip.size.x)
        {
          s32 text_width_half  = (s32)node->clean_string_dimensions.x/2;
          s32 clip_width_half  = (s32)node->clip.size.x/2;
          text_position.x += abs(text_width_half - clip_width_half);
        }
      }
      _ui_push_draw_text_command(v2f32((f32)text_position.x, (f32)text_position.y), ((f32)node->clip.size.x - node->cursor.x), (f32)node->clip.size.y, node->target_text_color, node->string_clean);
    }
  }

  for (UI_Node *child = node->first; child; child = child->next)
  {
    _ui_render_ui_tree(child);
  }
}

function void
_ui_add_node_child(UI_Node *parent, UI_Node *child)
{
  if (parent == &UINodeNilSentinel)
  {
    // If we're the UIContext.root (screen)
    child->first    = NULL;
    child->last     = NULL;
    child->next     = NULL;
    child->previous = NULL;
    child->parent   = NULL;
  }
  else
  {
    child->parent   = parent;
    child->next     = NULL;
    child->previous = parent->last;

    if (parent->last)
    {
      parent->last->next = child;
    }
    else
    {
      parent->first = child;
    }

    parent->last = child;
  }
}

function UI_Node_Cache*
_ui_get_cached_node(u64 hash)
{
  // Is this node cached?
  for (u32 i = 0; i < UIContext.cached_nodes_count; i += 1)
  {
    if (UIContext.cached_nodes[i].hash == hash)
    {
      UIContext.cached_nodes[i].last_time_touched = time_milliseconds();
      return &UIContext.cached_nodes[i];
    }
  }

  if (UIContext.cached_nodes_available <= 0)
  {
    ui_error(S("UI: Too many UI_Nodes. Increase cache capacity."));
  }

  // Are tehre empty nodes we can add it to?
  for (u32 i = 0; i < UIContext.cached_nodes_count; i += 1)
  {
    UI_Node_Cache *cache = &UIContext.cached_nodes[i];
    if (UIContext.cached_nodes[i].hash == 0) // If this slot is clear, we put it here.
    {
      cache->hash = hash;
      cache->last_time_touched = time_milliseconds();
      UIContext.cached_nodes_available -= 1;
      return &UIContext.cached_nodes[i];
    }
  }

  // Make a new one
  UI_Node_Cache* cached_node = &UIContext.cached_nodes[UIContext.cached_nodes_count];
  memory_zero_struct(cached_node);
  cached_node->hash = hash;
  cached_node->last_time_touched = time_milliseconds();
  UIContext.cached_nodes_count     += 1;
  UIContext.cached_nodes_available -= 1;

  return cached_node;
}

function b32
_ui_is_mouse_in_node(UI_Node* node)
{
  UI_Rect bounds;
  memory_zero_struct(&bounds);

  bounds.size = node->bounds.size;
  V2s32 absolute_top_left = _ui_get_node_absolute_top_left(node);
  
  // Accumulate drag offsets from all ancestor windows
  V2s32 total_drag_offset = v2s32(0, 0);
  for (UI_Node* n = node; n != NULL; n = n->parent)
  {
    UI_Node_Cache* cache = NULL;
    if (has_flags(n->flags, UI_Node_Flags_Draggable_Panel))
    {
      if (!cache)
      {
        cache = _ui_get_cached_node(n->hash);
      }
      total_drag_offset = v2s32_add(total_drag_offset, cache->accumulated_drag_offset);
    }
  }
  
  bounds.top_left = v2s32_add(absolute_top_left, total_drag_offset);

  b32 result = _ui_is_mouse_in_rect(bounds);
  return result;
}

function b32
_ui_is_mouse_in_rect(UI_Rect rect)
{
  b32 result = (UIContext.mouse_x >= rect.top_left.x &&
                UIContext.mouse_x <= rect.top_left.x + rect.size.x &&
                UIContext.mouse_y >= rect.top_left.y &&
                UIContext.mouse_y <= rect.top_left.y + rect.size.y);
  return result;
}

function void
_ui_fill_signals_from_node(UI_Signal* signal)
{
  if (signal->node == NULL)
  {
    ui_error(S("Called _ui_fill_signals_from_node with a signal that doesn't contain a node."));
  }
  if (_ui_is_mouse_in_node(signal->node))
  {
    if (is_button_down(Mouse_Button_Left))      set_flags(signal->flags, UI_Signal_Flags_Left_Pressed);
    if (is_button_down(Mouse_Button_Middle))    set_flags(signal->flags, UI_Signal_Flags_Middle_Pressed);
    if (is_button_down(Mouse_Button_Right))     set_flags(signal->flags, UI_Signal_Flags_Right_Pressed);
    if (is_button_clicked(Mouse_Button_Left))   set_flags(signal->flags, UI_Signal_Flags_Left_Clicked);
    if (is_button_clicked(Mouse_Button_Middle)) set_flags(signal->flags, UI_Signal_Flags_Middle_Clicked);
    if (is_button_clicked(Mouse_Button_Right))  set_flags(signal->flags, UI_Signal_Flags_Right_Clicked);
    set_flags(signal->flags, UI_Signal_Flags_Mouse_Hovered);
  }
}

function String
_ui_clean_string(Arena* arena, String string)
{
  String result;
  if (string.count > 0)
  {
    String_List strings = string_split(arena, string, S("##"));
    result = strings.first->value;
  }
  else
  {
    result = string_copy(arena, string);;
  }
  return result;
}

function void
_ui_purge_old_cached_nodes()
{
  u64 time_now = time_milliseconds();
  for (u32 i = 0; i < UIContext.cached_nodes_count; i += 1)
  {
    UI_Node_Cache *cache = &UIContext.cached_nodes[i];
    if (cache->hash != 0)
    {
      if (time_now - cache->last_time_touched > UIContext.cached_nodes_max_idle_lifetime_ms)
      {
        cache->hash = 0;
        UIContext.cached_nodes_available += 1;
      }
    }
  }
}

function UI_Node *
_ui_tree_next(UI_Node *node)
{
  while(node && !node->next) node = node->parent;
  return node ? node->next : NULL;
}

function void
_ui_push_draw_rect_command(V2f32 top_left, V2f32 size, V4f32 color, f32 rotation, f32 roundness, f32 smoothness, f32 border_thickness, V4f32 border_color)
{
  UI_Command *command;
  array_add(command, UIContext.commands);

  command->kind     = UI_Command_Kind_Draw_Rect;
  command->top_left = top_left;
  command->size     = size;
  command->color    = color;

  command->rect.rotation         = rotation;
  command->rect.roundness        = roundness;
  command->rect.smoothness       = smoothness;
  command->rect.border_thickness = border_thickness;
  command->rect.border_color     = border_color;
}

function void
_ui_push_draw_rect_with_texture_command(s32 texture_index, V2f32 uv_min, V2f32 uv_max, V2f32 top_left, V2f32 size, V4f32 color, f32 rotation, f32 roundness, f32 smoothness, f32 border_thickness, V4f32 border_color)
{
  UI_Command *command;
  array_add(command, UIContext.commands);

  command->kind     = UI_Command_Kind_Draw_Rect_With_Texture;
  command->top_left = top_left;
  command->size     = size;
  command->color    = color;

  command->rect.rotation         = rotation;
  command->rect.roundness        = roundness;
  command->rect.smoothness       = smoothness;
  command->rect.border_thickness = border_thickness;
  command->rect.border_color     = border_color;

  command->rect.texture_index    = texture_index;
  command->rect.uv_min           = uv_min;
  command->rect.uv_max           = uv_max;
}

function void
_ui_push_draw_text_command(V2f32 top_left, f32 max_width, f32 max_height, V4f32 color, String text)
{
  // @TODO(fz): This max_width/max_height are probably just a V2f32 size?

  UI_Command *command;
  array_add(command, UIContext.commands);

  command->kind      = UI_Command_Kind_Draw_Text;
  command->top_left  = top_left;
  command->color     = color;

  command->text.max_width  = max_width;
  command->text.max_height = max_height;
  command->text.text = string_copy(UIContext.arena, text);
}

#if DEBUG
function void inline
_ui_error(String message, String file, u32 line)
{
  Scratch scratch = scratch_begin(0,0);
  String body = Sf(scratch.arena, "UI Error\n"S_FMT"\n\nat "S_FMT":%u", S_ARG(message), S_ARG(file), line);
  message_box(S("UI Error"), body, file, line);
  raddbg_break();
  scratch_end(&scratch);
  assert(false);
}
#endif
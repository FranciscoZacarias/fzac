#define ui_window(text, x, y, width, height) defer_loop(ui_window_begin((text), (x), (y), (width), (height)), ui_window_end())

fz_internal UI_Signal ui_window_begin(String text, s32 x, s32 y, s32 width, s32 height);
fz_internal void      ui_window_end();
fz_internal UI_Signal ui_button(String text);
fz_internal UI_Signal ui_text(String text);
fz_internal UI_Signal ui_textf(String fmt, ...);
fz_internal UI_Signal ui_text_colored(String text, V4f32 color);
fz_internal UI_Signal ui_textf_colored(V4f32 color, String fmt, ...);
fz_internal UI_Signal ui_checkbox(String text, b8 *checked);
fz_internal UI_Signal ui_color(String text, V4f32 *color, b8 show_alpha, b8 editable, b8 show_sliders);
fz_internal UI_Signal ui_image(String unique, s32 texture_index, V2f32 uv_min, V2f32 uv_max);
fz_internal UI_Signal ui_slider_f32(String unique, String text, f32 *val, f32 min, f32 max, f32 width);
fz_internal UI_Signal ui_slider_s32(String unique, String text, s32 *val, s32 min, s32 max, f32 width);
fz_internal UI_Signal ui_slider_u32(String unique, String text, u32 *val, u32 min, u32 max, f32 width);

// Debug stuff
fz_internal void ui_toggle_color_theme_window();
fz_internal void ui_draw_color_theme_window();
fz_internal void ui_toggle_debug_window();
fz_internal void ui_draw_debug_window();

fz_internal UI_Signal
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

fz_internal void
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

fz_internal UI_Signal
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

fz_internal UI_Signal
ui_text(String text)
{
  return ui_textf(text);
}

fz_internal UI_Signal
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

fz_internal UI_Signal
ui_text_colored(String text, V4f32 color)
{
  return ui_textf_colored(color, text);
}

fz_internal UI_Signal
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

fz_internal UI_Signal
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

fz_internal UI_Signal
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

fz_internal UI_Signal
ui_image(String unique, s32 texture_handle, V2f32 uv_min, V2f32 uv_max)
{
  UI_Texture texture = {texture_handle};
  UI_Signal image_signal = {0};
  {
    ui_color_theme_style(UI_Color_Theme_White)
    ui_size_kind_x(UI_Size_Kind_Copy_Y)
    ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
    ui_texture(texture)
    ui_padding_fixed(0)
    {
      UI_Node_Flags flags = UI_Node_Flags_Hoverable|UI_Node_Flags_Mouse_Clickable;
      image_signal.node = _ui_node_from_string(
        unique,
        Sf(UIContext.frame_arena, "##_image_" S_FMT, S_ARG(unique)),
        flags);

      image_signal.node->texture = texture;
      image_signal.node->uv_min  = uv_min;
      image_signal.node->uv_max  = uv_max;

      _ui_fill_signals_from_node(&image_signal);
    }
  }

  return image_signal;
}

fz_internal UI_Signal
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

fz_internal UI_Signal
ui_slider_s32(String unique, String text, s32 *val, s32 min, s32 max, f32 width)
{
  *val = clamp(*val, min, max);

  String complete_text = Sf(UIContext.frame_arena, "%s%d", text.cstring, *val);
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
  f32 percentage = (f32)(*val - min) / (f32)(max - min);
  f32 new_size   = percentage * full_width;

  if (slider_node_cached->slider.is_slider_dragging && is_button_down(Mouse_Button_Left))
  {
    f32 relative_x = (f32)((s32)UIContext.mouse_x - (s32)start_x);
    f32 normalized = clamp(relative_x / (f32)full_width, 0.0f, 1.0f);

    *val = min + (s32)(normalized * (f32)(max - min));
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

fz_internal UI_Signal
ui_slider_u32(String unique, String text, u32 *val, u32 min, u32 max, f32 width)
{
  *val = clamp(*val, min, max);

  String complete_text = Sf(UIContext.frame_arena, "%s%u", text.cstring, *val);
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
  f32 percentage = (f32)(*val - min) / (f32)(max - min);
  f32 new_size   = percentage * full_width;

  if (slider_node_cached->slider.is_slider_dragging && is_button_down(Mouse_Button_Left))
  {
    f32 relative_x = (f32)((s32)UIContext.mouse_x - (s32)start_x);
    f32 normalized = clamp(relative_x / (f32)full_width, 0.0f, 1.0f);

    *val = min + (u32)(normalized * (f32)(max - min));
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

fz_internal void
ui_toggle_color_theme_window()
{
  UIContext.debug.draw_color_theme_window = !UIContext.debug.draw_color_theme_window;
}

fz_internal void
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
    ui_slider_s32(S("Roundness"), S("Roundness (Pixels): "), &UIContext.style.corner_roundness, 0, 16, 150.0f);
  }
}

fz_internal void
ui_toggle_debug_window()
{
  UIContext.debug.draw_debug_window = !UIContext.debug.draw_debug_window;
}

fz_internal void
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
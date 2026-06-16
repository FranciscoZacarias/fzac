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

  UIContext.style.corner_roundness = 0;
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
  node->texture           = ui_stack_texture_top();

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

  node->color_theme.border_thickness_px  = ui_stack_border_thickness_top();
  if (!ui_stack_border_color_is_at_bottom())
  {
    node->color_theme.border_color = ui_stack_border_color_top();
  }

  node->color_theme.corner_roundness_px  = UIContext.style.corner_roundness * ui_stack_corner_roundness_px_top();
  node->color_theme.corner_roundness_px  = clamp(node->color_theme.corner_roundness_px, 0.0f, 0.5f);

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
      parent->cursor.y -= node->color_theme.border_thickness_px;
      s32 max_cursor_y = parent->clip.top_left.y + parent_clip_size.y;
      parent->cursor.y = clamp(parent->cursor.y, parent->clip.top_left.y, max_cursor_y);
    }
    break;
    case UI_Alignment_Kind_X:
    {
      parent->cursor.x += (node->bounds.size.x + parent->child_spacing);
      parent->cursor.x -= node->color_theme.border_thickness_px;
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
    if (node->texture.handle >= 0)
    {
      _ui_push_draw_rect_with_texture_command(node->texture.handle, node->uv_min, node->uv_max, v2f32((f32)absolute_top_left.x, (f32)absolute_top_left.y), v2f32((f32)node->bounds.size.x, (f32)node->bounds.size.y), node->target_background_color, 0, node->color_theme.corner_roundness_px, 0.0f, (f32)node->color_theme.border_thickness_px, node->target_border_color);
    }
    else
    {
      _ui_push_draw_rect_command(v2f32((f32)absolute_top_left.x, (f32)absolute_top_left.y), v2f32((f32)node->bounds.size.x, (f32)node->bounds.size.y), node->target_background_color, 0, node->color_theme.corner_roundness_px, 0.0f, (f32)node->color_theme.border_thickness_px, node->target_border_color);
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
      _ui_push_draw_rect_command(v2f32_add(v2f32((f32)absolute_top_left.x, (f32)absolute_top_left.y), v2f32((f32)node->clip.top_left.x, (f32)node->clip.top_left.y)), v2f32((f32)node->clip.size.x,(f32)node->clip.size.y), background_color, 0, node->color_theme.corner_roundness_px, 0.0f, (f32)node->color_theme.border_thickness_px, COLOR_YELLOW(0.6f));
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
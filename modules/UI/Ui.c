// -----------------------------------------------------------------------
// ui_init
// -----------------------------------------------------------------------

function void
ui_init(R_Font *font)
{
  memory_zero_struct(&UIContext);
  UIContext.arena       = arena_alloc();
  UIContext.frame_arena = arena_alloc();

  UIContext.animation_speed   = 10.0f;
  UIContext.hash_hot_depth    = 1.0f;
  UIContext.hash_active_depth = 1.0f;
  UIContext.font              = font;
  UIContext.resize_area_side  = 20;
  UIContext.window_min_width  = 150;
  UIContext.window_min_height = 50;

  UIContext.style.corner_roundness = 0.2f;
  UIContext.style.border_thickness = 1;

  array_init_with_arena(UIContext.arena, UIContext.commands, UI_Command, UI_MAX_COMMANDS);

  array_init_with_arena(UIContext.arena, UIContext.cached_nodes, UI_Node_Cache, UI_MAX_CACHED_NODES);
  UIContext.cached_nodes_available             = UIContext.cached_nodes_capacity;
  UIContext.cached_nodes_max_idle_lifetime_ms  = UI_MAX_NODE_IDLE_LIFETIME_MS;

  UIContext.previous_frame.rects_arena = arena_alloc();
  array_init_with_arena(UIContext.previous_frame.rects_arena,
                        UIContext.previous_frame.rects,
                        UI_Rect,
                        UI_MAX_PREVIOUS_FRAME_RECTS);

  // Stack defaults
  ui_stack_init(parent,                 &UINodeNilSentinel);
  ui_stack_init(top_left,               v2s32(0, 0));
  ui_stack_init(size_kind_x,            UI_Size_Kind_Relative);
  ui_stack_init(size_kind_y,            UI_Size_Kind_Relative);
  ui_stack_init(size_fixed_x,           0);
  ui_stack_init(size_fixed_y,           0);
  ui_stack_init(size_relative_x,        1.0f);
  ui_stack_init(size_relative_y,        1.0f);
  ui_stack_init(padding_fixed,          0);
  ui_stack_init(node_spacing,           0);
  ui_stack_init(border_thickness,       UIContext.style.border_thickness);
  ui_stack_init(corner_roundness_factor, 1.0f);
  ui_stack_init(alignment_kind,         UI_Alignment_Kind_Y);
}

// -----------------------------------------------------------------------
// ui_begin
// -----------------------------------------------------------------------

function void
ui_begin(u32 window_width, u32 window_height,
         s32 mouse_x, s32 mouse_y,
         s32 mouse_delta_x, s32 mouse_delta_y,
         f32 delta_time, u64 frame_count)
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

  // Root node (represents the OS window)
  {
    String root_name = S("__root_window__");
    UI_Node *root_node = _ui_node_from_string(root_name, root_name, UI_Node_Flags_None);
    UIContext.root = root_node;
    UIContext.root->depth = 1.0f;
    ui_stack_parent_push(UIContext.root);
  }
}

// -----------------------------------------------------------------------
// ui_end
// -----------------------------------------------------------------------

function void
ui_end(void)
{
  _ui_update_tree_nodes(UIContext.root);
  _ui_render_ui_tree(UIContext.root);
  _ui_purge_old_cached_nodes();

  // Save root-child rects so ui_is_mouse_in_ui can query them next frame
  UIContext.previous_frame.rects_count = 0;
  for (UI_Node *child = UIContext.root->first; child; child = child->next)
  {
    UI_Rect *rect;
    array_add(rect, UIContext.previous_frame.rects);
    *rect = child->bounds;
  }

  UIContext.hash_active_previous = UIContext.hash_active;
  UIContext.hash_hot             = 0;
  UIContext.hash_hot_depth       = 1.0f;

  UIContext.root->first    = NULL;
  UIContext.root->last     = NULL;
  UIContext.root->next     = NULL;
  UIContext.root->previous = NULL;
  UIContext.root->parent   = NULL;

  ui_stack_parent_pop(); // pop the root node pushed in ui_begin
  arena_clear(UIContext.frame_arena);
}

// -----------------------------------------------------------------------
// ui_is_mouse_in_ui
// -----------------------------------------------------------------------

function b32
ui_is_mouse_in_ui(void)
{
  for (u32 i = 0; i < UIContext.previous_frame.rects_count; i += 1)
  {
    if (_ui_is_mouse_in_rect(UIContext.previous_frame.rects[i]))
    {
      return true;
    }
  }
  return false;
}

// -----------------------------------------------------------------------
// ui_window_begin / ui_window_end
// -----------------------------------------------------------------------

function UI_Signal
ui_window_begin(String text, s32 x, s32 y, s32 width, s32 height)
{
  UI_Signal signal = {0};

  // Outer window node — invisible container that owns drag/resize state
  UI_Node *window_node = NULL;
  {
    ui_top_left(v2s32(x, y))
    ui_size_kind_x(UI_Size_Kind_Fixed) ui_size_fixed_x(width)
    ui_size_kind_y(UI_Size_Kind_Fixed) ui_size_fixed_y(height)
    ui_corner_roundness_factor(0.0f)
    {
      String window_text = Sf(UIContext.frame_arena, S_FMT"##_window_", S_ARG(text));
      window_node = _ui_node_from_string(window_text, window_text,
                                         UI_Node_Flags_Draggable_Panel | UI_Node_Flags_Resizable);
      window_node->should_render = false;
      ui_stack_parent_push(window_node);
      signal.node = window_node;
    }
  }

  // Title bar row
  UI_Node *title_bar_node = NULL;
  {
    ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(1.0f)
    ui_size_kind_y(UI_Size_Kind_Fixed)    ui_size_fixed_y(UIContext.font->height * 2)
    ui_alignment_kind(UI_Alignment_Kind_X)
    ui_padding_fixed(1)
    ui_corner_roundness_factor(0.0f)
    {
      String title_bar_text = Sf(UIContext.frame_arena, S_FMT"##_title_bar_", S_ARG(text));
      title_bar_node = _ui_node_from_string(title_bar_text, text,
                                             UI_Node_Flags_Hoverable    |
                                             UI_Node_Flags_Draggable    |
                                             UI_Node_Flags_Mouse_Clickable |
                                             UI_Node_Flags_Text_Display  |
                                             UI_Node_Flags_Text_Center_Y);
      UI_Signal title_bar_signal = {.node = title_bar_node};
      _ui_fill_signals_from_node(&title_bar_signal);
      set_flags(signal.flags, title_bar_signal.flags);
      ui_stack_parent_push(title_bar_node);
    }
  }

  // Close button
  {
    ui_size_kind_x(UI_Size_Kind_Fixed) ui_size_fixed_x(UIContext.font->height * 2)
    ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
    ui_padding_fixed(1)
    ui_corner_roundness_factor(0.0f)
    {
      String close_text = Sf(UIContext.frame_arena, S_FMT"##_close_button_", S_ARG(text));
      UI_Node *close_node = _ui_node_from_string(close_text, S("X"),
                                                  UI_Node_Flags_Hoverable         |
                                                  UI_Node_Flags_Mouse_Clickable   |
                                                  UI_Node_Flags_Text_Display      |
                                                  UI_Node_Flags_Text_Center_X     |
                                                  UI_Node_Flags_Text_Center_Y);
      UI_Signal close_signal = {.node = close_node};
      _ui_fill_signals_from_node(&close_signal);
      if (ui_clicked(close_signal))
      {
        set_flags(signal.flags, UI_Signal_Flags_Close);
      }
    }
  }

  ui_stack_parent_pop(); // title_bar_node

  // Body — callers push their widgets here
  {
    ui_size_kind_x(UI_Size_Kind_Relative) ui_size_relative_x(1.0f)
    ui_size_kind_y(UI_Size_Kind_Relative) ui_size_relative_y(1.0f)
    ui_alignment_kind(UI_Alignment_Kind_Y)
    ui_padding_fixed(5)
    ui_node_spacing(5)
    ui_corner_roundness_factor(0.0f)
    {
      String body_text = Sf(UIContext.frame_arena, S_FMT"##_window_body_", S_ARG(text));
      UI_Node *body_node = _ui_node_from_string(body_text, body_text, UI_Node_Flags_None);
      ui_stack_parent_push(body_node);
    }
  }

  return signal;
}

function void
ui_window_end(void)
{
  ui_stack_parent_pop(); // body_node

  // Resize handle (bottom-right corner grab area)
  UI_Node *window_node = ui_stack_parent_top();
  assert(string_contains(window_node->string, S("##_window_")));

  UI_Node_Cache *window_cache   = _ui_get_cached_node(window_node->hash);
  V2s32          resized_size   = v2s32_add(window_node->clip.size, window_cache->accumulated_resize_offset);

  {
    ui_top_left(v2s32(window_node->clip.top_left.x + resized_size.x - UIContext.resize_area_side - 2,
                      window_node->clip.top_left.y + resized_size.y - UIContext.resize_area_side - 2))
    ui_size_kind_x(UI_Size_Kind_Fixed) ui_size_fixed_x(UIContext.resize_area_side)
    ui_size_kind_y(UI_Size_Kind_Fixed) ui_size_fixed_y(UIContext.resize_area_side)
    ui_padding_fixed(1)
    {
      String resize_text = Sf(UIContext.frame_arena, S_FMT"##_resize_", S_ARG(window_node->string));
      _ui_node_from_string(resize_text, resize_text,
                           UI_Node_Flags_Hoverable       |
                           UI_Node_Flags_Mouse_Clickable |
                           UI_Node_Flags_Float_On_Parent);
    }
  }

  ui_stack_parent_pop(); // window_node
  _ui_update_tree_nodes(window_node);
}

// -----------------------------------------------------------------------
// Command buffer accessors
// -----------------------------------------------------------------------

function u32
ui_get_commands_count(void)
{
  return UIContext.commands_count;
}

function UI_Command *
ui_get_commands(void)
{
  return UIContext.commands;
}

// -----------------------------------------------------------------------
// _ui_node_from_string  — allocates and wires up one node for this frame
// -----------------------------------------------------------------------

function UI_Node *
_ui_node_from_string(String unique, String display, UI_Node_Flags flags)
{
  UI_Node *parent = ui_stack_parent_top();
  UI_Node *node   = push_array(UIContext.frame_arena, UI_Node, 1);

  // Special-case: no parent yet → this is the root (OS window) node
  if (parent == &UINodeNilSentinel)
  {
    node->string        = S("__root_window__");
    node->hash          = string_hash(node->string);
    node->bounds.top_left = v2s32(0, 0);
    node->bounds.size     = v2s32(UIContext.window_width, UIContext.window_height);
    node->clip            = node->bounds;
    node->cursor          = node->bounds.top_left;
    node->depth           = 1.0f;
    node->should_render   = false;
    node->alignment       = UI_Alignment_Kind_Float;
    UIContext.root        = node;
    return node;
  }

  node->hash          = parent->hash ^ string_hash(unique);
  node->flags         = flags;
  node->should_render = true;
  node->child_spacing = ui_stack_node_spacing_top();
  node->alignment     = ui_stack_alignment_kind_top();
  node->texture_index = -1;

  node->string            = string_copy(UIContext.frame_arena, display);
  node->string_clean      = _ui_clean_string(UIContext.frame_arena, display);
  R_Text_Metrics metrics  = r_measure_text(UIContext.font, node->string_clean);
  node->clean_string_dimensions = v2f32(metrics.width, metrics.height);

  // Default colors (flat — caller can override via color_theme after this returns)
  node->color_theme.border_color            = v4f32(0.22f, 0.14f, 0.08f, 1.0f);
  node->color_theme.border_hover_color      = v4f32(0.32f, 0.20f, 0.12f, 1.0f);
  node->color_theme.border_active_color     = v4f32(0.42f, 0.26f, 0.16f, 1.0f);
  node->color_theme.background_color        = v4f32(0.07f, 0.06f, 0.05f, 1.0f);
  node->color_theme.background_hover_color  = v4f32(0.05f, 0.05f, 0.05f, 1.0f);
  node->color_theme.background_active_color = v4f32(0.10f, 0.08f, 0.06f, 1.0f);
  node->color_theme.text_color              = v4f32(0.79f, 0.76f, 0.71f, 1.0f);
  node->color_theme.text_hover_color        = v4f32(1.0f,  1.0f,  1.0f,  1.0f);
  node->color_theme.text_active_color       = v4f32(0.92f, 0.92f, 0.92f, 1.0f);
  node->color_theme.border_thickness        = ui_stack_border_thickness_top();
  node->color_theme.corner_roundness        = UIContext.style.corner_roundness *
                                               ui_stack_corner_roundness_factor_top();
  node->color_theme.corner_roundness        = clamp(node->color_theme.corner_roundness, 0.0f, 0.5f);

  node->target_border_color     = node->color_theme.border_color;
  node->target_background_color = node->color_theme.background_color;
  node->target_text_color       = node->color_theme.text_color;

  UI_Node_Cache *parent_cached_node = _ui_get_cached_node(parent->hash);
  _ui_add_node_child(parent, node);

  node->depth = node->parent->depth - UI_EPSILON;

  s32 padding = ui_stack_padding_fixed_top();

  // Bounds size -----------------------------------------------------------
  V2s32 parent_clip_size = v2s32_add(parent->clip.size, parent_cached_node->accumulated_resize_offset);

  s32 available_width  = parent_clip_size.x - (parent->cursor.x - parent->clip.top_left.x);
  s32 available_height = parent_clip_size.y - (parent->cursor.y - parent->clip.top_left.y);
  if (has_flags(node->flags, UI_Node_Flags_Float_On_Parent))
  {
    available_width  = parent->bounds.size.x;
    available_height = parent->bounds.size.y;
  }

  UI_Size_Kind size_x_kind = ui_stack_size_kind_x_top();
  switch (size_x_kind)
  {
    case UI_Size_Kind_Fixed:
    {
      node->bounds.size.x = clamp(ui_stack_size_fixed_x_top(), 0, available_width);
    }
    break;
    case UI_Size_Kind_Relative:
    {
      f32 relative_factor = clamp(ui_stack_size_relative_x_top(), 0.0f, 1.0f);
      node->bounds.size.x = (s32)(relative_factor * available_width);
    }
    break;
    case UI_Size_Kind_Copy_Y: { /* resolved below */ } break;
    default: { ui_error(S("Unhandled UI_Size_Kind for X")); } break;
  }

  UI_Size_Kind size_y_kind = ui_stack_size_kind_y_top();
  switch (size_y_kind)
  {
    case UI_Size_Kind_Fixed:
    {
      node->bounds.size.y = clamp(ui_stack_size_fixed_y_top(), 0, available_height);
    }
    break;
    case UI_Size_Kind_Relative:
    {
      f32 relative_factor = clamp(ui_stack_size_relative_y_top(), 0.0f, 1.0f);
      node->bounds.size.y = (s32)(relative_factor * available_height);
    }
    break;
    case UI_Size_Kind_Copy_X: { /* resolved below */ } break;
    default: { ui_error(S("Unhandled UI_Size_Kind for Y")); } break;
  }

  if (size_y_kind == UI_Size_Kind_Copy_X) { node->bounds.size.y = node->bounds.size.x; }
  if (size_x_kind == UI_Size_Kind_Copy_Y) { node->bounds.size.x = node->bounds.size.y; }

  // Bounds position -------------------------------------------------------
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

  // Advance parent cursor -------------------------------------------------
  if (!has_flags(node->flags, UI_Node_Flags_Float_On_Parent))
  {
    switch (parent->alignment)
    {
      case UI_Alignment_Kind_Y:
      {
        parent->cursor.y += node->bounds.size.y + parent->child_spacing;
        parent->cursor.y -= node->color_theme.border_thickness;
        parent->cursor.y  = clamp(parent->cursor.y,
                                   parent->clip.top_left.y,
                                   parent->clip.top_left.y + parent_clip_size.y);
      }
      break;
      case UI_Alignment_Kind_X:
      {
        parent->cursor.x += node->bounds.size.x + parent->child_spacing;
        parent->cursor.x -= node->color_theme.border_thickness;
        parent->cursor.x  = clamp(parent->cursor.x,
                                   parent->clip.top_left.x,
                                   parent->clip.top_left.x + parent_clip_size.x);
      }
      break;
      case UI_Alignment_Kind_Float: break;
    }
  }

  // Clip ------------------------------------------------------------------
  padding = clamp_top(padding, node->bounds.size.y / 3);
  s32 double_padding = padding * 2;

  node->clip.top_left = v2s32(padding, padding);
  node->clip.size.x   = clamp_bot(node->bounds.size.x - double_padding, 2);
  node->clip.size.y   = clamp_bot(node->bounds.size.y - double_padding, 2);
  node->cursor        = node->clip.top_left;

  // Interaction -----------------------------------------------------------
  if (_ui_is_mouse_in_node(node))
  {
    if (node->depth < UIContext.hash_hot_depth)
    {
      if (has_flags(node->flags, UI_Node_Flags_Hoverable))
      {
        UIContext.hash_hot       = node->hash;
        UIContext.hash_hot_depth = node->depth;
      }
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

  // Resize ----------------------------------------------------------------
  if (has_flags(node->flags, UI_Node_Flags_Resizable))
  {
    UI_Node_Cache *cached_node    = _ui_get_cached_node(node->hash);
    V2s32          absolute_top_left = _ui_get_node_absolute_top_left(node);

    // Accumulate drag from all ancestor draggable panels
    V2s32 total_drag_offset = v2s32(0, 0);
    for (UI_Node *ancestor = node; ancestor != NULL; ancestor = ancestor->parent)
    {
      if (has_flags(ancestor->flags, UI_Node_Flags_Draggable_Panel))
      {
        UI_Node_Cache *ancestor_cache = _ui_get_cached_node(ancestor->hash);
        total_drag_offset = v2s32_add(total_drag_offset, ancestor_cache->accumulated_drag_offset);
      }
    }
    absolute_top_left = v2s32_add(absolute_top_left, total_drag_offset);

    V2s32    resized_node_size  = v2s32_add(node->bounds.size, cached_node->accumulated_resize_offset);
    V2s32    absolute_bot_right = v2s32_add(absolute_top_left, resized_node_size);
    UI_Rect  resize_pick_rect   = {0};
    resize_pick_rect.top_left.x = absolute_bot_right.x - UIContext.resize_area_side;
    resize_pick_rect.top_left.y = absolute_bot_right.y - UIContext.resize_area_side;
    resize_pick_rect.size       = v2s32(UIContext.resize_area_side, UIContext.resize_area_side);

    if (_ui_is_mouse_in_rect(resize_pick_rect) && is_button_clicked(Mouse_Button_Left))
    {
      UIContext.hash_resizing       = node->hash;
      UIContext.hash_resizing_depth = node->depth;
    }
  }

  if (UIContext.hash_resizing == node->hash)
  {
    UI_Node_Cache *cached_node = _ui_get_cached_node(node->hash);
    if (cached_node->accumulated_resize_offset.x + node->bounds.size.x + UIContext.mouse_delta_x > UIContext.window_min_width)
    {
      cached_node->accumulated_resize_offset.x += UIContext.mouse_delta_x;
    }
    if (cached_node->accumulated_resize_offset.y + node->bounds.size.y + UIContext.mouse_delta_y > UIContext.window_min_height)
    {
      cached_node->accumulated_resize_offset.y += UIContext.mouse_delta_y;
    }
  }

  // Dragging --------------------------------------------------------------
  if (UIContext.hash_active == node->hash && has_flags(node->flags, UI_Node_Flags_Draggable))
  {
    UI_Node *window_node = node;
    while (window_node && !has_flags(window_node->flags, UI_Node_Flags_Draggable_Panel))
    {
      window_node = window_node->parent;
    }
    if (window_node)
    {
      UI_Node_Cache *window_cache = _ui_get_cached_node(window_node->hash);
      window_cache->accumulated_drag_offset.x += UIContext.mouse_delta_x;
      window_cache->accumulated_drag_offset.y += UIContext.mouse_delta_y;
    }
  }

  // Reset active/resizing on mouse release --------------------------------
  if (is_button_up(Mouse_Button_Left) && was_button_down(Mouse_Button_Left))
  {
    UIContext.hash_active       = 0;
    UIContext.hash_active_depth = 1.0f;
    UIContext.hash_resizing       = 0;
    UIContext.hash_resizing_depth = 1.0f;
  }

  return node;
}

// -----------------------------------------------------------------------
// _ui_add_node_child
// -----------------------------------------------------------------------

function void
_ui_add_node_child(UI_Node *parent, UI_Node *child)
{
  if (parent == &UINodeNilSentinel)
  {
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

// -----------------------------------------------------------------------
// _ui_update_tree_nodes  — applies drag/resize offsets + animates colors
// -----------------------------------------------------------------------

function void
_ui_update_tree_nodes(UI_Node *node)
{
  if (!node) { return; }

  if (node != UIContext.root)
  {
    UI_Node_Cache *cached_node = _ui_get_cached_node(node->hash);

    node->bounds.top_left = v2s32_add(node->bounds.top_left, cached_node->accumulated_drag_offset);

    if (has_flags(node->flags, UI_Node_Flags_Resizable))
    {
      node->bounds.size = v2s32_add(node->bounds.size, cached_node->accumulated_resize_offset);
      node->clip.size   = v2s32_add(node->clip.size,   cached_node->accumulated_resize_offset);
    }

    // Hover animation
    if (has_flags(node->flags, UI_Node_Flags_Hoverable))
    {
      if (UIContext.hash_hot == node->hash)
      {
        cached_node->hover_t = clamp(cached_node->hover_t + UIContext.delta_time * UIContext.animation_speed, 0.0f, 1.0f);
      }
      else
      {
        cached_node->hover_t = clamp(cached_node->hover_t - UIContext.delta_time * UIContext.animation_speed, 0.0f, 1.0f);
      }

      node->target_border_color     = v4f32_lerp(node->color_theme.border_color,     node->color_theme.border_hover_color,     cached_node->hover_t);
      node->target_background_color = v4f32_lerp(node->color_theme.background_color, node->color_theme.background_hover_color, cached_node->hover_t);
      node->target_text_color       = v4f32_lerp(node->color_theme.text_color,       node->color_theme.text_hover_color,       cached_node->hover_t);
    }

    // Active animation
    if (has_flags(node->flags, UI_Node_Flags_Mouse_Clickable))
    {
      if (UIContext.hash_active == node->hash)
      {
        cached_node->active_t = clamp(cached_node->active_t + UIContext.delta_time * UIContext.animation_speed, 0.0f, 1.0f);
      }
      else
      {
        cached_node->active_t = clamp(cached_node->active_t - UIContext.delta_time * UIContext.animation_speed, 0.0f, 1.0f);
      }

      node->target_border_color     = v4f32_lerp(node->target_border_color,     node->color_theme.border_active_color,     cached_node->active_t);
      node->target_background_color = v4f32_lerp(node->target_background_color, node->color_theme.background_active_color, cached_node->active_t);
      node->target_text_color       = v4f32_lerp(node->target_text_color,       node->color_theme.text_active_color,       cached_node->active_t);
    }
  }

  for (UI_Node *child = node->first; child; child = child->next)
  {
    _ui_update_tree_nodes(child);
  }
}

// -----------------------------------------------------------------------
// _ui_render_ui_tree
// -----------------------------------------------------------------------

function void
_ui_render_ui_tree(UI_Node *node)
{
  if (!node->should_render)
  {
    for (UI_Node *child = node->first; child; child = child->next)
    {
      _ui_render_ui_tree(child);
    }
    return;
  }

  V2s32 absolute_top_left = _ui_get_node_absolute_top_left(node);

  if (node->texture_index >= 0)
  {
    _ui_push_draw_rect_with_texture_command(node->texture_index, node->uv_min, node->uv_max,
                                            v2f32((f32)absolute_top_left.x, (f32)absolute_top_left.y),
                                            v2f32((f32)node->bounds.size.x,  (f32)node->bounds.size.y),
                                            node->target_background_color,
                                            0.0f,
                                            node->color_theme.corner_roundness,
                                            0.0f,
                                            (f32)node->color_theme.border_thickness,
                                            node->target_border_color);
  }
  else
  {
    _ui_push_draw_rect_command(v2f32((f32)absolute_top_left.x, (f32)absolute_top_left.y),
                               v2f32((f32)node->bounds.size.x,  (f32)node->bounds.size.y),
                               node->target_background_color,
                               0.0f,
                               node->color_theme.corner_roundness,
                               0.0f,
                               (f32)node->color_theme.border_thickness,
                               node->target_border_color);
  }

  if (has_flags(node->flags, UI_Node_Flags_Text_Display))
  {
    V2s32 text_position = v2s32_add(absolute_top_left, node->cursor);

    if (has_flags(node->flags, UI_Node_Flags_Text_Center_Y))
    {
      if (node->clean_string_dimensions.y < node->clip.size.y)
      {
        s32 text_height_half = (s32)node->clean_string_dimensions.y / 2;
        s32 clip_height_half = (s32)node->clip.size.y / 2;
        text_position.y += abs(text_height_half - clip_height_half);
      }
    }
    if (has_flags(node->flags, UI_Node_Flags_Text_Center_X))
    {
      if (node->clean_string_dimensions.x < node->clip.size.x)
      {
        s32 text_width_half = (s32)node->clean_string_dimensions.x / 2;
        s32 clip_width_half = (s32)node->clip.size.x / 2;
        text_position.x += abs(text_width_half - clip_width_half);
      }
    }

    _ui_push_draw_text_command(v2f32((f32)text_position.x, (f32)text_position.y),
                               (f32)node->clip.size.x - node->cursor.x,
                               (f32)node->clip.size.y,
                               node->target_text_color,
                               node->string_clean);
  }

  for (UI_Node *child = node->first; child; child = child->next)
  {
    _ui_render_ui_tree(child);
  }
}

// -----------------------------------------------------------------------
// _ui_get_cached_node
// -----------------------------------------------------------------------

function UI_Node_Cache *
_ui_get_cached_node(u64 hash)
{
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
    ui_error(S("UI: Too many cached nodes. Increase UI_MAX_CACHED_NODES."));
    return NULL;
  }

  // Reuse an evicted slot if one exists
  for (u32 i = 0; i < UIContext.cached_nodes_count; i += 1)
  {
    if (UIContext.cached_nodes[i].hash == 0)
    {
      UIContext.cached_nodes[i].hash               = hash;
      UIContext.cached_nodes[i].last_time_touched  = time_milliseconds();
      UIContext.cached_nodes_available            -= 1;
      return &UIContext.cached_nodes[i];
    }
  }

  // Append a new slot
  UI_Node_Cache *cached_node = &UIContext.cached_nodes[UIContext.cached_nodes_count];
  memory_zero_struct(cached_node);
  cached_node->hash              = hash;
  cached_node->last_time_touched = time_milliseconds();
  UIContext.cached_nodes_count     += 1;
  UIContext.cached_nodes_available -= 1;

  return cached_node;
}

// -----------------------------------------------------------------------
// _ui_purge_old_cached_nodes
// -----------------------------------------------------------------------

function void
_ui_purge_old_cached_nodes(void)
{
  u64 time_now = time_milliseconds();
  for (u32 i = 0; i < UIContext.cached_nodes_count; i += 1)
  {
    UI_Node_Cache *cached_node = &UIContext.cached_nodes[i];
    if (cached_node->hash != 0)
    {
      if (time_now - cached_node->last_time_touched > UIContext.cached_nodes_max_idle_lifetime_ms)
      {
        cached_node->hash = 0;
        UIContext.cached_nodes_available += 1;
      }
    }
  }
}

// -----------------------------------------------------------------------
// _ui_fill_signals_from_node
// -----------------------------------------------------------------------

function void
_ui_fill_signals_from_node(UI_Signal *signal)
{
  if (!signal->node)
  {
    ui_error(S("_ui_fill_signals_from_node: signal has no node."));
    return;
  }

  if (_ui_is_mouse_in_node(signal->node))
  {
    if (is_button_down(Mouse_Button_Left))    { set_flags(signal->flags, UI_Signal_Flags_Left_Pressed);  }
    if (is_button_down(Mouse_Button_Right))   { set_flags(signal->flags, UI_Signal_Flags_Right_Pressed); }
    if (is_button_clicked(Mouse_Button_Left)) { set_flags(signal->flags, UI_Signal_Flags_Left_Clicked);  }
    if (is_button_clicked(Mouse_Button_Right)){ set_flags(signal->flags, UI_Signal_Flags_Right_Clicked); }
    set_flags(signal->flags, UI_Signal_Flags_Mouse_Hovered);
  }
}

// -----------------------------------------------------------------------
// _ui_is_mouse_in_node
// -----------------------------------------------------------------------

function b32
_ui_is_mouse_in_node(UI_Node *node)
{
  UI_Rect bounds = {0};
  bounds.size = node->bounds.size;

  V2s32 absolute_top_left = _ui_get_node_absolute_top_left(node);

  V2s32 total_drag_offset = v2s32(0, 0);
  for (UI_Node *ancestor = node; ancestor != NULL; ancestor = ancestor->parent)
  {
    if (has_flags(ancestor->flags, UI_Node_Flags_Draggable_Panel))
    {
      UI_Node_Cache *ancestor_cache = _ui_get_cached_node(ancestor->hash);
      total_drag_offset = v2s32_add(total_drag_offset, ancestor_cache->accumulated_drag_offset);
    }
  }

  bounds.top_left = v2s32_add(absolute_top_left, total_drag_offset);
  return _ui_is_mouse_in_rect(bounds);
}

// -----------------------------------------------------------------------
// _ui_is_mouse_in_rect
// -----------------------------------------------------------------------

function b32
_ui_is_mouse_in_rect(UI_Rect rect)
{
  return (UIContext.mouse_x >= rect.top_left.x &&
          UIContext.mouse_x <= rect.top_left.x + rect.size.x &&
          UIContext.mouse_y >= rect.top_left.y &&
          UIContext.mouse_y <= rect.top_left.y + rect.size.y);
}

// -----------------------------------------------------------------------
// _ui_get_node_absolute_top_left
// -----------------------------------------------------------------------

function V2s32
_ui_get_node_absolute_top_left(UI_Node *node)
{
  V2s32 top_left = v2s32(0, 0);
  for (UI_Node *ancestor = node; ancestor != NULL; ancestor = ancestor->parent)
  {
    top_left = v2s32_add(top_left, ancestor->bounds.top_left);
  }
  return top_left;
}

// -----------------------------------------------------------------------
// _ui_clean_string  — strips everything after ## for display
// -----------------------------------------------------------------------

function String
_ui_clean_string(Arena *arena, String string)
{
  if (string.count > 0)
  {
    String_List parts = string_split(arena, string, S("##"));
    return parts.first->value;
  }
  return string_copy(arena, string);
}

// -----------------------------------------------------------------------
// Draw command helpers
// -----------------------------------------------------------------------

function void
_ui_push_draw_rect_command(V2f32 top_left, V2f32 size, V4f32 color,
                           f32 rotation, f32 roundness, f32 smoothness,
                           f32 border_thickness, V4f32 border_color)
{
  UI_Command *command;
  array_add(command, UIContext.commands);

  command->kind                  = UI_Command_Kind_Draw_Rect;
  command->top_left              = top_left;
  command->size                  = size;
  command->color                 = color;
  command->rect.rotation         = rotation;
  command->rect.roundness        = roundness;
  command->rect.smoothness       = smoothness;
  command->rect.border_thickness = border_thickness;
  command->rect.border_color     = border_color;
}

function void
_ui_push_draw_rect_with_texture_command(s32 texture_index, V2f32 uv_min, V2f32 uv_max,
                                        V2f32 top_left, V2f32 size, V4f32 color,
                                        f32 rotation, f32 roundness, f32 smoothness,
                                        f32 border_thickness, V4f32 border_color)
{
  UI_Command *command;
  array_add(command, UIContext.commands);

  command->kind                  = UI_Command_Kind_Draw_Rect_With_Texture;
  command->top_left              = top_left;
  command->size                  = size;
  command->color                 = color;
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
  UI_Command *command;
  array_add(command, UIContext.commands);

  command->kind            = UI_Command_Kind_Draw_Text;
  command->top_left        = top_left;
  command->color           = color;
  command->text.max_width  = max_width;
  command->text.max_height = max_height;
  command->text.text       = string_copy(UIContext.arena, text);
}

// -----------------------------------------------------------------------
// _ui_error (debug only)
// -----------------------------------------------------------------------

#if DEBUG
function void inline
_ui_error(String message, String file, u32 line)
{
  Scratch scratch = scratch_begin(0, 0);
  String body = Sf(scratch.arena, "UI Error\n"S_FMT"\n\nat "S_FMT":%u", S_ARG(message), S_ARG(file), line);
  message_box(S("UI Error"), body, file, line);
  raddbg_break();
  scratch_end(&scratch);
  assert(false);
}
#endif
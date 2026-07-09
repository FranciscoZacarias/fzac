#ifndef FZ_IMMEDIATE_UI_H
#define FZ_IMMEDIATE_UI_H

typedef struct UI_Node UI_Node;
struct UI_Node
{
  u64 hash;

  UI_Node *first; /* First child */
  UI_Node *last;  /* Last child */
  UI_Node *next;  /* Next child, after current node */
  UI_Node *previous; /* Previous child, before current node */
  UI_Node *parent;   /* Parent of current node */

  String string;
  String string_clean;
  V2f32  string_clean_dimensions;
};

typedef struct UI_Context UI_Context;
struct UI_Context
{
  Arena *arena;
  Arena *frame_arena;

  UI_Node *root;

  // Application stuff
  u32 window_width;
  u32 window_height;
  s32 mouse_x;
  s32 mouse_y;
  s32 mouse_delta_x;
  s32 mouse_delta_y;
  
  f32 delta_time;
};

#endif // FZ_IMMEDIATE_UI_H

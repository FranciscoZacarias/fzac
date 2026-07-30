#ifndef FZ_SIMPLE_RENDERER2D_H
#define FZ_SIMPLE_RENDERER2D_H

/*
  This is just easily put quads on the screen. 
  The renderer is very dumb and could use a lot of work (just for rendering quads!).
  The point is just to get something on the screen and try stuff.

  Example usage:

  #include "Base.h"
  #include "Window.h"
  #include "Simple_Renderer2d.h"

  #include "metaprogram\global_headers.h"

  b8 should_quit;

  fz_internal void 
  entry_point(Command_Line *command_line)
  {
    console_attach();

    window_create(S("FZac Dev"), 1280, 720, 30, 30);
    sr2d_init(get_window_width(), get_window_height(), false);

    while (!should_quit)
    {
      window_update_events();
      handle_window_events();
      handle_input();

      sr2d_draw_quad_color_ext(v2f32(10, 10), v2f32(100, 100), rgba(1.0f, 0.0f, 0.0f, 1.0f),
                               0.0f, 2.0f, 0.0f, 2.0f, COLOR_GREEN(1));
    
      sr2d_end_frame(get_window_width(), get_window_height(), rgba(0.4f, 0.0f, 0.4f, 1.0f));
      clear_temporary_storage();
    }
  }

  fz_internal void
  handle_window_events()
  {
    for (u32 event_index = 0; event_index < get_total_events_this_frame(); event_index += 1)
    {
      Window_Event* event = get_event_this_frame(event_index);

      if (event->kind == Event_Quit)
      {
        should_quit = true;
        break;
      }

      if (event->kind == Event_Window)
      {
        s32 w = get_window_width();
        s32 h = get_window_height();
        sr2d_set_screen_size(w, h);
      }
    }
  }

  fz_internal void
  handle_input()
  {
    if (is_key_clicked(Keyboard_Key_ESCAPE))
    {
      should_quit = true;
      return;
    }
  }
*/

#ifndef PROJECT_FULL_PATH
#define PROJECT_FULL_PATH "."
#endif

//@TODO(fz): Cull quads outside screen

#include "OpenGL.h"
#include "Art.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "Extern/stb_rect_pack.h"

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include "Extern/stb_truetype.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "Extern/stb_image.h"

#define FONT_UBUNTU_MONO_PATH      PROJECT_FULL_PATH "/assets/fonts/UbuntuMono-Regular.ttf"
#define FONT_PROGGY_VECTOR_PATH    "C:/work/fzac_dev/src/ProggyVector-Regular.ttf"
#define FONT_KARMINA_REGULAR_PATH  PROJECT_FULL_PATH "/assets/fonts/Karmina Regular.otf"

#define R_MAX_QUADS_COUNT         thousand(500)
#define R_MAX_TEXTURE_ATLAS       16
#define FONT_CAPACITY             2
#define FONT_HEIGHT               14
#define MAX_FONT_GLYPHS           95

typedef struct SR2D_Texture_Atlas SR2D_Texture_Atlas;
struct SR2D_Texture_Atlas
{
  String name;
  u32    index;
  u32    handle;
  s32    width;
  s32    height;
  u8    *data;
};

typedef struct SR2D_Texture SR2D_Texture;
struct SR2D_Texture
{
  String           name;
  SR2D_Texture_Atlas *atlas;
  V2f32            uv_min;
  V2f32            uv_max;
};

typedef struct SR2D_Font SR2D_Font;
struct SR2D_Font
{
  SR2D_Texture_Atlas  *atlas;
  stbtt_packedchar *char_data;   // MAX_FONT_GLYPHS entries
  f32  height;
  f32  line_height;
  f32  ascent;
  f32  descent;
  f32  line_gap;
};

typedef struct SR2D_Text_Metrics SR2D_Text_Metrics;
struct SR2D_Text_Metrics
{
  f32 width;
  f32 height;
  u64 line_count;
  f32 last_line_width;
};

typedef struct SR2D_Quad SR2D_Quad;
struct SR2D_Quad
{
  V2f32 top_left;
  V2f32 size;
  V2f32 uv_min;
  V2f32 uv_max;
  u32   color;
  u32   texture_id;
  f32   rotation;
  f32   roundness;
  f32   smoothness;
  f32   border_thickness;
  u32   border_color;
};

typedef struct SR2D_Shader_Program SR2D_Shader_Program;
struct SR2D_Shader_Program
{
  u32 pipeline_handle;
  u32 vertex_program_handle;
  u32 fragment_program_handle;

  struct
  {
    s32 time;
    s32 screen_size;
    s32 texture_unit;
  } uniforms;
};

typedef struct SR2D_Text_Params SR2D_Text_Params;
struct SR2D_Text_Params
{
  SR2D_Font         *font;
  String          text;
  V2f32           top_left;
  RGBA            color;
  f32             rotation;
  f32             roundness;
  f32             smoothness;
  f32             border_thickness;
  RGBA            border_color;

  f32          max_width;
  f32          max_height;
  f32          line_spacing;
  f32          letter_spacing;
};

typedef struct SR2D_Frame_Stats SR2D_Frame_Stats;
struct SR2D_Frame_Stats
{
  u32 draw_calls;
  u32 quads_drawn;
  f32 gpu_time_ms;  // one frame behind
};

typedef struct SR2D_Context SR2D_Context;
struct SR2D_Context
{
  Arena *arena;
  f32 time;

  SR2D_Shader_Program screen_program;

  // GPU geometry (shared by all passes)
  u32 vao;
  u32 ebo;
  u32 unit_vbo;
  u32 instance_vbo;

  // Texture atlases
  SR2D_Texture_Atlas *texture_atlas;
  u32              texture_atlas_count;
  u32              texture_atlas_capacity;

  // Fonts
  SR2D_Font          *fonts;
  u32              fonts_count;
  u32              fonts_capacity;

  // Defaults / cache
  SR2D_Texture_Atlas *white_texture;

  u32 screen_width;
  u32 screen_height;

  SR2D_Quad *screen_quads;
  u32    screen_quads_count;
  u32    quads_capacity;

#if DEBUG
  u32 gpu_timer_queries[2];
  u32 gpu_timer_frame;

  SR2D_Frame_Stats previous_frame_stats;
#endif
};

fz_global SR2D_Context SR2D_RenderContext;
fz_global V2f32        SR2D_UnitQuadVertices[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
fz_global u16          SR2D_UnitQuadIndices[] = { 0, 1, 2, 2, 3, 0 };

#if DEBUG
# define sr2d_error(msg) _sr2d_error((msg), S(__FILE__), __LINE__)
fz_internal_no_reflection void inline _sr2d_error(String message, String file, u32 line);
#else
# define sr2d_error(msg)
#endif

// @Section: Shaders
const char *sr2d_screen_vs_src =
  "#version 460 core                                                       \n"
  "                                                                        \n"
  "layout(location = 0) in vec2 a_unit_pos;                                \n"
  "layout(location = 1) in vec2 a_top_left;                                \n"
  "layout(location = 2) in vec2 a_size;                                    \n"
  "layout(location = 3) in vec2 a_uv_min;                                  \n"
  "layout(location = 4) in vec2 a_uv_max;                                  \n"
  "layout(location = 5) in uint a_color;                                   \n"
  "layout(location = 6) in uint a_texture_id;                              \n"
  "layout(location = 7) in float a_rotation;                               \n"
  "layout(location = 8) in float a_roundness;                              \n"
  "layout(location = 9) in float a_smoothness;                             \n"
  "layout(location = 10) in float a_border_thickness;                      \n"
  "layout(location = 11) in uint a_border_color;                           \n"
  "                                                                        \n"
  "uniform float u_time;                                                   \n"
  "uniform vec2  u_screen_size;                                            \n"
  "                                                                        \n"
  "out vec2 v_uv;                                                          \n"
  "out vec4 v_color;                                                       \n"
  "out vec4 v_border_color;                                                \n"
  "out vec2 v_local_pos;                                                   \n"
  "out vec2 v_box_size;                                                    \n"
  "out float v_roundness;                                                  \n"
  "out float v_smoothness;                                                 \n"
  "out float v_border_thickness;                                           \n"
  "out flat uint v_texture_id;                                             \n"
  "out gl_PerVertex { vec4 gl_Position; };                                 \n"
  "                                                                        \n"
  "void main()                                                             \n"
  "{                                                                       \n"
  "  vec2 base = a_top_left + a_size * a_unit_pos;                         \n"
  "  vec2 center = a_top_left + a_size * vec2(0.5, 0.5);                   \n"
  "  vec2 centered = base - center;                                        \n"
  "  float c = cos(a_rotation);                                            \n"
  "  float s = sin(a_rotation);                                            \n"
  "  vec2 rotated = vec2(centered.x * c - centered.y * s,                  \n"
  "                      centered.x * s + centered.y * c);                 \n"
  "  vec2 local_pos = center + rotated;                                    \n"
  "  vec2 ndc = (local_pos / u_screen_size) * 2.0 - 1.0;                   \n"
  "  gl_Position = vec4(ndc.x, -ndc.y, 0.0, 1.0);                          \n"
  "  v_local_pos = a_unit_pos;                                             \n"
  "  v_box_size = a_size;                                                  \n"
  "  v_uv = mix(a_uv_min, a_uv_max, a_unit_pos);                           \n"
  "  v_color = vec4(float((a_color      ) & 0xFFu) / 255.0,                \n"
  "                 float((a_color >>  8) & 0xFFu) / 255.0,                \n"
  "                 float((a_color >> 16) & 0xFFu) / 255.0,                \n"
  "                 float((a_color >> 24) & 0xFFu) / 255.0);               \n"
  "  v_border_color = vec4(float((a_border_color      ) & 0xFFu) / 255.0,  \n"
  "                        float((a_border_color >>  8) & 0xFFu) / 255.0,  \n"
  "                        float((a_border_color >> 16) & 0xFFu) / 255.0,  \n"
  "                        float((a_border_color >> 24) & 0xFFu) / 255.0); \n"
  "  v_roundness = a_roundness;                                            \n"
  "  v_smoothness = a_smoothness;                                          \n"
  "  v_border_thickness = a_border_thickness;                              \n"
  "  v_texture_id = a_texture_id;                                          \n"
  "}                                                                       \n";

const char *sr2d_screen_fs_src =
  "#version 460 core                                                            \n"
  "                                                                             \n"
  "in vec2 v_uv;                                                                \n"
  "in vec4 v_color;                                                             \n"
  "in vec4 v_border_color;                                                      \n"
  "in vec2 v_local_pos;                                                         \n"
  "in vec2 v_box_size;                                                          \n"
  "in float v_roundness;                                                        \n"
  "in float v_smoothness;                                                       \n"
  "in float v_border_thickness;                                                 \n"
  "in flat uint v_texture_id;                                                   \n"
  "out vec4 frag_color;                                                         \n"
  "                                                                             \n"
  "uniform sampler2D u_textures[16];                                            \n"
  "                                                                             \n"
  "float rounded_box_sdf(vec2 p, vec2 b, float r)                               \n"
  "{                                                                            \n"
  "  vec2 q = abs(p - 0.5) - b * 0.5 + r;                                       \n"
  "  return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - r;                  \n"
  "}                                                                            \n"
  "                                                                             \n"
  "void main()                                                                  \n"
  "{                                                                            \n"
  "  vec2 p = v_local_pos;                                                      \n"
  "  vec2 box = vec2(1.0, 1.0);                                                 \n"
  "  float size_scale = max(min(v_box_size.x, v_box_size.y), 1.0);              \n"
  "  float radius = max(0.0, v_roundness) / size_scale;                         \n"
  "  float softness = max(0.0, v_smoothness) / size_scale;                      \n"
  "  float border = max(0.0, v_border_thickness) / size_scale;                  \n"
  "  float d = rounded_box_sdf(p, box, radius);                                 \n"
  "  float alpha = 1.0;                                                         \n"
  "  if (v_roundness > 0.0)                                                     \n"
  "  {                                                                          \n"
  "    alpha = 1.0 - smoothstep(-softness, softness, d);                        \n"
  "  }                                                                          \n"
  "  float border_alpha = 0.0;                                                  \n"
  "  if (v_border_thickness > 0.0)                                              \n"
  "  {                                                                          \n"
  "    vec2 border_box = box - vec2(border * 2.0);                              \n"
  "    float inner = rounded_box_sdf(p, border_box, max(0.0, radius - border)); \n"
  "    float inner_alpha = 1.0 - smoothstep(-softness, softness, inner);        \n"
  "    border_alpha = clamp(alpha - inner_alpha, 0.0, 1.0);                     \n"
  "    alpha = max(alpha, border_alpha);                                        \n"
  "  }                                                                          \n"
  "  vec4 tex = texture(u_textures[v_texture_id], v_uv);                        \n"
  "  vec4 base = v_color;                                                       \n"
  "  if (v_texture_id != 0u)                                                    \n"
  "  {                                                                          \n"
  "    base = tex * v_color;                                                    \n"
  "  }                                                                          \n"
  "  vec4 out_color = base;                                                     \n"
  "  if (v_border_thickness > 0.0)                                              \n"
  "  {                                                                          \n"
  "    out_color = mix(base, v_border_color, border_alpha);                     \n"
  "  }                                                                          \n"
  "  frag_color = vec4(out_color.rgb, out_color.a * alpha);                     \n"
  "}                                                                            \n";

// @Section: API

fz_internal void sr2d_init(s32 window_width, s32 window_height, b32 vsync);
fz_internal void sr2d_set_screen_size(s32 width, s32 height);
fz_internal void sr2d_end_frame(u32 window_width, u32 window_height, RGBA background_color);

fz_internal void sr2d_draw_quad(SR2D_Texture *texture, V2f32 top_left, V2f32 size, RGBA color);

fz_internal void               sr2d_draw_text(SR2D_Font *font, String text, V2f32 top_left, RGBA color);
fz_internal void               sr2d_draw_text_ext(SR2D_Font *font, String text, V2f32 top_left, RGBA color, f32 rotation, f32 roundness, f32 smoothness, f32 border_thickness, RGBA border_color);
fz_internal void               sr2d_draw_text_clamped(SR2D_Font *font, String text, V2f32 top_left, f32 max_width, RGBA color);
fz_internal void               sr2d_draw_text_params(SR2D_Text_Params params);
fz_internal SR2D_Text_Metrics   sr2d_measure_text(SR2D_Font *font, String text);
fz_internal SR2D_Texture_Atlas* sr2d_load_texture_atlas(Arena *arena, String path, String name);
fz_internal SR2D_Font*          sr2d__load_font(Arena *arena, String full_path);
fz_internal u32                sr2d_compile_shader_from_cstring(String name, const char* source, GLenum shader_type);
fz_internal u32                sr2d_compile_shader_from_string(String name, String *source, GLenum shader_type);
fz_internal V2f32              sr2d_center_text_in_rect(SR2D_Text_Metrics metrics, V2f32 top_left, V2f32 size);

fz_internal void sr2d_draw_quad_ext(SR2D_Texture *texture, V2f32 top_left, V2f32 size, RGBA color, f32 rotation, f32 roundness, f32 smoothness, f32 border_thickness, RGBA border_color);
fz_internal void sr2d_draw_quad_color(V2f32 top_left, V2f32 size, RGBA color);
fz_internal void sr2d_draw_quad_color_ext(V2f32 top_left, V2f32 size, RGBA color, f32 rotation, f32 roundness, f32 smoothness, f32 border_thickness, RGBA border_color);
fz_internal void sr2d_draw_quad_atlas_ext(SR2D_Texture_Atlas *texture_atlas, V2f32 uv_min, V2f32 uv_max, V2f32 top_left, V2f32 size, RGBA color, f32 rotation, f32 roundness, f32 smoothness, f32 border_thickness, RGBA border_color);
fz_internal SR2D_Text_Params sr2d_text_params_default();

fz_internal u32  _sr2d_pack_color(RGBA color);
fz_internal void _sr2d_draw_quad(SR2D_Texture_Atlas *texture_atlas, V2f32 top_left, V2f32 size, V2f32 uv_min, V2f32 uv_max, f32 rotation, u32 color, f32 roundness, f32 smoothness, f32 border_thickness, u32 border_color);

// @Section: Implementation

fz_internal void
sr2d_init(s32 window_width, s32 window_height, b32 vsync)
{
  Scratch scratch = scratch_begin(0, 0);

  opengl_init(vsync);

  memory_zero_struct(&SR2D_RenderContext);
  SR2D_RenderContext.arena = arena_alloc_sized(gigabytes(1), megabytes(64));

  SR2D_RenderContext.texture_atlas_capacity = R_MAX_TEXTURE_ATLAS;
  SR2D_RenderContext.texture_atlas          = push_array(SR2D_RenderContext.arena, SR2D_Texture_Atlas, SR2D_RenderContext.texture_atlas_capacity);

  SR2D_RenderContext.quads_capacity     = R_MAX_QUADS_COUNT;
  SR2D_RenderContext.screen_quads       = push_array(SR2D_RenderContext.arena, SR2D_Quad, SR2D_RenderContext.quads_capacity);
  SR2D_RenderContext.screen_quads_count = 0;
  SR2D_RenderContext.screen_width       = (u32)window_width;
  SR2D_RenderContext.screen_height      = (u32)window_height;

  // Compile shaders
  {
    SR2D_RenderContext.screen_program.vertex_program_handle   = sr2d_compile_shader_from_cstring(S("Screen VS"), sr2d_screen_vs_src, GL_VERTEX_SHADER);
    SR2D_RenderContext.screen_program.fragment_program_handle = sr2d_compile_shader_from_cstring(S("Quad FS"), sr2d_screen_fs_src, GL_FRAGMENT_SHADER);
    SR2D_RenderContext.screen_program.uniforms.screen_size    = glGetUniformLocation(SR2D_RenderContext.screen_program.vertex_program_handle, "u_screen_size");
    SR2D_RenderContext.screen_program.uniforms.time           = glGetUniformLocation(SR2D_RenderContext.screen_program.vertex_program_handle, "u_time");
    SR2D_RenderContext.screen_program.uniforms.texture_unit   = glGetUniformLocation(SR2D_RenderContext.screen_program.fragment_program_handle, "u_textures");
  }

  // Build program pipeline
  {
    glCreateProgramPipelines(1, &SR2D_RenderContext.screen_program.pipeline_handle);
    glUseProgramStages(SR2D_RenderContext.screen_program.pipeline_handle, GL_VERTEX_SHADER_BIT, SR2D_RenderContext.screen_program.vertex_program_handle);
    glUseProgramStages(SR2D_RenderContext.screen_program.pipeline_handle, GL_FRAGMENT_SHADER_BIT, SR2D_RenderContext.screen_program.fragment_program_handle);
  }

  // OpenGL state
  {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  // Geometry buffers (shared by every target's draw calls)
  {
    glCreateBuffers(1, &SR2D_RenderContext.unit_vbo);
    glNamedBufferStorage(SR2D_RenderContext.unit_vbo, sizeof(SR2D_UnitQuadVertices), SR2D_UnitQuadVertices, 0);

    glCreateBuffers(1, &SR2D_RenderContext.instance_vbo);
    glNamedBufferStorage(SR2D_RenderContext.instance_vbo, sizeof(SR2D_Quad) * R_MAX_QUADS_COUNT, NULL, GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &SR2D_RenderContext.vao);

    glCreateBuffers(1, &SR2D_RenderContext.ebo);
    glNamedBufferStorage(SR2D_RenderContext.ebo, sizeof(SR2D_UnitQuadIndices), SR2D_UnitQuadIndices, 0);
    glVertexArrayElementBuffer(SR2D_RenderContext.vao, SR2D_RenderContext.ebo);

    // Binding 0 – unit quad positions (per-vertex)
    glVertexArrayVertexBuffer(SR2D_RenderContext.vao, 0, SR2D_RenderContext.unit_vbo, 0, sizeof(V2f32));

    // Binding 1 – instance data (per-instance)
    glVertexArrayVertexBuffer(SR2D_RenderContext.vao, 1, SR2D_RenderContext.instance_vbo, 0, sizeof(SR2D_Quad));

    // Attrib 0 – a_unit_pos (per-vertex)
    glEnableVertexArrayAttrib(SR2D_RenderContext.vao, 0);
    glVertexArrayAttribFormat(SR2D_RenderContext.vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(SR2D_RenderContext.vao, 0, 0);

    // Per-instance attribs
    glEnableVertexArrayAttrib(SR2D_RenderContext.vao, 1); // top_left
    glVertexArrayAttribFormat(SR2D_RenderContext.vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(SR2D_Quad, top_left));
    glVertexArrayAttribBinding(SR2D_RenderContext.vao, 1, 1);
    glVertexArrayBindingDivisor(SR2D_RenderContext.vao, 1, 1);

    glEnableVertexArrayAttrib(SR2D_RenderContext.vao, 2); // size
    glVertexArrayAttribFormat(SR2D_RenderContext.vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(SR2D_Quad, size));
    glVertexArrayAttribBinding(SR2D_RenderContext.vao, 2, 1);
    glVertexArrayBindingDivisor(SR2D_RenderContext.vao, 2, 1);

    glEnableVertexArrayAttrib(SR2D_RenderContext.vao, 3); // uv_min
    glVertexArrayAttribFormat(SR2D_RenderContext.vao, 3, 2, GL_FLOAT, GL_FALSE, offsetof(SR2D_Quad, uv_min));
    glVertexArrayAttribBinding(SR2D_RenderContext.vao, 3, 1);
    glVertexArrayBindingDivisor(SR2D_RenderContext.vao, 3, 1);

    glEnableVertexArrayAttrib(SR2D_RenderContext.vao, 4); // uv_max
    glVertexArrayAttribFormat(SR2D_RenderContext.vao, 4, 2, GL_FLOAT, GL_FALSE, offsetof(SR2D_Quad, uv_max));
    glVertexArrayAttribBinding(SR2D_RenderContext.vao, 4, 1);
    glVertexArrayBindingDivisor(SR2D_RenderContext.vao, 4, 1);

    glEnableVertexArrayAttrib(SR2D_RenderContext.vao, 5); // color
    glVertexArrayAttribIFormat(SR2D_RenderContext.vao, 5, 1, GL_UNSIGNED_INT, offsetof(SR2D_Quad, color));
    glVertexArrayAttribBinding(SR2D_RenderContext.vao, 5, 1);
    glVertexArrayBindingDivisor(SR2D_RenderContext.vao, 5, 1);

    glEnableVertexArrayAttrib(SR2D_RenderContext.vao, 6); // texture_id
    glVertexArrayAttribIFormat(SR2D_RenderContext.vao, 6, 1, GL_UNSIGNED_INT, offsetof(SR2D_Quad, texture_id));
    glVertexArrayAttribBinding(SR2D_RenderContext.vao, 6, 1);
    glVertexArrayBindingDivisor(SR2D_RenderContext.vao, 6, 1);

    glEnableVertexArrayAttrib(SR2D_RenderContext.vao, 7); // rotation
    glVertexArrayAttribFormat(SR2D_RenderContext.vao, 7, 1, GL_FLOAT, GL_FALSE, offsetof(SR2D_Quad, rotation));
    glVertexArrayAttribBinding(SR2D_RenderContext.vao, 7, 1);
    glVertexArrayBindingDivisor(SR2D_RenderContext.vao, 7, 1);

    glEnableVertexArrayAttrib(SR2D_RenderContext.vao, 8); // roundness
    glVertexArrayAttribFormat(SR2D_RenderContext.vao, 8, 1, GL_FLOAT, GL_FALSE, offsetof(SR2D_Quad, roundness));
    glVertexArrayAttribBinding(SR2D_RenderContext.vao, 8, 1);
    glVertexArrayBindingDivisor(SR2D_RenderContext.vao, 8, 1);

    glEnableVertexArrayAttrib(SR2D_RenderContext.vao, 9); // smoothness
    glVertexArrayAttribFormat(SR2D_RenderContext.vao, 9, 1, GL_FLOAT, GL_FALSE, offsetof(SR2D_Quad, smoothness));
    glVertexArrayAttribBinding(SR2D_RenderContext.vao, 9, 1);
    glVertexArrayBindingDivisor(SR2D_RenderContext.vao, 9, 1);

    glEnableVertexArrayAttrib(SR2D_RenderContext.vao, 10); // border_thickness
    glVertexArrayAttribFormat(SR2D_RenderContext.vao, 10, 1, GL_FLOAT, GL_FALSE, offsetof(SR2D_Quad, border_thickness));
    glVertexArrayAttribBinding(SR2D_RenderContext.vao, 10, 1);
    glVertexArrayBindingDivisor(SR2D_RenderContext.vao, 10, 1);

    glEnableVertexArrayAttrib(SR2D_RenderContext.vao, 11); // border_color
    glVertexArrayAttribIFormat(SR2D_RenderContext.vao, 11, 1, GL_UNSIGNED_INT, offsetof(SR2D_Quad, border_color));
    glVertexArrayAttribBinding(SR2D_RenderContext.vao, 11, 1);
    glVertexArrayBindingDivisor(SR2D_RenderContext.vao, 11, 1);
  }

  // GPU timer queries (double-buffered)
  glGenQueries(2, SR2D_RenderContext.gpu_timer_queries);

  // White 1×1 texture atlas
  {
    GLuint tex;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
    glTextureStorage2D(tex, 1, GL_RGBA8, 1, 1);
    u8 white[4] = { 255, 255, 255, 255 };
    glTextureSubImage2D(tex, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, white);
    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    SR2D_Texture_Atlas *white_texture = &SR2D_RenderContext.texture_atlas[SR2D_RenderContext.texture_atlas_count];
    white_texture->name   = S("White Texture");
    white_texture->index  = SR2D_RenderContext.texture_atlas_count;
    white_texture->handle = tex;
    white_texture->width  = 1;
    white_texture->height = 1;
    SR2D_RenderContext.white_texture = white_texture;
    glBindTextureUnit(SR2D_RenderContext.texture_atlas_count, tex);
    SR2D_RenderContext.texture_atlas_count += 1;
  }

  // Fonts
  {
    SR2D_RenderContext.fonts_capacity = FONT_CAPACITY;
    SR2D_RenderContext.fonts          = push_array(SR2D_RenderContext.arena, SR2D_Font, SR2D_RenderContext.fonts_capacity);
  }

  scratch_end(&scratch);
}

fz_internal void
sr2d_set_screen_size(s32 width, s32 height)
{
  if (width < 1) width = 1;
  if (height < 1) height = 1;

  SR2D_RenderContext.screen_width  = (u32)width;
  SR2D_RenderContext.screen_height = (u32)height;
}

fz_internal void
sr2d_end_frame(u32 window_width, u32 window_height, RGBA background_color)
{
#if DEBUG
  // GPU timer readback (one frame behind to avoid stall)
  {
    u32 read_slot  =  SR2D_RenderContext.gpu_timer_frame & 1;
    u32 write_slot = read_slot ^ 1;

    if (SR2D_RenderContext.gpu_timer_frame >= 2)
    {
      GLuint64 elapsed_ns = 0;
      glGetQueryObjectui64v(SR2D_RenderContext.gpu_timer_queries[read_slot], GL_QUERY_RESULT, &elapsed_ns);
      SR2D_RenderContext.previous_frame_stats.gpu_time_ms = (f32)(elapsed_ns / 1000000.0);
    }

    glBeginQuery(GL_TIME_ELAPSED, SR2D_RenderContext.gpu_timer_queries[write_slot]);
  }

  SR2D_RenderContext.previous_frame_stats.draw_calls  = 0;
  SR2D_RenderContext.previous_frame_stats.quads_drawn = 0;
#endif

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, (s32)SR2D_RenderContext.screen_width, (s32)SR2D_RenderContext.screen_height);
  glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  if (SR2D_RenderContext.screen_quads_count > 0)
  {
    glBindVertexArray(SR2D_RenderContext.vao);
    glBindProgramPipeline(SR2D_RenderContext.screen_program.pipeline_handle);

    if (SR2D_RenderContext.screen_program.uniforms.time >= 0)
    {
      glProgramUniform1f(SR2D_RenderContext.screen_program.vertex_program_handle, SR2D_RenderContext.screen_program.uniforms.time, SR2D_RenderContext.time);
    }
    if (SR2D_RenderContext.screen_program.uniforms.screen_size >= 0)
    {
      glProgramUniform2f(SR2D_RenderContext.screen_program.vertex_program_handle, SR2D_RenderContext.screen_program.uniforms.screen_size, (f32)SR2D_RenderContext.screen_width, (f32)SR2D_RenderContext.screen_height);
    }
    if (SR2D_RenderContext.screen_program.uniforms.texture_unit >= 0)
    {
      GLint texture_units[16];
      for (u32 i = 0; i < 16; i += 1)
      {
        texture_units[i] = (GLint)i;
      }
      glProgramUniform1iv(SR2D_RenderContext.screen_program.fragment_program_handle, SR2D_RenderContext.screen_program.uniforms.texture_unit, 16, texture_units);
    }

    glNamedBufferSubData(SR2D_RenderContext.instance_vbo, 0, SR2D_RenderContext.screen_quads_count * sizeof(SR2D_Quad), SR2D_RenderContext.screen_quads);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, (s32)SR2D_RenderContext.screen_quads_count);

#if DEBUG
    SR2D_RenderContext.previous_frame_stats.draw_calls  += 1;
    SR2D_RenderContext.previous_frame_stats.quads_drawn += SR2D_RenderContext.screen_quads_count;
#endif
  }

#if DEBUG
  glEndQuery(GL_TIME_ELAPSED);
  SR2D_RenderContext.gpu_timer_frame += 1;
#endif

  SR2D_RenderContext.screen_quads_count = 0;
  window_swap_buffers();
}

fz_internal u32
_sr2d_pack_color(RGBA color)
{
  f32 r = color.r;
  f32 g = color.g;
  f32 b = color.b;
  f32 a = color.a;

  if (r < 0.0f) r = 0.0f; else if (r > 1.0f) r = 1.0f;
  if (g < 0.0f) g = 0.0f; else if (g > 1.0f) g = 1.0f;
  if (b < 0.0f) b = 0.0f; else if (b > 1.0f) b = 1.0f;
  if (a < 0.0f) a = 0.0f; else if (a > 1.0f) a = 1.0f;

  u8 ru = (u8)(r * 255.0f);
  u8 gu = (u8)(g * 255.0f);
  u8 bu = (u8)(b * 255.0f);
  u8 au = (u8)(a * 255.0f);
  return ((u32)au << 24) | ((u32)bu << 16) | ((u32)gu << 8) | ru;
}

fz_internal void
_sr2d_draw_quad(SR2D_Texture_Atlas *texture_atlas, V2f32 top_left, V2f32 size, V2f32 uv_min, V2f32 uv_max, f32 rotation, u32 color, f32 roundness, f32 smoothness, f32 border_thickness, u32 border_color)
{
  if (SR2D_RenderContext.screen_quads_count >= SR2D_RenderContext.quads_capacity)
  {
    sr2d_error(S("Render quad capacity exceeded"));
    return;
  }

  SR2D_Quad *quad = &SR2D_RenderContext.screen_quads[SR2D_RenderContext.screen_quads_count++];
  quad->top_left         = top_left;
  quad->size             = size;
  quad->uv_min           = uv_min;
  quad->uv_max           = uv_max;
  quad->color            = color;
  quad->texture_id       = texture_atlas->index;
  quad->rotation         = rotation;
  quad->roundness        = roundness;
  quad->smoothness       = smoothness;
  quad->border_thickness = border_thickness;
  quad->border_color     = border_color;
}

fz_internal void
sr2d_draw_quad(SR2D_Texture *texture, V2f32 top_left, V2f32 size, RGBA color)
{
  _sr2d_draw_quad(texture->atlas, top_left, size, texture->uv_min, texture->uv_max, 0, _sr2d_pack_color(color), 0, 0, 0, COLOR_PACKED_CLEAR.v);
}

fz_internal void
sr2d_draw_quad_ext(SR2D_Texture *texture, V2f32 top_left, V2f32 size, RGBA color, f32 rotation, f32 roundness, f32 smoothness, f32 border_thickness, RGBA border_color)
{
  _sr2d_draw_quad(texture->atlas, top_left, size, texture->uv_min, texture->uv_max, rotation, _sr2d_pack_color(color), roundness, smoothness, border_thickness, _sr2d_pack_color(border_color));
}

fz_internal void
sr2d_draw_quad_color(V2f32 top_left, V2f32 size, RGBA color)
{
  _sr2d_draw_quad(SR2D_RenderContext.white_texture, top_left, size, v2f32(0.0f, 0.0f), v2f32(1.0f, 1.0f), 0, _sr2d_pack_color(color), 0, 0, 0, COLOR_PACKED_CLEAR.v);
}

fz_internal void
sr2d_draw_quad_color_ext(V2f32 top_left, V2f32 size, RGBA color, f32 rotation, f32 roundness, f32 smoothness, f32 border_thickness, RGBA border_color)
{
  _sr2d_draw_quad(SR2D_RenderContext.white_texture, top_left, size, v2f32(0.0f, 0.0f), v2f32(1.0f, 1.0f), rotation, _sr2d_pack_color(color), roundness, smoothness, border_thickness, _sr2d_pack_color(border_color));
}

fz_internal void
sr2d_draw_quad_atlas_ext(SR2D_Texture_Atlas *texture_atlas, V2f32 uv_min, V2f32 uv_max, V2f32 top_left, V2f32 size, RGBA color, f32 rotation, f32 roundness, f32 smoothness, f32 border_thickness, RGBA border_color)
{
  _sr2d_draw_quad(texture_atlas, top_left, size, uv_min, uv_max, rotation, _sr2d_pack_color(color), roundness, smoothness, border_thickness, _sr2d_pack_color(border_color));
}

fz_internal SR2D_Text_Params
sr2d_text_params_default()
{
  SR2D_Text_Params params;
  memory_zero_struct(&params);
  params.text             = S("");
  params.top_left         = v2f32(0.0f, 0.0f);
  params.color            = rgba(0.0f, 0.0f, 0.0f, 1.0f);
  params.border_color     = rgba(0.0f, 0.0f, 0.0f, 0.0f);
  params.max_width        = F32_MAX;
  params.max_height       = F32_MAX;
  params.line_spacing     = 1.0f;
  return params;
}

fz_internal void
sr2d_draw_text(SR2D_Font *font, String text, V2f32 top_left, RGBA color)
{
  sr2d_draw_text_ext(font, text, top_left, color, 0, 0, 0, 0, rgba(0.0f, 0.0f, 0.0f, 0.0f));
}

fz_internal void
sr2d_draw_text_ext(SR2D_Font *font, String text, V2f32 top_left, RGBA color, f32 rotation, f32 roundness, f32 smoothness, f32 border_thickness, RGBA border_color)
{
  SR2D_Text_Params p = sr2d_text_params_default();
  p.font             = font;
  p.text             = text;
  p.top_left         = top_left;
  p.color            = color;
  p.rotation         = rotation;
  p.roundness        = roundness;
  p.smoothness       = smoothness;
  p.border_thickness = border_thickness;
  p.border_color     = border_color;
  sr2d_draw_text_params(p);
}

fz_internal void
sr2d_draw_text_clamped(SR2D_Font *font, String text, V2f32 top_left, f32 max_width, RGBA color)
{
  SR2D_Text_Params p = sr2d_text_params_default();
  p.font      = font;
  p.text      = text;
  p.top_left  = top_left;
  p.max_width = max_width;
  p.color     = color;
  sr2d_draw_text_params(p);
}

fz_internal void
sr2d_draw_text_params(SR2D_Text_Params params)
{
  if (params.text.count == 0)    
  {
    return;
  }
  if (params.font == NULL)
  {
    return;
  }

  SR2D_Font *font    = params.font;
  Scratch scratch = scratch_begin(0, 0);
  String_List lines = string_split(scratch.arena, params.text, S("\n"));

  V2f32 cursor = params.top_left;
  cursor.y += font->ascent;

  for (String_Node *node = lines.first; node; node = node->next)
  {
    cursor.x = params.top_left.x;
    String line = node->value;

    for (u64 i = 0; i < line.count; i += 1)
    {
      u8 c = line.cstring[i];
      if (c < 32 || c > 126)
      {
        continue;
      }

      stbtt_aligned_quad quad;
      stbtt_GetPackedQuad(font->char_data, font->atlas->width, font->atlas->height, c - 32, &cursor.x, &cursor.y, &quad, 0);

      V2f32 uv_min   = v2f32(quad.s0, quad.t0);
      V2f32 uv_max   = v2f32(quad.s1, quad.t1);
      V2f32 top_left = v2f32(quad.x0, quad.y0);
      V2f32 size     = v2f32(quad.x1 - quad.x0, quad.y1 - quad.y0);

      if (params.max_width > 0)
      {
        f32 right_edge = top_left.x + size.x;
        f32 max_x      = params.top_left.x + params.max_width;
        if (top_left.x >= max_x)
        {
          continue;
        }
        if (right_edge > max_x)
        {
          f32 visible  = max_x - top_left.x;
          f32 uv_range = uv_max.x - uv_min.x;
          uv_max.x     = uv_min.x + uv_range * (visible / size.x);
          size.x       = visible;
        }
      }

      if (params.max_height > 0)
      {
        f32 bottom_edge = top_left.y + size.y;
        f32 max_y       = params.top_left.y + params.max_height;
        if (top_left.y >= max_y) continue;
        if (bottom_edge > max_y)
        {
          f32 visible  = max_y - top_left.y;
          f32 uv_range = uv_max.y - uv_min.y;
          uv_max.y     = uv_min.y + uv_range * (visible / size.y);
          size.y       = visible;
        }
      }

      sr2d_draw_quad_atlas_ext(font->atlas, uv_min, uv_max, top_left, size, params.color, params.rotation, params.roundness, params.smoothness, params.border_thickness, params.border_color);
    }

    cursor.y += font->line_height;

    if (params.max_height > 0 && cursor.y - font->ascent >= params.top_left.y + params.max_height)
    {
      break;
    }
  }

  scratch_end(&scratch);
}

fz_internal SR2D_Text_Metrics
sr2d_measure_text(SR2D_Font *font, String text)
{
  SR2D_Text_Metrics result;
  memory_zero_struct(&result);
  if (text.count == 0)
  {
    return result;
  }

  f32 x                  = 0.0f;
  f32 current_line_width = 0.0f;
  f32 max_line_width     = 0.0f;
  u64 line_count         = 1;

  for (u64 i = 0; i < text.count; i += 1)
  {
    u8 c = text.cstring[i];
    if (c == '\n')
    {
      if (current_line_width > max_line_width) max_line_width = current_line_width;
      current_line_width = 0.0f;
      x = 0.0f;
      line_count += 1;
      continue;
    }
    if (c < 32 || c > 126)
    {
      continue;
    }

    stbtt_aligned_quad q;
    f32 y = 0.0f;
    stbtt_GetPackedQuad(font->char_data, font->atlas->width, font->atlas->height, c - 32, &x, &y, &q, 0);
    current_line_width = x;
  }

  if (current_line_width > max_line_width)
  {
    max_line_width = current_line_width;
  }

  result.width           = max_line_width;
  result.height          = font->line_height * (f32)line_count;
  result.line_count      = line_count;
  result.last_line_width = current_line_width;
  return result;
}

fz_internal SR2D_Texture_Atlas *
sr2d_load_texture_atlas(Arena *arena, String path, String name)
{
  stbi_set_flip_vertically_on_load(1);
  s32 width, height, channels;
  u8 *data = stbi_load((const char *)path.cstring, &width, &height, &channels, 4);
  if (!data || width <= 0 || height <= 0)
  {
    Scratch scratch = scratch_begin(0, 0);
    sr2d_error(Sf(scratch.arena, "Failed to load texture: %s", path.cstring));
    scratch_end(&scratch);
    return NULL;
  }

  GLuint tex;
  glCreateTextures(GL_TEXTURE_2D, 1, &tex);
  glTextureStorage2D(tex, 1, GL_RGBA8, width, height);
  glTextureSubImage2D(tex, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  SR2D_Texture_Atlas *result = &SR2D_RenderContext.texture_atlas[SR2D_RenderContext.texture_atlas_count];
  result->data   = data;
  result->name   = string_copy(arena, name);
  result->index  = SR2D_RenderContext.texture_atlas_count;
  result->handle = tex;
  result->width  = width;
  result->height = height;

  glBindTextureUnit(SR2D_RenderContext.texture_atlas_count, tex);
  SR2D_RenderContext.texture_atlas_count += 1;

  return result;
}

fz_internal SR2D_Font *
sr2d__load_font(Arena *arena, String full_path)
{
  Scratch scratch = scratch_begin(0, 0);

  String file_data = file_load(scratch.arena, full_path);

  if (file_data.count == 0)
  {
    sr2d_error(Sf(scratch.arena, "Error loading font: " S_FMT, full_path.count, full_path.cstring));
    scratch_end(&scratch);
    return NULL;
  }

  stbtt_fontinfo font_info;
  if (!stbtt_InitFont(&font_info, (const unsigned char *)file_data.cstring, 0))
  {
    sr2d_error(S("stbtt_InitFont failed"));
    scratch_end(&scratch);
    return NULL;
  }

  s32 ascent, descent, line_gap;
  stbtt_GetFontVMetrics(&font_info, &ascent, &descent, &line_gap);
  f32 scale = stbtt_ScaleForMappingEmToPixels(&font_info, FONT_HEIGHT);

  s32 atlas_w = 512, atlas_h = 512;
  u8 *atlas_bitmap = push_array(scratch.arena, u8, atlas_w * atlas_h * 4);
  u8 *temp_bitmap  = push_array(scratch.arena, u8, atlas_w * atlas_h);

  stbtt_packedchar char_data[MAX_FONT_GLYPHS];
  stbtt_pack_context pack;
  stbtt_PackBegin(&pack, temp_bitmap, atlas_w, atlas_h, atlas_w, 1, NULL);
  stbtt_PackSetOversampling(&pack, 1, 1);
  stbtt_PackFontRange(&pack, (const unsigned char *)file_data.cstring, 0, FONT_HEIGHT, 32, MAX_FONT_GLYPHS, char_data);
  stbtt_PackEnd(&pack);

  for (s32 y = 0; y < atlas_h; y += 1)
  {
    for (s32 x = 0; x < atlas_w; x += 1)
    {
      u8  a = temp_bitmap[y * atlas_w + x];
      u8 *p = &atlas_bitmap[(y * atlas_w + x) * 4];
      p[0] = 255; p[1] = 255; p[2] = 255; p[3] = a;
    }
  }

  GLuint tex;
  glCreateTextures(GL_TEXTURE_2D, 1, &tex);
  glTextureStorage2D(tex, 1, GL_RGBA8, atlas_w, atlas_h);
  glTextureSubImage2D(tex, 0, 0, 0, atlas_w, atlas_h, GL_RGBA, GL_UNSIGNED_BYTE, atlas_bitmap);
  glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  u64 idx = 0;
  string_find_last(full_path, S("/"), &idx);
  String file_name = string_substring(arena, full_path, idx + 1, full_path.count);
  string_find_last(file_name, S("."), &idx);
  file_name = string_substring(arena, file_name, 0, idx);

  SR2D_Texture_Atlas *atlas = &SR2D_RenderContext.texture_atlas[SR2D_RenderContext.texture_atlas_count];
  atlas->name   = string_copy(arena, file_name);
  atlas->index  = SR2D_RenderContext.texture_atlas_count;
  atlas->handle = tex;
  atlas->width  = atlas_w;
  atlas->height = atlas_h;

  glBindTextureUnit(atlas->index, tex);
  SR2D_RenderContext.texture_atlas_count += 1;

  SR2D_Font *font   = &SR2D_RenderContext.fonts[SR2D_RenderContext.fonts_count];
  font->atlas     = atlas;
  font->char_data = push_array(arena, stbtt_packedchar, MAX_FONT_GLYPHS);
  memory_copy(font->char_data, char_data, sizeof(char_data));
  font->height      = FONT_HEIGHT;
  font->ascent      = ascent  * scale;
  font->descent     = descent * scale;
  font->line_gap    = line_gap * scale;
  font->line_height = (ascent - descent + line_gap) * scale;
  SR2D_RenderContext.fonts_count += 1;

  scratch_end(&scratch);
  return font;
}

fz_internal u32
sr2d_compile_shader_from_cstring(String name, const char* source, GLenum shader_type)
{
  if (!shader_type)
  {
    sr2d_error(S("Invalid shader type"));
    return 0;
  }
  if (strlen(source) == 0)
  {
    sr2d_error(S("Empty shader source"));
    return 0;
  }

  Scratch scratch = scratch_begin(0, 0);

  u32 program = glCreateShaderProgramv(shader_type, 1, &source);
  opengl_check_errors();

  if (program == 0)
  {
    sr2d_error(S("glCreateShaderProgramv returned 0"));
  }

  GLint link_status = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &link_status);
  opengl_check_errors();

  if (!link_status)
  {
    GLint log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    GLchar *log = push_array(scratch.arena, GLchar, log_length + 1);
    glGetProgramInfoLog(program, log_length, 0, log);
    sr2d_error(Sf(scratch.arena, "Shader %s compile/link failed:\n%s", name.cstring, log));
  }

  scratch_end(&scratch);
  return program;
}

fz_internal u32
sr2d_compile_shader_from_string(String name, String *source, GLenum shader_type)
{
  if (!source->cstring || source->count == 0)
  {
    sr2d_error(S("Empty shader source"));
    return 0;
  }
  return sr2d_compile_shader_from_cstring(name, (const char*)source->cstring, shader_type);
}

fz_internal V2f32
sr2d_center_text_in_rect(SR2D_Text_Metrics metrics, V2f32 top_left, V2f32 size)
{
  V2f32 result;
  result.x = top_left.x + (size.x - metrics.width) * 0.5f;
  result.y = top_left.y + (size.y - metrics.height) * 0.5f;
  return result;
}

#if DEBUG
fz_internal_no_reflection void inline
_sr2d_error(String message, String file, u32 line)
{
  Scratch scratch = scratch_begin(0, 0);
  String body = Sf(scratch.arena, "Renderer Error\n" S_FMT "\n\nat " S_FMT ":%u", S_ARG(message), S_ARG(file), line);
  message_box(S("Renderer Error"), body, file, line);
  raddbg_break();
  scratch_end(&scratch);
  assert(false);
}
#endif // DEBUG

#endif // FZ_SIMPLE_RENDERER2D_H
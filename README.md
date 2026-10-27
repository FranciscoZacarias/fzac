# fzac

Personal C library. Include one base header, then opt into modules.

- `Base.h`: fzac's own platform layer (Window, OpenGL, Simple_Renderer2D...).
- `Base_Raylib.h`: same base, with raylib as the platform, graphics, input, audio and math layer.

Both share arenas, strings, arrays, math (raymath), Os and the `entry_point` convention.

# Example: Base_Raylib.h

```c
#include "Base_Raylib.h"
#include "Logging.h"
#include "Hash_Map.h"

#include "metaprogram/global_headers.h"

fz_internal void
entry_point(Command_Line* command_line)
{
  logging_init(Log_Level_Info, S("app.log"), true); // raylib logs go through Logging too

  InitWindow(1280, 720, "app");
  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Hello", 10, 10, 20, DARKGRAY);
    EndDrawing();
    clear_temporary_storage();
  }
  CloseWindow();
}
```

# Example: Base.h

```c
#include "Base.h"
#include "Window.h"
#include "OpenGL.h"

#include "metaprogram/global_headers.h"

fz_internal void
entry_point(Command_Line* command_line)
{
  console_attach();
  window_create(S("app"), 1280, 720, 30, 30);
  opengl_init(true);

  while (!is_key_clicked(Keyboard_Key_ESCAPE))
  {
    window_update_events();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    window_swap_buffers();
    clear_temporary_storage();
  }
}
```

# Building

Copy `templates\build.bat` next to your `src\` folder, with fzac at `src\fzac`. Set `USE_RAYLIB=1` for Base_Raylib projects; raylib.lib is built automatically the first time.

```
build              debug build, runs the metaprogram first
build release      optimized build
build all          rebuild metaprogram.exe, then the app
build metaprogram  only rebuild metaprogram.exe
build profile      defines USE_PROFILER
build raylib       force a rebuild of raylib.lib
```

Expected layout:

```
build.bat
src\entry_point.c
src\metaprogram\metaprogram_entry_point.c
src\fzac\
```

# Metaprogram

Generates forward declarations for every `fz_internal` function, struct and enum in `src\` into `src\metaprogram\global_headers.h`, so nothing needs declaring in order.

```c
// metaprogram_entry_point.c
#include "Base.h"

fz_internal void
metaprogram_entry_point(Default_Metaprogram *dm, Command_Line *command_line, String project_path, b32 *run_default_metaprogram, String *global_headers_extra_data)
{
}
```

Enums are forward declared through `enum_type`. The third argument is `1` to generate a `<name>_to_string` array, `0` otherwise:

```c
enum_type(My_Enum, u32, 1)
{
  My_Enum_None = 0,
  My_Enum_One,
};
```

# Base

- `Core`, `Context_Cracking`: types, macros, platform detection.
- `Memory`, `Arena`, `Allocator`, `Thread_Context`: arenas, scratch and temporary memory.
- `Array`: `Static_Array(T)` (fixed capacity) and `Dynamic_Array(T)` (allocator backed).
- `String`: count based strings and string lists.
- `Math`: raymath plus `V2s32`.
- `Os`: message boxes, console, `fatal_error`.
- `Custom_Entry_Point`: calls the user's `entry_point`.

```c
Static_Array(Entity) entities;
static_array_init_arena(&entities, arena, 128);
static_array_add(&entities, entity);

Dynamic_Array(u32) ids;
dynamic_array_init(&ids);
dynamic_array_add(&ids, 42);
dynamic_array_free(&ids);
```

# Modules

Usable with both bases:
- `Hash_Map`: arena backed map with u64 or String keys.
- `String_Builder`: append strings into a caller's arena.
- `Logging`: leveled logs to terminal, file and hooks.
- `Files`, `Timing`, `Command_Line`, `Lexer`, `Profiler`, `Code_Generation`, `List_Todos`.

```c
Hash_Map map = hash_map_init(arena, 64, sizeof(Entity));
hash_map_put_string(&map, S("player"), &player);
Entity* found = hash_map_get_string(&map, S("player"));
```

Base.h only:
- `Window`, `Platform`, `OpenGL`, `Vulkan`, `Simple_Renderer2D`, `Camera2D`, `Camera3D`, `Art`, `Geometry`, `UI`, `Immediate_UI`.

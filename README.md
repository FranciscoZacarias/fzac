# Structure
Include `Base.h` to include all the `base\` headers in the project. These are the  bare minimum functionality for the library which are to be included in each project.
For `modules\` each file can be included individually into the program as a single module.

# Example

```c
#include "Base.h"
#include "Window.h"
#include "OpenGL.h"

global Window *gWindow = NULL;
global Arena *gArena   = NULL;
global b8 gShouldQuit  = false;

global u32 TriangleVAO = 0;
global u32 TriangleVBO = 0;

global u32 TriangleVSProgram = 0;
global u32 TriangleFSProgram = 0;
global u32 TrianglePipeline  = 0;

// Opengl setup
global f32 vertices[] = {
  // pos         // color
  0.0f,  0.7f,   1.0f, 0.0f, 0.0f,
 -0.7f, -0.7f,   0.0f, 1.0f, 0.0f,
  0.7f, -0.7f,   0.0f, 0.0f, 1.0f,
};

global const char *TriangleVS =
  "#version 460 core                         \n"
  "layout(location=0) in vec2 a_pos;         \n"
  "layout(location=1) in vec3 a_color;       \n"
  "out vec3 v_color;                         \n"
  "out                                       \n"
  "gl_PerVertex                              \n"
  "{                                         \n"
  "  vec4 gl_Position;                       \n"
  "};                                        \n"
  "void main()                               \n"
  "{                                         \n"
  "  gl_Position = vec4(a_pos,0.0,1.0);      \n"
  "  v_color = a_color;                      \n"
  "}                                         \n";

global const char *TriangleFS =
  "#version 460 core                         \n"
  "in vec3 v_color;                          \n"
  "out vec4 frag_color;                      \n"
  "void main()                               \n"
  "{                                         \n"
  "  frag_color = vec4(v_color,1.0);         \n"
  "}                                         \n";

function void
handle_input()
{
  if (input_is_key_clicked(Keyboard_Key_ESCAPE))
  {
    gShouldQuit = true;
  }
}

function void
entry_point(Command_Line *command_line)
{ 
  console_attach();
  gWindow = window_create(NULL, S("OpenGL Example "), 600, 400, 30, 30);
  opengl_init(gWindow);

  // Opengl Setup
  {
    TriangleVSProgram = glCreateShaderProgramv(GL_VERTEX_SHADER,   1, &TriangleVS);
    TriangleFSProgram = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &TriangleFS);

    glCreateProgramPipelines(1, &TrianglePipeline);
    glUseProgramStages(TrianglePipeline, GL_VERTEX_SHADER_BIT,   TriangleVSProgram);
    glUseProgramStages(TrianglePipeline, GL_FRAGMENT_SHADER_BIT, TriangleFSProgram);

    glCreateBuffers(1, &TriangleVBO);
    glNamedBufferStorage(TriangleVBO, sizeof(vertices), vertices, 0);

    glCreateVertexArrays(1, &TriangleVAO);

    glVertexArrayVertexBuffer(TriangleVAO, 0, TriangleVBO, 0, sizeof(f32)*5);

    glEnableVertexArrayAttrib(TriangleVAO, 0);
    glVertexArrayAttribFormat(TriangleVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(TriangleVAO, 0, 0);

    glEnableVertexArrayAttrib(TriangleVAO, 1);
    glVertexArrayAttribFormat(TriangleVAO, 1, 3, GL_FLOAT, GL_FALSE, sizeof(f32)*2);
    glVertexArrayAttribBinding(TriangleVAO, 1, 0);
  }


  while (!gShouldQuit)
  {
    window_update_events();
    handle_input();

    // Opengl Draw
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(0,0,gWindow->width,gWindow->height);
      glClearColor(0.1f,0.1f,0.1f,1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      glBindProgramPipeline(TrianglePipeline);
      glBindVertexArray(TriangleVAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    window_swap_buffers(gWindow);
    clear_temporary_storage();
  }
}
```

# Structure
- `base\`: Core library.
- `modules\`: Opt-In modules provided by the library.
- `Extern\`: Opt-in external code imported into this library.

> `Opt-in` means that it has to be explicitly included by the user

# Base

The base layer includes:
- `Math`: Linear math functions
- `Allocators`: Custom allocators
  - Arena Allocator
  - Pool Allocator
  - Scratch memory functions for short lived memory.
- `Context_Cracking`: Parses platform (OS, Compiler, CPU Arch ... ) into macros.
- `Core`: Defines data types and some helper macros
- `Os`: Operating system specific core functions
- `Custom_Entry_Point`: Defines the entry_point function that has to be implemented by the user. 
- `Memory`: OS Dependent implementation of memory functions (Reserve, Free, etc... ).
- `String`: Immutable count based strings.
- `Thread_Context`: Implementes a thread context that includes temporary memory.

# Modules

- `OpenGL`: Opengl macros, bindings and helpers.
- `Platform`: Generic platform functions.
- `Window`: Window and Input implementation.
- `Art`: Colors and color related functions.
- `Camera2D`: Implementation for 2D camera.
- `Code_Generation`: Code generation module that powers all `.cgen` files.
- `String_Builder`: String builder 
- `Command_Line`: Command line parser
- `Files`: File related functions
- `Introspection`: Parses `my style of C code` and stores it in a structured way.
- `Lexer`: Lexer implementation
- `Timing`: Timing functions
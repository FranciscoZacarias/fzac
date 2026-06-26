#ifndef OPENGL_H
#define OPENGL_H

// Opengl Debug:
// Define DEBUG_OPENGL_CHECK_ERRORS to call opengl_check_errors() after EVERY gl function call
// Define DEBUG to enable debug output and opengl debug message callback

#include "Platform.h"
#include "Window.h"
#include "Files.h"

#include "OpenGL/OpenGL_Constants.h"

typedef struct Opengl_Shader_Program Opengl_Shader_Program;
struct Opengl_Shader_Program
{
  String label;
  GLenum type;
  GLuint handle;
};

typedef struct Opengl_Compile_Shader_Result Opengl_Compile_Shader_Result;
struct Opengl_Compile_Shader_Result
{
  Opengl_Shader_Program shader_program;
  b32 result;
  String error_log;
};

// @Section: Opengl entry point
function b32   opengl_init(b32 set_vsync); /* Initializes opengl context */
function void  opengl_end();  /* Deletes opengl context */
function Opengl_Compile_Shader_Result opengl_compile_shader_from_source(Arena *arena, String label, String source, GLenum shader_type);
function Opengl_Compile_Shader_Result opengl_compile_shader_from_file(Arena *arena, String label, String file_path, GLenum shader_type);
#define opengl_check_errors() _opengl_check_error(S(__FILE__), __LINE__)

// @Section: Settings
function void opengl_set_vsync(b32 state); /* Enables vsync */

// @Section: Opengl helpers
function void  APIENTRY _opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user); /* Opengl debug callback */
function void           _opengl_check_error(String file, u32 line); /* Checks for opengl errors and terminates the program. */
function void*          _load_gl_function(const char *name); /* Helper to load a single opengl function */

// @Section: Implementation
#include "OpenGL/generated/Opengl.cgen.h"
#include "OpenGL/generated/Opengl.cgen.c"

function Opengl_Compile_Shader_Result 
opengl_compile_shader_from_source(Arena *arena, String label, String source, GLenum shader_type)
{
  Opengl_Compile_Shader_Result result = {0}; 
  if (!source.cstring || source.count == 0)
  {
    result.result = false;
    result.error_log = S("Unable to compile shader. Shader source is empty.");
    return result;
  }

  const char *source_cstr = (const char *)source.cstring;
  u32 program = glCreateShaderProgramv(shader_type, 1, &source_cstr);
  opengl_check_errors();

  GLint link_status = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &link_status);

  if (!link_status)
  {
    GLint log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    GLchar *log = push_array(arena, GLchar, log_length + 1);
    glGetProgramInfoLog(program, log_length, 0, log);
    
    result.result = false;
    result.error_log = Sf(arena, "Shader "S_FMT" compile/link failed:\n%s", S_ARG(label), log);
  }
  else
  {
    result.result = true;
    result.shader_program.label  = string_copy(arena, label);
    result.shader_program.type   = shader_type;
    result.shader_program.handle = program;
  }

  return result;
}

function Opengl_Compile_Shader_Result 
opengl_compile_shader_from_file(Arena* arena, String label, String file_path, GLenum shader_type)
{
  Opengl_Compile_Shader_Result result = {0};
  Scratch scratch = scratch_begin(0,0);
  String shader_source = file_load(scratch.arena, file_path);
  if (shader_source.count > 0)
  {
    result = opengl_compile_shader_from_source(arena, label, shader_source, shader_type);
  }
  else
  {
    result.result = false;
    result.error_log = Sf(arena, "Unable to compile shader. Could not to load file path \""S_FMT"\"", S_ARG(file_path));
  }
  scratch_end(&scratch);
  return result;
}

function void
_opengl_check_error(String file, u32 line)
{
  GLenum err = glGetError();
  if (err != GL_NO_ERROR)
  {
    const char *error_string = "Unknown";
    switch (err)
    {
      case GL_INVALID_ENUM:                  error_string = "GL_INVALID_ENUM"; break;
      case GL_INVALID_VALUE:                 error_string = "GL_INVALID_VALUE"; break;
      case GL_INVALID_OPERATION:             error_string = "GL_INVALID_OPERATION"; break;
      case GL_STACK_OVERFLOW:                error_string = "GL_STACK_OVERFLOW"; break;
      case GL_STACK_UNDERFLOW:               error_string = "GL_STACK_UNDERFLOW"; break;
      case GL_OUT_OF_MEMORY:                 error_string = "GL_OUT_OF_MEMORY"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: error_string = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
    }

    Scratch scratch = scratch_begin(0,0);
    String msg = Sf(scratch.arena, "OpenGL error 0x%X (%s) at "S_FMT":%u", err, error_string, S_ARG(file), line);
    message_box(S("OpenGL Error"), msg, file, line);
    scratch_end(&scratch);
    assert(false);
  }
}

function void APIENTRY
_opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user)
{
  if (id == 131218) return; // @TODO(fz): Deal with this

  Scratch scratch = scratch_begin(0,0);

  // Decode source
  const char *source_str = "Unknown";
  switch (source)
  {
    case GL_DEBUG_SOURCE_API:             source_str = "API";             break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_str = "Window System";   break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     source_str = "Third Party";     break;
    case GL_DEBUG_SOURCE_APPLICATION:     source_str = "Application";     break;
    case GL_DEBUG_SOURCE_OTHER:           source_str = "Other";           break;
  }

  // Decode type
  const char *type_str = "Unknown";
  switch (type)
  {
    case GL_DEBUG_TYPE_ERROR:               type_str = "Error";               break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "Deprecated Behavior"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_str = "Undefined Behavior";  break;
    case GL_DEBUG_TYPE_PORTABILITY:         type_str = "Portability";         break;
    case GL_DEBUG_TYPE_PERFORMANCE:         type_str = "Performance";         break;
    case GL_DEBUG_TYPE_MARKER:              type_str = "Marker";              break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          type_str = "Push Group";          break;
    case GL_DEBUG_TYPE_POP_GROUP:           type_str = "Pop Group";           break;
    case GL_DEBUG_TYPE_OTHER:               type_str = "Other";               break;
  }

  // Decode severity
  const char *severity_str = "Unknown";
  switch (severity)
  {
    case GL_DEBUG_SEVERITY_HIGH:         severity_str = "High";         break;
    case GL_DEBUG_SEVERITY_MEDIUM:       severity_str = "Medium";       break;
    case GL_DEBUG_SEVERITY_LOW:          severity_str = "Low";          break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: severity_str = "Notification"; break;
  }

  u8 buffer[4096];
  if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
  {
    stbsp_sprintf((char*)buffer,
      "OpenGL Debug Message\n"
      "  Source: %s\n"
      "  Type: %s\n"
      "  Severity: %s\n"
      "  ID: %u\n"
      "  Message: " S_FMT,
      source_str, type_str, severity_str, id, length, message);
  }

  if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM)
  {
    String error = Sf(scratch.arena, "OpenGL Debug Triggered (Severity: %s, Type: %s, Source: %s)\n%s\n", severity_str, type_str, source_str, buffer);
    printf(S_FMT, S_ARG(error));
    // @TODO(fz): Error handling
    assert(false);
  }

  scratch_end(&scratch);
}

function void*
_load_gl_function(const char *name)
{
  void* proc = (void*)wglGetProcAddress(name);

  // Check for invalid pointer values
  if (!proc || proc == (void *)0x1 || proc == (void *)0x2 || proc == (void *)0x3 || proc == (void *)-1)
  {
    local_persist HMODULE opengl32_module = NULL;
    if (!opengl32_module)
    {
      opengl32_module = GetModuleHandleA("opengl32.dll");
      if (!opengl32_module)
      {
        printf("opengl32.dll not loaded. Trying to load it dynamically.");
        opengl32_module = LoadLibraryA("opengl32.dll");
        if (opengl32_module)
        {
          printf("Unable to load opengl32.dll");
          return NULL;
        }
      }
    }
    proc = (void *)GetProcAddress(opengl32_module, name);
  }
  return proc;
}

#if OS_WINDOWS
# include "OpenGL/OpenGL_Win32.h"
#else
# error Operating System not supported
#endif

#endif // OPENGL_H
#ifndef OPENGL_H
#define OPENGL_H

#include "Platform.h"
#include "Window.h"

#include "OpenGL/OpenGL_Constants.h"
#include "OpenGL/generated/Opengl.cgen.h"

// @Section: Opengl entry point
function b32   opengl_init(); /* Initializes opengl context */
function void  opengl_end();  /* Deletes opengl context */
#define opengl_check_errors() _opengl_check_error(S(__FILE__), __LINE__)

// @Section: Settings
function void window_set_vsync(b32 state); /* Enables vsync */

// @Section: Opengl helpers
function void  APIENTRY _opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user); /* Opengl debug callback */
function void           _opengl_check_error(String file, u32 line); /* Checks for opengl errors and terminates the program. */
function void*          _load_gl_function(const char *name); /* Helper to load a single opengl function */

// @Section: Implementation
#include "OpenGL/generated/Opengl.cgen.c"

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
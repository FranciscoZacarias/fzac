#ifndef OPENGL_WIN32_H
#define OPENGL_WIN32_H

#pragma comment(lib, "opengl32.lib")

// @Section: WGL

// WGL Pixel format attributes
#define WGL_NUMBER_PIXEL_FORMATS_ARB      0x2000
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_DRAW_TO_BITMAP_ARB            0x2002
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_NEED_PALETTE_ARB              0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB       0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB        0x2006
#define WGL_SWAP_METHOD_ARB               0x2007
#define WGL_NUMBER_OVERLAYS_ARB           0x2008
#define WGL_NUMBER_UNDERLAYS_ARB          0x2009
#define WGL_TRANSPARENT_ARB               0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB     0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB   0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB    0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB   0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB   0x203B
#define WGL_SHARE_DEPTH_ARB               0x200C
#define WGL_SHARE_STENCIL_ARB             0x200D
#define WGL_SHARE_ACCUM_ARB               0x200E
#define WGL_SUPPORT_GDI_ARB               0x200F
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_STEREO_ARB                    0x2012
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_RED_BITS_ARB                  0x2015
#define WGL_RED_SHIFT_ARB                 0x2016
#define WGL_GREEN_BITS_ARB                0x2017
#define WGL_GREEN_SHIFT_ARB               0x2018
#define WGL_BLUE_BITS_ARB                 0x2019
#define WGL_BLUE_SHIFT_ARB                0x201A
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_ALPHA_SHIFT_ARB               0x201C
#define WGL_ACCUM_BITS_ARB                0x201D
#define WGL_ACCUM_RED_BITS_ARB            0x201E
#define WGL_ACCUM_GREEN_BITS_ARB          0x201F
#define WGL_ACCUM_BLUE_BITS_ARB           0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB          0x2021
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_AUX_BUFFERS_ARB               0x2024
#define WGL_NO_ACCELERATION_ARB           0x2025
#define WGL_GENERIC_ACCELERATION_ARB      0x2026
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_SWAP_EXCHANGE_ARB             0x2028
#define WGL_SWAP_COPY_ARB                 0x2029
#define WGL_SWAP_UNDEFINED_ARB            0x202A
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_TYPE_COLORINDEX_ARB           0x202C

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB 0x20A9

// WGL Context creation attributes
#define WGL_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB       0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB         0x2093
#define WGL_CONTEXT_FLAGS_ARB               0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB        0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB    0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define WGL_CONTEXT_DEBUG_BIT_ARB           0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002

// Optional extensions
#define WGL_SWAP_METHOD_ARB         0x2007
#define WGL_SWAP_EXCHANGE_ARB       0x2028
#define WGL_SWAP_COPY_ARB           0x2029
#define WGL_SWAP_UNDEFINED_ARB      0x202A

// Commoon pixel format results
#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#define WGL_SAMPLES_ARB              0x2042
#define WGL_SAMPLE_BUFFERS_ARB       0x2041

// @Section: Define wgl function pointers
#define WGL_FUNC(ret,name,params) typedef ret (* PFN##name##PROC) params;
  #include "OpenGL_Win32_WGL.inl"
#undef WGL_FUNC

// @Section: Generate wgl definitions
#define WGL_FUNC(ret,name,params) global PFN##name##PROC name = NULL;
  #include "OpenGL_Win32_WGL.inl"
#undef WGL_FUNC

// @Section: Functions
function b32 _win32_load_wgl_functions(); /* Opens webgl functions */

// @Section: Implementation

function b32
opengl_init(Window* window)
{
  if (!_win32_load_wgl_functions())
  {
    // emit_fatal(S("Failed to load wgl functions."));
    // @TODO(Fz): Handle error
    assert(false);
  }

  // set pixel format for OpenGL context
  {
    int attrib[] =
    {
      WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
      WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
      WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
      WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
      WGL_COLOR_BITS_ARB,     24,
      WGL_DEPTH_BITS_ARB,     24,
      WGL_STENCIL_BITS_ARB,   8,

      // uncomment for sRGB framebuffer, from WGL_ARB_framebuffer_sRGB extension
      // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_framebuffer_sRGB.txt
      //WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,

      // uncomment for multisampled framebuffer, from WGL_ARB_multisample extension
      // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_multisample.txt
      //WGL_SAMPLE_BUFFERS_ARB, 1,
      //WGL_SAMPLES_ARB,        4, // 4x MSAA

      0,
    };

    int format;
    UINT formats;
    if (!wglChoosePixelFormatARB(window->dc, attrib, NULL, 1, &format, &formats) || formats == 0)
    {
      //emit_fatal(S("OpenGL does not support required pixel format!"));
      // @TODO(Fz): Handle error
      assert(false);
    }

    PIXELFORMATDESCRIPTOR desc =
    {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
      PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
      32,                   // Colordepth of the framebuffer.
      0, 0, 0, 0, 0, 0,
      0,
      0,
      0,
      0, 0, 0, 0,
      24,                   // Number of bits for the depthbuffer
      8,                    // Number of bits for the stencilbuffer
      0,                    // Number of Aux buffers in the framebuffer.
      PFD_MAIN_PLANE,
      0,
      0,
      0,
      0
    };
    if(!DescribePixelFormat(window->dc, format, sizeof(desc), &desc))
    {
      // emit_error(S("Failed to describe OpenGL pixel format"));
      // @TODO(fz): Handle error.
      assert(0);
    }
    if (!SetPixelFormat(window->dc, format, &desc))
    {
      // emit_fatal(S("Cannot set OpenGL selected pixel format!"));
      // @TODO(fz): Handle error.
      assert(0);
    }
  }

  // create modern OpenGL context
  {
    int attrib[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
      WGL_CONTEXT_MINOR_VERSION_ARB, 5,
      WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,

#ifndef DEBUG
      // ask for debug context for non "Release" builds
      // this is so we can enable debug callback
      WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
      0,
    };

    window->rc = wglCreateContextAttribsARB(window->dc, NULL, attrib);
    if (!window->rc)
    {
      // emit_fatal(S("Cannot create modern OpenGL context! OpenGL version 4.5 not supported?"));
      // @TODO(Fz): Handle error
      assert(false);
    }

    b32 ok = wglMakeCurrent(window->dc, window->rc);
    if (!ok)
    {
      // win32_check_error();
      // emit_fatal(S("Failed to make current OpenGL context"));
      // @TODO(Fz): Handle error
      assert(false);
    }

    if (!_os_opengl_load_functions())
    {
      // emit_fatal(S("Opengl failed to load functions"));
      // @TODO(Fz): Handle error
      assert(false);
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(&_os_opengl_debug_callback, NULL);
  }

  // Set viewport
  glViewport(0, 0, window->width, window->height);

  // Check for errors
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    printf("OpenGL error after init: 0x%x\n", error);
  }

  return true;
}

function void
opengl_end(Window* window)
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(window->rc);
}

function void
window_set_vsync(b32 state)
{
  wglSwapIntervalEXT(state);
}

function b32
_win32_load_wgl_functions()
{
  // to get WGL functions we need valid GL context, so create dummy window for dummy GL context
  HWND dummy = CreateWindowExW(
    0, L"STATIC", L"DummyWindow", WS_OVERLAPPED,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, NULL, NULL);
  if (!dummy)
  {
    printf("Failed to create dummy window");
    return false;
  }

  HDC device_context = GetDC(dummy);
  if (!device_context)
  {
    printf("Failed to get device context for dummy window");
    return false;
  }

  PIXELFORMATDESCRIPTOR desc = {
    .nSize      = sizeof(desc),
    .nVersion   = 1,
    .dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    .iPixelType = PFD_TYPE_RGBA,
    .cColorBits = 24,
  };

  int format = ChoosePixelFormat(device_context, &desc);
  if (!format) 
  {
    printf("Cannot choose OpenGL pixel format for dummy window!");
    return false;
  }

  if (!DescribePixelFormat(device_context, format, sizeof(desc), &desc))
  {
    printf("Failed to describe OpenGL pixel format");
    return false;
  }

  // reason to create dummy window is that SetPixelFormat can be called only once for the window
  if (!SetPixelFormat(device_context, format, &desc))
  {
    printf("Cannot set OpenGL pixel format for dummy window!");
    return false;
  }

  HGLRC rc = wglCreateContext(device_context);
  if (!rc)
  {
    printf("Failed to create OpenGL context for dummy window");
    return false;
  }

  if (!wglMakeCurrent(device_context, rc))
  {
    printf("Failed to make current OpenGL context for dummy window");
    return false;
  }

  {
    #define WGL_FUNC(ret, name, params) \
      name = (PFN##name##PROC)wglGetProcAddress(#name); \
      if (!name) { printf("%s not loaded", #name); return false; }
    # include "OpenGL_Win32_WGL.inl"
    #undef WGL_FUNC
  }

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(rc);
  ReleaseDC(dummy, device_context);
  DestroyWindow(dummy);

  return true;
}

#endif // OPENGL_WIN32_H
#ifndef FZAC_BASE_RAYLIB_H
#define FZAC_BASE_RAYLIB_H

/* Same base as Base.h, with raylib as the platform, graphics, input, audio and math layer.
   Build raylib once with Extern/raylib/build_raylib.bat, then link with /LIBPATH:<fzac>/Extern/raylib/lib
   Not available in this mode (raylib replaces them): Window, Platform, OpenGL, Vulkan, Simple_Renderer2D, Camera2D, Camera3D
   Not ported to this mode yet: Geometry, Art, UI, Immediate_UI */

#ifdef FZAC_BASE_H
# error "Include either Base.h or Base_Raylib.h, not both"
#endif

#define FZ_RAYLIB 1

#include "base/Warnings.h"

#define RADDBG_MARKUP_IMPLEMENTATION
#include "Extern/raddbg_markup.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

// @Section: Dynamic String Buffer
#define STB_SPRINTF_IMPLEMENTATION
#include "Extern/stb_sprintf.h"

// @Section: Base includes
#include "base/Context_Cracking.h"
#include "base/Core.h"

// @Section: Raylib
#include "Extern/raylib/raylib.h"
#include "Extern/raylib/rlgl.h"
#include "Extern/raylib/rcamera.h"

// raylib.lib must come before user32.lib in the link order, both define CloseWindow
#if OS_WINDOWS
# pragma comment(lib, "raylib.lib")
# pragma comment(lib, "winmm.lib")
# pragma comment(lib, "gdi32.lib")
# pragma comment(lib, "user32.lib")
# pragma comment(lib, "shell32.lib")
#endif

#include "base/Memory.h"
#include "base/Arena.h"
#include "base/Allocator.h"
#include "base/Array.h"
#include "base/Thread_Context.h"

#include "base/String.h"
#include "base/Math.h"
#include "base/Os.h"

#include "base/Custom_Entry_Point.h"

#endif // FZAC_BASE_RAYLIB_H

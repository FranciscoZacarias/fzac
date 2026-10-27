#ifndef FZAC_BASE_H
#define FZAC_BASE_H

#ifdef FZAC_BASE_RAYLIB_H
# error "Include either Base.h or Base_Raylib.h, not both"
#endif

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

#include "base/Memory.h"
#include "base/Arena.h"
#include "base/Allocator.h"
#include "base/Array.h"
#include "base/Thread_Context.h"

#include "base/String.h"
#include "base/Math.h"
#include "base/Os.h"

#include "base/Custom_Entry_Point.h"

#if METAPROGRAM
#include "Default_Metaprogram.h"
#endif

#endif // FZAC_BASE_H

#ifndef BASE_H
#define BASE_H

// @TODO(fz): We probably want to rename all if guards in the library and prefix them with FZAC_

#pragma warning(disable: 4201) // Nonstandard extension used: nameless struct/union
#pragma warning(disable: 4700) // Uninitalized local variable used
#pragma warning(disable: 4094) // Untagged structs
#pragma warning(disable: 4100) // Unreferenced parameter
#pragma warning(disable: 4244) // Type conversion - possible loss of data
#pragma warning(disable: 4189) // Local variable is initialized but not referenced

#if 1 // There are specifically because of raddbg_markup.h
# pragma warning(disable: 4431) // Name in formal parameter list illegal. This warning messes up some macros
# pragma warning(disable: 4131) // Use old-style declarator
# pragma warning(disable: 4218) // nonstandard extension used: must specify at least a storage class or a type
#endif

#if DEBUG
# pragma warning(disable: 4101) // Unused variable
# pragma warning(disable: 4100) // Unreferenced parameters
# pragma warning(disable: 4702) // Unreachable code
#endif

#define RADDBG_MARKUP_IMPLEMENTATION
#include "extern\raddbg_markup.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

// @Section: Dynamic String Buffer
#define STB_SPRINTF_IMPLEMENTATION
#include "Extern/stb_sprintf.h"

// @Section: Base includes
#include "base\Context_Cracking.h"
#include "base\Core.h" 

#include "base\Memory.h"
#include "base\Arena.h"
#include "base\Allocator.h"
#include "base\Thread_Context.h"

#include "base\String.h"
#include "base\Math\Math.h"
#include "base\Os.h"

#include "base\Custom_Entry_Point.h"

#if METAPROGRAM
#include "Default_Metaprogram.h"
#endif

#endif // BASE_H
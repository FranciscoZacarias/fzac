#ifndef BASE_H
#define BASE_H

#pragma warning(disable: 4201) // Nonstandard extension used: nameless struct/union
#pragma warning(disable: 4700) // Uninitalized local variable used
#pragma warning(disable: 4094) // Untagged structs
#pragma warning(disable: 4100) // Unreferenced parameter

#if 1 // There are specifically because of raddbg_markup.h
# pragma warning(disable: 4431) // Name in formal parameter list illegal. This warning messes up some macros
# pragma warning(disable: 4131) // Use old-style declarator
# pragma warning(disable: 4218) // nonstandard extension used: must specify at least a storage class or a type
#endif

#if DEBUG
# pragma warning(disable: 4101) // Unused variable
# pragma warning(disable: 4100) // Unreferenced parameters
# pragma warning(disable: 4702) // Unreachable code
# pragma warning(disable: 4189) // Local variable is initialized but not referenced
#endif

#define RADDBG_MARKUP_IMPLEMENTATION
#include "Extern\raddbg_markup.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// @NOTE(fz): Belongs to Introspection.h. See [Introspection.h@Section: Introspection macros] for more information.
#define META_ENUM_LINK(enum_name)

// @Section: Base includes

#include "base\Context_Cracking.h"
#include "base\Core.h" 

#include "base\Memory.h"
#include "base\Allocator.h"

#include "base\String.h"
#include "base\Math.h"

#include "base\Data_Structures.h"
#include "base\Thread_Context.h"
#include "base\Custom_Entry_Point.h"

#endif // BASE_H
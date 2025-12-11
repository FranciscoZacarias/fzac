#ifndef BASE_H
#define BASE_H

#pragma warning(disable: 4201) // Nonstandard extension used: nameless struct/union
#pragma warning(disable: 4700) // Uninitalized local variable used
#pragma warning(disable: 4189) // Local variable is initialized but not referenced
#pragma warning(disable: 4100) // Unreferenced parameters
#pragma warning(disable: 4094) // Untagged structs


#include <stdlib.h>
#include <string.h> // @TODO(fz): Implement memset, memmove, memcmp, memcpy
#include <stdio.h>
#include <math.h>

#include "src\base\Context_Cracking.h"
#include "src\base\Core.h" 

#include "src\base\Math.h"

#include "src\base\Memory.h"
#include "src\base\Allocator.h"
#include "src\base\String.h"

#include "src\base\Data_Structures.h"

#include "src\base\Entry_Point.h"

#endif // BASE_H
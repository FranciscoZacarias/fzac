#ifndef BASE_H
#define BASE_H

#pragma warning(disable: 4201) // Nonstandard extension used: nameless struct/union

#include <string.h> // TODO(fz): Implement memset, memmove, memcmp, memcpy
#include <stdio.h>
#include <math.h>

// Header only
#include "src\base\context_cracking.h"
#include "src\base\core.h" 
#include "src\base\test.h" 

// Headers
#include "src\base\math.h"
#include "src\base\memory.h"
#include "src\base\thread_context.h"
#include "src\os\os.h"
#include "src\base\string.h"

// Implementation
#include "src\base\math.c"
#include "src\base\memory.c"
#include "src\base\thread_context.c"
#include "src\os\os.c"
#include "src\base\string.c"

#endif // BASE_H
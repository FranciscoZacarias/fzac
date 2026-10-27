#ifndef FZAC_WARNINGS_H
#define FZAC_WARNINGS_H

// Check native compiler macros directly so diagnostic pragmas execute
// regardless of header include ordering.
#if defined(_MSC_VER) && !defined(__clang__)
  #pragma warning(disable: 4201) // Nonstandard extension used: nameless struct/union
  #pragma warning(disable: 4700) // Uninitialized local variable used
  #pragma warning(disable: 4094) // Untagged structs
  #pragma warning(disable: 4100) // Unreferenced parameter
  #pragma warning(disable: 4244) // Type conversion - possible loss of data
  #pragma warning(disable: 4189) // Local variable is initialized but not referenced
  #pragma warning(disable: 4505) // Unreferenced local function has been removed

  #if 1 // These are specifically because of raddbg_markup.h
    #pragma warning(disable: 4431) // Name in formal parameter list illegal. This warning messes up some macros
    #pragma warning(disable: 4131) // Use old-style declarator
    #pragma warning(disable: 4218) // Nonstandard extension used: must specify at least a storage class or a type
  #endif

  #if DEBUG
    #pragma warning(disable: 4101) // Unused variable
    #pragma warning(disable: 4100) // Unreferenced parameters
    #pragma warning(disable: 4702) // Unreachable code
  #endif
#endif

#if defined(__GNUC__) || defined(__clang__)
  #pragma GCC diagnostic ignored "-Wunused-parameter"
  #pragma GCC diagnostic ignored "-Wunused-variable"
  #pragma GCC diagnostic ignored "-Wunused-function"
  #pragma GCC diagnostic ignored "-Wswitch"
  #pragma GCC diagnostic ignored "-Wconversion"
  #pragma GCC diagnostic ignored "-Wpedantic"
  #pragma GCC diagnostic ignored "-Wsign-compare"

  #if defined(__clang__)
  # pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
  #endif

  #if DEBUG
  # pragma GCC diagnostic ignored "-Wunreachable-code"
  #endif
#endif

#endif // FZAC_WARNINGS_H

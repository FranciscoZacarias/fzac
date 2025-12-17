#ifndef PLATFORM_H
#define PLATFORM_H

// @Section: Console
function void console_attach(); /* Opens application terminal */

// @Section: Path
typedef struct Path Path;
struct Path
{
  String header_string; /* e.g. "C:", "" */
  u32    number_of_leading_slashes;
  b32    trailing_slash;

  String* words; /* Array of parsed components */
  u32     word_count;
};

function b32    path_is_char8_separator_slash(u8 c);
function Path   path_from_string(Arena* arena, String input);
function String string_from_path(Arena* arena, Path path);

// @Section: File
function b32    file_create(String path); /* Creates file. If file exists, returns true anyway. */
function b32    file_delete(String path); /* Deletes a file */
function b32    file_exists(String path); /* Returns true if file exists */
function u32    file_write(String path, u8* data, u64 data_size); /* Wipes file contents and writes data */
function u32    file_append(String path, u8* data, u64 data_size); /* Appends data to the file */
function b32    file_wipe(String path); /* Clears file */
function u32    file_size(String path); /* Returns the size of the file */
function String file_load(Arena* arena, String path); /* Loads file into memory */

#if OS_WINDOWS
# include "Platform/Platform_Win32.c"
#else
# error Operating System not supported
#endif

#endif // PLATFORM_H
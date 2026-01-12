#ifndef PLATFORM_H
#define PLATFORM_H

// @Section: Console
function void console_attach(); /* Opens application terminal */

// @Section: Path
function b32    path_is_char8_separator_slash(u8 c);
function String fullpath_from_relative_path(Arena* arena, String relative_path);

// @Section: File
function b32    file_create(String path); /* Creates file. If file exists, returns true anyway. */
function b32    file_delete(String path); /* Deletes a file */
function b32    file_exists(String path); /* Returns true if file exists */
function u32    file_write(String path, u8* data, u64 data_size); /* Wipes file contents and writes data */
function u32    file_append(String path, u8* data, u64 data_size); /* Appends data to the file */
function b32    file_wipe(String path); /* Clears file */
function u32    file_size(String path); /* Returns the size of the file */
function String file_load(Arena* arena, String path); /* Loads file into memory */
function String_List file_get_files_in_path(Arena* arena, String path, b32 recursive); /* Returns all files in a given path. If recursive is false: returns immediate files and directories in path. If recursive is true: returns all files recursively from directory specified. */
function String_View file_get_extension(String path); /* Returns file extension. */

function b32    is_directory(String path);
function b32    is_file(String path);


#if OS_WINDOWS
# include "Platform/Platform_Win32.c"
#else
# error Operating System not supported
#endif

#endif // PLATFORM_H
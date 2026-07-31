#ifndef FILES_H
#define FILES_H

#include "Timing.h"

typedef struct File_Watcher File_Watcher;
struct File_Watcher
{
  String path;
  u64 last_write_time;
  f64 last_triggered_time;
};

// @Section: File and directory
fz_internal b32    file_create(String path); /* Creates file. If file exists, returns true anyway. */
fz_internal b32    file_delete(String path); /* Deletes a file */
fz_internal u32    file_write(String path, u8* data, u64 data_size); /* Wipes file contents and writes data */
fz_internal u32    file_write_string(String path, String data); /* Same as file_write */
fz_internal u32    file_append(String path, u8* data, u64 data_size); /* Appends data to the file */
fz_internal b32    file_wipe(String path); /* Clears file */
fz_internal u32    file_size(String path); /* Returns the size of the file */
fz_internal String file_load(Arena* arena, String path); /* Loads file into memory */
fz_internal String_List file_get_files_in_path(Arena* arena, String path, b32 recursive); /* Returns all files in a given path. If recursive is false: returns immediate files and directories in path. If recursive is true: returns all files recursively from directory specified. */
fz_internal String file_get_extension(String path); /* Returns file extension. */
fz_internal String file_get_name_no_extension(String path); /* Returns the file name without extension */

// File watch
fz_internal void file_watch_init(Arena *arena, File_Watcher *watch, String path);
fz_internal u64  file_get_last_write_time(String path);
fz_internal b32  file_watch_changed(File_Watcher *watch);

fz_internal b32 file_exists(String path);
fz_internal b32 is_file(String path);

fz_internal b32    directory_create(String path);
fz_internal b32    is_directory(String path);
fz_internal b32    directory_exists(String path);
fz_internal String directory_get_current_working_directory(void); /* Returns the current working directory */
fz_internal String directory_pop(String path); /* Given a path, pops one directory. E.g. 'D:\work\make_project\build' becomes 'D:\work\make_project'*/
fz_internal String directory_push(Arena *arena, String path, String dir); /* Given a path, goes up one directory. E.g. 'D:\work\make_project' with 'build' becomes 'D:\work\make_project\build'*/

fz_internal String full_path_from_relative_path(Arena* arena, String relative_path);

#if OS_WINDOWS

fz_internal wchar_t* utf8_to_wide(Arena* arena, u8* utf8, s64 utf8_size);
fz_internal void     win32_log_error(const char* msg, String path);
fz_internal b32      win32_is_dot_dir(const char* name);
fz_internal b32      win32_collect_recursive(Arena* arena, String_List* out, String dir_path);
fz_internal void     win32_collect_non_recursive(Arena* arena, String_List* out, String dir_path);

fz_internal wchar_t*
utf8_to_wide(Arena* arena, u8* utf8, s64 utf8_size)
{
  if (utf8 == 0 || utf8_size <= 0) return NULL;
  s32 required = MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,(char*)utf8,(s32)utf8_size,0,0);
  if (required <= 0) return NULL;
  wchar_t* wstr = push_array(arena, wchar_t, (u64)required + 1);
  MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,(char*)utf8,(s32)utf8_size,wstr,required);
  wstr[required] = '\0';
  return wstr;
}

fz_internal void
win32_log_error(const char* msg, String path)
{
  char buffer[512];
  wsprintfA(buffer, "%s: %.*s\n", msg, (s32)path.count, path.cstring);
  OutputDebugStringA(buffer);
}

fz_internal b32
win32_is_dot_dir(const char* name)
{
  b32 result = (name[0] == '.' && (name[1] == 0 || (name[1] == '.' && name[2] == 0)));
  return result;
}

fz_internal b32
win32_collect_recursive(Arena* arena, String_List* out, String dir_path)
{
  b32 found_file = 0;

  String search = string_join(arena, dir_path, S("\\*"));

  WIN32_FIND_DATAA find_data = {0};
  HANDLE handle = FindFirstFileA((char*)search.cstring, &find_data);

  if(handle == INVALID_HANDLE_VALUE) return 0;

  do
  {
    if(win32_is_dot_dir(find_data.cFileName))
    {
      continue;
    }

    String name = string_new((u64)strlen(find_data.cFileName), (u8*)find_data.cFileName);

    String full_path = string_join(arena, string_join(arena, dir_path, S("\\")), name);

    if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      b32 subtree_has_files = win32_collect_recursive(arena, out, full_path);

      if(!subtree_has_files)
      {
        string_list_push(arena, out, full_path);
      }

      found_file |= subtree_has_files;
    }
    else
    {
      string_list_push(arena, out, full_path);
      found_file = 1;
    }

  } while(FindNextFileA(handle, &find_data));

  FindClose(handle);
  return found_file;
}

fz_internal void
win32_collect_non_recursive(Arena* arena, String_List* out, String dir_path)
{
  String search = string_join(arena, dir_path, S("\\*"));

  WIN32_FIND_DATAA find_data = {0};
  HANDLE handle = FindFirstFileA((char*)search.cstring, &find_data);

  if(handle == INVALID_HANDLE_VALUE)
  {
    win32_log_error("FindFirstFileA failed", dir_path);
    return;
  }

  do
  {
    if(win32_is_dot_dir(find_data.cFileName))
    {
      continue;
    }
    String name = string_new((u64)strlen(find_data.cFileName), (u8*)find_data.cFileName);
    String full_path = string_join(arena, string_join(arena, dir_path, S("\\")), name);
    string_list_push(arena, out, full_path);
  } while(FindNextFileA(handle, &find_data));

  FindClose(handle);
}

fz_internal b32
file_create(String path)
{
  b32 result  = true;
  Scratch scratch = scratch_begin(0,0);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.count);
  HANDLE file = CreateFileW(wpath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  scratch_end(&scratch);
  if((file != INVALID_HANDLE_VALUE))
  {
    CloseHandle(file);
  }
  else
  {
    result = false;
  }
  return result;
}

fz_internal b32
file_delete(String path)
{
  Scratch scratch = scratch_begin(0,0);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.count);
  b32 result = DeleteFileW(wpath);
  scratch_end(&scratch);
  return result;
}

fz_internal u32
file_write(String path, u8* data, u64 data_size)
{
  Scratch scratch = scratch_begin(0,0);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.count);
  HANDLE file = CreateFileW(wpath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  scratch_end(&scratch);
  if(file == INVALID_HANDLE_VALUE) return 0;

  DWORD written = 0;
  WriteFile(file, data, (DWORD)data_size, &written, 0);
  CloseHandle(file);
  return (u32)written;
}

fz_internal u32
file_write_string(String path, String data)
{
  return file_write(path, data.cstring, data.count);
}

fz_internal u32
file_append(String path, u8* data, u64 data_size)
{
  Scratch scratch = scratch_begin(0,0);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.count);
  HANDLE file = CreateFileW(wpath, FILE_APPEND_DATA, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  scratch_end(&scratch);
  if(file == INVALID_HANDLE_VALUE) return 0;

  SetFilePointer(file, 0, 0, FILE_END);
  DWORD written = 0;
  WriteFile(file, data, (DWORD)data_size, &written, 0);
  CloseHandle(file);
  return (u32)written;
}

fz_internal b32
file_wipe(String path)
{
  Scratch scratch = scratch_begin(0,0);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.count);
  HANDLE file = CreateFileW(wpath, GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  scratch_end(&scratch);
  if(file == INVALID_HANDLE_VALUE) return 0;

  SetFilePointer(file, 0, 0, FILE_BEGIN);
  b32 result = SetEndOfFile(file);
  CloseHandle(file);
  return result;
}

fz_internal u32
file_size(String path)
{
  Scratch scratch = scratch_begin(0,0);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.count);
  WIN32_FILE_ATTRIBUTE_DATA attr;
  b32 ok = GetFileAttributesExW(wpath, GetFileExInfoStandard, &attr);
  scratch_end(&scratch);
  if(!ok) return 0;

  ULARGE_INTEGER size;
  size.LowPart  = attr.nFileSizeLow;
  size.HighPart = attr.nFileSizeHigh;
  return (u32)size.QuadPart;
}

fz_internal String
file_load(Arena* arena, String path)
{
  String result = {0};

  Scratch scratch = scratch_begin(&arena,1);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.count);
  HANDLE file = CreateFileW(wpath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  scratch_end(&scratch);

  if(file == INVALID_HANDLE_VALUE) return result;

  DWORD file_size = GetFileSize(file, 0);
  u8* buffer = push_array(arena, u8, file_size + 1);
  DWORD read = 0;
  if(ReadFile(file, buffer, file_size, &read, 0) && read == file_size)
  {
    result.cstring = buffer;
    result.cstring[file_size] = '\0';
    result.count    = file_size;
  }

  CloseHandle(file);
  return result;
}

fz_internal String_List
file_get_files_in_path(Arena* arena, String path, b32 recursive)
{
  String_List result = string_list_new();

  DWORD attrib = GetFileAttributesA((char*)path.cstring);
  if(attrib == INVALID_FILE_ATTRIBUTES)
  {
    win32_log_error("Path does not exist", path);
    return result;
  }

  if(!(attrib & FILE_ATTRIBUTE_DIRECTORY))
  {
    string_list_push(arena, &result, string_copy(arena, path));
    return result;
  }

  if(recursive)
  {
    win32_collect_recursive(arena, &result, path);
  }
  else
  {
    win32_collect_non_recursive(arena, &result, path);
  }

  return result;
}

fz_internal String
directory_get_current_working_directory(void)
{
  static u8 buffer[MAX_PATH];

  DWORD len = GetCurrentDirectoryA(MAX_PATH, (LPSTR)buffer);
  if (len == 0 || len >= MAX_PATH)
  {
    return S("");
  }

  String result = (String){ .count = (u64)len, .cstring = buffer };
  return result;
}

fz_internal u64
file_get_last_write_time(String path)
{
  u64 result = 0;
  WIN32_FILE_ATTRIBUTE_DATA data;
  if(GetFileAttributesExA((char*)path.cstring, GetFileExInfoStandard, &data))
  {
    FILETIME t = data.ftLastWriteTime;
    result = ((u64)t.dwHighDateTime << 32) | (u64)t.dwLowDateTime;
  }
  return result;
}

fz_internal b32
is_directory(String path)
{
  if (path.count == 0 || path.cstring == 0) return false;
  char buffer[MAX_PATH];
  if (path.count >= MAX_PATH) return false;
  memory_copy(buffer, path.cstring, path.count);
  buffer[path.count] = 0;
  DWORD attributes = GetFileAttributesA(buffer);
  if (attributes == INVALID_FILE_ATTRIBUTES) return false;
  b32 result = (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
  return result;
}

fz_internal b32
is_file(String path)
{
  if (path.count == 0 || path.cstring == 0) return false;
  char buffer[MAX_PATH];
  if (path.count >= MAX_PATH) return false;
  memory_copy(buffer, path.cstring, path.count);
  buffer[path.count] = 0;
  DWORD attributes = GetFileAttributesA(buffer);
  if (attributes == INVALID_FILE_ATTRIBUTES) return false;
  b32 result = (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
  return result;
}

fz_internal b32
directory_exists(String path)
{
  if (path.count == 0 || path.cstring == 0) return false;
  if (path.count >= MAX_PATH) return false;
  char buffer[MAX_PATH];
  memory_copy(buffer, path.cstring, path.count);
  buffer[path.count] = 0;
  DWORD attributes = GetFileAttributesA(buffer);
  if (attributes == INVALID_FILE_ATTRIBUTES) return false;
  return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

fz_internal b32
file_exists(String path)
{
  if (path.count == 0 || path.cstring == 0) return false;
  if (path.count >= MAX_PATH) return false;
  char buffer[MAX_PATH];
  memory_copy(buffer, path.cstring, path.count);
  buffer[path.count] = 0;
  DWORD attributes = GetFileAttributesA(buffer);
  if (attributes == INVALID_FILE_ATTRIBUTES) return false;
  return (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

fz_internal b32
directory_create(String path)
{
  if (path.count == 0 || path.cstring == 0) return false;
  if (path.count >= MAX_PATH) return false;

  char buffer[MAX_PATH];
  memory_copy(buffer, path.cstring, path.count);
  buffer[path.count] = 0;

  if (CreateDirectoryA(buffer, 0))
  {
    return true;
  }

  DWORD error = GetLastError();
  if (error == ERROR_ALREADY_EXISTS)
  {
    DWORD attributes = GetFileAttributesA(buffer);
    if (attributes != INVALID_FILE_ATTRIBUTES &&
      (attributes & FILE_ATTRIBUTE_DIRECTORY))
    {
      return true;
    }
  }

  return false;
}

fz_internal String
full_path_from_relative_path(Arena* arena, String relative_path)
{
  String result = {0};
  if(relative_path.count == 0 || relative_path.cstring == 0)
  {
    return result;
  }
  
  char exe_path[MAX_PATH];
  DWORD exe_len = GetModuleFileNameA(NULL, exe_path, MAX_PATH);
  if(exe_len == 0)
  {
    return result;
  }
  
  char* last_slash = exe_path + exe_len - 1;
  while(last_slash > exe_path && *last_slash != '\\' && *last_slash != '/')
  {
    last_slash--;
  }
  *(last_slash + 1) = '\0';
  
  size_t exe_dir_len = strlen(exe_path);
  size_t combined_len = exe_dir_len + relative_path.count + 1;
  char* combined = push_array(arena, char, combined_len);
  memcpy(combined, exe_path, exe_dir_len);
  memcpy(combined + exe_dir_len, relative_path.cstring, relative_path.count);
  combined[combined_len - 1] = '\0';
  
  DWORD required_size = GetFullPathNameA(combined, 0, 0, 0);
  if(required_size == 0)
  {
    return result;
  }
  
  u8* buffer = push_array(arena, u8, required_size);
  DWORD written = GetFullPathNameA(combined, required_size, (char*)buffer, 0);
  if(written == 0)
  {
    return result;
  }
  
  result.count = (u64)written;
  result.cstring = buffer;
  return result;
}

#elif OS_LINUX

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

#ifndef PATH_MAX
  #define PATH_MAX 4096
#endif

fz_internal b32 linux_is_dot_dir(const char* name);
fz_internal b32 linux_collect_recursive(Arena* arena, String_List* out, String dir_path);
fz_internal void linux_collect_non_recursive(Arena* arena, String_List* out, String dir_path);

fz_internal b32
linux_is_dot_dir(const char* name)
{
  return (name[0] == '.' && (name[1] == 0 || (name[1] == '.' && name[2] == 0)));
}

fz_internal b32
linux_collect_recursive(Arena* arena, String_List* out, String dir_path)
{
  b32 found_file = 0;
  Scratch scratch = scratch_begin(&arena, 1);
  char* cpath = (char*)push_array(scratch.arena, u8, dir_path.count + 1);
  memory_copy(cpath, dir_path.cstring, dir_path.count);
  cpath[dir_path.count] = 0;

  DIR* dir = opendir(cpath);
  if (!dir)
  {
    scratch_end(&scratch);
    return 0;
  }

  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL)
  {
    if (linux_is_dot_dir(entry->d_name)) continue;

    String name = string_new((u64)strlen(entry->d_name), (u8*)entry->d_name);
    String full_path = string_join(arena, string_join(arena, dir_path, S("/")), name);

    b32 is_dir = false;
    if (entry->d_type == DT_DIR)
    {
      is_dir = true;
    }
    else if (entry->d_type == DT_UNKNOWN)
    {
      is_dir = is_directory(full_path);
    }

    if (is_dir)
    {
      b32 subtree_has_files = linux_collect_recursive(arena, out, full_path);
      if (!subtree_has_files)
      {
        string_list_push(arena, out, full_path);
      }
      found_file |= subtree_has_files;
    }
    else
    {
      string_list_push(arena, out, full_path);
      found_file = 1;
    }
  }

  closedir(dir);
  scratch_end(&scratch);
  return found_file;
}

fz_internal void
linux_collect_non_recursive(Arena* arena, String_List* out, String dir_path)
{
  Scratch scratch = scratch_begin(&arena, 1);
  char* cpath = (char*)push_array(scratch.arena, u8, dir_path.count + 1);
  memory_copy(cpath, dir_path.cstring, dir_path.count);
  cpath[dir_path.count] = 0;

  DIR* dir = opendir(cpath);
  if (!dir)
  {
    scratch_end(&scratch);
    return;
  }

  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL)
  {
    if (linux_is_dot_dir(entry->d_name)) continue;

    String name = string_new((u64)strlen(entry->d_name), (u8*)entry->d_name);
    String full_path = string_join(arena, string_join(arena, dir_path, S("/")), name);
    string_list_push(arena, out, full_path);
  }

  closedir(dir);
  scratch_end(&scratch);
}

fz_internal b32
file_create(String path)
{
  Scratch scratch = scratch_begin(0, 0);
  char* cpath = (char*)push_array(scratch.arena, u8, path.count + 1);
  memory_copy(cpath, path.cstring, path.count);
  cpath[path.count] = 0;

  int fd = open(cpath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  scratch_end(&scratch);

  if (fd < 0) return false;
  close(fd);
  return true;
}

fz_internal b32
file_delete(String path)
{
  Scratch scratch = scratch_begin(0, 0);
  char* cpath = (char*)push_array(scratch.arena, u8, path.count + 1);
  memory_copy(cpath, path.cstring, path.count);
  cpath[path.count] = 0;

  int result = unlink(cpath);
  scratch_end(&scratch);
  return (result == 0);
}

fz_internal u32
file_write(String path, u8* data, u64 data_size)
{
  Scratch scratch = scratch_begin(0, 0);
  char* cpath = (char*)push_array(scratch.arena, u8, path.count + 1);
  memory_copy(cpath, path.cstring, path.count);
  cpath[path.count] = 0;

  int fd = open(cpath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  scratch_end(&scratch);
  if (fd < 0) return 0;

  ssize_t bytes_written = write(fd, data, data_size);
  close(fd);
  return (bytes_written > 0) ? (u32)bytes_written : 0;
}

fz_internal u32
file_write_string(String path, String data)
{
  return file_write(path, data.cstring, data.count);
}

fz_internal u32
file_append(String path, u8* data, u64 data_size)
{
  Scratch scratch = scratch_begin(0, 0);
  char* cpath = (char*)push_array(scratch.arena, u8, path.count + 1);
  memory_copy(cpath, path.cstring, path.count);
  cpath[path.count] = 0;

  int fd = open(cpath, O_WRONLY | O_CREAT | O_APPEND, 0644);
  scratch_end(&scratch);
  if (fd < 0) return 0;

  ssize_t bytes_written = write(fd, data, data_size);
  close(fd);
  return (bytes_written > 0) ? (u32)bytes_written : 0;
}

fz_internal b32
file_wipe(String path)
{
  Scratch scratch = scratch_begin(0, 0);
  char* cpath = (char*)push_array(scratch.arena, u8, path.count + 1);
  memory_copy(cpath, path.cstring, path.count);
  cpath[path.count] = 0;

  int fd = open(cpath, O_WRONLY | O_TRUNC);
  scratch_end(&scratch);
  if (fd < 0) return false;

  close(fd);
  return true;
}

fz_internal u32
file_size(String path)
{
  Scratch scratch = scratch_begin(0, 0);
  char* cpath = (char*)push_array(scratch.arena, u8, path.count + 1);
  memory_copy(cpath, path.cstring, path.count);
  cpath[path.count] = 0;

  struct stat st;
  int res = stat(cpath, &st);
  scratch_end(&scratch);

  if (res != 0) return 0;
  return (u32)st.st_size;
}

fz_internal String
file_load(Arena* arena, String path)
{
  String result = {0};

  Scratch scratch = scratch_begin(&arena, 1);
  char* cpath = (char*)push_array(scratch.arena, u8, path.count + 1);
  memory_copy(cpath, path.cstring, path.count);
  cpath[path.count] = 0;

  int fd = open(cpath, O_RDONLY);
  scratch_end(&scratch);
  if (fd < 0) return result;

  struct stat st;
  if (fstat(fd, &st) == 0)
  {
    u64 sz = (u64)st.st_size;
    u8* buffer = push_array(arena, u8, sz + 1);
    ssize_t bytes_read = read(fd, buffer, sz);
    if (bytes_read == (ssize_t)sz)
    {
      buffer[sz] = '\0';
      result.cstring = buffer;
      result.count = sz;
    }
  }

  close(fd);
  return result;
}

fz_internal String_List
file_get_files_in_path(Arena* arena, String path, b32 recursive)
{
  String_List result = string_list_new();

  if (!directory_exists(path))
  {
    if (file_exists(path))
    {
      string_list_push(arena, &result, string_copy(arena, path));
    }
    return result;
  }

  if (recursive)
  {
    linux_collect_recursive(arena, &result, path);
  }
  else
  {
    linux_collect_non_recursive(arena, &result, path);
  }

  return result;
}

fz_internal String
directory_get_current_working_directory(void)
{
  static u8 buffer[PATH_MAX];

  char* cwd = getcwd((char*)buffer, sizeof(buffer));
  if (!cwd)
  {
    return S("");
  }

  u64 len = (u64)strlen(cwd);
  return (String){ .count = len, .cstring = buffer };
}

fz_internal u64
file_get_last_write_time(String path)
{
  Scratch scratch = scratch_begin(0, 0);
  char* cpath = (char*)push_array(scratch.arena, u8, path.count + 1);
  memory_copy(cpath, path.cstring, path.count);
  cpath[path.count] = 0;

  struct stat st;
  int res = stat(cpath, &st);
  scratch_end(&scratch);

  if (res != 0) return 0;
  return ((u64)st.st_mtim.tv_sec * 1000000000ULL) + (u64)st.st_mtim.tv_nsec;
}

fz_internal b32
is_directory(String path)
{
  if (path.count == 0 || path.cstring == 0) return false;
  Scratch scratch = scratch_begin(0, 0);
  char* cpath = (char*)push_array(scratch.arena, u8, path.count + 1);
  memory_copy(cpath, path.cstring, path.count);
  cpath[path.count] = 0;

  struct stat st;
  int res = stat(cpath, &st);
  scratch_end(&scratch);

  if (res != 0) return false;
  return S_ISDIR(st.st_mode);
}

fz_internal b32
is_file(String path)
{
  if (path.count == 0 || path.cstring == 0) return false;
  Scratch scratch = scratch_begin(0, 0);
  char* cpath = (char*)push_array(scratch.arena, u8, path.count + 1);
  memory_copy(cpath, path.cstring, path.count);
  cpath[path.count] = 0;

  struct stat st;
  int res = stat(cpath, &st);
  scratch_end(&scratch);

  if (res != 0) return false;
  return S_ISREG(st.st_mode);
}

fz_internal b32
directory_exists(String path)
{
  return is_directory(path);
}

fz_internal b32
file_exists(String path)
{
  return is_file(path);
}

fz_internal b32
directory_create(String path)
{
  if (path.count == 0 || path.cstring == 0) return false;

  Scratch scratch = scratch_begin(0, 0);
  char* cpath = (char*)push_array(scratch.arena, u8, path.count + 1);
  memory_copy(cpath, path.cstring, path.count);
  cpath[path.count] = 0;

  if (mkdir(cpath, 0755) == 0)
  {
    scratch_end(&scratch);
    return true;
  }

  b32 exists = (errno == EEXIST) && is_directory(path);
  scratch_end(&scratch);
  return exists;
}

fz_internal String
full_path_from_relative_path(Arena* arena, String relative_path)
{
  String result = {0};
  if (relative_path.count == 0 || relative_path.cstring == 0) return result;

  Scratch scratch = scratch_begin(&arena, 1);
  char exe_path[PATH_MAX];
  ssize_t bytes_read = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
  if (bytes_read <= 0)
  {
    scratch_end(&scratch);
    return result;
  }
  exe_path[bytes_read] = 0;

  char* last_slash = strrchr(exe_path, '/');
  if (last_slash)
  {
    *(last_slash + 1) = 0;
  }

  size_t exe_dir_len = strlen(exe_path);
  size_t combined_len = exe_dir_len + relative_path.count + 1;
  char* combined = push_array(scratch.arena, char, combined_len);
  memory_copy(combined, exe_path, exe_dir_len);
  memory_copy(combined + exe_dir_len, relative_path.cstring, relative_path.count);
  combined[combined_len - 1] = 0;

  char resolved[PATH_MAX];
  if (realpath(combined, resolved) != NULL)
  {
    u64 res_len = (u64)strlen(resolved);
    u8* buffer = push_array(arena, u8, res_len + 1);
    memory_copy(buffer, resolved, res_len);
    buffer[res_len] = 0;

    result.count = res_len;
    result.cstring = buffer;
  }

  scratch_end(&scratch);
  return result;
}

#else
# error Operating System not supported
#endif

// Cross-Platform Utilities
fz_internal String
file_get_extension(String path)
{
  String result = {0};

  if(path.count == 0 || path.cstring == 0) return result;

  s64 last_dot = -1;

  for(s64 i = 0; i < (s64)path.count; ++i)
  {
    u8 c = path.cstring[i];

    if(c == '/' || c == '\\')
    {
      last_dot = -1;
    }
    else if(c == '.')
    {
      last_dot = i;
    }
  }

  if(last_dot < 0 || last_dot + 1 >= (s64)path.count) return result;

  result.cstring = path.cstring + last_dot + 1;
  result.count    = path.count - (last_dot + 1);

  return result;
}

fz_internal String 
file_get_name_no_extension(String path)
{
  String result = S("");
  u64 dot_position;
  if (string_find_last(path, S("."), &dot_position))
  {
    result.cstring = path.cstring;
    result.count   = dot_position;
    for (s64 i = (s64)result.count - 1; i >= 0; i -= 1)
    {
      if (result.cstring[i] == '\\' || result.cstring[i] == '/')
      {
        result.cstring = result.cstring + i + 1;
        result.count   = result.count - i - 1;
        break;
      }
    }
  }
  return result;
}

fz_internal String 
directory_pop(String path)
{
  s64 index = -1;
  for (s64 i = (s64)path.count - 1; i >= 0; i--)
  {
    if (path.cstring[i] == '\\' || path.cstring[i] == '/')
    {
      index = i;
      break;
    }
  }

  if (index < 0)
  {
    return S("");
  }
  String result;
  result.cstring = path.cstring;
  result.count   = (u64)index;

  return result;
}

fz_internal String 
directory_push(Arena *arena, String path, String dir)
{
#if OS_WINDOWS
  String sep = S("\\");
#else
  String sep = S("/");
#endif
  String result = string_join(arena, path, sep);
  result = string_join(arena, result, dir);
  return result;
}

fz_internal void
file_watch_init(Arena *arena, File_Watcher *watch, String path)
{
  if (file_exists(path))
  {
    watch->path = string_copy(arena, path);
    watch->last_write_time = file_get_last_write_time(path);
  }
  else
  {
    printf("Tried to init a file watch with a path that wasn't found: " S_FMT "\n", S_ARG(path));
  }
}

fz_internal b32
file_watch_changed(File_Watcher *watch)
{
  b32 result = false;
  u64 new_time = file_get_last_write_time(watch->path);
  if (new_time != watch->last_write_time)
  {
    watch->last_write_time = new_time;

    f64 current_time = time_seconds();
    if ((current_time - watch->last_triggered_time) > 0.1)
    {
      watch->last_triggered_time = current_time;
      result = true;
    }
  }
  return result;
}

#endif // FILES_H
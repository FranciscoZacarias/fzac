#ifndef FILES_H
#define FILES_H

typedef struct File_Watcher File_Watcher;
struct File_Watcher
{
  String path;
  u64 last_write_time;
  f64 last_triggered_time;
};

// @Section: File and directory
function b32    file_create(String path); /* Creates file. If file exists, returns true anyway. */
function b32    file_delete(String path); /* Deletes a file */
function u32    file_write(String path, u8* data, u64 data_size); /* Wipes file contents and writes data */
function u32    file_append(String path, u8* data, u64 data_size); /* Appends data to the file */
function b32    file_wipe(String path); /* Clears file */
function u32    file_size(String path); /* Returns the size of the file */
function String file_load(Arena* arena, String path); /* Loads file into memory */
function String_List file_get_files_in_path(Arena* arena, String path, b32 recursive); /* Returns all files in a given path. If recursive is false: returns immediate files and directories in path. If recursive is true: returns all files recursively from directory specified. */
function String file_get_extension(String path); /* Returns file extension. */

// File watch
function void file_watch_init(Arena *arena, File_Watcher *watch, String path);
function u64  file_get_last_write_time(String path);
function b32  file_watch_changed(File_Watcher *watch);

function b32 file_exists(String path);
function b32 is_file(String path);

function b32 directory_create(String path);
function b32 is_directory(String path);
function b32 directory_exists(String path);

function String full_path_from_relative_path(Arena* arena, String relative_path);

#if OS_WINDOWS

function wchar_t* utf8_to_wide(Arena* arena, u8* utf8, s64 utf8_size);
function void     win32_log_error(const char* msg, String path);
function b32      win32_is_dot_dir(const char* name);
function b32      win32_collect_recursive(Arena* arena, String_List* out, String dir_path);
function void     win32_collect_non_recursive(Arena* arena, String_List* out, String dir_path);

function wchar_t*
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

function void
win32_log_error(const char* msg, String path)
{
  char buffer[512];
  wsprintfA(buffer, "%s: %.*s\n", msg, (s32)path.count, path.cstring);
  OutputDebugStringA(buffer);
}

function b32
win32_is_dot_dir(const char* name)
{
  b32 result = (name[0] == '.' && (name[1] == 0 || (name[1] == '.' && name[2] == 0)));
  return result;
}

function b32
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

function void
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

function b32
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
    // @TODO(fz): Handle error
    result = false;
  }
  return result;
}

function b32
file_delete(String path)
{
  Scratch scratch = scratch_begin(0,0);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.count);
  b32 result = DeleteFileW(wpath);
  scratch_end(&scratch);
  return result;
}

function u32
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

function u32
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

function b32
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

function u32
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

function String
file_load(Arena* arena, String path)
{
  String result = {0};

  Scratch scratch = scratch_begin(&arena,1);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.count);
  HANDLE file = CreateFileW(wpath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  scratch_end(&scratch);

  if(file == INVALID_HANDLE_VALUE) return result;

  DWORD file_size = GetFileSize(file, 0);
  u8* buffer = push_array(arena, u8, file_size + 1); // +1 for null terminator
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

function String_List
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

function String
file_get_extension(String path)
{
  String result = {0};

  if(path.count == 0 || path.cstring == 0) return result;

  s64 last_dot = -1;
  s64 last_sep = -1;

  for(s64 i = 0; i < (s64)path.count; ++i)
  {
    u8 c = path.cstring[i];

    if(c == '/' || c == '\\')
    {
      last_sep = i;
      last_dot = -1; // dots before a separator don't count
    }
    else if(c == '.')
    {
      last_dot = i;
    }
  }

  if(last_dot < 0 || last_dot + 1 >= (s64)path.count) return result;

  result.cstring = path.cstring + last_dot + 1;
  result.count   = path.count - (last_dot + 1);

  return result;
}

function void
file_watch_init(Arena *arena, File_Watcher *watch, String path)
{
  watch->path = string_copy(arena, path);
  watch->last_write_time = file_get_last_write_time(path);
}

function u64
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

function b32
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

function b32
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

function b32
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

function b32
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

function b32
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

function b32
directory_create(String path)
{
  if (path.count == 0 || path.cstring == 0) return false;
  if (path.count >= MAX_PATH) return false;

  char buffer[MAX_PATH];
  memory_copy(buffer, path.cstring, path.count);
  buffer[path.count] = 0;

  // CreateDirectory succeeds only if the full parent path already exists
  if (CreateDirectoryA(buffer, 0))
  {
    return true;
  }

  // If it already exists and is a directory, treat as success
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

function String
full_path_from_relative_path(Arena* arena, String relative_path)
{
  String result = {0};
  if(relative_path.count == 0 || relative_path.cstring == 0)
  {
    return result;
  }
  
  // Get executable's full path
  char exe_path[MAX_PATH];
  DWORD exe_len = GetModuleFileNameA(NULL, exe_path, MAX_PATH);
  if(exe_len == 0)
  {
    return result;
  }
  
  // Find last backslash to get directory only
  char* last_slash = exe_path + exe_len - 1;
  while(last_slash > exe_path && *last_slash != '\\' && *last_slash != '/')
  {
    last_slash--;
  }
  *(last_slash + 1) = '\0';  // Keep the backslash
  
  // Combine exe directory with relative path
  size_t exe_dir_len = strlen(exe_path);
  size_t combined_len = exe_dir_len + relative_path.count + 1;
  char* combined = push_array(arena, char, combined_len);
  memcpy(combined, exe_path, exe_dir_len);
  memcpy(combined + exe_dir_len, relative_path.cstring, relative_path.count);
  combined[combined_len - 1] = '\0';
  
  // NOW resolve the path (this handles .. and . properly)
  DWORD required_size = GetFullPathNameA(combined, 0, 0, 0);
  if(required_size == 0)
  {
    printf("GetFullPathNameA failed (first call)\n");
    return result;
  }
  
  u8* buffer = push_array(arena, u8, required_size);
  DWORD written = GetFullPathNameA(combined, required_size, (char*)buffer, 0);
  if(written == 0)
  {
    printf("GetFullPathNameA failed (second call)\n");
    return result;
  }
  
  result.count = (u64)written;
  result.cstring = buffer;
  return result;
}

#else
# error Operating System not supported
#endif

#endif // FILES_H
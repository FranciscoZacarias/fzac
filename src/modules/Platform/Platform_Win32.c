function wchar_t*
utf8_to_wide(Arena* arena, u8* utf8, s64 utf8_size)
{
  if (utf8_size == 0 || utf8 == 0) return NULL;
  s32 required = MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,(char*)utf8,(s32)utf8_size,0,0);
  if (required <= 0) return NULL;

  wchar_t* wstr = push_array(arena, wchar_t, required);
  MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,(char*)utf8,(s32)utf8_size,wstr,required);
  return wstr;
}

function Path
path_from_string(Arena* arena, String input)
{
  Path result = {0};
  // @TODO(Fz): finish
  return result;
}

function b32
file_create(String path)
{
  b32 result  = true;
  Scratch scratch = scratch_begin(0,0);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.size);
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
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.size);
  b32 result = DeleteFileW(wpath);
  scratch_end(&scratch);
  return result;
}

function b32
file_exists(String path)
{
  Scratch scratch = scratch_begin(0,0);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.size);
  DWORD attr = GetFileAttributesW(wpath);
  scratch_end(&scratch);
  b32 result = (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
  return result;
}

function u32
file_write(String path, u8* data, u64 data_size)
{
  Scratch scratch = scratch_begin(0,0);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.size);
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
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.size);
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
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.size);
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
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.size);
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

  Scratch scratch = scratch_begin(0,0);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.size);
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
    result.size    = file_size;
  }

  CloseHandle(file);
  return result;
}
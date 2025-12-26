function void
use_console()
{
  if (!AttachConsole(ATTACH_PARENT_PROCESS))
  {
    // No parent console (e.g. double-clicked exe)
    if (!AllocConsole())
    {
      // @TODO(fz): Handle error
      return;
    }
  }

  FILE* fp;
  freopen_s(&fp, "CONOUT$", "w", stdout);
  freopen_s(&fp, "CONOUT$", "w", stderr);
  freopen_s(&fp, "CONIN$",  "r", stdin);

  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  if (handle != INVALID_HANDLE_VALUE)
  {
    DWORD mode = 0;
    if (GetConsoleMode(handle, &mode))
    {
      SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
  }
}


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

function b32
path_is_char8_separator_slash(u8 c)
{
  b32 result = (c == '/' || c == '\\' );
  return result;
}

function Path
path_from_string(Arena* arena, String input)
{
  Path result = {0};

  Scratch scratch = scratch_begin(0,0);

  u64 i = 0;
  while (i < input.count && path_is_char8_separator_slash(input.cstring[i]))
  {
    result.number_of_leading_slashes += 1;
    i += 1;
  }

  if (input.count > 0 && path_is_char8_separator_slash(input.cstring[input.count - 1]))
  {
    result.trailing_slash = true;
  }

  // Split into words
  String_List words = string_list_new();
  u64 word_start = i;
  for (; i <= input.count; i += 1)
  {
    b32 at_end   = (i == input.count);
    b32 is_slash = (!at_end && path_is_char8_separator_slash(input.cstring[i]));

    if (at_end || is_slash)
    {
      if (i > word_start)
      {
        String part = string_range(input.cstring + word_start, input.cstring + i);
        string_list_push(scratch.arena, &words, part);
      }
      word_start = i + 1;
    }
  }

  // Parse header (E.g. C:)
  if (words.node_count > 0)
  {
    String first = words.first->value;
    if (first.count >= 2 && first.cstring[first.count - 1] == ':')
    {
      result.header_string = string_copy(arena, first);
      string_list_remove_first(&words);
    }
  }

  // Final word array
  result.word_count = (u32)words.node_count;
  if (result.word_count > 0)
  {
    result.words = push_array(arena, String, result.word_count);

    String_Node* node = words.first;
    for (u32 w = 0; w < result.word_count; w += 1)
    {
      result.words[w] = string_copy(arena, node->value);
      node = node->next;
    }
  }

  scratch_end(&scratch);
  return result;
}

function String
string_from_path(Arena* arena, Path path)
{
  String_List out = string_list_new();

  if (path.header_string.count > 0)
  {
    string_list_push(arena, &out, path.header_string);
  }

  for (u32 i = 0; i < path.number_of_leading_slashes; i += 1)
  {
    string_list_push(arena, &out, S("\\"));
  }

  for (u32 i = 0; i < path.word_count; i += 1)
  {
    if (i > 0 || path.number_of_leading_slashes > 0 || path.header_string.count > 0)
    {
      string_list_push(arena, &out, S("\\"));
    }
    string_list_push(arena, &out, path.words[i]);
  }

  if (path.trailing_slash && path.word_count > 0)
  {
    string_list_push(arena, &out, S("\\"));
  }

  return string_list_join(arena, &out);
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

function b32
file_exists(String path)
{
  Scratch scratch = scratch_begin(0,0);
  wchar_t* wpath = utf8_to_wide(scratch.arena, path.cstring, path.count);
  DWORD attr = GetFileAttributesW(wpath);
  scratch_end(&scratch);
  b32 result = (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
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

  Scratch scratch = scratch_begin(0,0);
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
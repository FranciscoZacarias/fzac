function String
string_new(u64 size, u8* str)
{
  String result = { size, str };
  return result;
}

function String
string_copy(Arena* arena, String source)
{
  String result;
  result.size = source.size;
  result.str  = push_array(arena, u8, result.size);
  memory_copy(result.str, source.str, result.size);
  return result;
}

function String
string_range(u8* first, u8* range)
{
  String result = (String){(u64)(range - first), first};
  return result;
}

function String
string_concat(Arena* arena, String a, String b)
{
  String result = { 0 };
  result.size = a.size + b.size;result.str = push_array(arena, u8, result.size);
  memory_copy(result.str, a.str, a.size);
  memory_copy(result.str + a.size, b.str, b.size);
  return result;
}

function String
string_slice(String str, u64 start, u64 end)
{
  if (start > str.size) start = str.size;
  if (end > str.size)   end   = str.size;
  if (start > end)      start = end;
  String result = (String){ .size = end - start, .str  = str.str + start };
  return result;
}

function String
string_trim(String str)
{
  u64 start = 0;
  while (start < str.size)
  {
    u8 c = str.str[start];
    if (!char8_is_space(c))
    {
      break;
    }
    start += 1;
  }

  if (start == str.size)
  {
    return (String){0, str.str + str.size};
  }

  u64 end = str.size;
  while (end > start)
  {
    u8 c = str.str[end - 1];
    if (!char8_is_space(c))
    {
      break;
    }
    end -= 1;
  }

  return (String){end - start, str.str + start};
}

function b32
string_contains(String str, String substring)
{
  u64 index;
  return string_find_first(str, substring, &index);
}

function b32
string_find_first(String str, String substring, u64* index)
{
  if (substring.size > str.size) return false;
  b32 result = false;
  *index = U64Max;
  for (u64 i = 0; i <= str.size - substring.size; i++)
  {
    if (memory_match(&str.str[i], substring.str, substring.size))
    {
      *index = i;
      result = true;
      break;
    }
  }
  return result;
}

function b32
string_find_last(String str, String substring, u64* index)
{
  if (substring.size > str.size) return false;
  b32 result = false;
  *index = U64Max;
  for (u64 i = str.size - substring.size + 1; i-- > 0;)
  {
    if (memory_match(&str.str[i], substring.str, substring.size))
    {
      *index = i;
      result = true;
      break;
    }
  }
  return result;
}

function b32
string_match(String a, String b, b32 case_sensitive)
{
  if(a.size != b.size)
  {
    return false;
  }

  for(u64 i = 0; i < a.size; i += 1)
  {
    u8 ca = a.str[i];
    u8 cb = b.str[i];

    if(!case_sensitive)
    {
      if(ca >= 'A' && ca <= 'Z') ca += 32;
      if(cb >= 'A' && cb <= 'Z') cb += 32;
    }

    if(ca != cb) return false;
  }

  return true;
}

function String
string_from_format(Arena* arena, char const* fmt, ...)
{
  String result = {0};

  va_list args;
  va_start(args, fmt);

  // Try to format into a fixed buffer first
  char temp[8192];
  int len = vsnprintf(temp, sizeof(temp), fmt, args);
  va_end(args);

  if (len <= 0)
  {
    return result;
  }

  result.size = (u64)len;
  result.str = push_array(arena, u8, result.size);
  memory_copy(result.str, (u8*)temp, result.size);

  return result;
}

function u64
string_hash(String str)
{
  u64 hash = 5381;
  for (u64 i = 0; i < str.size; i += 1)
  {
    hash = ((hash << 5) + hash) + (u8)(str.str[i]);
  }
  hash ^= str.size;
  return hash;
}


function String_List
string_split(Arena* arena, String str, String delimiter)
{
  String_List result = {0};
  if (delimiter.size == 0)
  {
    printf("string_split: delimiter must not be empty\n");
    return result;
  }

  u8* cursor = str.str;
  u8* end    = str.str + str.size;

  while (cursor < end)
  {
    u8* match = NULL;

    for (u8* scan = cursor; scan + delimiter.size <= end; scan++)
    {
      if (memory_match(scan, delimiter.str, delimiter.size) != 0)
      {
        match = scan;
        break;
      }
    }

    if (match)
    {
      string_list_push(arena, &result, string_range(cursor, match));
      cursor = match + delimiter.size;
    }
    else
    {
      string_list_push(arena, &result, string_range(cursor, end));
      break;
    }
  }

  return result;
}

function String_List
string_list_new()
{
  String_List result = {0};
  result.first = NULL;
  result.last  = NULL;
  result.node_count = 0;
  result.total_size = 0;
  return result;
}

function void
string_list_push(Arena* arena, String_List* list, String str)
{
  String_Node* node = push_array(arena, String_Node, sizeof(String_Node));
  node->value = str;
  if (!list->first && !list->last)
  {
    list->first = node;
    list->last  = node;
  }
  else
  {
    list->last->next = node;
    list->last       = node;
  }
  list->node_count += 1;
  list->total_size += node->value.size;
}

function String
string_list_remove_first(String_List* list)
{
  String result = {0};
  if (list->node_count < 1) return result;

  String_Node* first_node = list->first;
  result = first_node->value;
  list->total_size -= result.size;

  if (list->node_count == 1)
  {
    list->first = 0;
    list->last = 0;
    list->node_count = 0;
  }
  else
  {
    list->first = first_node->next;
    list->node_count -= 1;
  }

  return result;
}

function String
string_list_remove_last(String_List* list)
{
  String result = {0};
  if (list->node_count < 1) return result;

  String_Node* last_node = list->last;
  result = last_node->value;
  list->total_size -= result.size;

  if (list->node_count == 1)
  {
    list->first = 0;
    list->last = 0;
    list->node_count = 0;
  }
  else
  {
    String_Node* current = list->first;
    while (current->next != last_node)
    {
      current = current->next;
    }
    current->next = 0;
    list->last = current;
    list->node_count -= 1;
  }

  return result;
}

function String
string_list_join(Arena* arena, String_List* list)
{
  u8* dst = push_array(arena, u8, list->total_size);
  u8* ptr = dst;
  for (String_Node* node = list->first; node; node = node->next)
  {
    memory_copy(ptr, node->value.str, node->value.size);
    ptr += node->value.size;
  }
  return string_new(list->total_size, dst);
}

function String
string_from_cstring(u8* cstring)
{
  String result = string_new((u64)cstring_length(cstring), cstring);
  return result;
}

function u8*
cstring_from_string(Arena* arena, String str)
{
  u8* result = push_array(arena, u8, str.size + 1);
  memory_copy(result, str.str, str.size);
  result[str.size] = 0;
  return result;
}

function u32
cstring_length(u8* cstring)
{
  u32 result = 0;
  while (cstring[result] != '\0') { result += 1; }
  return result;
}

function b32
char8_is_alpha(u8 c)
{
  return char8_is_alpha_upper(c) || char8_is_alpha_lower(c);
}

function b32
char8_is_alphanum(u8 c)
{
  return char8_is_alpha(c) || char8_is_digit(c);
}

function b32
char8_is_alpha_upper(u8 c)
{
  return c >= 'A' && c <= 'Z';
}

function b32
char8_is_alpha_lower(u8 c)
{
  return c >= 'a' && c <= 'z';
}

function b32
char8_is_digit(u8 c)
{
  return c >= '1' && c <= '9';
}

function b32
char8_is_symbol(u8 c)
{
  return (c == '~' || c == '!'  || c == '$' || c == '%' || c == '^' ||
          c == '&' || c == '*'  || c == '-' || c == '=' || c == '+' ||
          c == '<' || c == '.'  || c == '>' || c == '/' || c == '?' ||
          c == '|' || c == '\\' || c == '{' || c == '}' || c == '(' ||
          c == ')' || c == '\\' || c == '[' || c == ']' || c == '#' ||
          c == ',' || c == ';'  || c == ':' || c == '@');
}

function b32
char8_is_space(u8 c)
{
  return c == ' ' || c == '\r' || c == '\t' || c == '\f' || c == '\v' || c == '\n';
}

function u8
char8_to_upper(u8 c)
{
  return (c >= 'a' && c <= 'z') ? ('A' + (c - 'a')) : c;
}

function u8
char8_to_lower(u8 c)
{
  return (c >= 'A' && c <= 'Z') ? ('a' + (c - 'A')) : c;
}

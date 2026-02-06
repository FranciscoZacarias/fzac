#ifndef STRING_H
#define STRING_H

// @Section: 8 bit characters
function b32 char8_is_alpha(u8 c); /* Check if character is alphabetic. */
function b32 char8_is_alphanum(u8 c); /* Check if character is alphanumeric. */
function b32 char8_is_alpha_upper(u8 c); /* Check if character is uppercase letter. */
function b32 char8_is_alpha_lower(u8 c); /* Check if character is lowercase letter. */
function b32 char8_is_digit(u8 c); /* Check if character is digit 0-9. */
function b32 char8_is_symbol(u8 c); /* Check if character is symbol/punctuation. */
function b32 char8_is_space(u8 c); /* Check if character is whitespace. */
function u8  char8_to_upper(u8 c); /* Convert character to uppercase. */
function u8  char8_to_lower(u8 c); /* Convert character to lowercase. */

// @Section: 8 Bit strings
typedef struct String String; /* 8 bit string. */
raddbg_type_view(String, cstring);
struct String
{
  u64 count;    /* Length of string (excluding null terminator) */
  u8* cstring;  /* Null-terminated string */
};
#define S(s) (String){sizeof(s)-1,(u8*)(s)}
#define Sf(arena,fmt,...) string_from_format(arena, fmt, __VA_ARGS__)

#define S_FMT "%.*s"
#define S_ARG(s) (s32)s.count, s.cstring

typedef struct String_View String_View; /* This exists so that we explicitly know when a string is a view into another string's memory, and the caller is explicitly aware that the lifetime of this string depends on another */
struct String_View
{
  u64 count;
  u8* string;
};

typedef struct String_Node String_Node; /* 8 bit string node */
struct String_Node
{
  String value;
  String_Node* next;
};

typedef struct String_List String_List; /* 8 bit string list */
struct String_List
{
  String_Node* first;
  String_Node* last;
  u64 node_count;
  u64 total_size;
};

function String string_zero(); /* Initilizese a string with memory zero'd out */
function String string_new(u64 size, u8* str); /* Create a new String with given size and data pointer (allocates and null-terminates). */
function String string_copy(Arena* arena, String source); /* Allocate and copy source string into arena (null-terminated). */
function String string_range(Arena* arena, u8* first, u8* range); /* Create null-terminated String from first pointer to range pointer (exclusive). */
function String string_join(Arena* arena, String a, String b); /* Allocate concatenated string a+b in arena (null-terminated). */
function String string_replace_first(Arena* arena, String str, String a, String b); /* Replaces string a with string c in string str */
function String string_replace_all(Arena *arena, String str, String a, String b); /* Replaces all instances of a substr a with substr b */
function String string_replace_range(Arena* arena, String str, u64 start, u64 length, String replacement); /* Replaces a range starting at start up to length with replacement */
function String string_replace_backslash_n(Arena *arena, String str); /* Replaces the string "\n" with the characater \n */
function String string_trim(Arena* arena, String str); /* Remove leading and trailing whitespace (null-terminated). */
function String string_substring(Arena* arena, String str, u64 start, u64 end); /* Returns a null-terminated substring */
function b32    string_contains(String str, String substring); /* Check if str contains substring. */
function String string_to_lower(Arena* arena, String str); /* Returns the same string but in lowercase. */
function b32    string_find_first(String str, String substring, u64* index); /* Find first occurrence of substring, write index. */
function b32    string_find_last(String str, String substring, u64* index); /* Find last occurrence of substring, write index. */
function String string_from_format(Arena* arena, char const* fmt, ...); /* Printf-style string formatting into arena. */
function String string_from_format_va(Arena *arena, char const *fmt, va_list args); /* Creates a string from var args */
function b32    string_equals(String a, String b, b32 case_sensitive); /* Compare strings for equality with case sensitivity option. */
function String string_from_format(Arena* arena, char const* fmt, ...); /* Printf-style string formatting into arena (null-terminated). */
function u64    string_hash(String str); /* Hashes a string into a u64 */
function void   string_print(String str); /* Prints a string */

function String_List string_split(Arena* arena, String str, String split_character); /* Split string by delimiter into list. */
function String_List string_list_new(); /* Create new list with single string element. */
function void        string_list_push(Arena* arena, String_List* list, String str); /* Add string to end of list. */
function void        string_list_push_after(String_List* list, String_Node* prev, String_Node* node); /* Adds a <node> to a string list after the node <prev> */
function String      string_list_remove_first(String_List* list); /* Remove and return first element from list. */
function String      string_list_remove_last(String_List* list); /* Remove and return last element from list. */
function String      string_list_join(Arena* arena, String_List* list); /* Concatenate all list elements into single string (null-terminated). */

function String  string_from_cstring(u8* cstring); /* Create String from null-terminated C string. */
function u64     cstring_length(u8* cstring); /* Get length of null-terminated C string. */
function b32     s64_from_string(String str, s64* out); /* Converts a String to an s64 */

#if OS_WINDOWS
// @Section: 16 bit character
// Pretty much only used for interfacing with windows api
typedef struct String_Wide String_Wide;
struct String_Wide
{
  u64 size;
  LPCWSTR str;
};
#endif

// @Section: 32 bit characters
typedef u32 Rune; /* 32 bit character */

// @Section: 32 bit strings
typedef struct Text Text; /* 32 bit string */
struct Text
{
  u64 size;
  Rune* str;
};

// @Section: 8 Bit character implementation

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
  return c >= '0' && c <= '9';
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
  u8 is_lower = (c >= 'a') & (c <= 'z');
  return c - is_lower * ('a' - 'A');
}

function u8
char8_to_lower(u8 c)
{
  u8 is_upper = (c >= 'A') & (c <= 'Z');
  return c + is_upper * ('a' - 'A');
}

// @Section: 8 Bit string implementation

function String
string_zero()
{
  String result;
  memory_zero_struct(&result);
  return result;
}

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
  result.count = source.count;
  result.cstring  = arena_push(arena, u8, result.count + 1);
  memory_copy(result.cstring, source.cstring, result.count);
  result.cstring[result.count] = '\0';
  return result;
}

function String
string_range(Arena* arena, u8* first, u8* range)
{
  u64 count = (u64)(range - first);
  String result;
  result.count = count;
  result.cstring = arena_push(arena, u8, count + 1);
  memory_copy(result.cstring, first, count);
  result.cstring[count] = '\0';
  return result;
}

function String
string_join(Arena* arena, String a, String b)
{
  String result = { 0 };
  result.count = a.count + b.count;
  result.cstring = arena_push(arena, u8, result.count + 1);
  memory_copy(result.cstring, a.cstring, a.count);
  memory_copy(result.cstring + a.count, b.cstring, b.count);
  result.cstring[result.count] = '\0';
  return result;
}

function String
string_replace_first(Arena* arena, String str, String a, String b)
{
  String result = str;
  u64 index = 0;
  
  if (string_find_first(str, a, &index))
  {
    u64 new_size = str.count - a.count + b.count;
    u8* new_str = arena_push(arena, u8, new_size + 1);
    
    memory_copy(new_str, str.cstring, index);
    memory_copy(new_str + index, b.cstring, b.count);
    memory_copy(new_str + index + b.count, str.cstring + index + a.count, str.count - index - a.count);
    
    new_str[new_size] = 0;
    result.count = new_size;
    result.cstring = new_str;
  }
  
  return result;
}

function String
string_replace_all(Arena *arena, String str, String a, String b)
{
  String result = str;
  u64 index = 0;
  
  while (string_find_first(result, a, &index))
  {
    u64 new_size = result.count - a.count + b.count;
    u8* new_str = arena_push(arena, u8, new_size + 1);
    
    memory_copy(new_str, result.cstring, index);
    memory_copy(new_str + index, b.cstring, b.count);
    memory_copy(new_str + index + b.count, result.cstring + index + a.count, result.count - index - a.count);
    
    new_str[new_size] = 0;
    result.count = new_size;
    result.cstring = new_str;
  }
  
  return result;
}

function String
string_replace_range(Arena* arena, String str, u64 start, u64 length, String replacement)
{
  u64 new_size = str.count - length + replacement.count;
  u8* new_str  = arena_push(arena, u8, new_size + 1);
  
  memory_copy(new_str, str.cstring, start);
  memory_copy(new_str + start, replacement.cstring, replacement.count);
  memory_copy(new_str + start + replacement.count, str.cstring + start + length, str.count - start - length);
  
  new_str[new_size] = 0;
  String result;
  result.count = new_size;
  result.cstring = new_str;
  
  return result;
}

function String
string_replace_backslash_n(Arena *arena, String in)
{
  u8 *destination = arena_push(arena, u8, in.count + 1);
  u64 destination_count = 0;

  for(u64 i = 0; i < in.count; i++)
  {
    if(in.cstring[i] == '\\' && i + 1 < in.count && in.cstring[i + 1] == 'n')
    {
      destination[destination_count++] = '\n';
      i += 1;
    }
    else
    {
      destination[destination_count++] = in.cstring[i];
    }
  }

  destination[destination_count] = 0;

  String result;
  result.count   = destination_count;
  result.cstring = destination;

  return result;
}

function String
string_trim(Arena* arena, String str)
{
  u64 start = 0;
  while (start < str.count)
  {
    u8 c = str.cstring[start];
    if (!char8_is_space(c))
    {
      break;
    }
    start += 1;
  }

  if (start == str.count)
  {
    String result;
    result.count = 0;
    result.cstring = arena_push(arena, u8, 1);
    result.cstring[0] = '\0';
    return result;
  }

  u64 end = str.count;
  while (end > start)
  {
    u8 c = str.cstring[end - 1];
    if (!char8_is_space(c))
    {
      break;
    }
    end -= 1;
  }

  u64 count = end - start;
  String result;
  result.count = count;
  result.cstring = arena_push(arena, u8, count + 1);
  memory_copy(result.cstring, str.cstring + start, count);
  result.cstring[count] = '\0';
  return result;
}

function String
string_substring(Arena* arena, String str, u64 start, u64 end)
{
  String result = {0};
  if (end > start)
  {
    u64 count = end - start;
    result.count = count;
    result.cstring = arena_push(arena, u8, count + 1);
    memory_copy(result.cstring, str.cstring + start, count);
    result.cstring[count] = '\0';
  }
  else
  {
    result.cstring = arena_push(arena, u8, 1);
    result.cstring[0] = '\0';
  }
  return result;
}

function b32
string_contains(String str, String substring)
{
  u64 index;
  return string_find_first(str, substring, &index);
}

function String
string_to_lower(Arena* arena, String str)
{
  String result = string_copy(arena, str);
  for (u32 i = 0; i < str.count; i += 1)
  {
    result.cstring[i] = char8_to_lower(result.cstring[i]);
  }
  return result;
}

function b32
string_find_first(String str, String substring, u64* index)
{
  if (substring.count > str.count) return false;
  b32 result = false;
  *index = U64_MAX;
  for (u64 i = 0; i <= str.count - substring.count; i++)
  {
    if (memory_match(&str.cstring[i], substring.cstring, substring.count))
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
  if (substring.count > str.count) return false;
  b32 result = false;
  *index = U64_MAX;
  for (u64 i = str.count - substring.count + 1; i-- > 0;)
  {
    if (memory_match(&str.cstring[i], substring.cstring, substring.count))
    {
      *index = i;
      result = true;
      break;
    }
  }
  return result;
}

function b32
string_equals(String a, String b, b32 case_sensitive)
{
  if(a.count != b.count)
  {
    return false;
  }

  for(u64 i = 0; i < a.count; i += 1)
  {
    u8 ca = a.cstring[i];
    u8 cb = b.cstring[i];

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
  Scratch scratch = scratch_begin(0,0);
  String result = {0};

  va_list args;
  va_start(args, fmt);

  // Try to format into a fixed buffer first
  int count = kilobytes(8);
  char *temp = arena_push(scratch.arena, char, count);
  int len = vsnprintf(temp, count, fmt, args);
  va_end(args);

  if (len <= 0)
  {
    result.cstring = arena_push(arena, u8, 1);
    result.cstring[0] = '\0';
    scratch_end(&scratch);
    return result;
  }

  result.count = (u64)len;
  result.cstring = arena_push(arena, u8, result.count + 1);
  memory_copy(result.cstring, (u8*)temp, result.count);
  result.cstring[result.count] = '\0';

  scratch_end(&scratch);
  return result;
}

function String
string_from_format_va(Arena *arena, char const *fmt, va_list args)
{
  Scratch scratch = scratch_begin(0,0);
  String result = {0};

  va_list args_copy;
  va_copy(args_copy, args);

  int count = kilobytes(8);
  char *temp = arena_push(scratch.arena, char, count);

  int len = vsnprintf(temp, count, fmt, args_copy);
  va_end(args_copy);

  if (len <= 0)
  {
    result.cstring = arena_push(arena, u8, 1);
    result.cstring[0] = 0;
    scratch_end(&scratch);
    return result;
  }

  // If buffer too small, re-run exactly sized
  if (len >= count)
  {
    temp = arena_push(scratch.arena, char, len + 1);

    va_list args_copy2;
    va_copy(args_copy2, args);
    vsnprintf(temp, len + 1, fmt, args_copy2);
    va_end(args_copy2);
  }

  result.count = (u64)len;
  result.cstring = arena_push(arena, u8, result.count + 1);
  memory_copy(result.cstring, (u8*)temp, result.count);
  result.cstring[result.count] = 0;

  scratch_end(&scratch);
  return result;
}

function u64
string_hash(String str)
{
  u64 hash = 5381;
  for (u64 i = 0; i < str.count; i += 1)
  {
    hash = ((hash << 5) + hash) + (u8)(str.cstring[i]);
  }
  hash ^= str.count;
  return hash;
}

function void
string_print(String str)
{
  printf(S_FMT, S_ARG(str));
}

function String_List
string_split(Arena* arena, String str, String delimiter)
{
  String_List result = {0};
  if (delimiter.count == 0)
  {
    printf("string_split: delimiter must not be empty\n");
    return result;
  }

  u8* cursor = str.cstring;
  u8* end    = str.cstring + str.count;

  while (cursor < end)
  {
    u8* match = NULL;

    for (u8* scan = cursor; scan + delimiter.count <= end; scan++)
    {
      if (memory_match(scan, delimiter.cstring, delimiter.count) != 0)
      {
        match = scan;
        break;
      }
    }

    if (match)
    {
      string_list_push(arena, &result, string_range(arena, cursor, match));
      cursor = match + delimiter.count;
    }
    else
    {
      string_list_push(arena, &result, string_range(arena, cursor, end));
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
  String_Node* node = arena_push(arena, String_Node, sizeof(String_Node));
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
  list->total_size += node->value.count;
}

function void
string_list_push_after(String_List* list, String_Node* prev, String_Node* node)
{
  assert(prev);
  assert(node);

  node->next = prev->next;
  prev->next = node;

  if (list->last == prev)
  {
    list->last = node;
  }

  list->node_count += 1;
  list->total_size += node->value.count;
}

function String
string_list_remove_first(String_List* list)
{
  String result = {0};
  if (list->node_count < 1) return result;

  String_Node* first_node = list->first;
  result = first_node->value;
  list->total_size -= result.count;

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
  list->total_size -= result.count;

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
  u8* dst = arena_push(arena, u8, list->total_size + 1);
  u8* ptr = dst;
  for (String_Node* node = list->first; node; node = node->next)
  {
    memory_copy(ptr, node->value.cstring, node->value.count);
    ptr += node->value.count;
  }
  *ptr = '\0';
  return string_new(list->total_size, dst);
}

function String
string_from_cstring(u8* cstring)
{
  String result = string_new(cstring_length(cstring), cstring);
  return result;
}

function u64
cstring_length(u8* cstring)
{
  u64 result = 0;
  while (cstring[result] != '\0') { result += 1; }
  return result;
}

function b32
s64_from_string(String str, s64* out)
{
  if(str.count == 0) return false;

  u8* cstring = str.cstring;
  u8* end = cstring + str.count;

  b32 negative = 0;
  if(*cstring == '-')
  {
    negative = 1;
    cstring += 1;
  }
  else if(*cstring == '+')
  {
    cstring += 1;
  }

  if(cstring == end) return false;

  s64 value = 0;
  for(; cstring < end; cstring += 1)
  {
    if(*cstring < '0' || *cstring > '9') return false;
    value = value * 10 + (*cstring - '0');
  }

  if(negative) value = -value;

  *out = value;
  return true;
}


#endif // STRING_H
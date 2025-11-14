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
typedef struct String String; /* 8 bit string */
struct String
{
  u64 size;
  u8* str;
};
#define S(s) (String){sizeof(s)-1, (u8*)(s)}
#define Sf(arena,fmt,...) string_from_format(arena, fmt, __VA_ARGS__)

#define S_FMT "%.*s"
#define S_ARG(str8) (s32)str8.size, str8.str

typedef struct String_Node String_Node; /* 8 bit string node */
struct String_Node
{
  String_Node* next;
  String value;
};

typedef struct String_List String_List; /* 8 bit string list */
struct String_List
{
  String_Node* first;
  String_Node* last;
  u64 node_count;
  u64 total_size;
};

function String string_new(u64 size, u8* str); /* Create a new String with given size and data pointer. */
function String string_copy(Arena* arena, String source); /* Allocate and copy source string into arena. */
function String string_range(u8* first, u8* range); /* Create String from first pointer to range pointer (exclusive). */
function String string_concat(Arena* arena, String a, String b); /* Allocate concatenated string a+b in arena. */
function String string_slice(String str, u64 start, u64 end); /* Extract substring from start to end (exclusive). */
function String string_trim(String str); /* Remove leading and trailing whitespace. */
function b32    string_contains(String str, String substring); /* Check if str contains substring. */
function b32    string_find_first(String str, String substring, u64* index); /* Find first occurrence of substring, write index. */
function b32    string_find_last(String str, String substring, u64* index); /* Find last occurrence of substring, write index. */
function b32    string_match(String a, String b, b32 case_sensitive); /* Compare strings for equality with case sensitivity option. */
function String string_from_format(Arena* arena, char const* fmt, ...); /* Printf-style string formatting into arena. */
function u64    string_hash(String str); /*  */

function String_List string_split(Arena* arena, String str, String split_character); /* Split string by delimiter into list. */
function String_List string_list_new(); /* Create new list with single string element. */
function void        string_list_push(Arena* arena, String_List* list, String str); /* Add string to end of list. */
function String      string_list_remove_first(String_List* list); /* Remove and return first element from list. */
function String      string_list_remove_last(String_List* list); /* Remove and return last element from list. */
function String      string_list_join(Arena* arena, String_List* list); /* Concatenate all list elements into single string. */

function String  string_from_cstring(u8* cstring); /* Create String from null-terminated C string. */
function u8*     cstring_from_string(Arena* arena, String str); /* Convert to null-terminated C string in arena. */
function u32     cstring_length(u8* cstring); /* Get length of null-terminated C string. */

// @Section: 32 bit characters
typedef u32 Rune; /* 32 bit character */

// @Section: 32 bit strings
typedef struct Text Text; /* 32 bit string */
struct Text
{
  u64 size;
  Rune* str;
};

#endif // STRING_H
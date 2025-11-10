#ifndef STRING_H
#define STRING_H

// 8 Bit strings
typedef struct String String;
struct String
{
  u64 size;
  u8* str;
};
#define S(s) (String){sizeof(s)-1, (u8*)(s)}
#define Sf(arena,fmt,...) // TODO(Fz): string_from_format(arena, fmt, __VA_ARGS__)

#define S_FMT "%.*s"
#define S_ARG(str8) (s32)str8.size, str8.str

typedef struct String_Node String_Node;
struct String_Node
{
  String_Node* next;
  String value;
};

typedef struct String_List String_List;
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
function u64    string_hash(String str);


#define test_string_equal(test_result, string1, string2)                         \
  test_typed_equal((test_result), u64, (string1), (string2))                     \
  test_memory_equal((test_result), (string1).str, (string2).str, (string1).size)

function void   string_test();

#endif // STRING_H
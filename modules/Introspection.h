#ifndef INTROSPECTION_H
#define INTROSPECTION_H

#include "../Base.h"
#include "Lexer.h"

#define MAX_FUNCTION_KEYWORDS 4 
#define MAX_FUNCTION_ARGUMENTS 16
#define MAX_FUNCTIONS 2048

#define MAX_ENUMS 256

typedef struct Source_Location Source_Location;
struct Source_Location 
{
  String file;
  u32 line;
};

typedef struct Data_Type Data_Type;
struct Data_Type
{
  b32 is_const;
  b32 is_pointer;
  u32 indirection_level; 
  b32 is_array;
  String name;
};

typedef struct Function_Argument Function_Argument;
struct Function_Argument
{
  b32 is_var_args; /* If true, overrides data_type and identifier, since they lose meaning. */
  Data_Type data_type;
  String identifier;
};

typedef struct Function_Object Function_Object;
struct Function_Object
{
  String* keywords; /* Things like 'function', 'inline', 'CALLBACK', 'WINAPI' etc... */
  u32 keywords_count;
  u32 keywords_capacity;

  Data_Type return_type;
  String name;

  Function_Argument* arguments;
  u32 arguments_count;
  u32 arguments_capacity;

  String body;
  String documentation;

  Source_Location declaration;
  Source_Location implementation;
};

typedef struct Struct_Member Struct_Member;
struct Struct_Member
{
  Data_Type data_type;
  String identifier;
  String documentation;
  String array_length; /* If data_type.is_array == true, we set here the size of the array, which can be a macro, so we just store a string. */
};

typedef struct Struct_Object Struct_Object; /* Any custom type defined in code: Typedefs, Enums, Structs ... */
struct Struct_Object
{
  Struct_Member members;
  u32 members_count;
  u32 members_capacity;

  String body;
  String documentation;

  Source_Location forward_declaration;
  Source_Location definition;
};

enum Dummy_Enum { DUMMY };
#define DEFAULT_ENUM_BITS ((u32)(sizeof(enum Dummy_Enum) * 8))

typedef struct Enum_Member Enum_Member;
struct Enum_Member
{
  String identifier;
  String documentation;
  b32 is_explicit_bit_flag; /* In case the value is something like Trivia_Line_Break = (1 << 5) ... */
  b32 is_one_or_more_enum_flags; /* In case the value is something like Trivia_All = Trivia_Whitespace|Trivia_Carriage_Return|Trivia_Tab|Trivia_Form_Feed|Trivia_Vertical_Tab|Trivia_Line_Break ... */
  s64 value;
};

typedef struct Enum_Object Enum_Object;
struct Enum_Object 
{
  String identifier;
  s64 value;
  u32 size_bits; /* Can either be DEFAULT_ENUM_BITS (which is implementation specific) or the type casted value from something like: typedef u32 My_Enum; */

  Enum_Member* members;
  u32 members_count;
  u32 members_capacity;

  String macro; /* In this enum has a macro inside (like an X macro), this just stores the macro from E.g. "#define X..."up to "#undef X" */
};

typedef struct Introspection Introspection;
struct Introspection
{
  Arena* functions_arena;
  Function_Object* functions;
  u32 functions_capacity;
  u32 functions_count;

  Arena* enums_arena;
  Enum_Object* enums;
  u32 enums_capacity;
  u32 enums_count;
};

function u32              parse_enum_members(Arena* arena, Lexer* lexer, Enum_Member* temp_members, u32 max_members); /* Parses the members of an enum. Anything after { and before }. Returns how many members were added */
function void             parse_enum(Lexer* lexer, Introspection* introspection, String file_path); /* Parses an anonymous enum  */
function void             parse_function(Lexer* lexer, Introspection* introspection, String file_path);
function Function_Object* find_function(Introspection* introspection, String function_name);
function void             eat_trivia(Lexer* lexer);
function void             eat_trivia_and_comment(Lexer* lexer);

function Introspection
get_introspection(String source_directory, b32 introspect_base_library)
{
  Scratch scratch = scratch_begin(0,0);

  Introspection result;
  memory_zero_struct(&result);

  result.functions_arena    = arena_alloc();
  result.functions_capacity = MAX_FUNCTIONS;
  result.functions          = push_array(result.functions_arena, Function_Object, result.functions_capacity);

  result.enums_arena    = arena_alloc();
  result.enums_capacity = MAX_ENUMS;
  result.enums          = push_array(result.enums_arena, Enum_Object, result.enums_capacity);

  String_List files = file_get_files_in_path(scratch.arena, source_directory, true);
  for (String_Node* next = files.first; next != NULL; next = next->next)
  {
    String it = next->value;

    if (!is_file(it)) continue; // Skip anything that is not a file

    // Directories we don't care about.
    if(string_contains(it, S("\\Extern\\"))  ||
       string_contains(it, S("\\.git\\"))    ||
       string_contains(it, S("\\.svn\\"))    ||
       string_contains(it, S("\\.idea\\"))   ||
       string_contains(it, S("\\.vs\\"))     ||
       string_contains(it, S("\\.vscode\\")) ||
       string_contains(it, S("\\.code\\")))
    {
      continue;
    }

    if (!introspect_base_library && string_contains(it, S("fzac"))) continue; // Skip anything from fzac library if we're introspecting it
    if (string_contains(it, S("metaprogram.c"))) continue; // Skip the metaprogram itself

    String_View extension = file_get_extension(it);
    String ext = string_new(extension.count, extension.string);

    // Make sure we introspect files we care about
    if (!(string_equals(ext, S("c"), true) ||
          string_equals(ext, S("h"), true) ||
          string_equals(ext, S("inl"), true)))
    {
      continue;
    }

    Lexer lexer;
    lexer_init_with_single_file_path(&lexer, it, Trivia_Line_Break|Trivia_Whitespace|Trivia_Tab, Emit_Character_Literals|Emit_String_Literals|Emit_Line_Comments|Emit_Block_Comments);

    for (;;)
    {
      Token* token = lexer_peek_token(&lexer);
      if (!token_is_trivia(token))
      {
        if (token->kind == Token_End_Of_File)
        {
          eof:
          break;
        }

        if (token->kind == Token_Hash)
        {
          while (token->kind != Token_Line_Break)
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
            if (token->kind == Token_End_Of_File) goto eof;
          }
        }

        if (token->kind == Token_Identifier)
        {
          // Parse function
          if (string_equals(token->value, S("function"), true))
          {
            parse_function(&lexer, &result, it);
            continue;
          }
          else if (string_equals(token->value, S("enum"), true))
          {
            parse_enum(&lexer, &result, it);
            continue;
          }

          else if (string_equals(token->value, S("typedef"), true))
          {
            lexer_eat_token(&lexer);
            eat_trivia(&lexer);

            if (string_equals(token->value, S("enum"), true))
            {
              lexer_eat_token(&lexer);
              continue;
            }
            else if (string_equals(token->value, S("struct"), true))
            {
              lexer_eat_token(&lexer);
              continue;
            }
            else
            {
              lexer_eat_token(&lexer);
              continue;
            }
          }
        }
      }
      lexer_eat_token(&lexer);
    }
  }

  scratch_end(&scratch);
  return result;
}

function Function_Object*
find_function(Introspection* introspection, String function_name)
{
  Function_Object* result = NULL;
  for (u32 i = 0; i < introspection->functions_count; i += 1)
  {
    Function_Object* function_object = &(introspection->functions[i]);
    if (string_equals(function_name, function_object->name, true))
    {
      result = function_object;
      break;
    }
  }
  return result;
}

function u32
parse_enum_members(Arena* arena, Lexer* lexer, Enum_Member* temp_members, u32 max_members)
{
  Token* token = lexer_peek_token(lexer);
  assert(token->kind == Token_Open_Brace);

  lexer_eat_token(lexer);
  eat_trivia(lexer);

  u32 members_count = 0;
  s64 next_enum_value = 0;

  for (;;)
  {
    token = lexer_peek_token(lexer);

    if(token->kind == Token_Close_Brace)
    {
      // Stop before the close brace; caller can eat it if needed
      break;
    }

    assert(token->kind == Token_Identifier);
    assert(members_count < max_members);

    Enum_Member* member = &temp_members[members_count++];
    memory_zero_struct(member);

    member->identifier = string_copy(arena, token->value);

    lexer_eat_token(lexer);
    eat_trivia(lexer);

    token = lexer_peek_token(lexer);

    // --- parse the enum value if any ---
    if(token->kind == Token_Equal)
    {
      lexer_eat_token(lexer);
      eat_trivia(lexer);

      token = lexer_peek_token(lexer);

      // Flag: (1 << N)
      if(token->kind == Token_Open_Parentheses)
      {
        member->is_explicit_bit_flag = true;

        lexer_eat_token(lexer);
        eat_trivia(lexer);

        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Number && token->value.cstring[0] == '1');
        lexer_eat_token(lexer);
        eat_trivia(lexer);

        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Less_Less);
        lexer_eat_token(lexer);
        eat_trivia(lexer);

        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Number);

        s64 shift;
        b32 ok = s64_from_string(token->value, &shift);
        assert(ok);

        member->value = ((s64)1 << shift);

        lexer_eat_token(lexer);
        eat_trivia(lexer);

        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Close_Parentheses);

        lexer_eat_token(lexer);
        eat_trivia(lexer);
      }
      // Symbol OR expression: A | B | C
      else if(token->kind == Token_Identifier)
      {
        member->is_one_or_more_enum_flags = true;
        member->value = 0;

        for(;;)
        {
          assert(token->kind == Token_Identifier);

          b32 found = 0;
          for(u32 i = 0; i < members_count; ++i)
          {
            if(string_equals(token->value, temp_members[i].identifier, true))
            {
              member->value |= temp_members[i].value;
              found = 1;
              break;
            }
          }
          assert(found);

          lexer_eat_token(lexer);
          eat_trivia(lexer);

          token = lexer_peek_token(lexer);
          if(token->kind != Token_Or) break;

          lexer_eat_token(lexer);
          eat_trivia(lexer);

          token = lexer_peek_token(lexer);
        }
      }
      // Plain number
      else
      {
        assert(token->kind == Token_Number);

        b32 ok = s64_from_string(token->value, &member->value);
        assert(ok);

        lexer_eat_token(lexer);
        eat_trivia(lexer);
      }

      next_enum_value = member->value + 1;
    }
    else
    {
      // No explicit assignment → implicit value
      member->value = next_enum_value++;
    }

    // Trailing commas
    token = lexer_peek_token(lexer);
    if(token->kind == Token_Comma)
    {
      lexer_eat_token(lexer);
      eat_trivia(lexer);
    }

    // --- parse trailing comment/documentation if any ---
    token = lexer_peek_token(lexer);
    if(token->kind == Token_Comment_Block || token->kind == Token_Comment_Line)
    {
      u64 start = 2;
      u64 end   = token->value.count - ((token->kind == Token_Comment_Block) ? 2 : 0);

      String comment = string_slice(arena, token->value, start, end - start);
      member->documentation = string_trim(arena, comment);

      lexer_eat_token(lexer);
      eat_trivia(lexer);
    }

    // Next token must be either next identifier or Close_Brace
    token = lexer_peek_token(lexer);
    assert(token->kind == Token_Identifier || token->kind == Token_Close_Brace);
  }

  return members_count;
}

function void
parse_enum(Lexer* lexer, Introspection* introspection, String file_path)
{
  // These are anonymous enums. They don't have an identifier because they are likely type-casted elsewhere.

  Token* token = lexer_peek_token(lexer);
  assert(string_equals(token->value, S("enum"), true));

  Enum_Object* enum_object = &(introspection->enums[introspection->enums_count++]);
  memory_zero_struct(enum_object);

  enum_object->size_bits = DEFAULT_ENUM_BITS;

  lexer_eat_token(lexer);
  eat_trivia(lexer);

  token = lexer_peek_token(lexer);

  // Look for Metaprogramming tags
  if(token->kind == Token_Identifier)
  {
    if(string_equals(token->value, S("META_ENUM_LINK"), true))
    {
      lexer_eat_token(lexer);
      token = lexer_peek_token(lexer);
      assert(token->kind == Token_Open_Parentheses);

      lexer_eat_token(lexer);
      token = lexer_peek_token(lexer);
      assert(token->kind == Token_Identifier);

      enum_object->identifier = string_copy(introspection->enums_arena, token->value);

      lexer_eat_token(lexer);
      token = lexer_peek_token(lexer);
      assert(token->kind == Token_Close_Parentheses);

      lexer_eat_token(lexer);
      eat_trivia(lexer);
      token = lexer_peek_token(lexer);
    }
    else
    {
      // @TODO(fz): Unknown metaprogramming tag
      return;
    }
  }

  Enum_Member temp_members[MAX_ENUMS];
  u32 members_count = parse_enum_members(introspection->enums_arena, lexer, temp_members, MAX_ENUMS);

  token = lexer_peek_token(lexer);
  assert(token->kind == Token_Close_Brace);
  lexer_eat_token(lexer);

  token = lexer_peek_token(lexer);
  assert(token->kind == Token_Semicolon);
  lexer_eat_token(lexer);

  enum_object->members_count = members_count;
  enum_object->members_capacity = members_count;
  enum_object->members = push_array(introspection->enums_arena, Enum_Member, members_count);

  for(u32 i = 0; i < members_count; ++i)
  {
    enum_object->members[i] = temp_members[i];
  }
}

function void
parse_function(Lexer* lexer, Introspection* introspection, String file_path)
{
  Scratch scratch = scratch_begin(0,0);
  
  u32 tokens_max   = 32;
  u32 tokens_count = 0;
  Token* tokens    = push_array(scratch.arena, Token, tokens_max);

  s64 line = -1;

  b32 is_declaration = false;

  for (;;)
  {
    Token* token = lexer_peek_token(lexer);
    if (line == -1) line = token->l0;

    if (token->kind == Token_End_Of_File)
    {
      // @TODO(fz): Something is wrong.
      break;
    }

    if (token_is_trivia(token))
    {
      lexer_eat_token(lexer);
      continue;
    }

    if (token->kind == Token_Line_Break)
    {
      lexer_eat_token(lexer);
      continue;
    }
    if (token->kind == Token_Semicolon)
    {
      is_declaration = true;
      lexer_eat_token(lexer);
      break;
    }
    if (token->kind == Token_Open_Brace)
    {
      is_declaration = false;
      lexer_eat_token(lexer);
      break;
    }

    if (tokens_count < tokens_max)
    {
      memory_copy(&tokens[tokens_count], token, sizeof(Token));
      tokens_count += 1;
    }
    else
    {
      // @TODO(fz): Handle errors
    }

    lexer_eat_token(lexer);
  }

  Data_Type return_type = {0};
  memory_zero_struct(&return_type);

  String function_name = string_zero();
  Function_Object* function_object = NULL;

  // Find function name;
  u32 name_index = U32_MAX;
  for (u32 i = 0; i < tokens_count; i++)
  {
    if (tokens[i].kind != Token_Identifier) continue;

    u32 j = i + 1;
    while (j < tokens_count && token_is_trivia(&tokens[j])) j++;

    if (j < tokens_count && tokens[j].kind == Token_Open_Parentheses)
    {
      function_name = string_copy(introspection->functions_arena, tokens[i].value);
      name_index = i;
      break;
    }
  }

  assert(name_index != U32_MAX);

  function_object = find_function(introspection, function_name);

  if (function_object == NULL)
  {
    function_object = &(introspection->functions[introspection->functions_count++]);
    function_object->name = string_copy(introspection->functions_arena, function_name);

    function_object->keywords_count = 0;
    function_object->keywords_capacity = MAX_FUNCTION_KEYWORDS;
    function_object->keywords = push_array(introspection->functions_arena, String, function_object->keywords_capacity);

    function_object->arguments_count = 0;
    function_object->arguments_capacity = MAX_FUNCTION_ARGUMENTS;
    function_object->arguments = push_array(introspection->functions_arena, Function_Argument, function_object->arguments_capacity);

    function_object->return_type = return_type;

    // Parse keywords and return_type
    // @NOTE(fz): We assume that all function arguments have the asterisk before the function name, and if there is const, it's always "const <type> <maybe_pointer> <function_name>"
    {
      s32 return_type_start = (s32)name_index - 1;

      while (return_type_start >= 0 && tokens[return_type_start].kind == Token_Asterisk)
      {
        return_type_start -= 1;
      }

      if (return_type_start >= 0 && tokens[return_type_start].kind == Token_Identifier)
      {
        // Check if there's a 'const' before the type name
        if (return_type_start > 0 && tokens[return_type_start - 1].kind == Token_Identifier && string_equals(tokens[return_type_start - 1].value, S("const"), true))
        {
          return_type_start -= 1;
        }
      }

      // Everything before return_type_start is a keyword
      for (u32 k = 0; k < (u32)return_type_start; k += 1)
      {
        Token* token = &tokens[k];
        if (token->kind == Token_Identifier)
        {
          if (function_object->keywords_count + 1 >= function_object->keywords_capacity)
          {
            // @TODO(fz): Error
            assert(0);
          }
          function_object->keywords[function_object->keywords_count++] =  string_copy(introspection->functions_arena, token->value);
        }
      }

      // Parse the return type from return_type_start to name_index
      s32 i = return_type_start;
      if (i >= 0 && i < (s32)name_index)
      {
        Token* token = &tokens[i];
        assert(token->kind == Token_Identifier);
  
        // Check for 'const'
        if (string_equals(token->value, S("const"), true))
        {
          function_object->return_type.is_const = true;
          i += 1;
          token = &tokens[i];
          assert(token->kind == Token_Identifier);
        }
  
        // Get base type name
        function_object->return_type.name = string_copy(introspection->functions_arena, token->value);
        i += 1;
  
        // Count pointer indirection levels
        u32 pointer_count = 0;
        while (i < (s32)name_index && tokens[i].kind == Token_Asterisk)
        {
          pointer_count += 1;
          i += 1;
        }
  
        function_object->return_type.indirection_level = pointer_count;
        if (pointer_count > 0)
        {
          function_object->return_type.is_pointer = true;
        }
      }
    }

    // Parse arguments
    assert(tokens[name_index+1].kind == Token_Open_Parentheses);
    assert(tokens[tokens_count-1].kind == Token_Close_Parentheses);
    for (u32 i = name_index + 2; i < tokens_count - 1; i += 1)
    {
      Function_Argument* argument = &function_object->arguments[function_object->arguments_count++];
      Token* token = &tokens[i];
  
      // Handle variadic arguments
      if (token->kind == Token_Dot)
      {
        i += 1;
        Token* next_dot = &tokens[i];
        assert(next_dot->kind == Token_Dot);
        argument->is_var_args = true;

        i += 1;
        next_dot = &tokens[i];
        assert(next_dot->kind == Token_Dot);

        i += 1;
        token = &tokens[i];
      }
      else
      {
        assert(token->kind == Token_Identifier);
  
        // Parse type qualifiers and base type
        b32 const_before = false;
  
        // Check for leading 'const'
        if (string_equals(token->value, S("const"), true))
        {
          const_before = true;
          i += 1;
          token = &tokens[i];
          assert(token->kind == Token_Identifier);
        }
  
        // Get base type name
        argument->data_type.name = string_copy(introspection->functions_arena, token->value);
        i += 1;
        token = &tokens[i];
  
        // Check for 'const' after type name (e.g., "char const")
        if (token->kind == Token_Identifier && string_equals(token->value, S("const"), true))
        {
          argument->data_type.is_const = true;
          i += 1;
          token = &tokens[i];
        }
        else if (const_before)
        {
          argument->data_type.is_const = true;
        }
  
        // Count pointer indirection levels
        u32 pointer_count = 0;
        while (token->kind == Token_Asterisk)
        {
          pointer_count += 1;
          i += 1;
          token = &tokens[i];
    
          // Handle 'const' after asterisk (e.g., "char* const")
          if (token->kind == Token_Identifier && string_equals(token->value, S("const"), true))
          {
            // This makes the pointer itself const, not what it points to
            // For simplicity, we'll track this as overall const
            argument->data_type.is_const = true;
            i += 1;
            token = &tokens[i];
          }
        }
  
        argument->data_type.indirection_level = pointer_count;
        if (pointer_count > 0)
        {
          argument->data_type.is_pointer = true;
        }
  
        // Get argument identifier
        assert(token->kind == Token_Identifier);
        argument->identifier = string_copy(introspection->functions_arena, token->value);
  
        // Handle array syntax (e.g., "int arr[10]" or "int arr[]")
        i += 1;
        token = &tokens[i];
        if (token->kind == Token_Open_Bracket)
        {
          argument->data_type.is_array = true;
          i += 1;
          token = &tokens[i];
    
          // Check if array has a size specified
          if (token->kind == Token_Number)
          {
            // Could store array size if needed
            i += 1;
            token = &tokens[i];
          }
    
          assert(token->kind == Token_Close_Bracket);
          i += 1;
          token = &tokens[i];
        }
      }
  
      if (token->kind == Token_Close_Parentheses)
      {
        break;
      }
      assert(token->kind == Token_Comma);
    }
  }

  if (is_declaration)
  {
    // Look for documentation
    eat_trivia(lexer);
    Token* token = lexer_peek_token(lexer);
    if (token->kind == Token_Comment_Block)
    {
      String comment = string_slice(introspection->functions_arena, token->value, 2, token->value.count - 2);
      comment = string_trim(introspection->functions_arena, comment);
      function_object->documentation = string_copy(introspection->functions_arena, comment);
    }

    function_object->declaration.file = string_copy(introspection->functions_arena, file_path);
    function_object->declaration.line = (u32)line;
  }
  else
  {
    String body = S("");
    u32 braces_stack = 0;

    for (;;)
    {
      Token* token = lexer_peek_token(lexer);
      b32 finished = false;

      switch (token->kind)
      {
        case Token_Open_Brace:
        {
          braces_stack += 1;
        }
        break;

        case Token_Close_Brace:
        {
          if (braces_stack == 0)
          {
            finished = true;
          }
          else
          {
            braces_stack -= 1;
          }
        }
      }

      lexer_eat_token(lexer);

      if (finished)
      {
        break;
      }
      else
      {
        body = string_concat(scratch.arena, body, token->value);
      }
    }

    function_object->body = string_copy(introspection->functions_arena, body);
    function_object->implementation.file = string_copy(introspection->functions_arena, file_path);
    function_object->implementation.line = (u32)line;
  }

  scratch_end(&scratch);
}

function void
eat_trivia(Lexer* lexer)
{
  for (;;)
  {
    Token* token = lexer_peek_token(lexer);
    if (!token_is_trivia(token)) break;
    lexer_eat_token(lexer);
  }
}

#endif // INTROSPECTION_H
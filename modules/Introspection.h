#ifndef INTROSPECTION_H
#define INTROSPECTION_H

#include "../Base.h"
#include "Lexer.h"

#define MAX_FUNCTION_KEYWORDS 4 
#define MAX_FUNCTION_ARGUMENTS 16
#define MAX_FUNCTIONS 1024

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

typedef struct Introspection Introspection;
struct Introspection
{
  Arena* arena;

  Function_Object* functions;
  u32 functions_capacity;
  u32 functions_count;
};

function void             parse_function(Lexer* lexer, Introspection* introspection, String file_path);
function Function_Object* find_function(Introspection* introspection, String function_name);
function void             eat_trivia(Lexer* lexer);

function Introspection
get_introspection(String source_directory, b32 introspect_base_library)
{
  Introspection result;
  memory_zero_struct(&result);
  result.arena = arena_alloc();
  result.functions_capacity = MAX_FUNCTIONS;
  result.functions          = push_array(result.arena, Function_Object, result.functions_capacity);

  Arena* arena = arena_alloc(); 
  String_List files = file_get_files_in_path(arena, source_directory, true);
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
    if (!(string_match(ext, S("c"), true) ||
          string_match(ext, S("h"), true) ||
          string_match(ext, S("inl"), true)))
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
          if (string_match(token->value, S("function"), true))
          {
            parse_function(&lexer, &result, it);
            continue;
          }
        }
      }

      lexer_eat_token(&lexer);
    }
  }

  return result;
}

function Function_Object*
find_function(Introspection* introspection, String function_name)
{
  Function_Object* result = NULL;
  for (u32 i = 0; i < introspection->functions_count; i += 1)
  {
    Function_Object* function_object = &(introspection->functions[i]);
    if (string_match(function_name, function_object->name, true))
    {
      result = function_object;
      break;
    }
  }
  return result;
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
      function_name = string_copy(introspection->arena, tokens[i].value);
      name_index = i;
      break;
    }
  }

  assert(name_index != U32_MAX);

  function_object = find_function(introspection, function_name);

  if (function_object == NULL)
  {
    function_object = &(introspection->functions[introspection->functions_count++]);
    function_object->name = string_copy(introspection->arena, function_name);

    function_object->keywords_count = 0;
    function_object->keywords_capacity = MAX_FUNCTION_KEYWORDS;
    function_object->keywords = push_array(introspection->arena, String, function_object->keywords_capacity);

    function_object->arguments_count = 0;
    function_object->arguments_capacity = MAX_FUNCTION_ARGUMENTS;
    function_object->arguments = push_array(introspection->arena, Function_Argument, function_object->arguments_capacity);

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
        if (return_type_start > 0 && tokens[return_type_start - 1].kind == Token_Identifier && string_match(tokens[return_type_start - 1].value, S("const"), true))
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
          function_object->keywords[function_object->keywords_count++] =  string_copy(introspection->arena, token->value);
        }
      }

      // Parse the return type from return_type_start to name_index
      s32 i = return_type_start;
      if (i >= 0 && i < (s32)name_index)
      {
        Token* token = &tokens[i];
        assert(token->kind == Token_Identifier);
  
        // Check for 'const'
        if (string_match(token->value, S("const"), true))
        {
          function_object->return_type.is_const = true;
          i += 1;
          token = &tokens[i];
          assert(token->kind == Token_Identifier);
        }
  
        // Get base type name
        function_object->return_type.name = string_copy(introspection->arena, token->value);
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
        if (string_match(token->value, S("const"), true))
        {
          const_before = true;
          i += 1;
          token = &tokens[i];
          assert(token->kind == Token_Identifier);
        }
  
        // Get base type name
        argument->data_type.name = string_copy(introspection->arena, token->value);
        i += 1;
        token = &tokens[i];
  
        // Check for 'const' after type name (e.g., "char const")
        if (token->kind == Token_Identifier && string_match(token->value, S("const"), true))
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
          if (token->kind == Token_Identifier && string_match(token->value, S("const"), true))
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
        argument->identifier = string_copy(introspection->arena, token->value);
  
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
      String comment = string_slice(introspection->arena, token->value, 2, token->value.count - 2);
      comment = string_trim(introspection->arena, comment);
      function_object->documentation = string_copy(introspection->arena, comment);
    }

    function_object->declaration.file = string_copy(introspection->arena, file_path);
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

    function_object->body = string_copy(introspection->arena, body);
    function_object->implementation.file = string_copy(introspection->arena, file_path);
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
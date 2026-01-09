#ifndef INTROSPECTION_H
#define INTROSPECTION_H

#include "../Base.h"
#include "Lexer.h"

typedef struct Source_Location Source_Location;
struct Source_Location 
{
  String file;
  u32 line;
};

typedef struct Function_Object Function_Object;
struct Function_Object
{
  Function_Object* next;

  String return_type;
  String name;

  String* arguments;
  u32 arguments_count;

  String body;
  String documentation;

  Source_Location declaration;
  Source_Location definition;
};

typedef struct Introspection Introspection;
struct Introspection
{
  Arena* arena;

  Function_Object* function_list;
};

function void parse_function(Lexer* lexer, Introspection* introspection, String file_path);

function Introspection
get_introspection(String source_directory, b32 introspect_base_library)
{
  Introspection result;
  memory_zero_struct(&result);
  result.arena = arena_alloc();

  Arena* arena = arena_alloc(); 
  String_List files = file_get_files_in_path(arena, source_directory, true);
  for (String_Node* next = files.first; next != NULL; next = next->next)
  {
    String it = next->value;

    if (!is_file(it)) continue; // Skip anything that is not a file

    // Directories we don't care about.
    if(string_contains(it, S("\\.git\\"))  ||
       string_contains(it, S("\\.svn\\"))   ||
       string_contains(it, S("\\.idea\\"))  ||
       string_contains(it, S("\\.vs\\"))    ||
       string_contains(it, S("\\.vscode\\"))||
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
          break;
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

function void
parse_function(Lexer* lexer, Introspection* introspection, String file_path)
{
  Scratch scratch = scratch_begin(0,0);
  
  u32 tokens_max   = 32;
  u32 tokens_count = 0;
  Token* tokens    = push_array(scratch.arena, Token, tokens_max);

  b32 is_declaration = false;
  s64 line = -1;

  for (;;)
  {
    Token* token = lexer_peek_token(lexer);
    if (line == -1) line = token->l0;

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

  // Get next function on the linked list
  Function_Object* function_object = push_array(introspection->arena, Function_Object, 1);
  if(introspection->function_list == NULL)
  {
    introspection->function_list = function_object;
  }
  else
  {
    Function_Object* it = introspection->function_list;
    while(it->next) it = it->next;
    it->next = function_object;
  }

  if (is_declaration)
  {
    // Look for documentation
    Token* token = lexer_peek_token(lexer);
    if (token->kind == Token_Comment_Block)
    {
      String comment = string_slice(token->value, 2, token->value.count - 2);
      comment = string_trim(comment);
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
    function_object->definition.file = string_copy(introspection->arena, file_path);
    function_object->definition.line = (u32)line;
  }

  scratch_end(&scratch);
}

#endif // INTROSPECTION_H
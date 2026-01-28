#ifndef INTROSPECTION_H
#define INTROSPECTION_H

#include "../Base.h"
#include "Lexer.h"
#include "Platform.h"

/* @File: Implementa introspection implemented specifically for my C code base. It makes very important assumptions and hardcodes things based on those assumptions.
          This is only meant to work on a codebase that follows my programming style. */

#define _intsp_error(message) \
  error_box(S("Introspection Error!"), message, S(__FILE__), __LINE__); \
  raddbg_break()

#define INITIAL_FILES_CAPACITY 8 // @TODO(Fz) Increase 
#define INITIAL_CODE_TAGS_CAPACITY 4
#define INITIAL_STRUCT_CAPACITY 64

typedef enum
{
  Code_Tag_None = 0,
  Code_Tag_Todo,
  Code_Tag_File,
  Code_Tag_Leak,
  Code_Tag_Speed,
} Code_Tag_Kind;

typedef enum
{
  Aggregate_Error = 0,
  Aggregate_Struct,
  Aggregate_Union,
} Aggregate_Kind;


typedef struct Intsp_Source_Location Intsp_Source_Location;
typedef struct Intsp_Context Intsp_Context;

typedef struct Intsp_Aggregate Intsp_Aggregate;
Make_Array_Type(Intsp_Aggregate);

typedef struct Intsp_Aggregate_Member Intsp_Aggregate_Member;
Make_Array_Type(Intsp_Aggregate_Member);

typedef struct Intsp_Code_Tag Intsp_Code_Tag;
Make_Array_Type(Intsp_Code_Tag);

typedef struct Intsp_File Intsp_File;
Make_Array_Type(Intsp_File);

struct Intsp_Source_Location
{
  Intsp_File* file;
  u32 line;
};

struct Intsp_Aggregate_Member
{
  String type;
  String name;
  b32    is_bit;

  Array(Intsp_Aggregate) nested_structs;
  Array(Intsp_Aggregate) nested_unions;
};

struct Intsp_Aggregate
{
  String name;
  Aggregate_Kind kind;

  Array(Intsp_Aggregate_Member) members;

  Intsp_Source_Location forward_declare_location;
  Intsp_Source_Location declaration_location;
};

struct Intsp_File
{
  String path;
  String documentation;

  Arena* arena;
  Array(Intsp_Code_Tag) code_tags;
  Array(Intsp_Aggregate) aggregates;
};

struct Intsp_Code_Tag
{
  Code_Tag_Kind kind;
  Intsp_File* file;
  u32 line;
  String text;
};

struct Intsp_Context 
{
  Arena* arena;

  Array(Intsp_File) files;
};

function Intsp_Context intsp_run(String source_directory, b32 introspect_base_library);

function void   _intsp_skip_line(Lexer *lexer, Intsp_File *file);
function void   _intsp_skip_spaces(Lexer *lexer, Intsp_File *file);
function Token* _intsp_peek_token(Lexer *lexer, Intsp_File* file);
function void   _intsp_expect_kind(Token *token, Token_Kind expected_kind);
function void   _intsp_parse_struct_members(Lexer *lexer, Intsp_File *file, Intsp_Aggregate *aggregate);

function Intsp_Context
intsp_run(String source_directory, b32 introspect_base_library)
{
  Scratch scratch = scratch_begin(0,0);

  Intsp_Context result;
  memory_zero_struct(&result);

  result.arena = arena_alloc();
  result.files = array_make(Intsp_File, INITIAL_FILES_CAPACITY);

  String_List files = file_get_files_in_path(scratch.arena, source_directory, true);
  for (String_Node* next = files.first; next != NULL; next = next->next)
  {
    String file_being_lexed = next->value;

    if (!is_file(file_being_lexed))
    {
      continue;
    }

    if(string_contains(file_being_lexed, S("\\Extern\\"))         ||
       string_contains(file_being_lexed, S("\\.git\\"))           ||
       string_contains(file_being_lexed, S("\\.svn\\"))           ||
       string_contains(file_being_lexed, S("\\.idea\\"))          ||
       string_contains(file_being_lexed, S("\\.vs\\"))            ||
       string_contains(file_being_lexed, S("\\.vscode\\"))        ||
       string_contains(file_being_lexed, S("\\cgen.generated\\")) ||
       string_contains(file_being_lexed, S("\\.code\\")))
    {
      continue;
    }

    if (!introspect_base_library && string_contains(file_being_lexed, S("fzac")))
    {
      continue;
    }

    if (string_contains(file_being_lexed, S("metaprogram.c")))
    {
      continue;
    }

    String_View extension = file_get_extension(file_being_lexed);
    String ext = string_new(extension.count, extension.string);

    if (!(string_equals(ext, S("c"), true) || string_equals(ext, S("h"), true)))
    {
      continue;
    }

    Lexer lexer;
    lexer_init_with_single_file_path(&lexer, file_being_lexed, Trivia_Line_Break|Trivia_Whitespace|Trivia_Tab, Emit_Character_Literals|Emit_String_Literals|Emit_Line_Comments|Emit_Block_Comments);

    Intsp_File* intsp_file;
    array_get_next(&result.files, Intsp_File, intsp_file);

    intsp_file->arena     = arena_alloc();
    intsp_file->path      = string_copy(intsp_file->arena, file_being_lexed);

    intsp_file->code_tags  = array_make(Intsp_Code_Tag, INITIAL_CODE_TAGS_CAPACITY);
    intsp_file->aggregates = array_make(Intsp_Aggregate, INITIAL_STRUCT_CAPACITY);
    
    for (;;)
    {
      Token *token = _intsp_peek_token(&lexer, intsp_file);
      if (token->kind == Token_End_Of_File) break;

      if (token->kind == Token_Hash)
      {
        _intsp_skip_line(&lexer, intsp_file);
        continue;
      }

      if (token->kind == Token_Identifier)
      {
        b32 is_struct = string_equals(token->value, S("struct"), true);
        b32 is_union  = string_equals(token->value, S("union"), true);

        if (is_struct || is_union)
        {
          Intsp_Aggregate *aggregate;
          array_get_next(&intsp_file->aggregates, Intsp_Aggregate, aggregate);

          aggregate->kind = (is_struct) ? Aggregate_Struct : (is_union) ? Aggregate_Union : Aggregate_Error;

          lexer_eat_token(&lexer);
          _intsp_skip_spaces(&lexer, intsp_file);
          token = _intsp_peek_token(&lexer, intsp_file);

          _intsp_expect_kind(token, Token_Identifier);
          aggregate->name = string_copy(intsp_file->arena, token->value);

          while (token->kind != Token_Open_Brace)
          {
            lexer_eat_token(&lexer);
            token = _intsp_peek_token(&lexer, intsp_file);
          }

          lexer_eat_token(&lexer); // eat '{'
          _intsp_skip_spaces(&lexer, intsp_file);
          token = _intsp_peek_token(&lexer, intsp_file);

          _intsp_expect_kind(token, Token_Identifier);
          _intsp_parse_struct_members(&lexer, intsp_file, aggregate);
        }
        else
        {
          _intsp_skip_line(&lexer, intsp_file);
          continue;
        }
      }
      else
      {
        _intsp_skip_line(&lexer, intsp_file);
        continue;
      }

      token = _intsp_peek_token(&lexer, intsp_file);
      lexer_eat_token(&lexer);
    }
  }

  return result;
}

function void
_intsp_parse_struct_members(Lexer *lexer, Intsp_File *file, Intsp_Aggregate *aggregate)
{
  Token* token = _intsp_peek_token(lexer, file);
  aggregate->declaration_location.file = file;
  aggregate->declaration_location.line = token->l0;

  while (token->kind != Token_Close_Brace && token->kind != Token_End_Of_File)
  {
    // This block recursively calls the function to parse nested aggregates and then continues to next iteration.
    {
      b32 is_nested_struct = (token->kind == Token_Identifier && string_equals(token->value, S("struct"), true));
      b32 is_nested_union = (token->kind == Token_Identifier && string_equals(token->value, S("union"), true));

      if (is_nested_struct || is_nested_union)
      {
        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
        token = _intsp_peek_token(lexer, file);
  
        // Expect '{'
        _intsp_expect_kind(token, Token_Open_Brace);
        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
  
        // Create the member that will hold the nested aggregate
        Intsp_Aggregate_Member *member;
        array_get_next(&aggregate->members, Intsp_Aggregate_Member, member);
        member->type = string_zero();
        member->is_bit = false;
        member->nested_structs = array_make(Intsp_Aggregate, 0);
        member->nested_unions = array_make(Intsp_Aggregate, 0);
  
        // Create the nested aggregate (always nameless)
        Intsp_Aggregate *nested_aggregate;
        if (is_nested_struct)
        {
          array_get_next(&member->nested_structs, Intsp_Aggregate, nested_aggregate);
        }
        else
        {
          array_get_next(&member->nested_unions, Intsp_Aggregate, nested_aggregate);
        }
  
        nested_aggregate->kind = is_nested_struct ? Aggregate_Struct : Aggregate_Union;
        nested_aggregate->name = S("_nameless");
        nested_aggregate->members = array_make(Intsp_Aggregate_Member, 0);
  
        // Recursively parse nested members
        _intsp_parse_struct_members(lexer, file, nested_aggregate);
  
        // Expect '}'
        token = _intsp_peek_token(lexer, file);
        _intsp_expect_kind(token, Token_Close_Brace);
        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
        token = _intsp_peek_token(lexer, file);
  
        // Optional member name
        if (token->kind == Token_Identifier)
        {
          member->name = string_copy(file->arena, token->value);
          lexer_eat_token(lexer);
          _intsp_skip_spaces(lexer, file);
          token = _intsp_peek_token(lexer, file);
        }
        else
        {
          member->name = string_zero();
        }
  
        // Consume semicolon
        if (token->kind == Token_Semicolon)
        {
          lexer_eat_token(lexer);
          _intsp_skip_spaces(lexer, file);
          token = _intsp_peek_token(lexer, file);
        }
  
        continue;
      }
    }

    Intsp_Aggregate_Member *member;
    array_get_next(&aggregate->members, Intsp_Aggregate_Member, member);
    member->is_bit = false;
  
    String_List type_tokens = string_list_new();
    String_List name_tokens = string_list_new();
    b32 is_function_pointer = false;
    b32 seen_asterisk = false;
    u64 paren_depth = 0;
  
    while (token->kind != Token_Semicolon && token->kind != Token_End_Of_File)
    {
      // Check for colon (bit field)
      if (token->kind == Token_Colon)
      {
        member->is_bit = true;
        // Skip the colon and the bit width number
        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
        token = _intsp_peek_token(lexer, file);
        // Skip the bit width value
        if (token->kind == Token_Number || token->kind == Token_Identifier)
        {
          lexer_eat_token(lexer);
          _intsp_skip_spaces(lexer, file);
          token = _intsp_peek_token(lexer, file);
        }
        break;
      }
      
      // Break on comma only if we're NOT inside a function pointer
      if (token->kind == Token_Comma && paren_depth == 0)
      {
        break;
      }
    
      if (token->kind == Token_Open_Parentheses)
      {
        paren_depth++;
        if (paren_depth == 1 && type_tokens.node_count > 0)
        {
          // This is a function pointer - everything from '(' onwards is the name
          is_function_pointer = true;
        }
      }
      else if (token->kind == Token_Close_Parentheses)
      {
        paren_depth--;
      }
    
      if (is_function_pointer)
      {
        // Collecting name tokens for function pointer (including commas and everything)
        string_list_push(file->arena, &name_tokens, token->value);
      }
      else if (token->kind == Token_Asterisk)
      {
        // Asterisk goes into name
        seen_asterisk = true;
        string_list_push(file->arena, &name_tokens, token->value);
      }
      else if (seen_asterisk)
      {
        // After asterisk, everything goes into name
        string_list_push(file->arena, &name_tokens, token->value);
      }
      else
      {
        // Before asterisk, everything is part of the type
        string_list_push(file->arena, &type_tokens, token->value);
      }
    
      lexer_eat_token(lexer);
      _intsp_skip_spaces(lexer, file);
      token = _intsp_peek_token(lexer, file);
    }
  
    if (is_function_pointer)
    {
      // For function pointers: type is collected in type_tokens, name is in name_tokens
      member->type = string_list_join(file->arena, &type_tokens);
      member->name = string_list_join(file->arena, &name_tokens);
    }
    else if (seen_asterisk)
    {
      // Pointer member: type in type_tokens, name (with *) in name_tokens
      member->type = string_list_join(file->arena, &type_tokens);
      member->name = string_list_join(file->arena, &name_tokens);
    }
    else
    {
      // Regular member or bit field: last token in type_tokens is the name
      if (type_tokens.node_count > 1)
      {
        String last = string_list_remove_last(&type_tokens);
        member->type = string_list_join(file->arena, &type_tokens);
        member->name = string_copy(file->arena, last);
      }
      else if (type_tokens.node_count == 1)
      {
        member->type = string_zero();
        member->name = string_list_join(file->arena, &type_tokens);
      }
      else
      {
        member->type = string_zero();
        member->name = string_zero();
      }
    }
  
    // Handle comma-separated members (int a, b, c;) - only for non-function-pointers and non-bit-fields
    if (!is_function_pointer && !member->is_bit)
    {
      while (token->kind == Token_Comma)
      {
        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
        token = _intsp_peek_token(lexer, file);
      
        if (token->kind == Token_Identifier || token->kind == Token_Asterisk)
        {
          // New member with same type
          Intsp_Aggregate_Member *extra_member;
          array_get_next(&aggregate->members, Intsp_Aggregate_Member, extra_member);
          extra_member->type = string_copy(file->arena, member->type);
          extra_member->is_bit = false;
        
          // Collect the name (including any asterisks)
          String_List extra_name_tokens = string_list_new();
          while (token->kind != Token_Comma && token->kind != Token_Semicolon && token->kind != Token_End_Of_File && token->kind != Token_Colon)
          {
            string_list_push(file->arena, &extra_name_tokens, token->value);
            lexer_eat_token(lexer);
            _intsp_skip_spaces(lexer, file);
            token = _intsp_peek_token(lexer, file);
          }
          extra_member->name = string_list_join(file->arena, &extra_name_tokens);
          
          // Check if this extra member is also a bit field
          if (token->kind == Token_Colon)
          {
            extra_member->is_bit = true;
            // Skip the colon and bit width
            lexer_eat_token(lexer);
            _intsp_skip_spaces(lexer, file);
            token = _intsp_peek_token(lexer, file);
            if (token->kind == Token_Number || token->kind == Token_Identifier)
            {
              lexer_eat_token(lexer);
              _intsp_skip_spaces(lexer, file);
              token = _intsp_peek_token(lexer, file);
            }
          }
        }
      }
    }
  
    // Consume the semicolon
    if (token->kind == Token_Semicolon)
    {
      lexer_eat_token(lexer);
      _intsp_skip_spaces(lexer, file);
      token = _intsp_peek_token(lexer, file);
    }
  }

  token = _intsp_peek_token(lexer, file);
}

function void
_intsp_eat_scope(Lexer *lexer, Intsp_File *file)
{
  Token *t = _intsp_peek_token(lexer, file);
  assert(t->kind == Token_Open_Brace);
  lexer_eat_token(lexer);

  s32 depth = 1;
  for (;;)
  {
    t = _intsp_peek_token(lexer, file);
    if (t->kind == Token_Open_Brace)  depth++;
    if (t->kind == Token_Close_Brace) depth--;
    if (depth == 0) break;
    lexer_eat_token(lexer);
  }
  lexer_eat_token(lexer); // Eat Token_Close_Brace
}

function void
_intsp_skip_line(Lexer *lexer, Intsp_File *file)
{
  b32 multi_line = false;
  for (;;)
  {
    Token *token = _intsp_peek_token(lexer, file);
    lexer_eat_token(lexer);

    if (token->kind == Token_End_Of_File)
    {
      break;
    }

    if (token->kind == Token_Backslash)
    {
      multi_line = true;
    }
    if (token->kind == Token_Line_Break)
    {
      if (multi_line)
      {
        multi_line = false;
      }
      else
      {
        break;
      }
    }
  }
}

function void
_intsp_skip_spaces(Lexer *lexer, Intsp_File *file)
{
  Token *token = _intsp_peek_token(lexer, file);

  while (token->kind == Token_Whitespace || token->kind == Token_Line_Break)
  {
    lexer_eat_token(lexer);
    token = _intsp_peek_token(lexer, file);
  }
}

function void
_intsp_expect_kind(Token *token, Token_Kind expected_kind)
{
  if (token->kind != expected_kind)
  {
    Scratch scratch = scratch_begin(0,0);
    _intsp_error(Sf(scratch.arena, "Expected token kind wrong. Token: '%s', Expected: '%d'\n", token->value.cstring, expected_kind));
    assert(0);
    scratch_end(&scratch);
  }
}

function Token*
_intsp_peek_token(Lexer *lexer, Intsp_File *file)
{
  for (;;)
  {
    Token *token = lexer_peek_token(lexer);

    if (token->kind != Token_Comment_Block && token->kind != Token_Comment_Line)
    {
      return token;
    }

    Scratch scratch = scratch_begin(0,0);

    Lexer comment_lexer;
    lexer_init_from_string_with_arena(&comment_lexer, scratch.arena, token->value, Trivia_None, Emit_None);

    for (;;)
    {
      Token* ct = lexer_peek_token(&comment_lexer);

      if (ct->kind == Token_End_Of_File)
      {
        break;
      }

      if (ct->kind == Token_At)
      {
        lexer_eat_token(&comment_lexer);

        Token* ident = lexer_peek_token(&comment_lexer);
        if (ident->kind != Token_Identifier)
        {
          lexer_eat_token(&comment_lexer);
          continue;
        }

        Code_Tag_Kind tag = Code_Tag_None;

        if (string_equals(ident->value, S("File"),  false)) { tag = Code_Tag_File;  }
        if (string_equals(ident->value, S("Leak"),  false)) { tag = Code_Tag_Leak;  }
        if (string_equals(ident->value, S("Speed"), false)) { tag = Code_Tag_Speed; }
        if (string_equals(ident->value, S("Todo"),  false)) { tag = Code_Tag_Todo;  }

        lexer_eat_token(&comment_lexer);

        if (tag == Code_Tag_None)
        {
          continue;
        }

        Token* t = lexer_peek_token(&comment_lexer);
        if (t->kind != Token_Open_Parentheses)
        {
          continue;
        }

        while (t->kind != Token_Close_Parentheses && t->kind != Token_End_Of_File)
        {
          lexer_eat_token(&comment_lexer);
          t = lexer_peek_token(&comment_lexer);
        }

        if (t->kind == Token_Close_Parentheses)
        {
          lexer_eat_token(&comment_lexer);
        }

        t = lexer_peek_token(&comment_lexer);
        if (t->kind != Token_Colon)
        {
          continue;
        }

        lexer_eat_token(&comment_lexer);

        u64 start = comment_lexer.current_character_index;
        u64 end   = token->value.count;

        if (token->kind == Token_Comment_Block && end >= 2)
        {
          end -= 2; // "*/"
        }

        if (tag == Code_Tag_File)
        {
          String text  = {0};
          text.cstring = token->value.cstring + start;
          text.count   = end - start;
          file->documentation = string_copy(file->arena, text);          
        }
        else
        {
          String text = {0};
          text.cstring = token->value.cstring + start;
          text.count = 0;
          if (start < end)
          {
            text.count = end - start;
          }

          Intsp_Code_Tag *slot;
          array_get_next(&file->code_tags, Intsp_Code_Tag, slot);

          slot->kind = tag;
          slot->file = file;
          slot->line = token->l0;
          if (text.count > 0) 
          {
            slot->text = string_copy(file->arena, text);
          }
        }

        break; // one tag per comment is enough
      }

      lexer_eat_token(&comment_lexer);
    }

    scratch_end(&scratch);

    // Skip the comment in the real lexer
    lexer_eat_token(lexer);
  }
}


#endif // INTROSPECTION_H
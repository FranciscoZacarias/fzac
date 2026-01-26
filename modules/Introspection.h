#ifndef INTROSPECTION_H
#define INTROSPECTION_H

#include "../Base.h"
#include "Lexer.h"
#include "Platform.h"

/* @File: Implementa introspection implemented specifically for my C code base. It makes very important assumptions and hardcodes things based on those assumptions.
          This is only meant to work on a codebase that follows my programming style. */

#define INITIAL_FILES_CAPACITY 8 // @TODO(Fz) Increase 
#define INITIAL_CODE_TAGS_CAPACITY 4

typedef enum
{
  Code_Tag_None = 0,
  Code_Tag_Todo,
  Code_Tag_File,
  Code_Tag_Leak,
  Code_Tag_Speed,
} Code_Tag_Kind;

typedef struct Intsp_Code_Tag Intsp_Code_Tag;
Make_Array_Type(Intsp_Code_Tag);

typedef struct Intsp_File Intsp_File;
Make_Array_Type(Intsp_File);
struct Intsp_File
{
  String path;
  String documentation;

  Arena* arena;
  Array(Intsp_Code_Tag) code_tags;
};

struct Intsp_Code_Tag
{
  Code_Tag_Kind kind;
  Intsp_File* file;
  u32 line;
  String text;
};

typedef struct Intsp_Context Intsp_Context;
struct Intsp_Context 
{
  Arena* arena;

  Array(Intsp_File) files;
};

function Intsp_Context intsp_run(String source_directory, b32 introspect_base_library);

function void   _intsp_skip_line(Lexer *lexer, Intsp_File *file);
function Token* _intsp_peek_token(Lexer *lexer, Intsp_File* file);

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
    intsp_file->code_tags = array_make(Intsp_Code_Tag, INITIAL_CODE_TAGS_CAPACITY);
    
    for (;;)
    {
      Token *token = _intsp_peek_token(&lexer, intsp_file);

      if (token->kind == Token_End_Of_File)
      {
        break;
      }

      while (token->kind == Token_Hash)
      {
        _intsp_skip_line(&lexer, intsp_file);
      }

      token = _intsp_peek_token(&lexer, intsp_file);

      lexer_eat_token(&lexer);
    }
  }

  return result;
}

function String
_intsp_parse_scope(Lexer *lexer, Intsp_File *file)
{
  Token *token = _intsp_peek_token(&lexer, file);
  assert(token->kind == Token_Open_Brace);
  
}

function void
_intsp_skip_line(Lexer *lexer, Intsp_File *file)
{
  b32 multi_line = false;
  for (;;)
  {
    Token *token = _intsp_peek_token(lexer, file);
    lexer_eat_token(lexer);

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
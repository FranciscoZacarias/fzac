#ifndef INTROSPECTION_H
#define INTROSPECTION_H

#include "../Base.h"
#include "Lexer.h"
#include "Platform.h"

/* @File: Implementa introspection implemented specifically for my C code base. It makes very important assumptions and hardcodes things based on those assumptions.
          This is only meant to work on a codebase that follows my programming style. */

#define INITIAL_FILES_CAPACITY 8 // @TODO(Fz) Increase 

typedef u32 Code_Tag_Kind;
enum
{
  Code_Tag_None = 0,
  Code_Tag_Todo,
  Code_Tag_File,
  Code_Tag_Leak,
  Code_Tag_Speed,
};

typedef struct Intsp_Code_Tag Intsp_Code_Tag;
Make_Array_Type(Intsp_Code_Tag);

typedef struct Intsp_File Intsp_File;
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

  Intsp_File* files;
  u32 files_count;
  u32 files_capacity;
};

function Intsp_Context* intsp_run(Arena* arena, String source_directory, b32 introspect_base_library);
function Token* intsp_peek_token(Lexer* lexer, Intsp_File* file);


function Intsp_Context*
intsp_run(Arena* arena, String source_directory, b32 introspect_base_library)
{
  Scratch scratch = scratch_begin(0,0);

  Intsp_Context* result = arena_push(arena, Intsp_Context, 1);
  memory_zero_struct(&result);

  result->arena = arena_alloc();

  result->files_capacity = INITIAL_FILES_CAPACITY;
  result->files_count = 0;
  result->files = arena_push(result->arena, Intsp_File, result->files_capacity);

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

    // @TODO(fz): Extract this as something like arena_backed_array_realloc
    if (result->files_count+1 >= result->files_capacity)
    {
      u32 new_capacity = result->files_capacity ? result->files_capacity * 2 : 2;
      Intsp_File *tmp = arena_push(scratch.arena, Intsp_File, result->files_count);
      memory_copy(tmp, result->files, sizeof(Intsp_File) * result->files_count);
      arena_clear(result->arena);
      result->files = arena_push(result->arena, Intsp_File, new_capacity);
      result->files_capacity = new_capacity;
      memory_copy(result->files, tmp, sizeof(Intsp_File) * result->files_count);
    }

    Intsp_File* intsp_file = &result->files[result->files_count++];
    intsp_file->arena     = arena_alloc();
    intsp_file->code_tags = array_make(Intsp_Code_Tag, 8);
    
    for (;;)
    {
      Token* token = lexer_peek_token(&lexer);


    }
  }

  return result;
}

function Token*
intsp_peek_token(Lexer* lexer, Intsp_File* file)
{
  Token* token = lexer_peek_token(lexer);
  while (token->kind == Token_Comment_Block || token->kind == Token_Comment_Line)
  {
    Scratch scratch = scratch_begin(0,0);

    Lexer comment_lexer;
    lexer_init_from_string_with_arena(&comment_lexer, scratch.arena, token->value, Trivia_None, Emit_None);

    for (;;)
    {
      Token* comment_token = lexer_peek_token(&comment_lexer);
      if (comment_token->kind == Token_End_Of_File)
      {
        break;
      }
      if (comment_token->kind == Token_At)
      {
        lexer_eat_token(&comment_lexer);
        comment_token = lexer_peek_token(&comment_lexer);

        Code_Tag_Kind tag = Code_Tag_None;

        if (token->kind == Token_Identifier)
        {
          if (string_equals(token->value, S("Todo"), false))       { tag = Code_Tag_None; }
          else if (string_equals(token->value, S("File"), false))  { tag = Code_Tag_File; }
          else if (string_equals(token->value, S("Leak"), false))  { tag = Code_Tag_Leak; }
          else if (string_equals(token->value, S("Speed"), false)) { tag = Code_Tag_Speed; }

          if (tag != Code_Tag_None)
          {
            Intsp_Code_Tag* slot;
            array_get_next(&file->code_tags, Intsp_Code_Tag, slot);
            slot->kind = tag;
            slot->file = file;
            slot->line = token->l0;

            while (comment_token->kind != Token_Colon)
            {
              lexer_peek_token(&comment_lexer);
              comment_token = lexer_peek_token(&comment_lexer);
            }

            String comment_text;
            comment_text.cstring += comment_lexer.current_character_index;
            comment_text.count    = comment_token->value.count - comment_lexer.current_character_index;

            if (comment_token->kind == Token_Comment_Block)
            {
              comment_text.count -= 2; // Remove '*/'
            }

            slot->text = string_copy(file->arena, comment_text);
          }
        }
      }
    }

    scratch_end(&scratch);
    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);
  }
}

#endif // INTROSPECTION_H
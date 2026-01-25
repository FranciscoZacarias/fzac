#ifndef INTROSPECTION_H
#define INTROSPECTION_H

#include "../Base.h"
#include "Lexer.h"
#include "Platform.h"

/* @File: Implementa introspection implemented specifically for my C code base. It makes very important assumptions and hardcodes things based on those assumptions.
          This is only meant to work on a codebase that follows my programming style. */

#define INITIAL_FILES_CAPACITY 8

typedef struct Intsp_File Intsp_File;
Make_Array_Type(Intsp_File);
struct Intsp_File
{
  String path;
  String documentation;

  Arena* arena;
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

    Intsp_File* intsp_file;
  }

  return result;
}

#endif // INTROSPECTION_H
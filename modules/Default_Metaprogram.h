#ifndef DEFAULT_METAPROGRAM_H
#define DEFAULT_METAPROGRAM_H

#define METAPROGRAM_MAX_FILES 64

typedef struct DM_File DM_File;
struct DM_File
{
  String name;
};

typedef struct Default_Metaprogram Default_Metaprogram;
struct Default_Metaprogram
{
  Arena *arena;

  DM_File *files;
  u32 files_count;
  u32 files_capacity;
};

function void default_metaprogram(Default_Metaprogram *default_metaprogram, String src_directory);

function void
default_metaprogram(Default_Metaprogram *default_metaprogram, String src_directory)
{
  arena_array_init(default_metaprogram->arena, default_metaprogram->files, DM_File, METAPROGRAM_MAX_FILES);
  String_List files = file_get_files_in_path(default_metaprogram->arena, src_directory, true);

  for (String_Node *next = files.first; next != NULL; next = next->next)
  {
    String file_being_lexed = next->value;
    if (!is_file(file_being_lexed)) continue;

    if (string_contains(file_being_lexed, S("\\Extern\\"))  ||
        string_contains(file_being_lexed, S("\\.git\\"))    ||
        string_contains(file_being_lexed, S("\\.svn\\"))    ||
        string_contains(file_being_lexed, S("\\.idea\\"))   ||
        string_contains(file_being_lexed, S("\\.vs\\"))     ||
        string_contains(file_being_lexed, S("\\.vscode\\")) ||
        string_contains(file_being_lexed, S("\\.code\\")))
    {
      continue;
    }

    String_View extension = file_get_extension(file_being_lexed);
    String ext = string_new(extension.count, extension.string);
    
    if (!string_equals(ext, S("c"), true) && !string_equals(ext, S("h"), true))
    {
      continue;
    }
    
    printf("%s\n", file_being_lexed.cstring);

    DM_File *dm_file;
    arena_array_push(dm_file, default_metaprogram->files, default_metaprogram->files_count, default_metaprogram->files_capacity);
    memory_zero_struct(dm_file);

    dm_file->name = string_copy(default_metaprogram->arena, file_being_lexed);

    Lexer lexer;
    lexer_init_with_single_file_path(&lexer, file_being_lexed, Trivia_None, Emit_String_Literals|Emit_Line_Comments|Emit_Block_Comments);

    for (;;)
    {
      Token *token = lexer_peek_token(&lexer);
      if (token->kind == Token_End_Of_File) break;

      if (0)
      {
        
      }
      else
      {
        lexer_eat_token(&lexer);
      }
    }
  }
}

#endif // DEFAULT_METAPROGRAM_H
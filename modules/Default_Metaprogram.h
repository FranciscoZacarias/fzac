#ifndef DEFAULT_METAPROGRAM_H
#define DEFAULT_METAPROGRAM_H

#define METAPROGRAM_MAX_FILES 64
#define METAPROGRAM_MAX_FUNTIONS 256
#define METAPROGRAM_MAX_STRUCTS 128

typedef struct DM_Code_Function DM_Code_Function;
struct DM_Code_Function
{
  String name;
  String return_type;
  String arguments; /* Without parentheses */
};

typedef struct DM_Code_Struct DM_Code_Struct;
struct DM_Code_Struct
{
  String name;
};

typedef struct DM_File DM_File;
struct DM_File
{
  String name;

  DM_Code_Function *function_definitions;
  u32 function_definitions_count;
  u32 function_definitions_capacity;

  DM_Code_Function *function_declaration; /* Function implementation */
  u32 function_declaration_count;
  u32 function_declaration_capacity;

  DM_Code_Struct   *struct_definitions;
  u32               struct_definitions_count;
  u32               struct_definitions_capacity;
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

    if (string_contains(file_being_lexed, S(".generated.")))
    {
      continue;
    }

    if (string_contains(file_being_lexed, S("\\fzac\\")))
    {
      continue;
    }
    
    printf("%s\n", file_being_lexed.cstring);

    DM_File *dm_file;
    arena_array_push(dm_file, default_metaprogram->files, default_metaprogram->files_count, default_metaprogram->files_capacity);
    memory_zero_struct(dm_file);

    dm_file->name = string_copy(default_metaprogram->arena, file_being_lexed);
    arena_array_init(default_metaprogram->arena, dm_file->function_definitions, DM_Code_Function, METAPROGRAM_MAX_FUNTIONS);
    arena_array_init(default_metaprogram->arena, dm_file->function_declaration, DM_Code_Function, METAPROGRAM_MAX_FUNTIONS);
    arena_array_init(default_metaprogram->arena, dm_file->struct_definitions, DM_Code_Struct, METAPROGRAM_MAX_STRUCTS);

    Lexer lexer;
    lexer_init_with_single_file_path(&lexer, file_being_lexed, Trivia_Whitespace|Trivia_Line_Break, Emit_String_Literals|Emit_Line_Comments|Emit_Block_Comments);

    b32 is_first_token_on_line = true;
    for (;;)
    {
      Token *token = lexer_peek_token(&lexer);
      if (token->kind == Token_End_Of_File) break;

      if (token->kind == Token_Line_Break)
      {
        is_first_token_on_line = true;
        lexer_eat_token(&lexer);
        continue;
      }

      if (token->kind == Token_Whitespace)
      {
        lexer_eat_token(&lexer);
        continue;
      }

      if (token->kind == Token_Identifier)
      {
        if (string_equals(token->value, S("function"), true) && is_first_token_on_line)
        {
          lexer_eat_token(&lexer);

          DM_Code_Function parsed_function = {0};
          b32 is_definition = false;

          String_Builder return_type_builder = string_builder_init(kilobytes(1));
          String last_identifier = string_zero();
          token = lexer_peek_token(&lexer);
          while (token->kind != Token_Open_Parentheses && token->kind != Token_End_Of_File)
          {
            if (token->kind == Token_Identifier)
            {
              if (last_identifier.count > 0)
              {
                string_builder_push_string(&return_type_builder, last_identifier);
                string_builder_push_char(&return_type_builder, ' ');
              }
              last_identifier = token->value;
            }
            else if (token->kind == Token_Asterisk)
            {
              string_builder_push_char(&return_type_builder, '*');
            }
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }

          assert(last_identifier.count > 0 && "Expected function name before '('");
          parsed_function.name = string_copy(default_metaprogram->arena, last_identifier);
          parsed_function.return_type = string_builder_to_string(default_metaprogram->arena, &return_type_builder);
          string_builder_free(&return_type_builder);

          assert(token->kind == Token_Open_Parentheses && "Expected '('");
          lexer_eat_token(&lexer);

          String_Builder arguments_builder = string_builder_init(kilobytes(1));
          token = lexer_peek_token(&lexer);
          while (token->kind != Token_Close_Parentheses && token->kind != Token_End_Of_File)
          {
            string_builder_push_string(&arguments_builder, token->value);
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert(token->kind == Token_Close_Parentheses && "Expected ')' to close argument list");
          lexer_eat_token(&lexer);
          parsed_function.arguments = string_builder_to_string(default_metaprogram->arena, &arguments_builder);
          string_builder_free(&arguments_builder);

          token = lexer_peek_token(&lexer);
          while (token->kind != Token_Semicolon  &&
                 token->kind != Token_Open_Brace &&
                 token->kind != Token_End_Of_File)
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert(token->kind != Token_End_Of_File && "Expected ';' or '{' after function signature");

          if (token->kind == Token_Semicolon)
          {
            is_definition = false;
            lexer_eat_token(&lexer);
          }
          else if (token->kind == Token_Open_Brace)
          {
            is_definition = true;
          }

          if (is_definition)
          {
            assert(dm_file->function_definitions_count < dm_file->function_definitions_capacity && "function_definitions capacity exceeded");
            dm_file->function_definitions[dm_file->function_definitions_count] = parsed_function;
            dm_file->function_definitions_count += 1;
          }
          else
          {
            assert(dm_file->function_declaration_count < dm_file->function_declaration_capacity && "function_declaration capacity exceeded");
            dm_file->function_declaration[dm_file->function_declaration_count] = parsed_function;
            dm_file->function_declaration_count += 1;
          }

          is_first_token_on_line = false;
        }
        else if (string_equals(token->value, S("struct"), true) && is_first_token_on_line)
        {
          lexer_eat_token(&lexer); // eat 'struct'

          token = lexer_peek_token(&lexer);
          while (token->kind == Token_Whitespace || token->kind == Token_Line_Break) 
          {   
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }

          if (token->kind != Token_Identifier)
          {
            is_first_token_on_line = false;
            continue;
          }

          String struct_name = token->value;
          lexer_eat_token(&lexer);

          token = lexer_peek_token(&lexer);
          while (token->kind == Token_Whitespace || token->kind == Token_Line_Break)
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }

          if (token->kind != Token_Open_Brace)
          {
            is_first_token_on_line = false;
            continue;
          }
          lexer_eat_token(&lexer);

          s32 brace_depth = 1;
          token = lexer_peek_token(&lexer);
          while (brace_depth > 0 && token->kind != Token_End_Of_File)
          {
            if (token->kind == Token_Open_Brace)
            {
              brace_depth += 1;
            }
            else if (token->kind == Token_Close_Brace)
            {
              brace_depth -= 1;
            }
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }

          token = lexer_peek_token(&lexer);
          while (token->kind == Token_Whitespace || token->kind == Token_Line_Break) 
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert(token->kind == Token_Semicolon && "Expected ';' after struct definition");
          lexer_eat_token(&lexer);

          assert(dm_file->struct_definitions_count < dm_file->struct_definitions_capacity && "struct_definitions capacity exceeded");
          dm_file->struct_definitions[dm_file->struct_definitions_count].name = string_copy(default_metaprogram->arena, struct_name);
          dm_file->struct_definitions_count += 1;

          is_first_token_on_line = false;
        }
        else
        {
          is_first_token_on_line = false;
          lexer_eat_token(&lexer);
        }
      }
      else
      {
        is_first_token_on_line = false;
        lexer_eat_token(&lexer);
      }
    }
  }
}

#endif // DEFAULT_METAPROGRAM_H
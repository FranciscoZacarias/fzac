#ifndef DEFAULT_METAPROGRAM_H
#define DEFAULT_METAPROGRAM_H

#define METAPROGRAM_MAX_FILES 64
#define METAPROGRAM_MAX_FUNTIONS 256

typedef struct DM_Code_Function DM_Code_Function;
struct DM_Code_Function
{
  String name;
  String return_type;
  String arguments; /* Without parentheses */
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
  Scratch scratch = scratch_begin(0,0);

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
    arena_array_init(default_metaprogram->arena, dm_file->function_definitions, DM_Code_Function, METAPROGRAM_MAX_FUNTIONS);
    arena_array_init(default_metaprogram->arena, dm_file->function_declaration, DM_Code_Function, METAPROGRAM_MAX_FUNTIONS);

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
          lexer_eat_token(&lexer); // eat 'function' keyword

          DM_Code_Function parsed_function = {0};
          b32 is_definition = false;

          // Accumulate everything into return type until we hit '('
          // The last identifier before '(' is the function name
          String_Builder return_type_builder = string_builder_init(kilobytes(1));
          String last_identifier = string_zero();
          token = lexer_peek_token(&lexer);
          while (token->kind != Token_Open_Parentheses && token->kind != Token_End_Of_File)
          {
            if (token->kind == Token_Identifier)
            {
              // Flush the previous last_identifier into the return type builder
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

          // last_identifier is the function name, return_type_builder has everything before it
          assert(last_identifier.count > 0 && "Expected function name before '('");
          parsed_function.name = string_copy(default_metaprogram->arena, last_identifier);
          parsed_function.return_type = string_builder_to_string(default_metaprogram->arena, &return_type_builder);
          string_builder_free(&return_type_builder);

          // Open parenthesis
          assert(token->kind == Token_Open_Parentheses && "Expected '('");
          lexer_eat_token(&lexer);

          // Collect arguments verbatim until closing paren
          String_Builder arguments_builder = string_builder_init(kilobytes(1));
          token = lexer_peek_token(&lexer);
          while (token->kind != Token_Close_Parentheses && token->kind != Token_End_Of_File)
          {
            string_builder_push_string(&arguments_builder, token->value);
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert(token->kind == Token_Close_Parentheses && "Expected ')' to close argument list");
          lexer_eat_token(&lexer); // eat ')'
          parsed_function.arguments = string_builder_to_string(default_metaprogram->arena, &arguments_builder);
          string_builder_free(&arguments_builder);

          // Consume tokens until we find ';' or '{', ignoring anything in between
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
            // Leave the brace for the outer parser to handle the body
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
        else if (string_equals(token->value, S("struct"), true))
        {
          is_first_token_on_line = false;
          lexer_eat_token(&lexer);
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
  scratch_end(&scratch);
}

#endif // DEFAULT_METAPROGRAM_H
#ifndef DEFAULT_METAPROGRAM_H
#define DEFAULT_METAPROGRAM_H

#include "Logging.h"
#include "List_Todos.h"

#define METAPROGRAM_METAPROGRAM_DIRECTORY "/metaprogram/"
#define METAPROGRAM_GLOBAL_HEADERS_PATH              METAPROGRAM_METAPROGRAM_DIRECTORY "/global_headers.h"
#define METAPROGRAM_METAPROGRAM_GLOBAL_HEADERS_PATH  METAPROGRAM_METAPROGRAM_DIRECTORY "/metaprogram_global_headers.h"
#define METAPROGRAM_METAPROGRAM_GENERATED_PATH       METAPROGRAM_METAPROGRAM_DIRECTORY "/metaprogram_generated.h"

#define METAPROGRAM_MAX_EXCLUDED_FILES 64
#define METAPROGRAM_MAX_FILES 64
#define METAPROGRAM_MAX_FUNTIONS 256
#define METAPROGRAM_MAX_STRUCTS 128
#define METAPROGRAM_MAX_ENUMS 128
#define METAPROGRAM_MAX_VALUES 128

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

typedef struct DM_Code_Enum DM_Code_Enum;
struct DM_Code_Enum
{
  String name;
  String type;
  String values[METAPROGRAM_MAX_VALUES];
  u32    values_count;
  b8     to_string;
};

typedef struct DM_File DM_File;
struct DM_File
{
  String name;

  Array_Members(enum_definitions, DM_Code_Enum);
  Array_Members(struct_definitions, DM_Code_Struct);
  Array_Members(function_definitions, DM_Code_Function);

  u32 lines_of_code;
  u32 significant_lines_of_code;
};

typedef struct Default_Metaprogram Default_Metaprogram;
struct Default_Metaprogram
{
  Arena *arena;
  
  Array_Members(files, DM_File);
  Array_Members(excluded_files, String);
};

fz_internal void default_metaprogram(Default_Metaprogram *dm, Command_Line *command_line, String src_directory, String *global_headers_extra_data);
fz_internal void default_metaprogram_exclude_file_from_being_forward_declared(Default_Metaprogram *dm, String file_name);

#define dm_log_fatal_if(cond, msg) \
  statement( \
    log_fatal_if(cond, msg "\nFile: " S_FMT "\nLine: %d", \
      S_ARG(file_being_lexed), lexer.current_line_number); \
  )

#define dm_log_fatal_if_fmt(cond, fmt, ...) \
  statement( \
    log_fatal_if(cond, fmt "\nFile: " S_FMT "\nLine: %d", \
      __VA_ARGS__, S_ARG(file_being_lexed), lexer.current_line_number); \
  )

fz_internal void 
default_metaprogram_exclude_file_from_being_forward_declared(Default_Metaprogram *dm, String file_name)
{
  if (dm->excluded_files == NULL)
  {
    array_members_init_with_arena(dm->arena, dm->excluded_files, String, METAPROGRAM_MAX_EXCLUDED_FILES);
  }

  String *out = array_members_add(dm->excluded_files);
  *out = string_copy(dm->arena, file_name);
}

fz_internal void
default_metaprogram(Default_Metaprogram *dm, Command_Line *command_line, String src_directory, String *global_headers_extra_data)
{
  Scratch scratch = scratch_begin(0,0);

  array_members_init_with_arena(dm->arena, dm->files, DM_File, METAPROGRAM_MAX_FILES);
  String_List files = file_get_files_in_path(dm->arena, src_directory, true);

  for (String_Node *next = files.first; next != NULL; next = next->next)
  {
    String file_being_lexed = next->value;
    if (!is_file(file_being_lexed)) continue;

    if (string_contains(file_being_lexed, S("\\extern\\"))  ||
        string_contains(file_being_lexed, S("\\.git\\"))    ||
        string_contains(file_being_lexed, S("\\.svn\\"))    ||
        string_contains(file_being_lexed, S("\\.idea\\"))   ||
        string_contains(file_being_lexed, S("\\.vs\\"))     ||
        string_contains(file_being_lexed, S("\\.vscode\\")) ||
        string_contains(file_being_lexed, S("\\.code\\")))
    {
      continue;
    }

    String extension = file_get_extension(file_being_lexed);
    
    if (!string_equals(extension, S("c"), true) && !string_equals(extension, S("h"), true))
    {
      continue;
    }
    if (string_contains(file_being_lexed, S(".cgen.")))
    {
      continue;
    }
    if (string_contains(file_being_lexed, S("\\fzac\\")))
    {
      continue;
    }
    if (string_contains(file_being_lexed, S("global_headers.h")))
    {
      continue;
    }
    if (string_contains(file_being_lexed, S("metaprogram_global_headers.h")))
    {
      continue;
    }
    if (string_contains(file_being_lexed, S("metaprogram_generated.h")))
    {
      continue;
    }

    b32 continue_flag = false;;
    for (u32 i = 0; i < dm->excluded_files_count; i += 1)
    {
      if (string_contains(file_being_lexed, dm->excluded_files[i]))
      {
        continue_flag = true;
        break;
      }
    }
    if (continue_flag)
    {
      continue;
    }

    DM_File *dm_file = array_members_add(dm->files);
    memory_zero_struct(dm_file);

    // Counts lines of code
    {
      dm_file->lines_of_code        = 1;
      dm_file->significant_lines_of_code = 0;
      String file_content = file_load(scratch.arena, file_being_lexed);
      u64 scratch_position        = scratch.arena->position;
      b32 current_line_has_code   = false;
      b32 inside_block_comment    = false;

      for (u64 char_index = 0; char_index < file_content.count; char_index += 1)
      {
        u8 current_char = file_content.cstring[char_index];
        u8 next_char    = (char_index + 1 < file_content.count) ? file_content.cstring[char_index + 1] : 0;

        if (inside_block_comment)
        {
          if (current_char == '*' && next_char == '/')
          {
            inside_block_comment = false;
            char_index += 1;
          }
          else if (current_char == '\n')
          {
            dm_file->lines_of_code += 1;
          }
          continue;
        }

        if (current_char == '/' && next_char == '/')
        {
          while (char_index < file_content.count && file_content.cstring[char_index] != '\n')
          {
            char_index += 1;
          }
          current_char = file_content.cstring[char_index];
        }

        if (current_char == '/' && next_char == '*')
        {
          inside_block_comment = true;
          char_index += 1;
          continue;
        }

        if (current_char == '\n')
        {
          dm_file->lines_of_code += 1;
          if (current_line_has_code)
          {
            dm_file->significant_lines_of_code += 1;
          }
          current_line_has_code = false;
          continue;
        }

        if (current_char != ' ' && current_char != '\t' && current_char != '\r')
        {
          current_line_has_code = true;
        }
      }

      if (current_line_has_code)
      {
        dm_file->significant_lines_of_code += 1;
      }

      arena_pop_to(scratch.arena, scratch_position);    
    }

    dm_file->name = string_copy(dm->arena, file_being_lexed);
    array_members_init_with_arena(dm->arena, dm_file->function_definitions, DM_Code_Function, METAPROGRAM_MAX_FUNTIONS);
    array_members_init_with_arena(dm->arena, dm_file->struct_definitions, DM_Code_Struct, METAPROGRAM_MAX_STRUCTS);
    array_members_init_with_arena(dm->arena, dm_file->enum_definitions, DM_Code_Enum, METAPROGRAM_MAX_ENUMS);

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
        if (string_equals(token->value, S("fz_internal"), true) && is_first_token_on_line)
        {
          lexer_eat_token(&lexer);
          DM_Code_Function parsed_function = {0};
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
                string_builder_push(&return_type_builder, " ");
              }
              last_identifier = token->value;
            }
            else if (token->kind == Token_Asterisk)
            {
              last_identifier = string_join(scratch.arena, last_identifier, S("*"));
            }
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert(last_identifier.count > 0 && "Expected fz_internal name before '('");
          parsed_function.name = string_copy(dm->arena, last_identifier);
          parsed_function.return_type = string_builder_to_string(dm->arena, &return_type_builder);
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
          parsed_function.arguments = string_builder_to_string(dm->arena, &arguments_builder);
          string_builder_free(&arguments_builder);
          token = lexer_peek_token(&lexer);
          while (token->kind != Token_Semicolon  &&
                token->kind != Token_Open_Brace &&
                token->kind != Token_End_Of_File)
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert(token->kind != Token_End_Of_File && "Expected ';' or '{' after fz_internal signature");

          if (token->kind == Token_Open_Brace)
          {
            assert(dm_file->function_definitions_count < dm_file->function_definitions_capacity && "function_definitions capacity exceeded");
            dm_file->function_definitions[dm_file->function_definitions_count] = parsed_function;
            dm_file->function_definitions_count += 1;
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
          dm_log_fatal_if(token->kind != Token_Semicolon, "Expected ';' after struct definition.");
          lexer_eat_token(&lexer);

          assert(dm_file->struct_definitions_count < dm_file->struct_definitions_capacity && "struct_definitions capacity exceeded");
          dm_file->struct_definitions[dm_file->struct_definitions_count].name = string_copy(dm->arena, struct_name);
          dm_file->struct_definitions_count += 1;

          is_first_token_on_line = false;
        }
        else if (string_equals(token->value, S("enum_type"), true) && is_first_token_on_line)
        {
          lexer_eat_token(&lexer);
          // Expect '('
          token = lexer_peek_token(&lexer);
          while (token->kind == Token_Whitespace || token->kind == Token_Line_Break)
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert(token->kind == Token_Open_Parentheses && "Expected '(' after enum_type");
          lexer_eat_token(&lexer);

          // Parse name
          token = lexer_peek_token(&lexer);
          while (token->kind == Token_Whitespace || token->kind == Token_Line_Break)
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert(token->kind == Token_Identifier && "Expected enum name");
          DM_Code_Enum parsed_enum = {0};
          parsed_enum.name = string_copy(dm->arena, token->value);
          lexer_eat_token(&lexer);

          // Expect ','
          token = lexer_peek_token(&lexer);
          while (token->kind == Token_Whitespace || token->kind == Token_Line_Break)
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert(token->kind == Token_Comma && "Expected ',' after enum name");
          lexer_eat_token(&lexer);

          // Parse type
          token = lexer_peek_token(&lexer);
          while (token->kind == Token_Whitespace || token->kind == Token_Line_Break)
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert(token->kind == Token_Identifier && "Expected enum underlying type");
          parsed_enum.type = string_copy(dm->arena, token->value);
          lexer_eat_token(&lexer);
          // Expect ','
          token = lexer_peek_token(&lexer);
          while (token->kind == Token_Whitespace || token->kind == Token_Line_Break)
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert(token->kind == Token_Comma && "Expected ',' after enum type");
          lexer_eat_token(&lexer);

          // Parse to_string (0, 1, false, true)
          token = lexer_peek_token(&lexer);
          while (token->kind == Token_Whitespace || token->kind == Token_Line_Break)
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert((token->kind == Token_Identifier || token->kind == Token_Number) && "Expected enum 0/false or 1/true for to_string");
          parsed_enum.to_string = string_equals(token->value, S("1"), false) || string_equals(token->value, S("true"), false);
          if (token->kind == Token_Identifier)
          {
            if (string_equals(token->value, S("true"), true)) { parsed_enum.to_string = true; }
          }
          else if (token->kind == Token_Number)
          {
            if (string_equals(token->value, S("1"), true)) { parsed_enum.to_string = true; }
          }
          lexer_eat_token(&lexer);

          // Expect ')'
          token = lexer_peek_token(&lexer);
          while (token->kind == Token_Whitespace || token->kind == Token_Line_Break)
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          assert(token->kind == Token_Close_Parentheses && "Expected ')' after enum_type");
          lexer_eat_token(&lexer);
          // Find opening '{'
          token = lexer_peek_token(&lexer);
          while (token->kind != Token_Open_Brace)
          {
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          lexer_eat_token(&lexer); // eat '{'
          // Parse values until '}'
          token = lexer_peek_token(&lexer);
          while (token->kind != Token_Close_Brace)
          {
            if (token->kind == Token_Whitespace || token->kind == Token_Line_Break)
            {
              lexer_eat_token(&lexer);
              token = lexer_peek_token(&lexer);
              continue;
            }
            if (token->kind == Token_Identifier)
            {
              assert(parsed_enum.values_count < 512 && "enum values capacity exceeded");
              parsed_enum.values[parsed_enum.values_count] = string_copy(dm->arena, token->value);
              parsed_enum.values_count += 1;
              lexer_eat_token(&lexer);
              // Skip everything until the next ',' or '}'
              token = lexer_peek_token(&lexer);
              while (token->kind != Token_Comma && token->kind != Token_Close_Brace)
              {
                lexer_eat_token(&lexer);
                token = lexer_peek_token(&lexer);
              }
              if (token->kind == Token_Comma)
              {
                lexer_eat_token(&lexer); // eat ','
                token = lexer_peek_token(&lexer);
              }
              continue;
            }
            lexer_eat_token(&lexer);
            token = lexer_peek_token(&lexer);
          }
          lexer_eat_token(&lexer); // eat '}'
          // Store
          assert(dm_file->enum_definitions_count < dm_file->enum_definitions_capacity && "enum_definitions capacity exceeded");
          dm_file->enum_definitions[dm_file->enum_definitions_count] = parsed_enum;
          dm_file->enum_definitions_count += 1;
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

  String global_header_path = string_join(scratch.arena, src_directory, S(METAPROGRAM_GLOBAL_HEADERS_PATH));
  if (file_exists(global_header_path))
  {
    file_delete(global_header_path);
  }
  file_create(global_header_path);
  String_Builder global_header_builder = string_builder_init(kilobytes(64));
  string_builder_push(&global_header_builder, "#ifndef GLOBAL_HEADERS_H\n#define GLOBAL_HEADERS_H\n\n/* Generated File */\n");

  String metaprogram_global_header_path = string_join(scratch.arena, src_directory, S(METAPROGRAM_METAPROGRAM_GLOBAL_HEADERS_PATH));
  if (file_exists(metaprogram_global_header_path))
  {
    file_delete(metaprogram_global_header_path);
  }
  file_create(metaprogram_global_header_path);
  String_Builder metaprogram_global_header_builder = string_builder_init(kilobytes(64));
  string_builder_push(&metaprogram_global_header_builder, "#ifndef METAPROGRAM_GLOBAL_HEADERS_H\n#define METAPROGRAM_GLOBAL_HEADERS_H\n\n/* Generated File */\n");

  //
  // @TODO(Fz): We don't need to separate the headers and the implementation into two different loops. 
  // @TODO(fz): We should forward declare ALL enums first, then ALL structs, then ALL functions
  //

  // Just headers
  for (u32 file_index = 0; file_index < dm->files_count; file_index += 1)
  {
    DM_File *file = &dm->files[file_index];
    String extension = file_get_extension(file->name);
    if (!string_equals(extension, S("h"), true))
    {
      continue;
    }

    if (file->struct_definitions_count == 0 && file->function_definitions_count == 0)
    {
      continue;
    }

    String_Builder *builder = &global_header_builder;
    if (string_contains(file->name, S("metaprogram_")))
    {
      builder = &metaprogram_global_header_builder;
    }

    string_builder_pushf(builder, "\n/* File: "S_FMT" */\n", S_ARG(file->name));

    for (u32 enums_index = 0; enums_index < file->enum_definitions_count; enums_index += 1)
    {
      DM_Code_Enum *code_enum = &file->enum_definitions[enums_index];
      string_builder_pushf(builder, "typedef "S_FMT" "S_FMT";\n", S_ARG(code_enum->type), S_ARG(code_enum->name));
      if (code_enum->to_string)
      {
        string_builder_pushf(builder, "const char* "S_FMT"_to_string[] = { ", S_ARG(string_to_lower(scratch.arena, code_enum->name)));
        for (u32 enum_values_index = 0; enum_values_index < code_enum->values_count; enum_values_index += 1)
        {
          string_builder_pushf(builder, "\""S_FMT"\", ", S_ARG(code_enum->values[enum_values_index]));
        }
        string_builder_push(builder, "};\n");
      }
    }

    for (u32 structs_index = 0; structs_index < file->struct_definitions_count; structs_index += 1)
    {
      DM_Code_Struct *code_struct = &file->struct_definitions[structs_index];
      string_builder_pushf(builder, "typedef struct "S_FMT" "S_FMT";\n", S_ARG(code_struct->name), S_ARG(code_struct->name));
    }
    string_builder_push(builder, " ");

    for (u32 function_index = 0; function_index < file->function_definitions_count; function_index += 1)
    {
      DM_Code_Function *code_function = &file->function_definitions[function_index];
      if (string_equals(code_function->name, S("entry_point"), true) || string_equals(code_function->name, S("metaprogram_entry_point"), true))
      {
        continue;
      }
      string_builder_pushf(builder, "fz_internal %-16s %s(", code_function->return_type.cstring, code_function->name.cstring);
      if (code_function->arguments.count > 0)
      {
        string_builder_pushf(builder, "%s", code_function->arguments.cstring);
      }
      string_builder_push(builder, ");\n");
    }
  }

  // Just implementation
  for (u32 file_index = 0; file_index < dm->files_count; file_index += 1)
  {
    DM_File *file = &dm->files[file_index];
    String extension = file_get_extension(file->name);
    if (!string_equals(extension, S("c"), true))
    {
      continue;
    }

    if (file->struct_definitions_count == 0 && file->function_definitions_count == 0)
    {
      continue;
    }

    String_Builder *builder = &global_header_builder;
    if (string_contains(file->name, S("metaprogram_")))
    {
      builder = &metaprogram_global_header_builder;
    }

    string_builder_pushf(builder, "\n/* File: "S_FMT" */\n", S_ARG(file->name));

    for (u32 enums_index = 0; enums_index < file->enum_definitions_count; enums_index += 1)
    {
      DM_Code_Enum *code_enum = &file->enum_definitions[enums_index];
      string_builder_pushf(builder, "typedef "S_FMT" "S_FMT";\n", S_ARG(code_enum->type), S_ARG(code_enum->name));
      if (code_enum->to_string)
      {
        string_builder_pushf(builder, "const char* "S_FMT"_to_string[] = { ", S_ARG(string_to_lower(scratch.arena, code_enum->name)));
        for (u32 enum_values_index = 0; enum_values_index < code_enum->values_count; enum_values_index += 1)
        {
          string_builder_pushf(builder, "\""S_FMT"\", ", S_ARG(code_enum->values[enum_values_index]));
        }
        string_builder_push(builder, "};\n");
      }
    }

    for (u32 structs_index = 0; structs_index < file->struct_definitions_count; structs_index += 1)
    {
      DM_Code_Struct *code_struct = &file->struct_definitions[structs_index];
      string_builder_pushf(builder, "typedef struct "S_FMT" "S_FMT";\n", S_ARG(code_struct->name), S_ARG(code_struct->name));
    }

    for (u32 function_index = 0; function_index < file->function_definitions_count; function_index += 1)
    {
      DM_Code_Function *code_function = &file->function_definitions[function_index];
      if (string_equals(code_function->name, S("entry_point"), true) || string_equals(code_function->name, S("metaprogram_entry_point"), true))
      {
        continue;
      }
      string_builder_pushf(builder, "fz_internal %-20s %s(", code_function->return_type.cstring, code_function->name.cstring);
      if (code_function->arguments.count > 0)
      {
        string_builder_pushf(builder, "%s", code_function->arguments.cstring);
      }
      string_builder_push(builder, ");\n");
    }
  }

  string_builder_push(&global_header_builder, "\n#endif // GLOBAL_HEADERS_H");
  
  String final_string = string_builder_to_string(scratch.arena, &global_header_builder);
  u32 written = file_write(global_header_path, final_string.cstring, final_string.count);
  if (written == 0)
  {
    message_box(S("Default Metaprogram"), S("Error writing buffer to file"), S(__FILE__), __LINE__);
  }

  string_builder_free(&global_header_builder);

  // Metaprogram fz_global headers
  
  string_builder_push(&metaprogram_global_header_builder, "\n#endif // METAPROGRAM_GLOBAL_HEADERS_H");
  
  String metaprogram_final_string = string_builder_to_string(scratch.arena, &metaprogram_global_header_builder);
  u32 metaprogram_written = file_write(metaprogram_global_header_path, metaprogram_final_string.cstring, metaprogram_final_string.count);
  if (metaprogram_written == 0)
  {
    message_box(S("Default Metaprogram"), S("Error writing buffer to file"), S(__FILE__), __LINE__);
  }

  string_builder_free(&metaprogram_global_header_builder);

  // Metaprogram generated code
  String_Builder metaprogram_generated_builder = string_builder_init(thousand(5));

  string_builder_push(&metaprogram_generated_builder, "#ifndef METAPROGRAM_GENERATED\n#define METAPROGRAM_GENERATED\n\n/* Extra coded passed by the user metaprogram */\n\n");
  string_builder_push_string(&metaprogram_generated_builder, *global_headers_extra_data);
  string_builder_push(&metaprogram_generated_builder, "#endif // METAPROGRAM_GENERATED");

  String result = string_builder_to_string(scratch.arena, &metaprogram_generated_builder);

  String metaprogram_generated_path = string_join(scratch.arena, src_directory, S(METAPROGRAM_METAPROGRAM_GENERATED_PATH));
  if (file_exists(metaprogram_generated_path))
  {
    file_delete(metaprogram_generated_path);
  }
  file_create(metaprogram_generated_path);

  written = file_write(metaprogram_generated_path, result.cstring, result.count);
  if (written == 0)
  {
    message_box(S("Default Metaprogram"), S("Error writing to metaprogram_generated.h"), S(__FILE__), __LINE__);
  }
    
  string_builder_free(&metaprogram_generated_builder);

  scratch_end(&scratch);

  // Default meta program arguments
  scratch = scratch_begin(0,0);
  {
    if (command_line->args_count > 0)
    {
      for (u32 i = 0; i < command_line->args_count; i += 1)
      {
        Command_Line_Arg arg = command_line->args[i];
        if (string_equals(arg.value, S("cgen"), true))
        {
          String path = full_path_from_relative_path(scratch.arena, src_directory);
          CGen_Context cgen = cgen_run(path);
          cgen_execute_commands(&cgen);
        }
        else if (string_equals(arg.value, S("list-todos"), true))
        {
          List_Todos todos = list_todos(src_directory);
          print_list_todos(&todos);
        }
        else if (string_equals(arg.value, S("loc"), true) || string_equals(arg.value, S("lines-of-code"), true))
        {
          String_Builder loc_builder = string_builder_init(thousand(32));

          u32 total_loc  = 0;
          u32 total_sloc = 0;

          string_builder_push(&loc_builder, "\n");
          string_builder_pushf(&loc_builder, "+----------------------------------+--------+--------+--------+\n");
          string_builder_pushf(&loc_builder, "| %-32s | %6s | %6s | %6s |\n", "File", "LOC", "SLOC", "NSLOC");
          string_builder_pushf(&loc_builder, "+----------------------------------+--------+--------+--------+\n");

          for (u32 file_index = 0; file_index < dm->files_count; file_index += 1)
          {
            DM_File *file = &dm->files[file_index];
            string_builder_pushf(&loc_builder, "| %-32s | %6d | %6d | %6d |\n", file->name.cstring, file->lines_of_code, file->significant_lines_of_code, file->lines_of_code - file->significant_lines_of_code);
            total_loc  += file->lines_of_code;
            total_sloc += file->significant_lines_of_code;
          }

          string_builder_pushf(&loc_builder, "+----------------------------------+--------+--------+--------+\n");
          string_builder_pushf(&loc_builder, "| %-32s | %6d | %6d | %6d |\n", "Total", total_loc, total_sloc, total_loc - total_sloc);
          string_builder_pushf(&loc_builder, "+----------------------------------+--------+--------+--------+\n");

          String final_str = string_builder_to_string(scratch.arena, &loc_builder);
          string_print(final_str);
        }
        else if (string_equals(arg.value, S("h"), true) || string_equals(arg.value, S("help"), true))
        {
          String_Builder builder = string_builder_init(thousand(1));
          string_builder_push(&builder, 
          "\nUsage:\n"
          "No args: Runs userspace metaprogram + default metaprogram\n"
          "-loc: Shows lines of code\n"
          "-cgen: Run Code Generator module on all .cgen files\n"
          "-list-todos: Lists all todos in the whole codebase\n"
          "\n");
          String final_str = string_builder_to_string(scratch.arena, &builder);
          string_print(final_str);
        }
  		}
  	}
  }
  scratch_end(&scratch);
}

#endif // DEFAULT_METAPROGRAM_H
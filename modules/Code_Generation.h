#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include "../Base.h"

#include "String_Builder.h"
#include "Lexer.h"

#define CGEN_FILES_CAPACITY 64
#define CGEN_TABLES_CAPACITY 16
#define CGEN_GENERATORS_CAPACITY 16
#define CGEN_COLUMNS_CAPACITY 32
#define CGEN_ROWS_CAPACITY 128
#define CGEN_COMMANDS_CAPACITY 128
#define CGEN_STRING_ARGUMENTS_CAPACITY 128

#define _cgen_error(message) \
  message_box(S("Code Generator Error!"), message, S(__FILE__), __LINE__); \
  raddbg_break(); \
  assert(0)

typedef struct CGen_File CGen_File;
typedef struct CGen_String_Argument CGen_String_Argument;
typedef struct CGen_Table CGen_Table;
typedef struct CGen_Table_Row CGen_Table_Row;
typedef struct CGen_Command CGen_Command;
typedef struct CGen_Generator CGen_Generator;

struct CGen_String_Argument
{
  String name; /* Name of the argument. E.g. for $(location), name is "location" */
  u32 start_index; /* Start index of this argument. E.g. for $(location), start_index is the index of '$' */
  u32 length; /* Length of the whole argument. E.g. for $(location), length is the size of the string "$(location)" */
};

typedef struct CGen_String CGen_String; /* Used to define a string that has arguments to be replaced. */
struct CGen_String
{
  String data;

  CGen_String_Argument *arguments;
  u64 arguments_count;
  u64 arguments_capacity;
};

struct CGen_Table_Row
{
  String *entries;
  u64 entries_count;
  u64 entries_capacity;
};

struct CGen_Table
{
  String name;

  String *columns;
  u64 columns_count;
  u64 columns_capacity;

  CGen_Table_Row *rows;
  u64 rows_count;
  u64 rows_capacity;
};

typedef enum
{
  CGen_Command_Kind_None = 0,
  CGen_Command_Kind_String, /* Just pasts the string */
  CGen_Command_Kind_Foreach, /* Runs the string for each row in the table */
} CGen_Command_Kind;

struct CGen_Command
{
  CGen_Command_Kind kind;
  CGen_Table *table;
  CGen_String string;
};

struct CGen_Generator
{
  CGen_Command *commands;
  u64 commands_count;
  u64 commands_capacity;

  String custom_file_name;
};

struct CGen_File
{
  String name;

  CGen_Table *tables;
  u64 tables_count;
  u64 tables_capacity;

  CGen_Generator *generators;
  u64 generators_count;
  u64 generators_capacity;
};

typedef struct CGen_Context CGen_Context;
struct CGen_Context
{
  Arena *arena;

  CGen_File *files;
  u64 files_count;
  u64 files_capacity;
};

function CGen_Context cgen_run(String source_directory); /* Runs the code generator on any .cgen file */
function void cgen_execute_commands(CGen_Context *ctx); /* Runs all commands in the Code Generator Context */
function void cgen_parse_table(CGen_Context *ctx, Lexer *lexer, CGen_File *file); /* Parses a table */
function void cgen_parse_generator(CGen_Context *ctx, Lexer *lexer, CGen_File *file); /* Parses a generator */

function String      _cgen_string_replace_arguments(Arena *arena, CGen_String cgen_str, CGen_Table *table, u64 row_index); /* Replaces a CGen_String with the appropriate row values of the given table */
function CGen_String _cgen_string_from_string(Arena *arena, String str); /* Creates a CGen_String from a String */
function b32         _cgen_token_is_acceptable_row_value(Token *token); /* Checks if a token is a valid row value */

// @Section: Implementation

function CGen_Context
cgen_run(String source_directory)
{
  Scratch scratch = scratch_begin(0,0);

  CGen_Context result;
  memory_zero_struct(&result);
  result.arena = arena_alloc();

  arena_array_init(result.arena, result.files, CGen_File, CGEN_FILES_CAPACITY);

  String_List files = file_get_files_in_path(scratch.arena, source_directory, true);
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
    if (!string_equals(ext, S("cgen"), true)) continue;

    CGen_File *cgen_file;
    arena_array_push(cgen_file, result.files, result.files_count, result.files_capacity);
    memory_zero_struct(cgen_file);

    cgen_file->name = string_copy(result.arena, file_being_lexed);
    arena_array_init(result.arena, cgen_file->tables, CGen_Table, CGEN_TABLES_CAPACITY);
    arena_array_init(result.arena, cgen_file->generators, CGen_Generator, CGEN_GENERATORS_CAPACITY);

    Lexer lexer;
    lexer_init_with_single_file_path(&lexer, file_being_lexed, Trivia_None, Emit_String_Backtick|Emit_Line_Comments|Emit_Block_Comments);

    for (;;)
    {
      Token *token = lexer_peek_token(&lexer);

      if (token->kind == Token_End_Of_File)
      {
        break;
      }

      if (token->kind == Token_At)
      {
        lexer_eat_token(&lexer);
        token = lexer_peek_token(&lexer);

        if (token->kind != Token_Identifier)
        {
          _cgen_error(Sf(scratch.arena, "Unexpected kind after global '@' command. Value: "S_FMT"\n", S_ARG(token->value)));
        }

        if (string_equals(token->value, S("Table"), true))
        {
          cgen_parse_table(&result, &lexer, cgen_file);
        }
        else if (string_equals(token->value, S("Generator"), true))
        {
          cgen_parse_generator(&result, &lexer, cgen_file);
        }
        else
        {
          _cgen_error(Sf(scratch.arena, "Unexpected tag after '@'. Value: "S_FMT"\n", S_ARG(token->value)));
        }
      }
    }
  }

  scratch_end(&scratch);
  return result;
}

function void
cgen_execute_commands(CGen_Context *ctx)
{
  Scratch scratch = scratch_begin(0,0);
  
  for (u32 file_index = 0; file_index < ctx->files_count; file_index += 1)
  {
    CGen_File *file = &ctx->files[file_index];

    u8 *str = file->name.cstring;
    u64 len = file->name.count;

    s64 slash_index = -1;
    s64 dot_index   = -1;

    for (s64 i = (s64)len - 1; i >= 0; --i)
    {
      u8 c = str[i];
      if (c == '.' && dot_index < 0)
      {
        dot_index = i;
        continue;
      }
      if (c == '/' || c == '\\')
      {
        slash_index = i;
        break;
      }
    }

    if (slash_index < 0) 
    {
      _cgen_error(S("Unable to find a file path separator."));
    }

    u8 separator   = str[slash_index];
    u64 name_start = (u64)slash_index + 1;
    u64 name_end   = (dot_index > slash_index) ? (u64)dot_index : len;

    String output_directory = string_zero();
    String name      = string_substring(scratch.arena, file->name, name_start, name_end);
    output_directory = string_substring(scratch.arena, file->name, 0, (u64)slash_index + 1);
    output_directory = string_join(scratch.arena, output_directory, S("generated"));
    if (!directory_exists(output_directory))
    {
      directory_create(output_directory);
    }
    output_directory = string_join(scratch.arena, output_directory, (separator == '/') ? S("/") : S("\\"));
    
    String output_file = string_join(scratch.arena, output_directory, name);
    output_file        = string_join(scratch.arena, output_file, S(".generated.inl"));

    for (u32 generator_index = 0; generator_index < file->generators_count; generator_index += 1)
    {
      CGen_Generator *generator = &file->generators[generator_index];

      String_Builder buffer = string_builder_init(kilobytes(32));
      string_builder_pushf(&buffer, "/* Generated code */\n\n");

      if (generator->custom_file_name.count > 0)
      {
        output_file = string_join(scratch.arena, output_directory, generator->custom_file_name);
      }

      for (u32 command_index = 0; command_index < generator->commands_count; command_index += 1)
      {
        CGen_Command *command = &generator->commands[command_index];

        switch (command->kind)
        {
          case CGen_Command_Kind_String:
          {
            string_builder_push(&buffer, (const char*)command->string.data.cstring);
          }
          break;

          case CGen_Command_Kind_Foreach:
          {
            for (u32 row_index = 0; row_index < command->table->rows_count; row_index += 1)
            {
              String final_string = _cgen_string_replace_arguments(scratch.arena, command->string, command->table, row_index);
              string_builder_pushf(&buffer, (const char*)final_string.cstring);
            }
          }
          break;

          default:
          {
            _cgen_error(Sf(scratch.arena, "Unhandled CGen_Command_Kind: %u\n", command->kind));
          }
          break;
        }
      }

      if (file_exists(output_file))
      {
        file_delete(output_file);
      }

      u32 written = file_write(output_file, buffer.data, buffer.count);
      if (written == 0)
      {
        _cgen_error(Sf(scratch.arena, "Unable to write buffer to "S_FMT"\n", S_ARG(output_file)));
      }
      string_builder_free(&buffer);
    }
  }
  scratch_end(&scratch);
}

function void
cgen_parse_table(CGen_Context *ctx, Lexer *lexer, CGen_File *file)
{
  Scratch scratch = scratch_begin(0,0);

  CGen_Table *result;
  arena_array_push(result, file->tables, file->tables_count, file->tables_capacity);
  memory_zero_struct(result);

  arena_array_init(ctx->arena, result->columns, String, CGEN_COLUMNS_CAPACITY);
  arena_array_init(ctx->arena, result->rows, CGen_Table_Row, CGEN_ROWS_CAPACITY);

  Token *token = lexer_peek_token(lexer);
  if (!string_equals(token->value, S("Table"), true))
  {
    _cgen_error(Sf(scratch.arena, "Expected 'Table' keyword, got: "S_FMT"\n", S_ARG(token->value)));
  }
  lexer_eat_token(lexer);

  token = lexer_peek_token(lexer);
  if (token->kind != Token_Identifier)
  {
    _cgen_error(Sf(scratch.arena, "Expected table name identifier, got: "S_FMT"\n", S_ARG(token->value)));
  }
  result->name = string_copy(ctx->arena, token->value);
  lexer_eat_token(lexer);

  token = lexer_peek_token(lexer);
  if (token->kind != Token_Open_Parentheses)
  {
    _cgen_error(Sf(scratch.arena, "Expected '(' after table name, got: "S_FMT"\n", S_ARG(token->value)));
  }
  lexer_eat_token(lexer);
  token = lexer_peek_token(lexer);

  for (;;)
  {
    if (token->kind == Token_Close_Parentheses) break;
    if (token->kind != Token_Identifier)
    {
      _cgen_error(Sf(scratch.arena, "Expected column name identifier, got: "S_FMT"\n", S_ARG(token->value)));
    }

    String *column;
    arena_array_push(column, result->columns, result->columns_count, result->columns_capacity);
    *column = string_copy(ctx->arena, token->value);

    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);
  }

  if (token->kind != Token_Close_Parentheses)
  {
    _cgen_error(Sf(scratch.arena, "Expected ')' after column list, got: "S_FMT"\n", S_ARG(token->value)));
  }
  lexer_eat_token(lexer);
  token = lexer_peek_token(lexer);

  if (token->kind != Token_Open_Brace)
  {
    _cgen_error(Sf(scratch.arena, "Expected '{' to begin table body, got: "S_FMT"\n", S_ARG(token->value)));
  }
  lexer_eat_token(lexer);
  token = lexer_peek_token(lexer);

  // parse rows
  for (;;)
  {
    if (token->kind == Token_Close_Brace)
    {
      break;
    }
    if (token->kind != Token_Open_Brace)
    {
      _cgen_error(Sf(scratch.arena, "Expected '{' to begin table row, got: "S_FMT"\n", S_ARG(token->value)));
    }
    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);

    CGen_Table_Row *row;
    arena_array_push(row, result->rows, result->rows_count, result->rows_capacity);
    memory_zero_struct(row);

    arena_array_init(ctx->arena, row->entries, String, CGEN_COLUMNS_CAPACITY);

    for (;;)
    {
      if (token->kind == Token_Close_Brace)
      {
        break;
      }

      if (_cgen_token_is_acceptable_row_value(token))
      {
        String *entry;
        arena_array_push(entry, row->entries, row->entries_count, row->entries_capacity);
        *entry = string_copy(ctx->arena, token->value);
      }
      else
      {
        _cgen_error(Sf(scratch.arena, "Unexpected row value. Value: "S_FMT"\n", S_ARG(token->value)));
      }

      lexer_eat_token(lexer);
      token = lexer_peek_token(lexer);
    }

    if (row->entries_count != result->columns_count)
    {
      _cgen_error(Sf(scratch.arena, "Row has %llu entries but table expects %llu columns\n", row->entries_count, result->columns_count));
    }

    if (token->kind != Token_Close_Brace)
    {
      _cgen_error(Sf(scratch.arena, "Expected '}' to close table row, got: "S_FMT"\n", S_ARG(token->value)));
    }
    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);
  }

  if (token->kind != Token_Close_Brace)
  {
    _cgen_error(Sf(scratch.arena, "Expected '}' to close table body, got: "S_FMT"\n", S_ARG(token->value)));
  }
  lexer_eat_token(lexer);

  scratch_end(&scratch);
}

function void
cgen_parse_generator(CGen_Context *ctx, Lexer *lexer, CGen_File *file)
{
  Scratch scratch = scratch_begin(0,0);

  CGen_Generator *generator;
  arena_array_push(generator, file->generators, file->generators_count, file->generators_capacity);
  memory_zero_struct(generator);
  arena_array_init(ctx->arena, generator->commands, CGen_Command, CGEN_COMMANDS_CAPACITY);

  generator->custom_file_name = string_zero();

  Token *token = lexer_peek_token(lexer);
  if (!string_equals(token->value, S("Generator"), true))
  {
    _cgen_error(Sf(scratch.arena, "Expected 'Generator' keyword, got: "S_FMT"\n", S_ARG(token->value)));
  }

  lexer_eat_token(lexer);
  token = lexer_peek_token(lexer);

  // parse optional custom file name in parentheses
  if (token->kind == Token_Open_Parentheses)
  {
    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);

    for (;;)
    {
      if (token->kind == Token_Close_Parentheses)
      {
        break;
      }

      generator->custom_file_name = string_join(ctx->arena, generator->custom_file_name, token->value);
      lexer_eat_token(lexer);
      token = lexer_peek_token(lexer);
    }

    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);
  }

  if (token->kind != Token_Open_Brace)
  {
    _cgen_error(Sf(scratch.arena, "Expected '{' to begin generator body, got: "S_FMT"\n", S_ARG(token->value)));
  }

  lexer_eat_token(lexer);
  token = lexer_peek_token(lexer);

  for (;;)
  {
    if (token->kind == Token_Close_Brace)
    {
      break;
    }

    if (token->kind == Token_At)
    {
      lexer_eat_token(lexer);
      token = lexer_peek_token(lexer);

      if (string_equals(token->value, S("foreach"), true))
      {
        lexer_eat_token(lexer);
        token = lexer_peek_token(lexer);

        if (token->kind != Token_Open_Parentheses)
        {
          _cgen_error(Sf(scratch.arena, "Expected '(' after foreach, got: "S_FMT"\n", S_ARG(token->value)));
        }
        lexer_eat_token(lexer);
        token = lexer_peek_token(lexer);

        if (token->kind != Token_Identifier)
        {
          _cgen_error(Sf(scratch.arena, "Expected table name identifier, got: "S_FMT"\n", S_ARG(token->value)));
        }

        String table_name = token->value;
        lexer_eat_token(lexer);
        token = lexer_peek_token(lexer);

        if (token->kind != Token_Close_Parentheses)
        {
          _cgen_error(Sf(scratch.arena, "Expected ')' after table name, got: "S_FMT"\n", S_ARG(token->value)));
        }
        lexer_eat_token(lexer);
        token = lexer_peek_token(lexer);

        if (token->kind != Token_String_Backtick)
        {
          _cgen_error(Sf(scratch.arena, "Expected backtick string after foreach, got: "S_FMT"\n", S_ARG(token->value)));
        }

        CGen_Table *found_table = NULL;
        for (u64 i = 0; i < file->tables_count; i++)
        {
          if (string_equals(file->tables[i].name, table_name, true))
          {
            found_table = &file->tables[i];
            break;
          }
        }

        if (!found_table)
        {
          _cgen_error(Sf(scratch.arena, "Table '"S_FMT"' not found in file\n", S_ARG(table_name)));
        }

        CGen_Command *command;
        arena_array_push(command, generator->commands, generator->commands_count, generator->commands_capacity);

        command->kind = CGen_Command_Kind_Foreach;
        command->table = found_table;
        command->string = _cgen_string_from_string(ctx->arena, token->value);

        lexer_eat_token(lexer);
        token = lexer_peek_token(lexer);
      }
      else
      {
        _cgen_error(Sf(scratch.arena, "Unexpected command in @Generator after @. Got: "S_FMT"\n", S_ARG(token->value)));
      }
    }
    else if (token->kind == Token_String_Backtick)
    {
      CGen_Command *command;
      arena_array_push(command, generator->commands, generator->commands_count, generator->commands_capacity);

      command->kind = CGen_Command_Kind_String;
      command->table = NULL;
      command->string = _cgen_string_from_string(ctx->arena, token->value);

      lexer_eat_token(lexer);
      token = lexer_peek_token(lexer);
    }
    else
    {
      _cgen_error(Sf(scratch.arena, "Expected 'foreach' or backtick string in generator body, got: "S_FMT"\n", S_ARG(token->value)));
    }
  }

  if (token->kind != Token_Close_Brace)
  {
    _cgen_error(Sf(scratch.arena, "Expected '}' to close generator body, got: "S_FMT"\n", S_ARG(token->value)));
  }
  lexer_eat_token(lexer);

  scratch_end(&scratch);
}

function CGen_String
_cgen_string_from_string(Arena *arena, String str)
{
  Scratch scratch = scratch_begin(0,0);

  str = string_replace_backslash_n(scratch.arena, str);

  CGen_String result;
  memory_zero_struct(&result);

  result.data = string_copy(arena, str);

  arena_array_init(arena, result.arguments, CGen_String_Argument, CGEN_STRING_ARGUMENTS_CAPACITY);

  for (u32 i = 0; i < str.count; i += 1)
  {
    if (i + 1 < str.count && str.cstring[i] == '$' && str.cstring[i + 1] == '(')
    {
      u32 start_index = i;

      u32 name_start = i + 2;
      u32 name_end = name_start;
      b32 found_close = false;

      for (u32 j = name_start; j < str.count; j += 1)
      {
        if (str.cstring[j] == ')')
        {
          name_end = j;
          found_close = true;
          break;
        }
      }

      if (found_close)
      {
        u32 name_length = name_end - name_start;

        u8 *name_str = push_array(arena, u8, name_length + 1);
        memory_copy(name_str, str.cstring + name_start, name_length);
        name_str[name_length] = 0;

        CGen_String_Argument *arg;
        arena_array_push(arg, result.arguments, result.arguments_count, result.arguments_capacity);

        arg->name.count = name_length;
        arg->name.cstring = name_str;
        arg->start_index = start_index;
        arg->length = name_end - start_index + 1; // include ')'

        i = name_end;
      }
    }
  }

  scratch_end(&scratch);
  return result;
}

function String
_cgen_string_replace_arguments(Arena *arena, CGen_String cgen_str, CGen_Table *table, u64 row_index)
{
  Scratch scratch = scratch_begin(&arena, 1);
  
  if (row_index >= table->rows_count)
  {
    _cgen_error(Sf(scratch.arena, "Row index %llu out of bounds for table '"S_FMT"'\n", row_index, S_ARG(table->name)));
  }
  
  String result = string_copy(arena, cgen_str.data);
  CGen_Table_Row *row = &table->rows[row_index];
  
  for (s64 i = cgen_str.arguments_count - 1; i >= 0; i -= 1)
  {
    CGen_String_Argument *arg = &cgen_str.arguments[i];
    
    s64 column_index = -1;
    for (u64 j = 0; j < table->columns_count; j += 1)
    {
      if (string_equals(table->columns[j], arg->name, true))
      {
        column_index = j;
        break;
      }
    }
    
    if (column_index == -1)
    {
      _cgen_error(Sf(scratch.arena, "Column '"S_FMT"' not found in table '"S_FMT"'\n", S_ARG(arg->name), S_ARG(table->name)));
    }
    
    if (column_index >= (s64)row->entries_count)
    {
      _cgen_error(Sf(scratch.arena, "Column index %lld out of bounds for row %llu in table '"S_FMT"'\n", column_index, row_index, S_ARG(table->name)));
    }
    
    String replacement = row->entries[column_index];
    result = string_replace_range(arena, result, arg->start_index, arg->length, replacement);
  }
  
  scratch_end(&scratch);
  return result;
}

function b32
_cgen_token_is_acceptable_row_value(Token *token)
{
  b32 result = false;
  if (token->kind == Token_Identifier || token->kind == Token_Number || token->kind == Token_String_Backtick)
  {
    result = true;
  }
  return result;
}

#endif // CODE_GENERATION_H
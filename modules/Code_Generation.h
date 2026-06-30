#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

/*
  How to:

  Create a table with any number of cols and rows with the following syntax:

  ```cgen
  @Table <Any_Table_Name> (col0 col1 col2 col3 col4 ...)
  {
    { col0_val col1_val col2_val col3_val col4_val ... }
    { col0_val col1_val col2_val col3_val col4_val ... }
    ...
  }
  ```

  Generate code from each table

  ````cgen
  @Generator(<file_name>)
  {
    `This will generate an arbitrary string`

    @foreach(<Any_Table_Name>) ` This line will be generated per row of <Any_Table_Name>. You can reference cells by their column like $(col0) which will give you the col0 value for this current row`
  }
  ```

  Built in variables

  @index
  You can use $(@index) to get the index of the foreach (row number)

  @datetime_now
  You can use $(@datetime_now) to get a string with the current date and time

  Built in functions

  When referencing any row/cell inside a @foreach with $(col_name), you can call some functions on it.
  Let's say $(col0) referes to a cell that has the string "code Generation".
  And let's say we have this string in our code generator: `This is the value of $(col0)`
  This will expand to "This is the value of code Generation"
  You can call the following functions:

  - to_upper:
  `This is the value of $(col0.to_upper())` -> "This is the value of CODE GENERATION"
  - to_lower:
  `This is the value of $(col0.to_lower())` -> "This is the value of code generation"
  - capitalize:
  `This is the value of $(col0.capitalize())` -> "This is the value of Code Generation"
  - to_snake_case
  `This is the value of $(col0.to_snake_case())` -> "This is the value of code_generation"
  - to_pascal_snake_case
  `This is the value of $(col0.to_pascal_snake_case())` -> "This is the value of Code_Generation"
*/

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

// Functions that you can call on a string variable, when you refer to it in a generator. 
// For example, you have a column called name and you would refer to it as $(name) which would resolve to, let's say, francisco.
// For the function capitalize(), you would call it like $(name.capitalize()), which would then resolve to Francisco
typedef enum
{
  CGen_Builtin_Function_None,
  CGen_Builtin_Function_Capitalize,           // Capitalize
  CGen_Builtin_Function_To_Lower,             // to_lower
  CGen_Builtin_Function_To_Upper,             // TO_UPPER
  CGen_Builtin_Function_To_Snake_Case,        // snake_case
  CGen_Builtin_Function_To_Pascal_Snake_Case, // Pascal_Snake_Case
} CGen_Builtin_Function;

typedef enum
{
  CGen_Command_Kind_None = 0,
  CGen_Command_Kind_String, /* Just pasts the string */
  CGen_Command_Kind_Foreach, /* Runs the string for each row in the table */
} CGen_Command_Kind;

// @NOTE(fz): Keep synced with CGen_Builtin_Function !!
global const char* cgen_builtin_function_names[] = {
  "capitalize",
  "to_lower",
  "to_upper",
  "to_snake_case",
  "to_pascal_snake_case"
};


typedef struct CGen_String_Argument CGen_String_Argument;
struct CGen_String_Argument
{
  String name; /* Name of the argument. E.g. for $(location), name is "location" */
  u32 start_index; /* Start index of this argument. E.g. for $(location), start_index is the index of '$' */
  u32 length; /* Length of the whole argument. E.g. for $(location), length is the size of the string "$(location)" */

  CGen_Builtin_Function builtin_function;
};

typedef struct CGen_String CGen_String; /* Used to define a string that has arguments to be replaced. */
struct CGen_String
{
  String data;

  Array(arguments, CGen_String_Argument);
};

typedef struct CGen_Table_Row CGen_Table_Row;
struct CGen_Table_Row
{
  Array(entries, String);
};

typedef struct CGen_Table CGen_Table;
struct CGen_Table
{
  String name;

  Array(columns, String);
  Array(rows, CGen_Table_Row);
};

typedef struct CGen_Command CGen_Command;
struct CGen_Command
{
  CGen_Command_Kind kind;
  CGen_Table *table;
  CGen_String string;
};

typedef struct CGen_Generator CGen_Generator;
struct CGen_Generator
{
  String custom_file_name;
  Array(commands, CGen_Command);
};

typedef struct CGen_File CGen_File;
struct CGen_File
{
  String name;

  Array(tables, CGen_Table);
  Array(generators, CGen_Generator);
};

typedef struct CGen_Context CGen_Context;
struct CGen_Context
{
  Arena *arena;
  Array(files, CGen_File);
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

  array_init(result.arena, result.files, CGen_File, CGEN_FILES_CAPACITY);

  String_List files = file_get_files_in_path(scratch.arena, source_directory, true);
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
    if (!string_equals(extension, S("cgen"), true)) continue;

    CGen_File *cgen_file = array_add(result.files);
    memory_zero_struct(cgen_file);

    cgen_file->name = string_copy(result.arena, file_being_lexed);
    array_init(result.arena, cgen_file->tables, CGen_Table, CGEN_TABLES_CAPACITY);
    array_init(result.arena, cgen_file->generators, CGen_Generator, CGEN_GENERATORS_CAPACITY);

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
    output_file        = string_join(scratch.arena, output_file, S(".cgen.inl"));

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

  CGen_Table *result = array_add(file->tables);
  memory_zero_struct(result);

  array_init(ctx->arena, result->columns, String, CGEN_COLUMNS_CAPACITY);
  array_init(ctx->arena, result->rows, CGen_Table_Row, CGEN_ROWS_CAPACITY);

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

    String *column = array_add(result->columns);
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
      _cgen_error(Sf(scratch.arena, "Cgen file: "S_FMT" :: line:%d\nExpected '{' to begin table row, got: "S_FMT"\n", S_ARG(file->name), lexer->current_line_number, S_ARG(token->value)));
    }
    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);

    CGen_Table_Row *row = array_add(result->rows);
    memory_zero_struct(row);

    array_init(ctx->arena, row->entries, String, CGEN_COLUMNS_CAPACITY);

    for (;;)
    {
      if (token->kind == Token_Close_Brace)
      {
        break;
      }

      if (_cgen_token_is_acceptable_row_value(token))
      {
        String *entry = array_add(row->entries);
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

  CGen_Generator *generator = array_add(file->generators);
  memory_zero_struct(generator);
  array_init(ctx->arena, generator->commands, CGen_Command, CGEN_COMMANDS_CAPACITY);

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

        CGen_Command *command = array_add(generator->commands);

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
      CGen_Command *command = array_add(generator->commands);

      command->kind = CGen_Command_Kind_String;
      command->table = NULL;
      command->string = _cgen_string_from_string(ctx->arena, token->value);

      lexer_eat_token(lexer);
      token = lexer_peek_token(lexer);
    }
    else if (token->kind == Token_Comment_Line)
    {
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
  Scratch scratch = scratch_begin(0, 0);

  CGen_String result;
  memory_zero_struct(&result);

  str = string_replace_backslash_n(scratch.arena, str);
  result.data = string_copy(arena, str);
  array_init(arena, result.arguments, CGen_String_Argument, CGEN_STRING_ARGUMENTS_CAPACITY);

  for (u32 i = 0; i < str.count; i += 1)
  {
    if (i + 1 < str.count && str.cstring[i] == '$' && str.cstring[i + 1] == '(')
    {
      u32 start_index = i;

      u32 name_start = i + 2;
      u32 name_end   = name_start;
      u32 arg_end    = 0;

      b32 found_close = false;

      String variable_name = {0};
      String function_name = {0};

      u32 paren_depth = 1;

      for (u32 j = name_start; j < str.count; j += 1)
      {
        if (str.cstring[j] == '.')
        {
          name_end = j;

          variable_name.cstring = str.cstring + name_start;
          variable_name.count   = j - name_start;

          u32 function_start = j + 1;
          u32 function_end   = function_start;

          while (function_end < str.count && str.cstring[function_end] != '(' && str.cstring[function_end] != ')')
          {
            function_end += 1;
          }

          function_name.cstring = str.cstring + function_start;
          function_name.count   = function_end - function_start;

          j = function_end - 1;
          continue;
        }

        if (str.cstring[j] == '(')
        {
          paren_depth += 1;
        }
        else if (str.cstring[j] == ')')
        {
          paren_depth -= 1;

          if (paren_depth == 0)
          {
            if (variable_name.count == 0)
            {
              variable_name.cstring = str.cstring + name_start;
              variable_name.count   = j - name_start;
            }

            arg_end = j;

            if (name_end == name_start)
            {
              name_end = j;
            }

            found_close = true;
            break;
          }
        }
      }

      if (!found_close)
      {
        _cgen_error(S("Failed to find closing ')' for argument."));
      }

      u8 *name_str = push_array(arena, u8, variable_name.count + 1);
      memory_copy(name_str, variable_name.cstring, variable_name.count);
      name_str[variable_name.count] = 0;

      CGen_String_Argument *arg = array_add(result.arguments);

      arg->name.count   = variable_name.count;
      arg->name.cstring = name_str;
      arg->builtin_function = CGen_Builtin_Function_None;

      for (u32 builtin_func = 0; builtin_func < array_count(cgen_builtin_function_names); builtin_func += 1)
      {
        if (string_equals(function_name, string_from_cstring((u8*)cgen_builtin_function_names[builtin_func]), true))
        {
          arg->builtin_function = (CGen_Builtin_Function)(builtin_func + 1);
          break;
        }
      }

      arg->start_index = start_index;
      arg->length      = arg_end - start_index + 1;
      i = arg_end;
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
    _cgen_error(Sf(scratch.arena, "Row index %llu out of bounds for table '" S_FMT "'", row_index, S_ARG(table->name)));
  }

  String result = string_copy(arena, cgen_str.data);

  CGen_Table_Row *row = &table->rows[row_index];

  for (s64 i = cgen_str.arguments_count - 1; i >= 0; i -= 1)
  {
    CGen_String_Argument *arg = &cgen_str.arguments[i];
    String replacement = {0};

    if (string_equals(arg->name, S("@index"), true))
    {
      replacement = Sf(arena, "%llu", row_index);
    }
    else if (string_equals(arg->name, S("@datetime_now"), true))
    {
      replacement = datetime_to_string(arena, datetime_now(), false);
    }
    else
    {
      s64 column_index = -1;

      for (u64 j = 0; j < table->columns_count; j += 1)
      {
        if (string_equals(table->columns[j], arg->name, true))
        {
          column_index = (s64)j;
          break;
        }
      }

      if (column_index == -1)
      {
        _cgen_error(Sf(scratch.arena, "Column '" S_FMT "' not found in table '" S_FMT "'", S_ARG(arg->name), S_ARG(table->name)));
      }

      replacement = string_copy(arena, row->entries[column_index]);
    }

    switch (arg->builtin_function)
    {
      case CGen_Builtin_Function_To_Upper:
      {
        for (u32 c = 0; c < replacement.count; c += 1)
        {
          if (char_is_alpha_lower(replacement.cstring[c]))
          {
            replacement.cstring[c] = char_to_upper(replacement.cstring[c]);
          }
        }
      } break;

      case CGen_Builtin_Function_To_Lower:
      {
        for (u32 c = 0; c < replacement.count; c += 1)
        {
          if (char_is_alpha_upper(replacement.cstring[c]))
          {
            replacement.cstring[c] = char_to_lower(replacement.cstring[c]);
          }
        }
      } break;

      case CGen_Builtin_Function_Capitalize:
      {
        if (char_is_alpha_lower(replacement.cstring[0]))
        {
          replacement.cstring[0] = char_to_upper(replacement.cstring[0]);
        }
      } break;

      case CGen_Builtin_Function_To_Snake_Case:
      {
        for (u32 c = 0; c < replacement.count; c += 1)
        {
          if (replacement.cstring[c] == ' ')
          {
            replacement.cstring[c] = '_';
          }
          if (char_is_alpha_upper(replacement.cstring[c]))
          {
            replacement.cstring[c] = char_to_lower(replacement.cstring[c]);
          }
        }
      } break;

      case CGen_Builtin_Function_To_Pascal_Snake_Case:
      {
        b32 capitalize_next = true;

        for (u32 c = 0; c < replacement.count; c += 1)
        {
          if (replacement.cstring[c] == ' ')
          {
            replacement.cstring[c] = '_';
            capitalize_next = true;
            continue;
          }

          if (replacement.cstring[c] == '_')
          {
            capitalize_next = true;
            continue;
          }

          if (capitalize_next)
          {
            if (char_is_alpha_lower(replacement.cstring[c]))
            {
              replacement.cstring[c] = char_to_upper(replacement.cstring[c]);
            }
            capitalize_next = false;
          }
          else
          {
            if (char_is_alpha_upper(replacement.cstring[c]))
            {
              replacement.cstring[c] = char_to_lower(replacement.cstring[c]);
            }
          }
        }
      } break;

      case CGen_Builtin_Function_None: case_fallthrough
      default:
      {
      } break;
    }

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
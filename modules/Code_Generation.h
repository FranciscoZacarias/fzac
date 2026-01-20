#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include "../Base.h"
#include "Lexer.h"

#define _cgen_error(message) \
  error_box(S("Code Generator Error!"), message, S(__FILE__), __LINE__); \
  raddbg_break(); \
  assert(0)

typedef struct CGen_File CGen_File;
Make_Array_Type(CGen_File);

typedef struct CGen_String_Argument CGen_String_Argument;
Make_Array_Type(CGen_String_Argument);

typedef struct CGen_Table CGen_Table;
Make_Array_Type(CGen_Table);

typedef struct CGen_Table_Row CGen_Table_Row;
Make_Array_Type(CGen_Table_Row);

typedef struct CGen_Command CGen_Command;
Make_Array_Type(CGen_Command);

typedef struct CGen_Generator CGen_Generator;
Make_Array_Type(CGen_Generator);

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
  Array(CGen_String_Argument) arguments;
};

struct CGen_Table_Row
{
  Array(String) entries;
};

struct CGen_Table
{
  String name;
  Array(String) columns;
  Array(CGen_Table_Row) rows;
};

typedef enum
{
  CGen_Command_None = 0,
  CGen_Command_String, /* Just pasts the string */
  CGen_Command_Foreach, /* Runs the string for each row in the table */
} CGen_Command_Kind;

struct CGen_Command
{
  CGen_Command_Kind kind;
  CGen_File* file;
  CGen_Table* table;
  CGen_String template_string;
};

struct CGen_Generator
{
  Array(CGen_Command) commands; /* Commands specified in this generator */
  String path; /* Defaults to the directory of the current file being lexed. Can be overridden as backtick argument for a generator, like @Generator(path=`some\custom\path`) */
};

struct CGen_File
{
  String name;
  Array(CGen_Table) tables;
  Array(CGen_Generator) generators;
};

typedef struct CGen_Context CGen_Context;
struct CGen_Context
{
  Arena* arena;
  Array(CGen_File) files;
};

function CGen_Context cgen_run(String source_directory); /* Runs the code generator on any .cgen file */
function void cgen_parse_table(CGen_Context* ctx, Lexer* lexer, CGen_File* file); /* Parses a table */
function void cgen_parse_generator(CGen_Context* ctx, Lexer* lexer, CGen_File* file); /* Parses a generator */

function CGen_String _cgen_string_from_string(Arena* arena, String str); /* Creates a CGen_String from a String */
function b32 _cgen_token_is_acceptable_row_value(Token* token); /* Checks if a token is a valid row value */

// @Section: Implementation

function CGen_Context
cgen_run(String source_directory)
{
  Scratch scratch = scratch_begin(0,0);

  CGen_Context result;
  memory_zero_struct(&result);

  result.arena = arena_alloc();
  result.files = array_make(CGen_File, 8);

  String_List files = file_get_files_in_path(scratch.arena, source_directory, true);
  for (String_Node* next = files.first; next != NULL; next = next->next)
  {
    String file_being_lexed = next->value;

    if (!is_file(file_being_lexed))
    {
      continue;
    }

    if(string_contains(file_being_lexed, S("\\Extern\\"))  ||
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

    if (!string_equals(ext, S("cgen"), true))
    {
      continue;
    }

    CGen_File* cgen_file;
    array_get_next(&result.files, CGen_File, cgen_file);
    cgen_file->name = string_copy(result.arena, file_being_lexed);

    Lexer lexer;
    lexer_init_with_single_file_path(&lexer, file_being_lexed, Trivia_None, Emit_String_Backtick|Emit_Line_Comments|Emit_Block_Comments);

    for (;;)
    {
      Token* token = lexer_peek_token(&lexer);

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
cgen_parse_table(CGen_Context* ctx, Lexer* lexer, CGen_File* file)
{
  Scratch scratch = scratch_begin(0,0);
  CGen_Table* result;
  array_get_next(&file->tables, CGen_Table, result);
  memory_zero_struct(result);
  result->columns = array_make(String, 8);
  result->rows    = array_make(CGen_Table_Row, 32);
  
  Token* token = lexer_peek_token(lexer);
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
    
    String* column;
    array_get_next(&result->columns, String, column);
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
    
    CGen_Table_Row* row;
    array_get_next(&result->rows, CGen_Table_Row, row);
    memory_zero_struct(row);
    row->entries = array_make(String, result->columns.count);
    
    for (;;)
    {
      if (token->kind == Token_Close_Brace)
      {
        break;
      }
      
      if (_cgen_token_is_acceptable_row_value(token))
      {
        String* entry;
        array_get_next(&row->entries, String, entry);
        *entry = string_copy(ctx->arena, token->value);
      }
      else
      {
        _cgen_error(Sf(scratch.arena, "Unexpected row value. Value: "S_FMT"\n", S_ARG(token->value)));
      }
      
      lexer_eat_token(lexer);
      token = lexer_peek_token(lexer);
    }
    
    if (result->columns.count != row->entries.count)
    {
      _cgen_error(Sf(scratch.arena, "Row has %llu entries but table expects %llu columns\n", row->entries.count, result->columns.count));
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
cgen_parse_generator(CGen_Context* ctx, Lexer* lexer, CGen_File* file)
{
  Scratch scratch = scratch_begin(0,0);
  
  CGen_Generator* generator;
  array_get_next(&file->generators, CGen_Generator, generator);
  memory_zero_struct(generator);
  generator->commands = array_make(CGen_Command, 16);
  generator->path = string_zero();
  
  Token* token = lexer_peek_token(lexer);
  if (!string_equals(token->value, S("Generator"), true))
  {
    _cgen_error(Sf(scratch.arena, "Expected 'Generator' keyword, got: "S_FMT"\n", S_ARG(token->value)));
  }
  
  lexer_eat_token(lexer);
  token = lexer_peek_token(lexer);
  
  if (token->kind == Token_Open_Parentheses)
  {
    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);
    
    if (!string_equals(token->value, S("path"), true))
    {
      _cgen_error(Sf(scratch.arena, "Expected 'path' argument, got: "S_FMT"\n", S_ARG(token->value)));
    }

    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);
    
    if (token->kind != Token_Equal)
    {
      _cgen_error(Sf(scratch.arena, "Expected '=' after path, got: "S_FMT"\n", S_ARG(token->value)));
    }

    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);
    
    if (token->kind != Token_String_Backtick)
    {
      _cgen_error(Sf(scratch.arena, "Expected path string value, got: "S_FMT"\n", S_ARG(token->value)));
    }

    generator->path = string_copy(ctx->arena, token->value);

    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);
    
    if (token->kind != Token_Close_Parentheses)
    {
      _cgen_error(Sf(scratch.arena, "Expected ')' after path argument, got: "S_FMT"\n", S_ARG(token->value)));
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
      
        CGen_Table* found_table = NULL;
        for (u64 i = 0; i < file->tables.count; i += 1)
        {
          if (string_equals(file->tables.data[i].name, table_name, true))
          {
            found_table = &file->tables.data[i];
            break;
          }
        }
      
        if (!found_table)
        {
          _cgen_error(Sf(scratch.arena, "Table '"S_FMT"' not found in file\n", S_ARG(table_name)));
        }
      
        CGen_Command* command;
        array_get_next(&generator->commands, CGen_Command, command);
        command->kind = CGen_Command_Foreach;
        command->file = file;
        command->table = found_table;
        command->template_string = _cgen_string_from_string(ctx->arena, token->value);
      
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
      CGen_Command* command;
      array_get_next(&generator->commands, CGen_Command, command);
      command->kind = CGen_Command_String;
      command->file = file;
      command->table = NULL;
      command->template_string = _cgen_string_from_string(ctx->arena, token->value);
      
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
_cgen_string_from_string(Arena* arena, String str)
{
  CGen_String result;
  result.data = string_copy(arena, str);
  result.arguments = array_make(CGen_String_Argument, 8);
  
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
        u8* name_str = push_array(arena, u8, name_length + 1);
        memory_copy(name_str, str.cstring + name_start, name_length);
        name_str[name_length] = 0;
        
        CGen_String_Argument* arg;
        array_get_next(&result.arguments, CGen_String_Argument, arg);
        arg->name.count = name_length;
        arg->name.cstring = name_str;
        arg->start_index = start_index;
        arg->length = name_end - start_index + 1; // +1 to include the ')'
        
        i = name_end;
      }
    }
  }
  
  return result;
}

function b32
_cgen_token_is_acceptable_row_value(Token* token)
{
  b32 result = false;
  if (token->kind == Token_Identifier      ||
      token->kind == Token_Number          ||
      token->kind == Token_String_Backtick)
  {
    result = true;
  }
  return result;
}

#endif // CODE_GENERATION_H
#ifndef INTROSPECTION_H
#define INTROSPECTION_H

/* @File: Implementa introspection implemented specifically for my C code base. It makes very important assumptions and hardcodes things based on those assumptions.
          This is only meant to work on a codebase that follows my programming style. */

/*
@TODO:
- Rewrite parse_data_type to use the lexer directly, and not a view into a token array. Also, exceptionally, recognize Array(Some_Type) as a data type.
*/

#include "../Base.h"
#include "Lexer.h"

#define intsp_error(intsp, message) intsp->fatal_error=true;raddbg_break();error_box(S("Introspection Error!"), message, S(__FILE__), __LINE__)

typedef struct Intsp_File Intsp_File;
Make_Array_Type(Intsp_File);
struct Intsp_File
{
  String path;
  String documentation;
};

typedef struct Intsp_Source_Location Intsp_Source_Location;
struct Intsp_Source_Location
{
  Intsp_File* file;
  u32 line;
};

typedef struct Intsp_Data_Type Intsp_Data_Type;
struct Intsp_Data_Type
{
  String name;

  b32 is_c_array; /* [] */
  String array_length; /* If data_type.is_array == true, we set here the size of the array, which can be a macro, so we just store a string. */

  b32 is_pointer;
  u32 indirection_level;
};

typedef struct Intsp_Parse_Data_Type_Result Intsp_Parse_Data_Type_Result;
struct Intsp_Parse_Data_Type_Result
{
  Array(String) names; /* It's an array in case this is a multiple declaration. Like: f32 x, y, z; */
  Intsp_Data_Type data_type;
};

typedef struct Intsp_Typedef_Object Intsp_Typedef_Object; /* For when the user typedefs something that isn't a struct or enum. E.g. "typedef unsigned char c8", unsigned char is data_type, c8 is identifier */
Make_Array_Type(Intsp_Typedef_Object);
struct Intsp_Typedef_Object
{
  String identifier;
  Intsp_Data_Type data_type;
  Intsp_Source_Location location;
};

typedef struct Intsp_Function_Argument Intsp_Function_Argument;
Make_Array_Type(Intsp_Function_Argument);
struct Intsp_Function_Argument
{
  String identifier;
  b32 is_var_args; /* If true, overrides data_type and identifier, since they lose meaning. */
  Intsp_Data_Type data_type;
};

typedef struct Intsp_Function_Object Intsp_Function_Object;
Make_Array_Type(Intsp_Function_Object);
struct Intsp_Function_Object
{
  String identifier;
  Intsp_Data_Type return_type;

  Array(Intsp_Function_Argument) arguments;

  String body;
  String documentation;

  Intsp_Source_Location declaration;
  Intsp_Source_Location implementation;
};

typedef u32 Intsp_Aggregate_Kind;
enum META_ENUM_LINK(Intsp_Aggregate_Kind)
{
  Intsp_Aggregate_Kind_Struct,
  Intsp_Aggregate_Kind_Union,
};

typedef struct Intsp_Aggregate_Member Intsp_Aggregate_Member;
Make_Array_Type(Intsp_Aggregate_Member);

typedef struct Intsp_Aggregate_Object Intsp_Aggregate_Object;
Make_Array_Type(Intsp_Aggregate_Object);

struct Intsp_Aggregate_Object
{
  String identifier;
  u64 hash;

  Intsp_Aggregate_Kind kind;

  b32 is_nested; /* If this is nested inside another Intsp_Aggregate_Object, we don't enforce it to be named. */
  Intsp_Aggregate_Object* parent; /* if is_nested=false then this is NULL. Pointer to struct where this object is nested at. */

  Array(Intsp_Aggregate_Member) members;

  String documentation;

  Intsp_Source_Location forward_declaration;
  Intsp_Source_Location definition;
};

struct Intsp_Aggregate_Member
{
  String identifier;
  String documentation;

  b32 is_aggregate; /* If true, only aggregate_data is relavant, if it's false, only data_type is relevant*/
  union
  {
    Intsp_Data_Type data_type; /* if is_aggregate=false */
    Intsp_Aggregate_Object aggregate_data; /* if is_aggregate=true */
  };

};

typedef struct Intsp_Enum_Member Intsp_Enum_Member;
Make_Array_Type(Intsp_Enum_Member);
struct Intsp_Enum_Member
{
  String identifier;
  String documentation;
  b32 is_explicit_bit_flag; /* In case the value is something like Trivia_Line_Break = (1 << 5) ... */
  b32 is_one_or_more_enum_flags; /* In case the value is something like Trivia_All = Trivia_Whitespace|Trivia_Carriage_Return|Trivia_Tab|Trivia_Form_Feed|Trivia_Vertical_Tab|Trivia_Line_Break ... */
  s64 value;
};

typedef struct Intsp_Enum_Object Intsp_Enum_Object;
Make_Array_Type(Intsp_Enum_Object);
struct Intsp_Enum_Object
{
  String identifier;
  String type;

  Array(Intsp_Enum_Member) members;

  Intsp_Typedef_Object* type_casted_at;
  Intsp_Source_Location definition;
};

typedef struct Intsp_Global_Variable Intsp_Global_Variable;
Make_Array_Type(Intsp_Global_Variable);
struct Intsp_Global_Variable
{
  String identifier; /* Variable name */
  Intsp_Data_Type data_type; /* Data type */
  String body; // If it's initialized, this string will contain the value;
  String documentation; /* Documentation in front of the variable */
};

typedef u32 Intsp_Log_Severity;
enum META_ENUM_LINK(Intsp_Log_Severity)
{
  Intsp_Log_Severity_None = 0,
  Intsp_Log_Severity_Log,
  Intsp_Log_Severity_Warning,
  Intsp_Log_Severity_Error, /* Error just acts as a fatal error. */
};

typedef struct Intsp_Log Intsp_Log;
Make_Array_Type(Intsp_Log);
struct Intsp_Log
{
  Intsp_Log_Severity severity;
  String text;
};

typedef struct Introspection Introspection; /* Contains an introspection of a C program in my codebase. Note that reports are not exhaustive, they are only the ones caught on the lexing level. */
struct Introspection
{
  Arena* arena;

  b32 fatal_error; /* If true, signals intsp_run to stop. */

  Array(Intsp_Log) logs;
  Array(Intsp_File) files;
  Array(Intsp_Function_Object) functions;
  Array(Intsp_Aggregate_Object) aggregates;
  Array(Intsp_Enum_Object) enums;
  Array(Intsp_Typedef_Object) typedefs;
  Array(Intsp_Global_Variable) globals;
};

function Introspection intsp_run(String source_directory, b32 introspect_base_library); /* Parses a project directory. */
function b32           intsp_report_introspection(Introspection* introspection, b32 verbose, String* out); /* Produces a report of the introspection. Reports true if no fatal errors were found. If there are fatal errors, the introspection will be incomplete but the report still has an ERROR log. */
function void          intsp_report_functions(Introspection* introspection, b32 report_documentation, String* out);
function void          intsp_report_enums(Introspection* introspection, String* out);
function String        intsp_data_type_to_string(Arena* arena, Intsp_Data_Type type); /* Convers a Intsp_Data_Type to a string */
function String        intsp_function_signature_to_string(Arena* arena, Intsp_Function_Object* f); /* Convers a Intsp_Function_Object to a string */

function u32  _intsp_parse_enum_members(Arena* arena, Lexer* lexer, Intsp_Enum_Member* temp_members, u32 max_members); /* Parses the members of an enum. Anything after { and before }. Returns how many members were added */
function void _intsp_parse_enum(Lexer* lexer, Introspection* introspection, Intsp_File* file_obj); /* Parses enums */
function void _intsp_parse_typedef(Lexer* lexer, Introspection* introspection, Intsp_File* file_obj); /* Parses typedefs that are not structs nor enums */
function void _intsp_parse_function(Lexer* lexer, Introspection* introspection, Intsp_File* file_obj);
function void _intsp_parse_global(Lexer* lexer, Introspection* introspection, Intsp_File* file_obj);
function void _intsp_parse_preprocessor_define(Lexer* lexer, Introspection* introspection, Intsp_File* file_obj);
function void _intsp_eat_trivia(Lexer* lexer);
function void _intsp_report(Introspection* introspection, Intsp_Log_Severity severity, Intsp_File* file, u32 line, u32 col, String text); /* Adds a report to the introspection struct */
function u64  _intsp_aggregate_hash(Intsp_Aggregate_Object* obj);
function Intsp_Aggregate_Object*      _intsp_parse_aggregate(Lexer* lexer, Introspection* introspection, Intsp_File* file_obj, Intsp_Aggregate_Object* parent);
function Intsp_Parse_Data_Type_Result _intsp_parse_datatype(Introspection* introspection, Lexer* lexer); /* Parses a data type from a view into a token array */
function Intsp_Function_Object*       _intsp_find_function(Introspection* introspection, String function_name);
function Intsp_Enum_Object*           _intsp_find_enum(Introspection* introspection, String enum_name);
function Intsp_Aggregate_Object*      _intsp_find_struct(Introspection* introspection, u64 hash);
function Intsp_Typedef_Object*        _intsp_find_typedef(Introspection* introspection, String typedef_name);

function Introspection
intsp_run(String source_directory, b32 introspect_base_library)
{
  Scratch scratch = scratch_begin(0,0);

  Introspection result;
  memory_zero_struct(&result);

  result.arena = arena_alloc();

  result.files      = array_make(Intsp_File,             32);
  result.logs       = array_make(Intsp_Log,              32);
  result.functions  = array_make(Intsp_Function_Object,  512);
  result.aggregates = array_make(Intsp_Aggregate_Object, 256);
  result.enums      = array_make(Intsp_Enum_Object,      256);
  result.typedefs   = array_make(Intsp_Typedef_Object,   256);
  result.globals    = array_make(Intsp_Global_Variable,  256);

  String_List files = file_get_files_in_path(scratch.arena, source_directory, true);
  for (String_Node* next = files.first; next != NULL; next = next->next)
  {
    String file_being_lexed = next->value;

    if (!is_file(file_being_lexed)) continue; // Skip anything that is not a file

    // Directories we don't care about.
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

    if (!introspect_base_library && string_contains(file_being_lexed, S("fzac"))) continue; // Skip anything from fzac library if we're introspecting it
    if (string_contains(file_being_lexed, S("metaprogram.c"))) continue; // Skip the metaprogram itself

    String_View extension = file_get_extension(file_being_lexed);
    String ext = string_new(extension.count, extension.string);

    // Make sure we introspect files we care about
    if (!(string_equals(ext, S("c"), true) || string_equals(ext, S("h"), true)))
    {
      continue;
    }

    Lexer lexer;
    lexer_init_with_single_file_path(&lexer, file_being_lexed, Trivia_Line_Break|Trivia_Whitespace|Trivia_Tab, Emit_Character_Literals|Emit_String_Literals|Emit_Line_Comments|Emit_Block_Comments);

    Intsp_File* intsp_file;
    array_get_next(&result.files, Intsp_File, intsp_file);
    intsp_file->path = string_copy(result.arena, file_being_lexed);

    b32 at_line_start = true;
    for (;;)
    {
      if (result.fatal_error)
      {
        break;
      }

      Token* token = lexer_peek_token(&lexer);

      if (token->kind == Token_End_Of_File) break;

      if (token->kind == Token_Line_Break)
      {
        lexer_eat_token(&lexer);
        at_line_start = true;
        continue;
      }

      if (token->kind == Token_Comment_Line || token->kind == Token_Comment_Block)
      {
        if (string_contains(token->value, S("@File:")))
        {
          intsp_file->documentation = string_copy(result.arena, token->value);
        }
        lexer_eat_token(&lexer);
        continue;
      }

      if (token_is_trivia(token))
      {
        lexer_eat_token(&lexer);
        continue;
      }

      if (!at_line_start)
      {
        lexer_eat_token(&lexer);
        continue;
      }

      if (token->kind == Token_Hash)
      {
        b32 prev_was_backslash = false;
        while (token->kind != Token_End_Of_File)
        {
          if (token->kind == Token_Backslash)
          {
            prev_was_backslash = true;
          }
          else if (token->kind == Token_Line_Break)
          {
            if (!prev_was_backslash)
            {
              break;
            }
            prev_was_backslash = false;
          }
          else
          {
            prev_was_backslash = false;
          }
    
          lexer_eat_token(&lexer);
          token = lexer_peek_token(&lexer);
        }
        at_line_start = true;
        continue;
      }

      if (token->kind == Token_Identifier)
      {
        if (string_equals(token->value, S("function"), true))
        {
          _intsp_parse_function(&lexer, &result, intsp_file);
          at_line_start = true;
          continue;
        }
        else if (string_equals(token->value, S("enum"), true))
        {
          _intsp_parse_enum(&lexer, &result, intsp_file);
          at_line_start = true;
          continue;
        }
        else if (string_equals(token->value, S("struct"), true) || string_equals(token->value, S("union"), true))
        {
          _intsp_parse_aggregate(&lexer, &result, intsp_file, NULL);
          at_line_start = false;
          continue;
        }
        else if (string_equals(token->value, S("global"), true))
        {
          _intsp_parse_global(&lexer, &result, intsp_file);
          at_line_start = false;
          continue;
        }
        else if (string_equals(token->value, S("typedef"), true))
        {
          lexer_eat_token(&lexer);
          _intsp_eat_trivia(&lexer);
          token = lexer_peek_token(&lexer);

          if (string_equals(token->value, S("enum"), true))
          {
            _intsp_parse_enum(&lexer, &result, intsp_file);
            at_line_start = true;
            continue;
          }
          else if (string_equals(token->value, S("struct"), true) || string_equals(token->value, S("union"), true))
          {
            _intsp_parse_aggregate(&lexer, &result, intsp_file, NULL);
            at_line_start = true;
            continue;
          }
          else
          {
            _intsp_parse_typedef(&lexer, &result, intsp_file);
            at_line_start = true;
            continue;
          }
        }
      }

      lexer_eat_token(&lexer);
      at_line_start = false;
    }
  }

  scratch_end(&scratch);
  return result;
}

function b32
intsp_report_introspection(Introspection* introspection, b32 verbose, String* out)
{
  String_Buffer buffer = {0};
  string_buffer_init(&buffer, &MallocAllocator, kilobytes(8));

  u32 warnings_and_errors = 0;

  string_buffer_push(&buffer, "[Introspection report]:\n");

  for (u32 i = 0; i < introspection->logs.count; i += 1)
  {
    Intsp_Log* ilog = &array_get(&introspection->logs, i);

    if ((ilog->severity == Intsp_Log_Severity_Log || ilog->severity == Intsp_Log_Severity_None) && !verbose)
    {
      continue;
    }

    warnings_and_errors += 1;
    string_buffer_push(&buffer, S_FMT, S_ARG(ilog->text));
  }

  string_buffer_push(&buffer, "Total Reports: %u\nWarnings and Errors: %u\n\n", introspection->logs.count, warnings_and_errors);

  if (introspection->fatal_error) string_buffer_push(&buffer, "[Introspection is incomplete due to ERROR!]\n" "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  *out = string_buffer_to_string(introspection->arena, &buffer);
  string_buffer_free(&buffer);

  return !introspection->fatal_error;
}


function void
intsp_report_functions(Introspection* introspection, b32 report_documentation, String* out)
{
  String_Buffer buffer = {0};
  string_buffer_init(&buffer, &MallocAllocator, 1024);

  u32 num_mismatches = 0;
  String null = S("NULL");

  for (u32 i = 0; i < introspection->functions.count; i += 1)
  {
    Intsp_Function_Object* func_object = &introspection->functions.data[i];

    if (func_object->implementation.file == NULL || func_object->declaration.file == NULL)
    {
      string_buffer_push(&buffer, "Function "S_FMT" has a mismatch\n", S_ARG(func_object->identifier));
      num_mismatches += 1;
    }
  }

  string_buffer_push(&buffer, "[Function Report]:\n");
  if (num_mismatches == 0) string_buffer_push(&buffer, " No Function mismatches.\n");
  else                     string_buffer_push(&buffer, " Mismatches: %u/%u\n", num_mismatches, introspection->functions.count);

  *out = string_buffer_to_string(introspection->arena, &buffer);
  string_buffer_free(&buffer);
}

function void
intsp_report_enums(Introspection* introspection, String* out)
{
  String_Buffer buffer = {0};
  string_buffer_init(&buffer, &MallocAllocator, 512);

  u32 num_missing_identifier = 0;

  for (u32 i = 0; i < introspection->enums.count; i += 1)
  {
    Intsp_Enum_Object* e = &array_get(&introspection->enums, i);
    if (e->identifier.count == 0)
    {
      num_missing_identifier += 1;

      Intsp_Enum_Member enum_member = array_get(&e->members, 0);
      string_buffer_push(&buffer, "Enum missing identifier. First enum value: "S_FMT"\n", S_ARG(enum_member.identifier));
    }
  }

  string_buffer_push(&buffer, "\n[Enum Report]:\n");

  if (num_missing_identifier == 0) string_buffer_push(&buffer, " Enums are ok.\n\n");
  else                             string_buffer_push(&buffer, " Enums missing identifiers: %u/%u\n\n", num_missing_identifier, introspection->enums.count);

  *out = string_buffer_to_string(introspection->arena, &buffer);
  string_buffer_free(&buffer);
}

function String
intsp_data_type_to_string(Arena* arena, Intsp_Data_Type type)
{
  String_Buffer buffer;
  string_buffer_init(&buffer, &MallocAllocator, 64);

  string_buffer_push(&buffer, S_FMT, S_ARG(type.name));
  if (type.is_pointer) for (u32 i = 0; i < type.indirection_level; i += 1) string_buffer_push(&buffer, "*");
  if (type.is_c_array) string_buffer_push(&buffer, "[]");

  String result = string_buffer_to_string(arena, &buffer);
  string_buffer_free(&buffer);
  return result;
}

function String
intsp_function_signature_to_string(Arena* arena, Intsp_Function_Object* f)
{
  Scratch scratch = scratch_begin(0, 0);

  String_Buffer buffer;
  string_buffer_init(&buffer, &MallocAllocator, 128);

  String return_type = intsp_data_type_to_string(scratch.arena, f->return_type);
  string_buffer_push(&buffer, S_FMT " ", S_ARG(return_type));
  string_buffer_push(&buffer, S_FMT "(", S_ARG(f->identifier));

  for (u32 i = 0; i < f->arguments.count; i += 1)
  {
    Intsp_Function_Argument* arg = &array_get(&f->arguments, i);
    if (arg->is_var_args)
    {
      string_buffer_push(&buffer, "...");
    }
    else
    {
      String arg_type = intsp_data_type_to_string(scratch.arena, arg->data_type);
      string_buffer_push(&buffer, S_FMT " " S_FMT, S_ARG(arg_type), S_ARG(arg->identifier) );
    }
    if (i + 1 < f->arguments.count) string_buffer_push(&buffer, ", ");
  }

  string_buffer_push(&buffer, ")");
  String result = string_buffer_to_string(arena, &buffer);
  string_buffer_free(&buffer);

  scratch_end(&scratch);
  return result;
}

function Intsp_Enum_Object*
_intsp_find_enum(Introspection* introspection, String enum_name)
{
  Intsp_Enum_Object* result = NULL;
  for (u32 i = 0; i < introspection->enums.count; i += 1)
  {
    Intsp_Enum_Object* e = &array_get(&introspection->enums, i);
    if (string_equals(enum_name, e->identifier, true))
    {
      result = e;
      break;
    }
  }
  return result;
}

function Intsp_Function_Object*
_intsp_find_function(Introspection* introspection, String function_name)
{
  Intsp_Function_Object* result = NULL;
  for (u32 i = 0; i < introspection->functions.count; i += 1)
  {
    Intsp_Function_Object* function_object = &(introspection->functions.data[i]);
    if (string_equals(function_name, function_object->identifier, true))
    {
      result = function_object;
      break;
    }
  }
  return result;
}

function Intsp_Aggregate_Object*
_intsp_find_struct(Introspection* introspection, u64 hash)
{
  Intsp_Aggregate_Object* result = NULL;
  for (u32 i = 0; i < introspection->aggregates.count; i += 1)
  {
    Intsp_Aggregate_Object* struct_object = &array_get(&introspection->aggregates, i);
    if (struct_object->hash == hash)
    {
      result = struct_object;
      break;
    }
  }
  return result;
}

function Intsp_Typedef_Object*
_intsp_find_typedef(Introspection* introspection, String typedef_name)
{
  Intsp_Typedef_Object* result = NULL;
  for (u32 i = 0; i < introspection->typedefs.count; i += 1)
  {
    Intsp_Typedef_Object* typedef_object = &array_get(&introspection->typedefs, i);
    if (string_equals(typedef_name, typedef_object->identifier, true))
    {
      result = typedef_object;
      break;
    }
  }
  return result;
}

function void
_intsp_parse_global(Lexer* lexer, Introspection* introspection, Intsp_File* file_obj)
{
  Token* token = lexer_peek_token(lexer);
  assert(token->kind == Token_Identifier);

  lexer_eat_token(lexer);
  _intsp_eat_trivia(lexer);

  Scratch scratch = scratch_begin(0,0);

  Intsp_Global_Variable* gvariable;
  array_get_next(&introspection->globals, Intsp_Global_Variable, gvariable);
  
  Intsp_Parse_Data_Type_Result result = _intsp_parse_datatype(introspection, lexer);
  gvariable->data_type = result.data_type;

  if (result.names.count != 1)
  {
    intsp_error(introspection, S("We do not allow multiple declarations in global variables."));
    return;
  }
  gvariable->identifier = string_copy(introspection->arena, array_get(&result.names, 0));

  _intsp_eat_trivia(lexer);
  token = lexer_peek_token(lexer);

  for (;;)
  {
    token = lexer_peek_token(lexer);
    if (token->kind == Token_Line_Break) break;
    if(token->kind == Token_Comment_Block || token->kind == Token_Comment_Line)
    {
      u64 start = 2;
      u64 end   = token->value.count - ((token->kind == Token_Comment_Block) ? 2 : 0);
      String comment = string_slice(scratch.arena, token->value, start, end - start);
      gvariable->documentation = string_trim(introspection->arena, comment);
    }
    lexer_eat_token(lexer);
  }

  scratch_end(&scratch);
}

function void
_intsp_parse_preprocessor_define(Lexer* lexer, Introspection* introspection, Intsp_File* file_obj)
{
  
}

function Intsp_Parse_Data_Type_Result
_intsp_parse_datatype(Introspection* introspection, Lexer* lexer)
{
  Scratch scratch = scratch_begin(0, 0);
  Intsp_Parse_Data_Type_Result result;
  memory_zero_struct(&result);
  result.names = array_make(String, 1);
  Intsp_Data_Type* type = &result.data_type;
  
  Token tokens[64];
  u32 tokens_count = 0;
  
  // Collect all tokens until semicolon or equal sign
  for (;;)
  {
    Token* t = lexer_peek_token(lexer);
    if (t->kind == Token_Semicolon || t->kind == Token_Equal)
    {
      break;
    }
    tokens[tokens_count++] = *t;
    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
  }
  
  if (tokens_count == 0)
  {
    intsp_error(introspection, S("Expected declaration."));
    scratch_end(&scratch);
    return result;
  }
  
  // Determine what kind of declaration this is
  b32 is_function_pointer = false;
  u32 function_pointer_start = 0;
  b32 has_comma = false;
  u32 first_comma_index = 0;
  
  for (u32 i = 0; i < tokens_count; i++)
  {
    if (tokens[i].kind == Token_Comma)
    {
      if (!has_comma)
      {
        first_comma_index = i;
      }
      has_comma = true;
    }
    if (tokens[i].kind == Token_Open_Parentheses)
    {
      // Check if next token is asterisk (function pointer)
      if (i + 1 < tokens_count && tokens[i + 1].kind == Token_Asterisk)
      {
        is_function_pointer = true;
        function_pointer_start = i;
        break;
      }
    }
  }
  
  if (is_function_pointer && has_comma)
  {
    intsp_error(introspection, S("We do not support multiple declaration with function pointers."));
    scratch_end(&scratch);
    return result;
  }
  
  if (is_function_pointer)
  {
    // Function pointer: everything before '(' is the type, everything from '(' onward is the name
    String type_name = string_zero();
    String function_signature = string_zero();
    
    for (u32 i = 0; i < function_pointer_start; i++)
    {
      if (type_name.count > 0 && tokens[i].kind != Token_Asterisk)
      {
        type_name = string_join(scratch.arena, type_name, S(" "));
      }
      type_name = string_join(scratch.arena, type_name, tokens[i].value);
      
      if (tokens[i].kind == Token_Asterisk)
      {
        type->is_pointer = true;
        type->indirection_level += 1;
      }
    }
    
    for (u32 i = function_pointer_start; i < tokens_count; i++)
    {
      function_signature = string_join(scratch.arena, function_signature, tokens[i].value);
    }
    
    type->name = string_trim(introspection->arena, string_copy(introspection->arena, type_name));
    array_push(&result.names, String, string_trim(introspection->arena, string_copy(introspection->arena, function_signature)));
  }
  else if (has_comma)
  {
    // Multiple declaration: everything before first identifier is the type
    // Identifiers separated by commas are the names
    
    // Find where the type ends (last non-identifier token before first comma or first identifier)
    u32 type_end = first_comma_index-1;
    
    // Build type name
    String type_name = string_zero();
    for (u32 i = 0; i < type_end; i++)
    {
      if (type_name.count > 0 && tokens[i].kind != Token_Asterisk)
      {
        type_name = string_join(scratch.arena, type_name, S(" "));
      }
      type_name = string_join(scratch.arena, type_name, tokens[i].value);
      
      if (tokens[i].kind == Token_Asterisk)
      {
        type->is_pointer = true;
        type->indirection_level += 1;
      }
    }
    type->name = string_trim(introspection->arena, string_copy(introspection->arena, type_name));
    
    // Collect variable names (skip commas)
    for (u32 i = type_end; i < tokens_count; i++)
    {
      if (tokens[i].kind == Token_Identifier)
      {
        array_push(&result.names, String, string_copy(introspection->arena, tokens[i].value));
      }
    }
  }
  else
  {
    // Simple declaration: last identifier is the name, everything before is the type
    u32 name_index = tokens_count - 1;
    
    // Handle array syntax: name might be followed by [...]
    b32 found_array = false;
    for (u32 i = 0; i < tokens_count; i++)
    {
      if (tokens[i].kind == Token_Open_Bracket)
      {
        type->is_c_array = true;
        found_array = true;
        name_index = i - 1; // Identifier is before '['
        
        // Get array length if present
        if (i + 1 < tokens_count && tokens[i + 1].kind != Token_Close_Bracket)
        {
          type->array_length = string_copy(introspection->arena, tokens[i + 1].value);
        }
        break;
      }
    }
    
    if (!found_array)
    {
      // Find the last identifier as the name
      for (s32 i = tokens_count - 1; i >= 0; i--)
      {
        if (tokens[i].kind == Token_Identifier)
        {
          name_index = i;
          break;
        }
      }
    }
    
    // Build type name from tokens before name_index
    String type_name = string_zero();
    for (u32 i = 0; i < name_index; i++)
    {
      if (type_name.count > 0 && tokens[i].kind != Token_Asterisk)
      {
        type_name = string_join(scratch.arena, type_name, S(" "));
      }
      type_name = string_join(scratch.arena, type_name, tokens[i].value);
      
      if (tokens[i].kind == Token_Asterisk)
      {
        type->is_pointer = true;
        type->indirection_level += 1;
      }
    }
    type->name = string_trim(introspection->arena, string_copy(introspection->arena, type_name));
    
    // Store the variable name
    array_push(&result.names, String, string_copy(introspection->arena, tokens[name_index].value));
  }
  
  scratch_end(&scratch);
  return result;
}

function u32
_intsp_parse_enum_members(Arena* arena, Lexer* lexer, Intsp_Enum_Member* temp_members, u32 max_members)
{
  Token* token = lexer_peek_token(lexer);
  assert(token->kind == Token_Open_Brace);

  lexer_eat_token(lexer);
  _intsp_eat_trivia(lexer);

  u32 members_count = 0;
  s64 next_enum_value = 0;

  for (;;)
  {
    token = lexer_peek_token(lexer);

    if(token->kind == Token_Close_Brace)
    {
      // Stop before the close brace; caller can eat it if needed
      break;
    }

    assert(token->kind == Token_Identifier);
    assert(members_count < max_members);

    Intsp_Enum_Member* member = &temp_members[members_count++];
    memory_zero_struct(member);

    member->identifier = string_copy(arena, token->value);

    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);

    token = lexer_peek_token(lexer);

    if(token->kind == Token_Equal)
    {
      lexer_eat_token(lexer);
      _intsp_eat_trivia(lexer);

      token = lexer_peek_token(lexer);

      // Flag: (1 << N)
      if(token->kind == Token_Open_Parentheses)
      {
        member->is_explicit_bit_flag = true;

        lexer_eat_token(lexer);
        _intsp_eat_trivia(lexer);

        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Number && token->value.cstring[0] == '1');
        lexer_eat_token(lexer);
        _intsp_eat_trivia(lexer);

        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Less_Less);
        lexer_eat_token(lexer);
        _intsp_eat_trivia(lexer);

        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Number);

        s64 shift;
        b32 ok = s64_from_string(token->value, &shift);
        assert(ok);

        member->value = ((s64)1 << shift);

        lexer_eat_token(lexer);
        _intsp_eat_trivia(lexer);

        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Close_Parentheses);

        lexer_eat_token(lexer);
        _intsp_eat_trivia(lexer);
      }
      // Symbol OR expression: A | B | C
      else if(token->kind == Token_Identifier)
      {
        member->is_one_or_more_enum_flags = true;
        member->value = 0;

        for(;;)
        {
          assert(token->kind == Token_Identifier);

          b32 found = 0;
          for(u32 i = 0; i < members_count; ++i)
          {
            if(string_equals(token->value, temp_members[i].identifier, true))
            {
              member->value |= temp_members[i].value;
              found = 1;
              break;
            }
          }
          assert(found);

          lexer_eat_token(lexer);
          _intsp_eat_trivia(lexer);

          token = lexer_peek_token(lexer);
          if(token->kind != Token_Or) break;

          lexer_eat_token(lexer);
          _intsp_eat_trivia(lexer);

          token = lexer_peek_token(lexer);
        }
      }
      // Plain number
      else
      {
        assert(token->kind == Token_Number);

        b32 ok = s64_from_string(token->value, &member->value);
        assert(ok);

        lexer_eat_token(lexer);
        _intsp_eat_trivia(lexer);
      }

      next_enum_value = member->value + 1;
    }
    else
    {
      // No explicit assignment
      member->value = next_enum_value++;
    }

    // Trailing commas
    token = lexer_peek_token(lexer);
    if(token->kind == Token_Comma)
    {
      lexer_eat_token(lexer);
      _intsp_eat_trivia(lexer);
    }

    // Trailing comment/documentation
    token = lexer_peek_token(lexer);
    if(token->kind == Token_Comment_Block || token->kind == Token_Comment_Line)
    {
      u64 start = 2;
      u64 end   = token->value.count - ((token->kind == Token_Comment_Block) ? 2 : 0);

      String comment = string_slice(arena, token->value, start, end - start);
      member->documentation = string_trim(arena, comment);

      lexer_eat_token(lexer);
      _intsp_eat_trivia(lexer);
    }

    token = lexer_peek_token(lexer);
    assert(token->kind == Token_Identifier || token->kind == Token_Close_Brace);
  }

  return members_count;
}

function void
_intsp_parse_typedef(Lexer* lexer, Introspection* introspection, Intsp_File* file_obj)
{
  Scratch scratch = scratch_begin(0,0);
  Token* token = lexer_peek_token(lexer);
  Intsp_Typedef_Object* typedef_datatype;
  array_get_next(&introspection->typedefs, Intsp_Typedef_Object, typedef_datatype);
  typedef_datatype->location.file = file_obj;
  typedef_datatype->location.line = token->l0;
  
  _intsp_eat_trivia(lexer);
  
  // Parse the data type

  Intsp_Parse_Data_Type_Result result = _intsp_parse_datatype(introspection, lexer);
  typedef_datatype->data_type = result.data_type;

  if (result.names.count > 1)
  {
    intsp_error(introspection, S("Unexpected multiple declaration in typedef"));
    return;
  }
  typedef_datatype->identifier = array_get(&result.names, 0);
  
  _intsp_eat_trivia(lexer);
  token = lexer_peek_token(lexer);
  
  // Check if this is a function pointer typedef
  if (token->kind == Token_Open_Parentheses)
  {
    // Function pointer typedef
    // Collect everything from '(' to ';' as the identifier
    String declaration_string = string_zero();
    
    for (;;)
    {
      token = lexer_peek_token(lexer);
      if (token->kind == Token_Semicolon)
      {
        break;
      }
      lexer_eat_token(lexer);
      declaration_string = string_join(scratch.arena, declaration_string, token->value);
      _intsp_eat_trivia(lexer);
    }
    
    typedef_datatype->identifier = string_copy(introspection->arena, declaration_string);
  }
  else
  {
    // Normal typedef: next identifier is the typedef name
    assert(token->kind == Token_Semicolon);
    lexer_eat_token(lexer);
  }
  
  scratch_end(&scratch);
}

function Intsp_Aggregate_Object*
_intsp_parse_aggregate(Lexer* lexer, Introspection* introspection, Intsp_File* file_obj, Intsp_Aggregate_Object* parent)
{
  if (introspection->fatal_error)
  {
    return NULL;
  }

  Token* token = lexer_peek_token(lexer);

  Intsp_Aggregate_Kind aggregate_kind = Intsp_Aggregate_Kind_Struct;
  if (string_equals(token->value, S("struct"), true))     aggregate_kind = Intsp_Aggregate_Kind_Struct;
  else if (string_equals(token->value, S("union"), true)) aggregate_kind = Intsp_Aggregate_Kind_Union;
  else
  {
    assert(0);
  }

  u32 aggregate_line = token->l0;

  lexer_eat_token(lexer);
  _intsp_eat_trivia(lexer);

  String struct_name = string_zero();

  token = lexer_peek_token(lexer);

  if (parent == NULL)
  {
    if (token->kind != Token_Identifier)
    {
      _intsp_report(introspection, Intsp_Log_Severity_Error, file_obj, token->l0, token->c0, S("Expected identifier after 'struct'/'union'."));
      return NULL;
    }

    struct_name = token->value;
    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
  }

  token = lexer_peek_token(lexer);

  if (token->kind == Token_Identifier && parent == NULL)
  {
    if (!string_equals(struct_name, token->value, true))
    {
      _intsp_report(introspection, Intsp_Log_Severity_Error, file_obj, token->l0, token->c0,
        Sf(introspection->arena, "Struct tag and typedef name must match: typedef struct "S_FMT" "S_FMT";", S_ARG(struct_name), S_ARG(token->value)));
      return NULL;
    }

    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);

    token = lexer_peek_token(lexer);
    if (token->kind != Token_Semicolon)
    {
      _intsp_report(introspection, Intsp_Log_Severity_Error, file_obj, token->l0, token->c0, S("Expected ';' after forward declaration."));
      return NULL;
    }

    lexer_eat_token(lexer);

    Intsp_Aggregate_Object* obj = NULL;
    array_get_next(&introspection->aggregates, Intsp_Aggregate_Object, obj);

    obj->kind = aggregate_kind;
    obj->identifier = string_copy(introspection->arena, struct_name);
    obj->members = array_make(Intsp_Aggregate_Member, 32);
    obj->hash = _intsp_aggregate_hash(obj);
    obj->forward_declaration.file = file_obj;
    obj->forward_declaration.line = aggregate_line;

    return obj;
  }

  if (token->kind != Token_Open_Brace)
  {
    intsp_error(introspection, S("Expected '{' after aggregate declaration."));
    return NULL;
  }

  Intsp_Aggregate_Object* struct_object = NULL;

  if (parent)
  {
    Intsp_Aggregate_Member* m;
    array_get_next(&parent->members, Intsp_Aggregate_Member, m);

    m->is_aggregate = true;
    m->aggregate_data.kind = aggregate_kind;
    m->aggregate_data.parent = parent;
    m->aggregate_data.identifier = string_copy(introspection->arena, struct_name);
    m->aggregate_data.members = array_make(Intsp_Aggregate_Member, 32);

    struct_object = &m->aggregate_data;
  }
  else
  {
    struct_object = _intsp_find_struct(introspection, string_hash(struct_name));
    if (!struct_object)
    {
      intsp_error(introspection, S("Aggregate must be forward-declared before definition."));
      return NULL;
    }
  }

  struct_object->kind = aggregate_kind;
  struct_object->identifier = string_copy(introspection->arena, struct_name);
  struct_object->members = array_make(Intsp_Aggregate_Member, 32);
  struct_object->definition.file = file_obj;
  struct_object->definition.line = aggregate_line;

  lexer_eat_token(lexer); // eat '{'
  _intsp_eat_trivia(lexer);

  for (;;)
  {
    _intsp_eat_trivia(lexer);
    token = lexer_peek_token(lexer);

    if (token->kind == Token_Close_Brace)
    {
      break;
    }

    // Nested aggregate
    if (string_equals(token->value, S("struct"), true) || string_equals(token->value, S("union"), true))
    {
      Intsp_Aggregate_Object* nested = _intsp_parse_aggregate(lexer, introspection, file_obj, struct_object);

      token = lexer_peek_token(lexer);
      if (token->kind == Token_Identifier)
      {
        nested->identifier = string_copy(introspection->arena, token->value);
        lexer_eat_token(lexer);
        _intsp_eat_trivia(lexer);
      }

      token = lexer_peek_token(lexer);
      assert(token->kind == Token_Semicolon);
      lexer_eat_token(lexer);
      continue;
    }

    Intsp_Parse_Data_Type_Result parsed = _intsp_parse_datatype(introspection, lexer);

    token = lexer_peek_token(lexer);

    if (token->kind == Token_Equal)
    {
      while (lexer_peek_token(lexer)->kind != Token_Semicolon)
      {
        lexer_eat_token(lexer);
      }
    }

    token = lexer_peek_token(lexer);
    assert(token->kind == Token_Semicolon);

    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
    token = lexer_peek_token(lexer);
    

    if (token->kind == Token_Comment_Block || token->kind == Token_Comment_Line)
    {
      lexer_eat_token(lexer);
      _intsp_eat_trivia(lexer);
    }

    if (parsed.names.count > 1)
    {
      for (u32 i = 0; i < parsed.names.count; i += 1)
      {
        Intsp_Aggregate_Member* m;
        array_get_next(&struct_object->members, Intsp_Aggregate_Member, m);
        m->is_aggregate = false;
        m->data_type = parsed.data_type;
        m->identifier = array_get(&parsed.names, 0);
        m->documentation = string_copy(introspection->arena, token->value);
      }
    }
    else
    {
      Intsp_Aggregate_Member* m;
      array_get_next(&struct_object->members, Intsp_Aggregate_Member, m);
      m->is_aggregate = false;
      m->data_type = parsed.data_type;
      m->identifier = array_get(&parsed.names, 0);
      m->documentation = string_copy(introspection->arena, token->value);
    }
  }

  lexer_eat_token(lexer); // eat '}'
  _intsp_eat_trivia(lexer);

  return struct_object;
}

function void
_intsp_parse_enum(Lexer* lexer, Introspection* introspection, Intsp_File* file_obj)
{
  Token* token = lexer_peek_token(lexer);
  assert(string_equals(token->value, S("enum"), true));

  u32 enum_keyword_line = token->l0;
  u32 enum_keyword_col = token->c0;

  Intsp_Enum_Object* enum_object;
  array_get_next(&introspection->enums, Intsp_Enum_Object, enum_object);

  enum_object->definition.file = file_obj;
  enum_object->definition.line = token->l0;

  lexer_eat_token(lexer);
  _intsp_eat_trivia(lexer);

  token = lexer_peek_token(lexer);

  // Look for Metaprogramming tags
  if(token->kind == Token_Identifier && string_equals(token->value, S("META_ENUM_LINK"), true))
  {
    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);
    assert(token->kind == Token_Open_Parentheses);

    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
    token = lexer_peek_token(lexer);
    assert(token->kind == Token_Identifier);

    enum_object->identifier = string_copy(introspection->arena, token->value);

    // Try to link enum to typedef
    Intsp_Typedef_Object* typedef_object = _intsp_find_typedef(introspection, enum_object->identifier);
    if (typedef_object != NULL)
    {
      enum_object->type_casted_at = typedef_object;
    }
    else
    {
      _intsp_report(introspection, Intsp_Log_Severity_Warning, file_obj, enum_object->definition.line, 0,
        Sf(introspection->arena, "Enum was declared without previously being typedefed.\nConsider adding: typedef u32 "S_FMT"; in: "S_FMT"::%u,1", S_ARG(enum_object->identifier), S_ARG(file_obj->path), enum_object->definition.line-1));
    }

    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
    token = lexer_peek_token(lexer);
    assert(token->kind == Token_Close_Parentheses);

    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
    token = lexer_peek_token(lexer);
  }

  Scratch scratch = scratch_begin(0,0);

  u32 max_enums = 512;
  Intsp_Enum_Member* temp_members = push_array(scratch.arena, Intsp_Enum_Member, max_enums);
  u32 members_count = _intsp_parse_enum_members(introspection->arena, lexer, temp_members, max_enums);

  token = lexer_peek_token(lexer);
  assert(token->kind == Token_Close_Brace);

  lexer_eat_token(lexer);
  _intsp_eat_trivia(lexer);
  token = lexer_peek_token(lexer);

  if (token->kind == Token_Identifier)
  {
    enum_object->identifier = string_copy(introspection->arena, token->value);
    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
    token = lexer_peek_token(lexer);
  }

  assert(token->kind == Token_Semicolon);
  lexer_eat_token(lexer);

  enum_object->members = array_make(Intsp_Enum_Member, 16);

  for(u32 i = 0; i < members_count; ++i)
  {
    array_push(&enum_object->members, Intsp_Enum_Member, temp_members[i]);
  }

  scratch_end(&scratch);
}

function void
_intsp_parse_function(Lexer* lexer, Introspection* introspection, Intsp_File* file_obj)
{
  Scratch scratch = scratch_begin(0,0);

  _intsp_eat_trivia(lexer);
  Token* token = lexer_peek_token(lexer);
  assert(token->kind == Token_Identifier && string_equals(token->value, S("function"), true));

  Token* previous = token;

  String full_return_type = string_zero();
  String function_name = string_zero();

  for (;;)
  {
    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);

    if (token->kind == Token_Open_Parentheses)
    {
      lexer_eat_token(lexer);
      _intsp_eat_trivia(lexer);
      break;
    }

    full_return_type = string_join(introspection->arena, full_return_type, token->value);
    previous = token;
  }

  function_name = string_copy(introspection->arena, previous->value);

  Intsp_Function_Object* function_object = _intsp_find_function(introspection, function_name);
  if (function_object == NULL)
  {
    array_get_next(&introspection->functions, Intsp_Function_Object, function_object);
  }

  function_object->return_type.name = string_copy(introspection->arena, full_return_type);
  for (u32 i = 0; i < function_object->return_type.name.count; i += 1)
  {
    if (function_object->return_type.name.cstring[i] == '*')
    {
      function_object->return_type.is_pointer = true;
      function_object->return_type.indirection_level += 1;
    }
  }

  token = lexer_peek_token(lexer); // Data type or close parenthises

  if (token->kind == Token_Close_Parentheses)
  {
    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
  }
  else
  {
    previous = token;
    String full_type = string_zero();

    for (;;)
    {
      lexer_eat_token(lexer);
      token = lexer_peek_token(lexer);

      if (token->kind == Token_Dot)
      {
        Intsp_Function_Argument* arg;
        array_get_next(&function_object->arguments, Intsp_Function_Argument, arg);
        arg->is_var_args = true;
        arg->identifier = S("...");
        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Dot);
        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Dot);
      }

      if (token->kind == Token_Comma)
      {
        Intsp_Function_Argument* arg;
        array_get_next(&function_object->arguments, Intsp_Function_Argument, arg);
        arg->is_var_args = false;
        arg->identifier  = string_copy(introspection->arena, previous->value);
        arg->data_type.name = string_copy(introspection->arena, full_type);
        for (u32 i = 0; i < function_object->return_type.name.count; i += 1)
        {
          if (function_object->return_type.name.cstring[i] == '*')
          {
            function_object->return_type.is_pointer = true;
            function_object->return_type.indirection_level += 1;
          }
        }
        full_type = string_zero();
      }

      if (token->kind == Token_Close_Parentheses)
      {
        lexer_eat_token(lexer);
        _intsp_eat_trivia(lexer);
        break;
      }

      full_type = string_join(introspection->arena, full_return_type, token->value);
      previous = token;
    }
  }

  token = lexer_peek_token(lexer);
  
  if (token->kind == Token_Semicolon)
  {
    // Function declaration
    function_object.declaration.file = file_obj;
  }
  else if (token->kind == Token_Open_Brace)
  {
  
  }
  else
  {
  
  }

  scratch_end(&scratch);
}

function void
_intsp_eat_trivia(Lexer* lexer)
{
  for (;;)
  {
    Token* token = lexer_peek_token(lexer);
    if (!token_is_trivia(token)) break;
    lexer_eat_token(lexer);
  }
}

function void
_intsp_report(Introspection* introspection, Intsp_Log_Severity severity, Intsp_File* file, u32 line, u32 col, String text)
{
  if (introspection->logs.count >= introspection->logs.capacity)
  {
    return;
  }

  Intsp_Log* ilog;
  array_get_next(&introspection->logs, Intsp_Log, ilog);
  ilog->severity = severity;

  String text_to_log = string_join(introspection->arena, text, S("\n"));
  switch (severity)
  {
    case Intsp_Log_Severity_None:
    case Intsp_Log_Severity_Log:
    {
      ilog->text = Sf(introspection->arena, "[Introspection] LOG: "S_FMT"::(%u,%u)\n", S_ARG(file->path), line, col);
    } break;
    case Intsp_Log_Severity_Warning:
    {
      ilog->text = Sf(introspection->arena, "[Introspection] WARNING: "S_FMT"::(%u,%u)\n", S_ARG(file->path), line, col);
    } break;
    case Intsp_Log_Severity_Error:
    {
      ilog->text = Sf(introspection->arena, "[Introspection] ERROR: "S_FMT"::(%u,%u)\n", S_ARG(file->path), line, col);
      introspection->fatal_error = true;
    } break;
  };
  ilog->text = string_join(introspection->arena, ilog->text, text_to_log);
}

function u64
_intsp_aggregate_hash(Intsp_Aggregate_Object* obj)
{
  Scratch scratch = scratch_begin(0, 0);
  String result  = obj->identifier;

  for (Intsp_Aggregate_Object* parent = obj->parent; parent != 0; parent = parent->parent)
  {
    result = string_join(scratch.arena, parent->identifier, result);
  }

  u64 hash = string_hash(result);

  scratch_end(&scratch);
  return hash;
}

#endif // INTROSPECTION_H
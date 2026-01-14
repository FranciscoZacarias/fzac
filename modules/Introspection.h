#ifndef INTROSPECTION_H
#define INTROSPECTION_H

#include "../Base.h"
#include "Lexer.h"

#define INTSP_MAX_FUNCTION_KEYWORDS 4
#define INTSP_MAX_FUNCTION_ARGUMENTS 16
#define INTSP_MAX_FUNCTIONS 1024

#define INTSP_MAX_ENUMS 256

#define INTSP_MAX_TYPEDEFS 256

#define INTSP_MAX_STRUCTS 512
#define INTSP_MAX_STRUCTS_MEMBERS 128

#define INTSP_MAX_META_COMMANDS 128

#define INTSP_MAX_LOGS 512

typedef struct Intsp_Source_Location Intsp_Source_Location;
struct Intsp_Source_Location
{
  String file;
  u32 line;
};

typedef struct Intsp_Data_Type Intsp_Data_Type;
struct Intsp_Data_Type
{
  b32 is_const; /*const */
  b32 is_pointer; /* at least one level of indirection */

  b32 is_unsigned; /* unsigned */
  b32 is_signed; /* signed */

  b32 is_short; /* short */
  b32 is_long; /* long */
  b32 is_long_long; /* long long */

  b32 is_array; /* [] */

  u32 indirection_level;
  String array_length; /* If data_type.is_array == true, we set here the size of the array, which can be a macro, so we just store a string. */
  String name;
};

typedef struct Intsp_Typedef_Object Intsp_Typedef_Object; /* For when the user typedefs something that isn't a struct or enum. E.g. "typedef unsigned char c8", unsigned char is data_type, c8 is identifier */
struct Intsp_Typedef_Object
{
  Intsp_Data_Type data_type;
  String identifier;
  Intsp_Source_Location location;
};

typedef struct Intsp_Function_Argument Intsp_Function_Argument;
struct Intsp_Function_Argument
{
  b32 is_var_args; /* If true, overrides data_type and identifier, since they lose meaning. */
  Intsp_Data_Type data_type;
  String identifier;
};

typedef struct Intsp_Function_Object Intsp_Function_Object;
struct Intsp_Function_Object
{
  String* keywords; /* Things like 'function', 'inline', 'CALLBACK', 'WINAPI' etc... */
  u32 keywords_count;
  u32 keywords_capacity;

  Intsp_Data_Type return_type;
  String identifier;

  Intsp_Function_Argument* arguments;
  u32 arguments_count;
  u32 arguments_capacity;

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

/*
@TODO(fz): In order to parse structs
  fields
  anonymous structs/unions
  named nested structs/unions
  bitfields
  flexible array members
  alignment attributes
  packing pragmas
  typedef indirection
  unions (same syntax, different semantics)
*/
typedef struct Intsp_Agreggate_Object Intsp_Agreggate_Object;
typedef struct Intsp_Aggregate_Member Intsp_Aggregate_Member;

struct Intsp_Agreggate_Object // @TODO(fz): fix typo -> Aggregate
{
  u64 hash;

  Intsp_Aggregate_Kind kind;

  b32 is_nested; /* If this is nested inside another Intsp_Agreggate_Object, we don't enforce it to be named. */
  Intsp_Agreggate_Object* parent; /* if is_nested=false then this is NULL. Pointer to struct where this object is nested at. */

  String identifier;

  Intsp_Aggregate_Member* members; /* @NOTE(fz): Order matters, since struct order influences padding. We want to preserve this. */
  u32 members_count;
  u32 members_capacity;

  String documentation;

  Intsp_Source_Location forward_declaration;
  Intsp_Source_Location definition;
};

struct Intsp_Aggregate_Member
{
  String documentation;

  b32 is_aggregate; /* If true, only aggregate_data is relavant, if it's false, only data_type is relevant*/
  union
  {
    Intsp_Data_Type data_type; /* if is_aggregate=false */
    Intsp_Agreggate_Object aggregate_data; /* if is_aggregate=true */
  };

  String identifier;
};

typedef struct Intsp_Enum_Member Intsp_Enum_Member;
struct Intsp_Enum_Member
{
  String identifier;
  String documentation;
  b32 is_explicit_bit_flag; /* In case the value is something like Trivia_Line_Break = (1 << 5) ... */
  b32 is_one_or_more_enum_flags; /* In case the value is something like Trivia_All = Trivia_Whitespace|Trivia_Carriage_Return|Trivia_Tab|Trivia_Form_Feed|Trivia_Vertical_Tab|Trivia_Line_Break ... */
  s64 value;
};

typedef struct Intsp_Enum_Object Intsp_Enum_Object;
struct Intsp_Enum_Object
{
  String identifier;
  String type;

  Intsp_Enum_Member* members;
  u32 members_count;
  u32 members_capacity;

  Intsp_Typedef_Object* type_casted_at;
  Intsp_Source_Location definition;
};

typedef struct Intsp_Global_Variable Intsp_Global_Variable;
struct Intsp_Global_Variable
{
  Intsp_Data_Type data_type;
  String identifier;
  String initialization; // If it's initialized, this string will contain the value;
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
struct Intsp_Log
{
  Intsp_Log_Severity severity;
  String text;
};

typedef struct Intsp_Report Intsp_Report;
struct Intsp_Report
{
  Intsp_Log* logs;
  u32 logs_capacity;
  u32 logs_count;
};

typedef struct Introspection Introspection; /* Contains an introspection of a C program in my codebase. Note that reports are not exhaustive, they are only the ones caught on the lexing level. */
struct Introspection
{
  Arena* arena;
  Intsp_Report report;
  b32 fatal_error;

  Arena* functions_arena;
  Intsp_Function_Object* functions;
  u32 functions_capacity;
  u32 functions_count;

  Arena* aggregates_arena;
  Intsp_Agreggate_Object* aggregates;
  u32 aggregates_capacity;
  u32 aggregates_count;

  Arena* enums_arena;
  Intsp_Enum_Object* enums;
  u32 enums_capacity;
  u32 enums_count;

  Arena* typedefs_arena;
  Intsp_Typedef_Object* typedefs;
  u32 typedefs_capacity;
  u32 typedefs_count;
};

function Introspection intsp_run(String source_directory, b32 introspect_base_library); /* Parses a project directory. */
function b32           intsp_report_introspection(Introspection* introspection, b32 verbose, String* out); /* Produces a report of the introspection. Reports true if no fatal errors were found. If there are fatal errors, the introspection will be incomplete but the report still has an ERROR log. */
function void          intsp_report_functions(Introspection* introspection, b32 report_documentation, String* out);
function void          intsp_report_enums(Introspection* introspection, String* out);
function String        intsp_data_type_to_string(Arena* arena, Intsp_Data_Type type); /* Convers a Intsp_Data_Type to a string */
function String        intsp_function_signature_to_string(Arena* arena, Intsp_Function_Object* f); /* Convers a Intsp_Function_Object to a string */
function u64           intsp_aggregate_hash(Intsp_Agreggate_Object* obj);

function u32  _intsp_parse_enum_members(Arena* arena, Lexer* lexer, Intsp_Enum_Member* temp_members, u32 max_members); /* Parses the members of an enum. Anything after { and before }. Returns how many members were added */
function void _intsp_parse_enum(Lexer* lexer, Introspection* introspection, String file_path); /* Parses enums */
function void _intsp_parse_typedef(Lexer* lexer, Introspection* introspection, String file_path); /* Parses typedefs that are not structs nor enums */
function void _intsp_parse_function(Lexer* lexer, Introspection* introspection, String file_path);
function Intsp_Agreggate_Object* _intsp_parse_aggregate(Lexer* lexer, Introspection* introspection, String file_path, Intsp_Agreggate_Object* parent);
function void _intsp_eat_trivia(Lexer* lexer);
function void _intsp_report(Introspection* introspection, Intsp_Log_Severity severity, String file, u32 line, u32 col, String text); /* Adds a report to the introspection struct */
function Intsp_Data_Type         _intsp_parse_data_type(Arena* arena, Token* tokens, u32 start, u32 end); /* Parses a data type from a view into a token array */
function Intsp_Function_Object*  _intsp_find_function(Introspection* introspection, String function_name);
function Intsp_Enum_Object*      _intsp_find_enum(Introspection* introspection, String enum_name);
function Intsp_Agreggate_Object* _intsp_find_struct(Introspection* introspection, u64 hash);
function Intsp_Typedef_Object*   _intsp_find_typedef(Introspection* introspection, String typedef_name);

function Introspection
intsp_run(String source_directory, b32 introspect_base_library)
{
  Scratch scratch = scratch_begin(0,0);

  Introspection result;
  memory_zero_struct(&result);

  result.arena = arena_alloc();
  result.report.logs_capacity = INTSP_MAX_LOGS;
  result.report.logs          = push_array(result.arena, Intsp_Log, result.report.logs_capacity);

  result.functions_arena    = arena_alloc();
  result.functions_capacity = INTSP_MAX_FUNCTIONS;
  result.functions          = push_array(result.functions_arena, Intsp_Function_Object, result.functions_capacity);

  result.enums_arena    = arena_alloc();
  result.enums_capacity = INTSP_MAX_ENUMS;
  result.enums          = push_array(result.enums_arena, Intsp_Enum_Object, result.enums_capacity);

  result.typedefs_arena    = arena_alloc();
  result.typedefs_capacity = INTSP_MAX_TYPEDEFS;
  result.typedefs          = push_array(result.typedefs_arena, Intsp_Typedef_Object, result.enums_capacity);

  result.aggregates_arena    = arena_alloc();
  result.aggregates_capacity = INTSP_MAX_STRUCTS;
  result.aggregates          = push_array(result.aggregates_arena, Intsp_Agreggate_Object, result.aggregates_capacity);

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
    if (!(string_equals(ext, S("c"), true) ||
          string_equals(ext, S("h"), true) ||
          string_equals(ext, S("inl"), true)))
    {
      continue;
    }

    Lexer lexer;
    lexer_init_with_single_file_path(&lexer, file_being_lexed, Trivia_Line_Break|Trivia_Whitespace|Trivia_Tab, Emit_Character_Literals|Emit_String_Literals|Emit_Line_Comments|Emit_Block_Comments);

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
        // @TODO(fz): This is not implemented, it's just to make sure it works.
        while (token->kind != Token_Line_Break && token->kind != Token_End_Of_File)
        {
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
          _intsp_parse_function(&lexer, &result, file_being_lexed);
          at_line_start = true;
          continue;
        }
        else if (string_equals(token->value, S("enum"), true))
        {
          _intsp_parse_enum(&lexer, &result, file_being_lexed);
          at_line_start = true;
          continue;
        }
        else if (string_equals(token->value, S("struct"), true) || string_equals(token->value, S("union"), true))
        {
          _intsp_parse_aggregate(&lexer, &result, file_being_lexed, NULL);
          at_line_start = false;
          continue;
        }
        else if (string_equals(token->value, S("global"), true))
        {
          lexer_eat_token(&lexer);
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
            _intsp_parse_enum(&lexer, &result, file_being_lexed);
            at_line_start = true;
            continue;
          }
          else if (string_equals(token->value, S("struct"), true) || string_equals(token->value, S("union"), true))
          {
            _intsp_parse_aggregate(&lexer, &result, file_being_lexed, NULL);
          }
          else
          {
            _intsp_parse_typedef(&lexer, &result, file_being_lexed);
          }

          at_line_start = true;
          continue;
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

  Intsp_Report* report = &introspection->report;
  u32 warnings_and_errors = 0;

  string_buffer_push(&buffer,
    "[Introspection report]:\n");

  for (u32 i = 0; i < report->logs_count; i += 1)
  {
    Intsp_Log ilog = report->logs[i];

    if ((ilog.severity == Intsp_Log_Severity_Log || ilog.severity == Intsp_Log_Severity_None) && !verbose)
    {
      continue;
    }

    warnings_and_errors += 1;
    string_buffer_push(&buffer, S_FMT, S_ARG(ilog.text));
  }

  string_buffer_push(&buffer, "Total Reports: %u\nWarnings and Errors: %u\n\n", report->logs_count, warnings_and_errors);

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
  u32 num_no_documentation = 0;

  for (u32 i = 0; i < introspection->functions_count; i += 1)
  {
    Intsp_Function_Object* func_object = &introspection->functions[i];

    if (func_object->implementation.file.count == 0 || func_object->declaration.file.count == 0)
    {
      string_buffer_push(&buffer,
        "Function "S_FMT" has a mismatch:\n"
        " Decl: "S_FMT":%u\n"
        " Impl: "S_FMT":%u\n\n",
        S_ARG(func_object->identifier),
        S_ARG(func_object->declaration.file), func_object->declaration.line,
        S_ARG(func_object->implementation.file), func_object->implementation.line);
      num_mismatches += 1;
    }

    if (func_object->documentation.count == 0)
    {
      if (report_documentation)
      {
        string_buffer_push(&buffer, "Function "S_FMT" doesn't have documentation.\n\n", S_ARG(func_object->identifier));
      }
      num_no_documentation += 1;
    }
  }

  string_buffer_push(&buffer, "[Function Report]:\n");

  if (num_mismatches == 0) string_buffer_push(&buffer, " No Function mismatches.\n");
  else                     string_buffer_push(&buffer, " Mismatches: %u/%u\n", num_mismatches, introspection->functions_count);

  if (num_no_documentation == 0) string_buffer_push(&buffer, " No function without documentation.\n");
  else                           string_buffer_push(&buffer, " No documentation: %u/%u\n", num_no_documentation, introspection->functions_count);

  *out = string_buffer_to_string(introspection->arena, &buffer);
  string_buffer_free(&buffer);
}

function void
intsp_report_enums(Introspection* introspection, String* out)
{
  String_Buffer buffer = {0};
  string_buffer_init(&buffer, &MallocAllocator, 512);

  u32 num_missing_identifier = 0;

  for (u32 i = 0; i < introspection->enums_count; i += 1)
  {
    Intsp_Enum_Object* e = &introspection->enums[i];
    if (e->identifier.count == 0)
    {
      num_missing_identifier += 1;
      string_buffer_push(&buffer, "Enum missing identifier. First enum value: "S_FMT"\n", S_ARG(e->members[0].identifier));
    }
  }

  string_buffer_push(&buffer, "\n[Enum Report]:\n");

  if (num_missing_identifier == 0) string_buffer_push(&buffer, " Enums are ok.\n\n");
  else                             string_buffer_push(&buffer, " Enums missing identifiers: %u/%u\n\n", num_missing_identifier, introspection->enums_count);

  *out = string_buffer_to_string(introspection->arena, &buffer);
  string_buffer_free(&buffer);
}

function String
intsp_data_type_to_string(Arena* arena, Intsp_Data_Type type)
{
  String_Buffer buffer;
  string_buffer_init(&buffer, &MallocAllocator, 64);

  if (type.is_const) string_buffer_push(&buffer, "const ");
  string_buffer_push(&buffer, S_FMT, S_ARG(type.name));
  if (type.is_pointer) for (u32 i = 0; i < type.indirection_level; i += 1) string_buffer_push(&buffer, "*");
  if (type.is_array) string_buffer_push(&buffer, "[]");

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

  for (u32 i = 0; i < f->keywords_count; i += 1) string_buffer_push(&buffer, S_FMT " ", S_ARG(f->keywords[i]));
  String return_type = intsp_data_type_to_string(scratch.arena, f->return_type);
  string_buffer_push(&buffer, S_FMT " ", S_ARG(return_type));
  string_buffer_push(&buffer, S_FMT "(", S_ARG(f->identifier));

  for (u32 i = 0; i < f->arguments_count; i += 1)
  {
    Intsp_Function_Argument* arg = &f->arguments[i];
    if (arg->is_var_args)
    {
      string_buffer_push(&buffer, "...");
    }
    else
    {
      String arg_type = intsp_data_type_to_string(scratch.arena, arg->data_type);
      string_buffer_push(&buffer, S_FMT " " S_FMT, S_ARG(arg_type), S_ARG(arg->identifier) );
    }
    if (i + 1 < f->arguments_count) string_buffer_push(&buffer, ", ");
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
  for (u32 i = 0; i < introspection->enums_count; i += 1)
  {
    Intsp_Enum_Object* enum_object = &(introspection->enums[i]);
    if (string_equals(enum_name, enum_object->identifier, true))
    {
      result = enum_object;
      break;
    }
  }
  return result;
}

function Intsp_Function_Object*
_intsp_find_function(Introspection* introspection, String function_name)
{
  Intsp_Function_Object* result = NULL;
  for (u32 i = 0; i < introspection->functions_count; i += 1)
  {
    Intsp_Function_Object* function_object = &(introspection->functions[i]);
    if (string_equals(function_name, function_object->identifier, true))
    {
      result = function_object;
      break;
    }
  }
  return result;
}

function Intsp_Agreggate_Object*
_intsp_find_struct(Introspection* introspection, u64 hash)
{
  Intsp_Agreggate_Object* result = NULL;
  for (u32 i = 0; i < introspection->aggregates_count; i += 1)
  {
    Intsp_Agreggate_Object* struct_object = &(introspection->aggregates[i]);
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
  for (u32 i = 0; i < introspection->typedefs_count; i += 1)
  {
    Intsp_Typedef_Object* typedef_object = &(introspection->typedefs[i]);
    if (string_equals(typedef_name, typedef_object->identifier, true))
    {
      result = typedef_object;
      break;
    }
  }
  return result;
}

function Intsp_Data_Type
_intsp_parse_data_type(Arena* arena, Token* tokens, u32 start, u32 end)
{
  Intsp_Data_Type type;
  memory_zero_struct(&type);

  u32 i = start;
  for (; i < end; i += 1)
  {
    Token* t = &tokens[i];
    if (t->kind != Token_Identifier) break;

    if (string_equals(t->value, S("const"), true))
    {
      type.is_const = true;
    }
    else if (string_equals(t->value, S("unsigned"), true))
    {
      type.is_unsigned = true;
    }
    else if (string_equals(t->value, S("signed"), true))
    {
      type.is_signed = true;
    }
    else if (string_equals(t->value, S("short"), true))
    {
      type.is_short = true;
    }
    else if (string_equals(t->value, S("long"), true))
    {
      if (type.is_long)
      {
        type.is_long_long = true;
        type.is_long = false;
      }
      else
      {
        type.is_long = true;
      }
    }
    else
    {
      // this is the base type name
      type.name = string_copy(arena, t->value);
      i += 1;
      break;
    }
  }

  // @NOTE(fz): This assert is commented here as documentation. Before, I was asserting this so that something
  // like unsigned char would be parsed as: is_unsigned = true, identifier = "char". However we can have
  // types just like unsigned short, or long long, which are just flags.
  // assert(type.name.cstring != 0);

  while (i < end && tokens[i].kind == Token_Asterisk)
  {
    type.indirection_level += 1;
    type.is_pointer = true;
    i += 1;

    if (i < end &&
      tokens[i].kind == Token_Identifier &&
      string_equals(tokens[i].value, S("const"), true))
    {
      type.is_const = true;
      i += 1;
    }
  }

  if (i < end && tokens[i].kind == Token_Open_Bracket)
  {
    type.is_array = true;
    i += 1;

    if (tokens[i].kind == Token_Number)
    {
      type.array_length = string_copy(arena, tokens[i].value);
      i += 1;
    }

    assert(tokens[i].kind == Token_Close_Bracket);
  }

  return type;
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
_intsp_parse_typedef(Lexer* lexer, Introspection* introspection, String file_path)
{
  Scratch scratch = scratch_begin(0,0);
  Token* token = lexer_peek_token(lexer);

  Intsp_Typedef_Object* typedef_datatype = &(introspection->typedefs[introspection->typedefs_count++]);
  memory_zero_struct(typedef_datatype);
  typedef_datatype->location.file = string_copy(introspection->typedefs_arena, file_path);
  typedef_datatype->location.line = token->l0;

  u32 tokens_capacity = 32;
  u32 tokens_count = 0;
  Token* tokens = push_array(scratch.arena, Token, tokens_capacity);

  // Collect tokens until semicolon
  for (;;)
  {
    token = lexer_peek_token(lexer);
    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);

    if (token->kind == Token_Semicolon) break;
    if (tokens_count >= tokens_capacity)
    {
      assert(0);
    }

    tokens[tokens_count++] = *token;
  }

  assert(tokens_count > 0);

  // Determine if this is a function pointer
  b32 is_function_pointer = false;
  for (u32 i = 0; i < tokens_count; i++)
  {
    if (tokens[i].kind == Token_Open_Parentheses)
    {
      // check if next token is '*' (i.e., (*Name))
      if (i + 1 < tokens_count && tokens[i + 1].kind == Token_Asterisk)
      {
        is_function_pointer = true;
        break;
      }
    }
  }

  if (!is_function_pointer)
  {
    // normal typedef: last identifier is the typedef name
    u32 name_index = tokens_count - 1;
    typedef_datatype->identifier = string_copy(introspection->typedefs_arena, tokens[name_index].value);
    typedef_datatype->data_type  = _intsp_parse_data_type(introspection->typedefs_arena, tokens, 0, name_index);
  }
  else
  {
    // function pointer typedef: first identifier tokens are the base type
    u32 base_type_end = 0;
    for (u32 i = 0; i < tokens_count; i++)
    {
      if (tokens[i].kind == Token_Identifier)
      {
        base_type_end = i + 1;
      }
      else
      {
        break;
      }
    }

    typedef_datatype->data_type = _intsp_parse_data_type(introspection->typedefs_arena, tokens, 0, base_type_end);

    String declaration_string = string_zero();
    for (u32 i = base_type_end; i < tokens_count; i++)
    {
      declaration_string = string_join(scratch.arena, declaration_string, tokens[i].value);
    }

    typedef_datatype->identifier = string_copy(introspection->typedefs_arena, declaration_string);
  }

  scratch_end(&scratch);
}

function Intsp_Agreggate_Object*
_intsp_parse_aggregate(Lexer* lexer, Introspection* introspection, String file_path, Intsp_Agreggate_Object* parent)
{
  Intsp_Agreggate_Object* struct_object = NULL;

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

  token = lexer_peek_token(lexer);
  if (token->kind != Token_Identifier && parent == NULL)
  {
    // @TODO(fz): Change this report to Intsp_Log_Severity_Error, right now it's warning just for debug purposes.
    _intsp_report(introspection, Intsp_Log_Severity_Warning, file_path, token->l0, token->c0, S("Expected struct/union identifier after 'struct'/'union'."));
    return NULL;
  }

  String struct_name = S("");

  if (parent == NULL)
  {
    struct_name = token->value;
    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
  }
  else
  {
    assert(token->kind == Token_Open_Brace);
  }

  token = lexer_peek_token(lexer);

  if (token->kind == Token_Comment_Block || token->kind == Token_Comment_Line)
  {
    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
    token = lexer_peek_token(lexer);
  }
  
  // Forward declaration
  if (token->kind == Token_Identifier && parent == NULL)
  {
    Token* typedef_name = token;

    if (!string_equals(struct_name, typedef_name->value, true))
    {
      _intsp_report(introspection, Intsp_Log_Severity_Error, file_path, token->l0, token->c0, Sf(introspection->arena, "When forward declaring a struct, the struct tag and the typedef name must be the same.\nPlease fix this: typedef struct "S_FMT" "S_FMT";", S_ARG(struct_name), S_ARG(typedef_name->value)));
      return NULL;
    }

    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
    token = lexer_peek_token(lexer);
    if (token->kind != Token_Semicolon)
    {
      _intsp_report(introspection, Intsp_Log_Severity_Error, file_path, token->l0, token->c0, S("Expected ';' after struct forward declaration."));
      return NULL;
    }

    lexer_eat_token(lexer);

    struct_object = _intsp_find_struct(introspection, string_hash(struct_name));
    assert(struct_object == NULL); // In a forward declaration, we should never have collisions

    struct_object = &introspection->aggregates[introspection->aggregates_count++];
    memory_zero_struct(struct_object);
    struct_object->kind = aggregate_kind;
    struct_object->identifier = string_copy(introspection->aggregates_arena, struct_name);
    struct_object->members_capacity = INTSP_MAX_STRUCTS_MEMBERS;
    struct_object->members = push_array(introspection->aggregates_arena, Intsp_Aggregate_Member, struct_object->members_capacity);

    assert(parent == NULL); // forward declarations should never have parents, since they're global scope.
    assert(string_hash(struct_object->identifier) == intsp_aggregate_hash(struct_object)); // Sanity check
    struct_object->hash = intsp_aggregate_hash(struct_object);

    struct_object->forward_declaration.file = file_path;
    struct_object->forward_declaration.line = token->l0;

    return struct_object;
  }
  // Definition
  else if (token->kind == Token_Open_Brace)
  {
    if (parent != NULL)
    {
      Intsp_Aggregate_Member* member = &parent->members[parent->members_count++];
      member->is_aggregate = true; // Because parent is not null

      member->aggregate_data.kind = aggregate_kind;
      member->aggregate_data.parent = parent;
      member->aggregate_data.identifier = string_copy(introspection->aggregates_arena, struct_name);
      member->aggregate_data.members_capacity = INTSP_MAX_STRUCTS_MEMBERS;
      member->aggregate_data.members = push_array(introspection->aggregates_arena, Intsp_Aggregate_Member, member->aggregate_data.members_capacity);
      struct_object = &member->aggregate_data;
    }
    else
    {
      assert(parent == NULL);
      u64 hash = string_hash(struct_name); // Because it's not nested, I.e. has no parent, we just just hash the name and get the correct hash
      struct_object = _intsp_find_struct(introspection, hash);
      if (struct_object == NULL)
      {
        _intsp_report(introspection, Intsp_Log_Severity_Error, file_path, token->l0, token->c0, Sf(introspection->arena, "Struct '"S_FMT"' must be forward declared before its definition.", S_ARG(struct_name)));
        return NULL;
      }
    }

    struct_object->kind = aggregate_kind;
    struct_object->identifier = string_copy(introspection->aggregates_arena, struct_name); // Might not be set if it's nested.

    struct_object->members_capacity = INTSP_MAX_STRUCTS_MEMBERS;
    struct_object->members = push_array(introspection->aggregates_arena, Intsp_Aggregate_Member, struct_object->members_capacity);

    struct_object->definition.file = file_path;
    struct_object->definition.line = aggregate_line;

    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);

    for (;;)
    {
      b32 semicolon_found = false;
      _intsp_eat_trivia(lexer);
      token = lexer_peek_token(lexer);

      if (token->kind == Token_Close_Brace)
      {
        break;      
      }

      if (string_equals(token->value, S("union"), true) || string_equals(token->value, S("struct"), true))
      {
        //Intsp_Aggregate_Member* m = &struct_object->members[struct_object->members_count++];
        //m->is_aggregate = true;

        Intsp_Agreggate_Object* o = _intsp_parse_aggregate(lexer, introspection, file_path, struct_object);
        //m->aggregate_data = *o;

        // @TODO(fz): Documentation for this.
        //m->documentation = string_copy(introspection->structs_arena, documentation);
        // @TODO(fz): Parse identifier
        //m->identifier    = string_copy(introspection->structs_arena, member_identifier);

        token = lexer_peek_token(lexer);
        if (token->kind == Token_Semicolon)
        {
          lexer_eat_token(lexer);
          _intsp_eat_trivia(lexer);
        }
      }
      else
      {
        Scratch scratch = scratch_begin(0,0);

        u32 tokens_capacity = 32;
        u32 tokens_count = 0;
        Token* tokens = push_array(scratch.arena, Token, tokens_capacity);
        String member_identifier = S("");

        for (;;)
        {
          _intsp_eat_trivia(lexer);
          token = lexer_peek_token(lexer);
          
          // Member variable
          if (token->kind == Token_Semicolon)
          {
            member_identifier = string_copy(introspection->aggregates_arena, tokens[tokens_count-1].value);
            break;
          }
          // Function pointer
          else if (token->kind == Token_Open_Parentheses)
          {
            member_identifier = S("(");
            
            for (;;)
            {
              token = lexer_peek_token(lexer);
              if (token->kind == Token_Semicolon)
              {
                semicolon_found = true;
                break;
              }
              lexer_eat_token(lexer);
              member_identifier = string_join(scratch.arena, member_identifier, token->value);
            }
          }
          // Continue parsing
          else
          {
            tokens[tokens_count++] = *token;
            lexer_eat_token(lexer);
          }

          if (semicolon_found)
          {
            break;
          }
        }

        _intsp_eat_trivia(lexer);

        token = lexer_peek_token(lexer);

        if (parent == NULL)
        {
          assert(token->kind == Token_Semicolon);
        }
        else
        {
          if (token->kind == Token_Identifier)
          {
            member_identifier = string_copy(introspection->aggregates_arena, token->value);
            lexer_eat_token(lexer);
            _intsp_eat_trivia(lexer);

            token = lexer_peek_token(lexer);
            assert(token->kind == Token_Semicolon);
            lexer_eat_token(lexer);
            _intsp_eat_trivia(lexer);
          }
          else
          {
            // @NOTE(fz): It's a nameless aggregate.
          }
        }

        assert(lexer_current_token(lexer)->kind == Token_Semicolon);        
        lexer_eat_token(lexer);
        _intsp_eat_trivia(lexer);

        String documentation = S("");
        token = lexer_peek_token(lexer);
        if (token->kind == Token_Comment_Line || token->kind == Token_Comment_Block)
        {
          documentation = string_slice(scratch.arena, token->value, 2, token->value.count - 2);
          documentation = string_trim(scratch.arena, documentation);
          lexer_eat_token(lexer);
          _intsp_eat_trivia(lexer);
        }

        if (struct_object->members_count >= struct_object->members_capacity)
        {
          // @TODO(fz): Propper error logging
          assert(0);
        }

        Intsp_Aggregate_Member* m = &struct_object->members[struct_object->members_count++];
        m->is_aggregate = false;
        m->documentation = string_copy(introspection->aggregates_arena, documentation);
        m->data_type = _intsp_parse_data_type(introspection->aggregates_arena, tokens, 0, tokens_count);
        m->identifier = string_copy(introspection->aggregates_arena, member_identifier);

        scratch_end(&scratch);
      }
    }

    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);

    return struct_object;
  }

  // @TODO(fz): This should'nt be reachable. Anyway, we have to report an error here.
  assert(0);
  return NULL;
}

function void
_intsp_parse_enum(Lexer* lexer, Introspection* introspection, String file_path)
{
  // These are anonymous enums. They don't have an identifier because they are likely type-casted elsewhere.
  Token* token = lexer_peek_token(lexer);
  assert(string_equals(token->value, S("enum"), true));

  u32 enum_keyword_line = token->l0;
  u32 enum_keyword_col = token->c0;

  Intsp_Enum_Object* enum_object = &(introspection->enums[introspection->enums_count++]);
  memory_zero_struct(enum_object);

  enum_object->definition.file = string_copy(introspection->enums_arena, file_path);
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

    enum_object->identifier = string_copy(introspection->enums_arena, token->value);

    // Try to link enum to typedef
    Intsp_Typedef_Object* typedef_object = _intsp_find_typedef(introspection, enum_object->identifier);
    if (typedef_object != NULL)
    {
      enum_object->type_casted_at = typedef_object;
    }
    else
    {
      _intsp_report(introspection, Intsp_Log_Severity_Warning, file_path, enum_object->definition.line, 0,
        Sf(introspection->arena,
          "Enum was declared without previously being typedefed.\n"
          "Consider adding: typedef u32 "S_FMT"; in: "S_FMT"::%u,1", S_ARG(enum_object->identifier), S_ARG(file_path), enum_object->definition.line-1));
    }

    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
    token = lexer_peek_token(lexer);
    assert(token->kind == Token_Close_Parentheses);

    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
    token = lexer_peek_token(lexer);
  }

  Intsp_Enum_Member temp_members[INTSP_MAX_ENUMS];
  u32 members_count = _intsp_parse_enum_members(introspection->enums_arena, lexer, temp_members, INTSP_MAX_ENUMS);

  token = lexer_peek_token(lexer);
  assert(token->kind == Token_Close_Brace);

  lexer_eat_token(lexer);
  _intsp_eat_trivia(lexer);
  token = lexer_peek_token(lexer);

  if (token->kind == Token_Identifier)
  {
    enum_object->identifier = string_copy(introspection->enums_arena, token->value);
    lexer_eat_token(lexer);
    _intsp_eat_trivia(lexer);
    token = lexer_peek_token(lexer);
  }

  assert(token->kind == Token_Semicolon);
  lexer_eat_token(lexer);

  enum_object->members_count = members_count;
  enum_object->members_capacity = members_count;
  enum_object->members = push_array(introspection->enums_arena, Intsp_Enum_Member, members_count);

  for(u32 i = 0; i < members_count; ++i)
  {
    enum_object->members[i] = temp_members[i];
  }
}

function void
_intsp_parse_function(Lexer* lexer, Introspection* introspection, String file_path)
{
  Scratch scratch = scratch_begin(0,0);

  u32 tokens_max   = 32;
  u32 tokens_count = 0;
  Token* tokens    = push_array(scratch.arena, Token, tokens_max);

  s64 line = -1;
  s64 column = -1;

  b32 is_declaration = false;

  for (;;)
  {
    Token* token = lexer_peek_token(lexer);
    if (line == -1)
    {
      line = token->l0;
      column = token->c0;
    }

    if (token->kind == Token_End_Of_File)
    {
      // @TODO(fz): Something is wrong.
      break;
    }

    if (token_is_trivia(token))
    {
      lexer_eat_token(lexer);
      continue;
    }

    if (token->kind == Token_Line_Break)
    {
      lexer_eat_token(lexer);
      continue;
    }
    if (token->kind == Token_Semicolon)
    {
      is_declaration = true;
      lexer_eat_token(lexer);
      break;
    }
    if (token->kind == Token_Open_Brace)
    {
      is_declaration = false;
      lexer_eat_token(lexer);
      break;
    }

    if (tokens_count < tokens_max)
    {
      memory_copy(&tokens[tokens_count], token, sizeof(Token));
      tokens_count += 1;
    }
    else
    {
      // @TODO(fz): Handle errors
    }

    lexer_eat_token(lexer);
  }

  Intsp_Data_Type return_type;
  memory_zero_struct(&return_type);

  String function_name = string_zero();
  Intsp_Function_Object* function_object = NULL;

  // Find function name;
  u32 name_index = U32_MAX;
  for (u32 i = 0; i < tokens_count; i++)
  {
    if (tokens[i].kind != Token_Identifier) continue;

    u32 j = i + 1;
    while (j < tokens_count && token_is_trivia(&tokens[j])) j++;

    if (j < tokens_count && tokens[j].kind == Token_Open_Parentheses)
    {
      function_name = string_copy(introspection->functions_arena, tokens[i].value);
      name_index = i;
      break;
    }
  }

  assert(name_index != U32_MAX);

  function_object = _intsp_find_function(introspection, function_name);

  if (function_object == NULL)
  {
    function_object = &(introspection->functions[introspection->functions_count++]);
    memory_zero_struct(function_object);
    function_object->identifier = string_copy(introspection->functions_arena, function_name);

    function_object->keywords_count = 0;
    function_object->keywords_capacity = INTSP_MAX_FUNCTION_KEYWORDS;
    function_object->keywords = push_array(introspection->functions_arena, String, function_object->keywords_capacity);

    function_object->arguments_count = 0;
    function_object->arguments_capacity = INTSP_MAX_FUNCTION_ARGUMENTS;
    function_object->arguments = push_array(introspection->functions_arena, Intsp_Function_Argument, function_object->arguments_capacity);

    // Parse keywords + return type
    s32 return_type_start = (s32)name_index - 1;

    while (return_type_start >= 0 && tokens[return_type_start].kind == Token_Asterisk)
    {
      return_type_start -= 1;
    }

    if (return_type_start > 0 &&
      tokens[return_type_start - 1].kind == Token_Identifier &&
      string_equals(tokens[return_type_start - 1].value, S("const"), true))
    {
      return_type_start -= 1;
    }

    // keywords
    for (u32 k = 0; k < (u32)return_type_start; k += 1)
    {
      if (tokens[k].kind == Token_Identifier)
      {
        assert(function_object->keywords_count < function_object->keywords_capacity);
        function_object->keywords[function_object->keywords_count++] =
          string_copy(introspection->functions_arena, tokens[k].value);
      }
    }

    function_object->return_type = _intsp_parse_data_type(introspection->functions_arena, tokens, (u32)return_type_start, name_index);

    assert(tokens[name_index + 1].kind == Token_Open_Parentheses);
    assert(tokens[tokens_count - 1].kind == Token_Close_Parentheses);

    for (u32 i = name_index + 2; i < tokens_count - 1; )
    {
      Intsp_Function_Argument* argument = &function_object->arguments[function_object->arguments_count++];
      memory_zero_struct(argument);

      // Varargs
      if (tokens[i].kind == Token_Dot)
      {
        assert(tokens[i + 1].kind == Token_Dot);
        assert(tokens[i + 2].kind == Token_Dot);
        argument->is_var_args = true;
        break;
      }

      // Look for argument name
      u32 type_start = i;
      u32 name_index_arg = U32_MAX;
      for (u32 j = i; j < tokens_count - 1; j += 1)
      {
        if (tokens[j].kind != Token_Identifier)
        {
          continue;
        }

        u32 one_befor_argument_name = j + 1;
        while (one_befor_argument_name < tokens_count && token_is_trivia(&tokens[one_befor_argument_name]))
        {
          one_befor_argument_name += 1;
        }

        if (tokens[one_befor_argument_name].kind == Token_Comma || tokens[one_befor_argument_name].kind == Token_Close_Parentheses || tokens[one_befor_argument_name].kind == Token_Open_Bracket)
        {
          name_index_arg = j;
          break;
        }
      }

      assert(name_index_arg != U32_MAX);
      argument->data_type = _intsp_parse_data_type(introspection->functions_arena, tokens, type_start, name_index_arg);
      argument->identifier = string_copy(introspection->functions_arena, tokens[name_index_arg].value);

      // Advance index past identifier
      i = name_index_arg + 1;

      // Optional array suffix (already reflected in Intsp_Data_Type)
      if (tokens[i].kind == Token_Open_Bracket)
      {
        while (tokens[i].kind != Token_Close_Bracket)
        {
          i += 1;
        }
        i += 1;
      }

      // Separator
      if (tokens[i].kind == Token_Comma)
      {
        i += 1;
        continue;
      }
      else if (tokens[i].kind == Token_Close_Parentheses)
      {
        break;
      }

      assert(!"Unexpected token in function argument list");
    }
  }

  if (is_declaration)
  {
    // Look for documentation
    _intsp_eat_trivia(lexer);
    Token* token = lexer_peek_token(lexer);
    if (token->kind == Token_Comment_Block)
    {
      String comment = string_slice(introspection->functions_arena, token->value, 2, token->value.count - 2);
      comment = string_trim(introspection->functions_arena, comment);
      function_object->documentation = string_copy(introspection->functions_arena, comment);
    }

    function_object->declaration.file = string_copy(introspection->functions_arena, file_path);
    function_object->declaration.line = (u32)line;
  }
  else
  {
    String body = S("");
    u32 braces_stack = 0;

    for (;;)
    {
      Token* token = lexer_peek_token(lexer);
      b32 finished = false;

      switch (token->kind)
      {
        case Token_Open_Brace:
        {
          braces_stack += 1;
        }
        break;

        case Token_Close_Brace:
        {
          if (braces_stack == 0)
          {
            finished = true;
          }
          else
          {
            braces_stack -= 1;
          }
        }
      }

      lexer_eat_token(lexer);

      if (finished)
      {
        break;
      }
      else
      {
        body = string_join(scratch.arena, body, token->value);
      }
    }

    function_object->body = string_copy(introspection->functions_arena, body);
    function_object->implementation.file = string_copy(introspection->functions_arena, file_path);
    function_object->implementation.line = (u32)line;
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
_intsp_report(Introspection* introspection, Intsp_Log_Severity severity, String file, u32 line, u32 col, String text)
{
  Intsp_Report* report = &introspection->report;
  if (report->logs_count >= report->logs_capacity)
  {
    return;
  }
  Intsp_Log* ilog = &report->logs[report->logs_count++];
  ilog->severity = severity;

  String text_to_log = string_join(introspection->arena, text, S("\n"));
  switch (severity)
  {
    case Intsp_Log_Severity_None:
    case Intsp_Log_Severity_Log:
    {
      ilog->text = Sf(introspection->arena, "[Introspection] LOG: "S_FMT"::(%u,%u)\n", S_ARG(file), line, col);
    } break;
    case Intsp_Log_Severity_Warning:
    { 
      ilog->text = Sf(introspection->arena, "[Introspection] WARNING: "S_FMT"::(%u,%u)\n", S_ARG(file), line, col);
    } break;
    case Intsp_Log_Severity_Error:
    {
      ilog->text = Sf(introspection->arena, "[Introspection] ERROR: "S_FMT"::(%u,%u)\n", S_ARG(file), line, col);
      introspection->fatal_error = true;
    } break;
  };
  ilog->text = string_join(introspection->arena, ilog->text, text_to_log);
}

function u64
intsp_aggregate_hash(Intsp_Agreggate_Object* obj)
{
  Scratch scratch = scratch_begin(0, 0);
  String result  = obj->identifier;

  for (Intsp_Agreggate_Object* parent = obj->parent; parent != 0; parent = parent->parent)
  {
    result = string_join(scratch.arena, parent->identifier, result);
  }

  u64 hash = string_hash(result);

  scratch_end(&scratch);
  return hash;
}

#endif // INTROSPECTION_H
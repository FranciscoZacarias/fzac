#ifndef INTROSPECTION_H
#define INTROSPECTION_H

#include "../Base.h"
#include "Lexer.h"
#include "Platform.h"

/* @File: Implementa introspection implemented specifically for my C code base. It makes very important assumptions and hardcodes things based on those assumptions.
          This is only meant to work on a codebase that follows my programming style. */

#define _intsp_error(message) \
  message_box(S("Introspection Error!"), message, S(__FILE__), __LINE__); \
  raddbg_break()

#define INTSP_MAX_MEMBERS_CAPACITY 32
#define INTSP_FILES_CAPACITY 32
#define INTSP_CODE_TAGS_CAPACITY 8
#define INTSP_STRUCT_CAPACITY 32
#define INTSP_FUNCTION_CAPACITY 128
#define INTSP_ENUM_MEMBERS_CAPACITY 64
#define INTSP_NESTED_AGGREGATE_CAPACITY 8

typedef enum
{
  Code_Tag_None = 0,
  Code_Tag_Todo,
  Code_Tag_File,
  Code_Tag_Leak,
  Code_Tag_Speed,
} Code_Tag_Kind;

typedef enum
{
  Aggregate_Error = 0,
  Aggregate_Struct,
  Aggregate_Union,
} Aggregate_Kind;

typedef struct Intsp_Source_Location Intsp_Source_Location;
typedef struct Intsp_Context Intsp_Context;
typedef struct Intsp_Aggregate Intsp_Aggregate;
typedef struct Intsp_Aggregate_Member Intsp_Aggregate_Member;
typedef struct Intsp_Code_Tag Intsp_Code_Tag;
typedef struct Intsp_File Intsp_File;

struct Intsp_Source_Location
{
  Intsp_File* file;
  u32 line;
};

struct Intsp_Aggregate_Member
{
  String type;
  String name;
  String documentation;
  b32    is_bit;

  Arena *nested_structs_arena;
  Intsp_Aggregate *nested_structs;
  u32 nested_structs_count;
  u32 nested_structs_capacity;

  Arena *nested_unions_arena;
  Intsp_Aggregate *nested_unions;
  u32 nested_unions_count;
  u32 nested_unions_capacity;
};

struct Intsp_Aggregate
{
  String name;
  Aggregate_Kind kind;

  Arena *members_arena;
  Intsp_Aggregate_Member *members;
  u32 members_count;
  u32 members_capacity;

  Intsp_Source_Location location;
};

typedef enum
{
  Typedef_Alias = 0,           /* typedef int MyInt; */
  Typedef_Struct_Forward,      /* typedef struct MyStruct MyStruct; */
  Typedef_Function_Pointer,    /* typedef void (*Callback)(int); */
  Typedef_Enum,                /* typedef enum { ... } MyEnum; */
} Typedef_Kind;

typedef struct Intsp_Enum_Member Intsp_Enum_Member;
struct Intsp_Enum_Member
{
  String name;
  String value;  // Empty if no explicit value
  String documentation;
};

typedef struct Intsp_Typedef Intsp_Typedef;
struct Intsp_Typedef
{
  Typedef_Kind kind;
  String name;
  String type;
  String documentation;

  Arena *enum_members_arena;
  Intsp_Enum_Member *enum_members;
  u32 enum_members_count;
  u32 enum_members_capacity;

  Intsp_Source_Location location;
};

typedef struct Intsp_Function_Argument Intsp_Function_Argument;
struct Intsp_Function_Argument
{
  String type;
  String name;
};

typedef struct Intsp_Function Intsp_Function;
struct Intsp_Function
{
  String name;
  String type;
  String body;
  String documentation;

  Intsp_Function_Argument *arguments;
  u32 arguments_count;
  u32 arguments_capacity;

  Intsp_Source_Location forward_declare_location;
  Intsp_Source_Location implementation_location;
};

struct Intsp_File
{
  String path;
  String documentation;

  Arena* arena;

  Arena *code_tags_arena;
  Intsp_Code_Tag *code_tags;
  u32 code_tags_count;
  u32 code_tags_capacity;

  Arena *aggregates_arena;
  Intsp_Aggregate *aggregates;
  u32 aggregates_count;
  u32 aggregates_capacity;

  Arena *typedefs_arena;
  Intsp_Typedef *typedefs;
  u32 typedefs_count;
  u32 typedefs_capacity;

  Arena *functions_arena;
  Intsp_Function *functions;
  u32 functions_count;
  u32 functions_capacity;
};

struct Intsp_Code_Tag
{
  Code_Tag_Kind kind;
  Intsp_File* file;
  u32 line;
  String text;
};

struct Intsp_Context 
{
  Arena *files_arena;
  Intsp_File *files;
  u32 files_count;
  u32 files_capacity;
};


function Intsp_Context intsp_run(String source_directory, b32 introspect_base_library);

function void   _intsp_skip_line(Lexer *lexer, Intsp_File *file);
function void   _intsp_skip_spaces(Lexer *lexer, Intsp_File *file);
function Token* _intsp_peek_token(Lexer *lexer, Intsp_File* file);
function void   _intsp_expect_kind(Token *token, Token_Kind expected_kind);
function void   _intsp_parse_struct_members(Lexer *lexer, Intsp_File *file, Intsp_Aggregate *aggregate);
function void   _intsp_parse_typedef(Lexer *lexer, Intsp_File *file);
function void   _intsp_parse_function(Intsp_Context *ctx, Lexer *lexer, Intsp_File *file);
function String _intsp_get_next_comment_or_line_break(Lexer *lexer, Intsp_File *file);

function Intsp_Context
intsp_run(String source_directory, b32 introspect_base_library)
{
  Scratch scratch = scratch_begin(0,0);

  Intsp_Context result;
  memory_zero_struct(&result);

  result.files_arena = arena_alloc();
  arena_array_init(result.files_arena, result.files, Intsp_File, INTSP_FILES_CAPACITY);

  String_List files = file_get_files_in_path(scratch.arena, source_directory, true);
  for (String_Node* next = files.first; next != NULL; next = next->next)
  {
    String file_being_lexed = next->value;

    if (!is_file(file_being_lexed)) continue;

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

    Intsp_File *intsp_file;
    arena_array_push(intsp_file, result.files, result.files_count, result.files_capacity);
    intsp_file->arena = arena_alloc();
    intsp_file->path  = string_copy(intsp_file->arena, file_being_lexed);

    intsp_file->code_tags_arena = arena_alloc();
    arena_array_init(intsp_file->code_tags_arena, intsp_file->code_tags, Intsp_Code_Tag, INTSP_CODE_TAGS_CAPACITY);

    intsp_file->aggregates_arena = arena_alloc();
    arena_array_init(intsp_file->aggregates_arena, intsp_file->aggregates, Intsp_Aggregate, INTSP_STRUCT_CAPACITY);

    intsp_file->typedefs_arena = arena_alloc();
    arena_array_init(intsp_file->typedefs_arena, intsp_file->typedefs, Intsp_Typedef, INTSP_STRUCT_CAPACITY);

    intsp_file->functions_arena = arena_alloc();
    arena_array_init(intsp_file->functions_arena, intsp_file->functions, Intsp_Function, INTSP_FUNCTION_CAPACITY);

    for (;;)
    {
      Token *token = _intsp_peek_token(&lexer, intsp_file);
      if (token->kind == Token_End_Of_File)
      {
        break;
      }

      if (token->kind == Token_Hash)
      {
        _intsp_skip_line(&lexer, intsp_file);
        continue;
      }

      if (token->kind == Token_Identifier)
      {
        b32 is_struct = string_equals(token->value, S("struct"), true);
        b32 is_union  = string_equals(token->value, S("union"), true);

        if (is_struct || is_union)
        {
          Intsp_Aggregate *aggregate;
          arena_array_push(aggregate, intsp_file->aggregates, intsp_file->aggregates_count, intsp_file->aggregates_capacity);

          aggregate->members_arena = arena_alloc();
          arena_array_init(aggregate->members_arena, aggregate->members, Intsp_Aggregate_Member, INTSP_MAX_MEMBERS_CAPACITY);

          aggregate->kind = is_struct ? Aggregate_Struct : Aggregate_Union;
          aggregate->location.file = intsp_file;
          aggregate->location.line = token->l0;

          lexer_eat_token(&lexer);
          _intsp_skip_spaces(&lexer, intsp_file);
          token = _intsp_peek_token(&lexer, intsp_file);

          _intsp_expect_kind(token, Token_Identifier);
          aggregate->name = string_copy(intsp_file->arena, token->value);

          while (token->kind != Token_Open_Brace)
          {
            lexer_eat_token(&lexer);
            token = _intsp_peek_token(&lexer, intsp_file);
          }

          lexer_eat_token(&lexer);
          _intsp_skip_spaces(&lexer, intsp_file);

          _intsp_parse_struct_members(&lexer, intsp_file, aggregate);
        }
        else if (string_equals(token->value, S("typedef"), true))
        {
          _intsp_parse_typedef(&lexer, intsp_file);
        }
        else if (string_equals(token->value, S("function"), true))
        {
          _intsp_parse_function(&result, &lexer, intsp_file);
        }
        else
        {
          _intsp_skip_line(&lexer, intsp_file);
        }
      }
      else
      {
        _intsp_skip_line(&lexer, intsp_file);
      }
    }
  }

  return result;
}

function void
_intsp_parse_function(Intsp_Context *ctx, Lexer *lexer, Intsp_File *file)
{
  Scratch scratch = scratch_begin(0, 0);
  
  Token *token = _intsp_peek_token(lexer, file);
  if (!string_equals(token->value, S("function"), true))
  {
    _intsp_error(S("Parsing a function expects first token to be 'function'."));
  }
  
  lexer_eat_token(lexer);
  _intsp_skip_spaces(lexer, file);
  
  String_List return_type_list = string_list_new();
  string_list_push(scratch.arena, &return_type_list, S("function"));
  
  String function_name = string_zero();
  u32 name_line = 0;
  
  for (;;)
  {
    token = _intsp_peek_token(lexer, file);
    if (token->kind == Token_Identifier)
    {
      Token *next = lexer_peek_nth_token(lexer, 1);
      if (next->kind == Token_Open_Parentheses)
      {
        function_name = string_copy(file->arena, token->value);
        name_line = token->l0;
        lexer_eat_token(lexer);
        break;
      }
    }
    
    string_list_push(scratch.arena, &return_type_list, S(" "));
    string_list_push(scratch.arena, &return_type_list, token->value);
    lexer_eat_token(lexer);
    _intsp_skip_spaces(lexer, file);
  }
  
  token = _intsp_peek_token(lexer, file);
  if (token->kind != Token_Open_Parentheses)
  {
    _intsp_error(Sf(scratch.arena, "Expected '(' after function name, got: "S_FMT"\n", S_ARG(token->value)));
  }
  
  lexer_eat_token(lexer);
  _intsp_skip_spaces(lexer, file);
  token = _intsp_peek_token(lexer, file);
  
  Intsp_Function_Argument *arguments = NULL;
  u32 arguments_count = 0;
  u32 arguments_capacity = 0;
  Arena *args_arena = NULL;
  
  if (token->kind != Token_Close_Parentheses)
  {
    args_arena = arena_alloc();
    arena_array_init(args_arena, arguments, Intsp_Function_Argument, 16);

    for (;;)
    {
      token = _intsp_peek_token(lexer, file);
      if (token->kind == Token_Close_Parentheses) break;

      // Check for var args
      if (token->kind == Token_Dot)
      {
        Token *t1 = lexer_peek_nth_token(lexer, 1);
        Token *t2 = lexer_peek_nth_token(lexer, 2);

        if (t1->kind == Token_Dot && t2->kind == Token_Dot)
        {
          lexer_eat_token(lexer);
          lexer_eat_token(lexer);
          lexer_eat_token(lexer);

          Intsp_Function_Argument *arg;
          arena_array_push(arg, arguments, arguments_count, arguments_capacity);

          arg->type = S("__va_args__");
          arg->name = S("...");

          token = _intsp_peek_token(lexer, file);
          if (token->kind == Token_Comma)
          {
            lexer_eat_token(lexer);
            _intsp_skip_spaces(lexer, file);
          }

          continue;
        }
      }

      String_List arg_tokens = string_list_new();
      Token *arg_name_token = NULL;

      s32 paren_depth   = 0;
      s32 bracket_depth = 0;

      for (;;)
      {
        token = _intsp_peek_token(lexer, file);

        if (token->kind == Token_Comma || token->kind == Token_Close_Parentheses)
        {
          break;
        }

        if (token->kind == Token_Open_Parentheses)  paren_depth++;
        if (token->kind == Token_Close_Parentheses) paren_depth--;
        if (token->kind == Token_Open_Bracket)      bracket_depth++;
        if (token->kind == Token_Close_Bracket)     bracket_depth--;

        if (token->kind == Token_Identifier && paren_depth == 0 && bracket_depth == 0)
        {
          arg_name_token = token;
        }

        string_list_push(scratch.arena, &arg_tokens, token->value);
        string_list_push(scratch.arena, &arg_tokens, S(" "));

        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
      }

      string_list_remove_last(&arg_tokens);

      String arg_name = string_zero();
      if (arg_tokens.first != NULL)
      {
        arg_name = string_list_remove_last(&arg_tokens);
      }

      String arg_type = string_list_join(file->arena, &arg_tokens);
      arg_type = string_trim(file->arena, arg_type);

      Intsp_Function_Argument *arg;
      arena_array_push(arg, arguments, arguments_count, arguments_capacity);

      arg->type = arg_type;
      arg->name = arg_name;

      token = _intsp_peek_token(lexer, file);
      if (token->kind == Token_Comma)
      {
        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
      }
    }
  }
  
  token = _intsp_peek_token(lexer, file);
  if (token->kind != Token_Close_Parentheses)
  {
    _intsp_error(Sf(scratch.arena, "Expected ')' after function arguments, got: "S_FMT"\n", S_ARG(token->value)));
  }
  
  lexer_eat_token(lexer);
  _intsp_skip_spaces(lexer, file);
  token = _intsp_peek_token(lexer, file);
  
  // check if forward declare or implementation
  b32 is_forward_declare = (token->kind == Token_Semicolon);
  b32 is_implementation = (token->kind == Token_Open_Brace);
  
  if (!is_forward_declare && !is_implementation)
  {
    _intsp_error(Sf(scratch.arena, "Expected ';' or '{' after function signature, got: "S_FMT"\n", S_ARG(token->value)));
  }
  
  Intsp_Function *func = NULL;
  for (u32 files_index = 0; files_index < ctx->files_count; files_index += 1)
  {
    Intsp_File *it_file = &ctx->files[files_index];
    for (u32 func_in_file_idex = 0; func_in_file_idex < it_file->functions_count; func_in_file_idex++)
    {
      if (string_equals(it_file->functions[func_in_file_idex].name, function_name, false))
      {
        func = &it_file->functions[func_in_file_idex];
        break;
      }
    }
  }
  
  // create new function if doesn't exist
  if (func == NULL)
  {
    arena_array_push(func, file->functions, file->functions_count, file->functions_capacity);
    memory_zero_struct(func);
    func->name = function_name;
    func->type = string_list_join(file->arena, &return_type_list);
    func->arguments = arguments;
    func->arguments_count = arguments_count;
    func->arguments_capacity = arguments_capacity;
  }
  
  if (is_forward_declare)
  {
    func->forward_declare_location.file = file;
    func->forward_declare_location.line = name_line;
    
    u32 semicolon_line = token->l0;
    lexer_eat_token(lexer);
    
    // check for documentation comment on same line - use lexer_peek_token to see comments
    token = lexer_peek_token(lexer);
    if (token->l0 == (s32)semicolon_line && (token->kind == Token_Comment_Line || token->kind == Token_Comment_Block))
    {
      func->documentation = string_copy(file->arena, token->value);
      lexer_eat_token(lexer);
    }
  }
  else // is_implementation
  {
    func->implementation_location.file = file;
    func->implementation_location.line = name_line;
    
    // collect body
    String_List body_list = string_list_new();
    
    if (token->kind != Token_Open_Brace)
    {
      _intsp_error(Sf(scratch.arena, "Expected '{' for function body, got: "S_FMT"\n", S_ARG(token->value)));
    }
    
    lexer_eat_token(lexer);
    
    u32 brace_depth = 1;
    while (brace_depth > 0)
    {
      token = _intsp_peek_token(lexer, file);
      
      if (token->kind == Token_End_Of_File)
      {
        _intsp_error(S("Unexpected EOF in function body"));
      }
      
      if (token->kind == Token_Open_Brace)
      {
        brace_depth++;
      }
      else if (token->kind == Token_Close_Brace)
      {
        brace_depth--;
        if (brace_depth == 0)
          break;
      }
      
      string_list_push(scratch.arena, &body_list, token->value);
      
      lexer_eat_token(lexer);
    }
    
    func->body = string_list_join(file->arena, &body_list);
    
    token = _intsp_peek_token(lexer, file);
    if (token->kind != Token_Close_Brace)
    {
      _intsp_error(Sf(scratch.arena, "Expected '}' to close function body, got: "S_FMT"\n", S_ARG(token->value)));
    }
    
    lexer_eat_token(lexer);
  }
  
  scratch_end(&scratch);
}

function void
_intsp_parse_typedef(Lexer *lexer, Intsp_File *file)
{
  Token *token = _intsp_peek_token(lexer, file);

  Intsp_Typedef *typedef_item;
  arena_array_push(typedef_item, file->typedefs, file->typedefs_count, file->typedefs_capacity);
  typedef_item->documentation = string_zero();

  typedef_item->enum_members_arena = arena_alloc();
  arena_array_init(typedef_item->enum_members_arena, typedef_item->enum_members, Intsp_Enum_Member, INTSP_ENUM_MEMBERS_CAPACITY);

  typedef_item->location.file = file;
  typedef_item->location.line = token->l0;

  // Skip 'typedef'
  lexer_eat_token(lexer);
  _intsp_skip_spaces(lexer, file);
  token = _intsp_peek_token(lexer, file);

  if (token->kind == Token_Identifier && string_equals(token->value, S("struct"), true))
  {
    typedef_item->kind = Typedef_Struct_Forward;

    lexer_eat_token(lexer);
    _intsp_skip_spaces(lexer, file);
    token = _intsp_peek_token(lexer, file);

    _intsp_expect_kind(token, Token_Identifier);
    String struct_name = string_copy(file->arena, token->value);

    lexer_eat_token(lexer);
    _intsp_skip_spaces(lexer, file);
    token = _intsp_peek_token(lexer, file);

    _intsp_expect_kind(token, Token_Identifier);
    typedef_item->name = string_copy(file->arena, token->value);
    typedef_item->type = string_join(file->arena, S("struct "), struct_name);

    lexer_eat_token(lexer);
    _intsp_skip_spaces(lexer, file);
    token = _intsp_peek_token(lexer, file);

    _intsp_expect_kind(token, Token_Semicolon);
    lexer_eat_token(lexer);

    String doc = _intsp_get_next_comment_or_line_break(lexer, file);
    if (doc.count > 0)
    {
      typedef_item->documentation = string_copy(file->arena, doc);
    }

    _intsp_skip_spaces(lexer, file);
    return;
  }

  if (token->kind == Token_Identifier && string_equals(token->value, S("enum"), true))
  {
    typedef_item->kind = Typedef_Enum;

    lexer_eat_token(lexer);
    _intsp_skip_spaces(lexer, file);
    token = _intsp_peek_token(lexer, file);

    _intsp_expect_kind(token, Token_Open_Brace);
    lexer_eat_token(lexer);
    _intsp_skip_spaces(lexer, file);
    token = _intsp_peek_token(lexer, file);

    while (token->kind != Token_Close_Brace && token->kind != Token_End_Of_File)
    {
      if (token->kind == Token_Identifier)
      {
        Intsp_Enum_Member *enum_member;
        arena_array_push(enum_member, typedef_item->enum_members, typedef_item->enum_members_count, typedef_item->enum_members_capacity);

        enum_member->name  = string_copy(file->arena, token->value);
        enum_member->value = string_zero();

        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
        token = _intsp_peek_token(lexer, file);

        if (token->kind == Token_Equal)
        {
          lexer_eat_token(lexer);
          _intsp_skip_spaces(lexer, file);
          token = _intsp_peek_token(lexer, file);

          String_List value_tokens = string_list_new();
          while (token->kind != Token_Comma && token->kind != Token_Close_Brace && token->kind != Token_End_Of_File)
          {
            string_list_push(file->arena, &value_tokens, token->value);
            lexer_eat_token(lexer);
            _intsp_skip_spaces(lexer, file);
            token = _intsp_peek_token(lexer, file);
          }

          enum_member->value = string_list_join(file->arena, &value_tokens);
        }

        if (token->kind == Token_Comma)
        {
          lexer_eat_token(lexer);
          _intsp_skip_spaces(lexer, file);
          token = _intsp_peek_token(lexer, file);
        }
      }
      else
      {
        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
        token = _intsp_peek_token(lexer, file);
      }
    }

    _intsp_expect_kind(token, Token_Close_Brace);
    lexer_eat_token(lexer);
    _intsp_skip_spaces(lexer, file);
    token = _intsp_peek_token(lexer, file);

    _intsp_expect_kind(token, Token_Identifier);
    typedef_item->name = string_copy(file->arena, token->value);
    typedef_item->type = S("enum");

    lexer_eat_token(lexer);
    _intsp_skip_spaces(lexer, file);
    token = _intsp_peek_token(lexer, file);

    _intsp_expect_kind(token, Token_Semicolon);
    lexer_eat_token(lexer);
    return;
  }

  String_List type_tokens = string_list_new();
  String_List name_tokens = string_list_new();
  b32 is_function_pointer = false;
  u64 paren_depth = 0;

  while (token->kind != Token_Semicolon && token->kind != Token_End_Of_File)
  {
    if (token->kind == Token_Open_Parentheses)
    {
      paren_depth++;
      if (paren_depth == 1 && type_tokens.node_count > 0)
      {
        is_function_pointer = true;
      }
    }
    else if (token->kind == Token_Close_Parentheses)
    {
      paren_depth--;
    }

    if (is_function_pointer)
    {
      string_list_push(file->arena, &name_tokens, token->value);
      string_list_push(file->arena, &name_tokens, S(" "));
    }
    else
    {
      string_list_push(file->arena, &type_tokens, token->value);
    }

    lexer_eat_token(lexer);
    _intsp_skip_spaces(lexer, file);
    token = _intsp_peek_token(lexer, file);
  }

  if (is_function_pointer)
  {
    typedef_item->kind = Typedef_Function_Pointer;
    typedef_item->type = string_list_join(file->arena, &type_tokens);
    typedef_item->name = string_list_join(file->arena, &name_tokens);
  }
  else
  {
    typedef_item->kind = Typedef_Alias;

    if (type_tokens.node_count > 1)
    {
      String last = string_list_remove_last(&type_tokens);
      typedef_item->type = string_list_join(file->arena, &type_tokens);
      typedef_item->name = string_copy(file->arena, last);
    }
    else if (type_tokens.node_count == 1)
    {
      typedef_item->type = string_zero();
      typedef_item->name = string_list_join(file->arena, &type_tokens);
    }
    else
    {
      typedef_item->type = string_zero();
      typedef_item->name = string_zero();
    }
  }

  if (token->kind == Token_Semicolon)
  {
    lexer_eat_token(lexer);

    String doc = _intsp_get_next_comment_or_line_break(lexer, file);
    if (doc.count > 0)
    {
      typedef_item->documentation = string_copy(file->arena, doc);
    }
  }
}

function void
_intsp_parse_struct_members(Lexer *lexer, Intsp_File *file, Intsp_Aggregate *aggregate)
{
  Token* token = _intsp_peek_token(lexer, file);

  while (token->kind != Token_Close_Brace && token->kind != Token_End_Of_File)
  {
    b32 is_nested_struct = (token->kind == Token_Identifier && string_equals(token->value, S("struct"), true));
    b32 is_nested_union  = (token->kind == Token_Identifier && string_equals(token->value, S("union"), true));

    if (is_nested_struct || is_nested_union)
    {
      lexer_eat_token(lexer);
      _intsp_skip_spaces(lexer, file);
      token = _intsp_peek_token(lexer, file);

      _intsp_expect_kind(token, Token_Open_Brace);
      lexer_eat_token(lexer);
      _intsp_skip_spaces(lexer, file);

      Intsp_Aggregate_Member *member;
      arena_array_push(member, aggregate->members,aggregate->members_count, aggregate->members_capacity);

      member->type  = string_zero();
      member->is_bit = false;

      member->nested_structs_arena = arena_alloc();
      arena_array_init(member->nested_structs_arena, member->nested_structs, Intsp_Aggregate, INTSP_NESTED_AGGREGATE_CAPACITY);

      member->nested_unions_arena = arena_alloc();
      arena_array_init(member->nested_unions_arena, member->nested_unions, Intsp_Aggregate, INTSP_NESTED_AGGREGATE_CAPACITY);

      Intsp_Aggregate *nested_aggregate;
      if (is_nested_struct)
      {
        arena_array_push(nested_aggregate, member->nested_structs,member->nested_structs_count, member->nested_structs_capacity);
        nested_aggregate->kind = Aggregate_Struct;
        nested_aggregate->name = S("_nameless_struct_");
      }
      else
      {
        arena_array_push(nested_aggregate, member->nested_unions, member->nested_unions_count, member->nested_unions_capacity);
        nested_aggregate->kind = Aggregate_Union;
        nested_aggregate->name = S("_nameless_union_");
      }

      nested_aggregate->members_arena    = arena_alloc();
      arena_array_init(nested_aggregate->members_arena,nested_aggregate->members, Intsp_Aggregate_Member, INTSP_MAX_MEMBERS_CAPACITY);

      _intsp_parse_struct_members(lexer, file, nested_aggregate);

      token = _intsp_peek_token(lexer, file);
      _intsp_expect_kind(token, Token_Close_Brace);
      lexer_eat_token(lexer);
      _intsp_skip_spaces(lexer, file);
      token = _intsp_peek_token(lexer, file);

      if (token->kind == Token_Identifier)
      {
        member->name = string_copy(file->arena, token->value);
        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
        token = _intsp_peek_token(lexer, file);
      }
      else
      {
        member->name = string_zero();
      }

      if (token->kind == Token_Semicolon)
      {
        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
        token = _intsp_peek_token(lexer, file);
      }

      continue;
    }

    Intsp_Aggregate_Member *member;
    arena_array_push(member, aggregate->members, aggregate->members_count, aggregate->members_capacity);
    member->is_bit = false;

    String_List type_tokens = string_list_new();
    String_List name_tokens = string_list_new();
    b32 is_function_pointer = false;
    b32 seen_asterisk = false;
    u64 paren_depth = 0;

    while (token->kind != Token_Semicolon && token->kind != Token_End_Of_File)
    {
      if (token->kind == Token_Colon)
      {
        member->is_bit = true;

        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
        token = _intsp_peek_token(lexer, file);

        if (token->kind == Token_Number || token->kind == Token_Identifier)
        {
          lexer_eat_token(lexer);
          _intsp_skip_spaces(lexer, file);
          token = _intsp_peek_token(lexer, file);
        }
        break;
      }

      if (token->kind == Token_Comma && paren_depth == 0)
      {
        break;
      }

      if (token->kind == Token_Open_Parentheses)
      {
        paren_depth++;
        if (paren_depth == 1 && type_tokens.node_count > 0)
        {
          is_function_pointer = true;
        }
      }
      else if (token->kind == Token_Close_Parentheses)
      {
        paren_depth--;
      }

      if (is_function_pointer)
      {
        string_list_push(file->arena, &name_tokens, token->value);
        string_list_push(file->arena, &name_tokens, S(" "));
      }
      else if (token->kind == Token_Asterisk)
      {
        seen_asterisk = true;
        string_list_push(file->arena, &name_tokens, token->value);
      }
      else if (seen_asterisk)
      {
        string_list_push(file->arena, &name_tokens, token->value);
      }
      else
      {
        string_list_push(file->arena, &type_tokens, token->value);
      }

      lexer_eat_token(lexer);
      _intsp_skip_spaces(lexer, file);
      token = _intsp_peek_token(lexer, file);
    }

    if (is_function_pointer || seen_asterisk)
    {
      member->type = string_list_join(file->arena, &type_tokens);
      member->name = string_list_join(file->arena, &name_tokens);
    }
    else
    {
      if (type_tokens.node_count > 1)
      {
        String last = string_list_remove_last(&type_tokens);
        member->type = string_list_join(file->arena, &type_tokens);
        member->name = string_copy(file->arena, last);
      }
      else if (type_tokens.node_count == 1)
      {
        member->type = string_zero();
        member->name = string_list_join(file->arena, &type_tokens);
      }
      else
      {
        member->type = string_zero();
        member->name = string_zero();
      }
    }

    if (!is_function_pointer && !member->is_bit)
    {
      while (token->kind == Token_Comma)
      {
        lexer_eat_token(lexer);
        _intsp_skip_spaces(lexer, file);
        token = _intsp_peek_token(lexer, file);

        Intsp_Aggregate_Member *extra_member;
        arena_array_push(extra_member, aggregate->members, aggregate->members_count, aggregate->members_capacity);
        extra_member->type = string_copy(file->arena, member->type);
        extra_member->is_bit = false;

        String_List extra_name_tokens = string_list_new();

        while (token->kind != Token_Comma       &&
               token->kind != Token_Semicolon   &&
               token->kind != Token_End_Of_File &&
               token->kind != Token_Colon)
        {
          string_list_push(file->arena, &extra_name_tokens, token->value);
          lexer_eat_token(lexer);
          _intsp_skip_spaces(lexer, file);
          token = _intsp_peek_token(lexer, file);
        }

        extra_member->name = string_list_join(file->arena, &extra_name_tokens);

        if (token->kind == Token_Colon)
        {
          extra_member->is_bit = true;
          lexer_eat_token(lexer);
          _intsp_skip_spaces(lexer, file);
          token = _intsp_peek_token(lexer, file);

          if (token->kind == Token_Number || token->kind == Token_Identifier)
          {
            lexer_eat_token(lexer);
            _intsp_skip_spaces(lexer, file);
            token = _intsp_peek_token(lexer, file);
          }
        }
      }
    }

    if (token->kind == Token_Semicolon)
    {
      lexer_eat_token(lexer);
      _intsp_skip_spaces(lexer, file);
      token = _intsp_peek_token(lexer, file);
    }
  }
}

function void
_intsp_skip_line(Lexer *lexer, Intsp_File *file)
{
  b32 multi_line = false;
  for (;;)
  {
    Token *token = _intsp_peek_token(lexer, file);
    lexer_eat_token(lexer);

    if (token->kind == Token_End_Of_File)
    {
      break;
    }

    if (token->kind == Token_Backslash)
    {
      multi_line = true;
    }
    if (token->kind == Token_Line_Break)
    {
      if (multi_line)
      {
        multi_line = false;
      }
      else
      {
        break;
      }
    }
  }
}

function void
_intsp_skip_spaces(Lexer *lexer, Intsp_File *file)
{
  Token *token = _intsp_peek_token(lexer, file);

  while (token->kind == Token_Whitespace || token->kind == Token_Line_Break)
  {
    lexer_eat_token(lexer);
    token = _intsp_peek_token(lexer, file);
  }
}

function void
_intsp_expect_kind(Token *token, Token_Kind expected_kind)
{
  if (token->kind != expected_kind)
  {
    Scratch scratch = scratch_begin(0,0);
    _intsp_error(Sf(scratch.arena, "Expected token kind wrong. Token: '%s', Expected: '%d'\n", token->value.cstring, expected_kind));
    assert(0);
    scratch_end(&scratch);
  }
}

function Token*
_intsp_peek_token(Lexer *lexer, Intsp_File *file)
{
  for (;;)
  {
    Token *token = lexer_peek_token(lexer);

    if (token->kind != Token_Comment_Block && token->kind != Token_Comment_Line)
    {
      return token;
    }

    Scratch scratch = scratch_begin(0,0);

    Lexer comment_lexer;
    lexer_init_from_string_with_arena(&comment_lexer, scratch.arena, token->value, Trivia_None, Emit_None);

    for (;;)
    {
      Token* ct = lexer_peek_token(&comment_lexer);

      if (ct->kind == Token_End_Of_File)
      {
        break;
      }

      if (ct->kind == Token_At)
      {
        lexer_eat_token(&comment_lexer);

        Token* ident = lexer_peek_token(&comment_lexer);
        if (ident->kind != Token_Identifier)
        {
          lexer_eat_token(&comment_lexer);
          continue;
        }

        Code_Tag_Kind tag = Code_Tag_None;

        if (string_equals(ident->value, S("File"),  false)) { tag = Code_Tag_File;  }
        if (string_equals(ident->value, S("Leak"),  false)) { tag = Code_Tag_Leak;  }
        if (string_equals(ident->value, S("Speed"), false)) { tag = Code_Tag_Speed; }
        if (string_equals(ident->value, S("Todo"),  false)) { tag = Code_Tag_Todo;  }

        lexer_eat_token(&comment_lexer);

        if (tag == Code_Tag_None)
        {
          continue;
        }

        Token* t = lexer_peek_token(&comment_lexer);
        if (t->kind != Token_Open_Parentheses)
        {
          continue;
        }

        while (t->kind != Token_Close_Parentheses && t->kind != Token_End_Of_File)
        {
          lexer_eat_token(&comment_lexer);
          t = lexer_peek_token(&comment_lexer);
        }

        if (t->kind == Token_Close_Parentheses)
        {
          lexer_eat_token(&comment_lexer);
        }

        t = lexer_peek_token(&comment_lexer);
        if (t->kind != Token_Colon)
        {
          continue;
        }

        lexer_eat_token(&comment_lexer);

        u64 start = comment_lexer.current_character_index;
        u64 end   = token->value.count;

        if (token->kind == Token_Comment_Block && end >= 2)
        {
          end -= 2; // "*/"
        }

        if (tag == Code_Tag_File)
        {
          String text  = {0};
          text.cstring = token->value.cstring + start;
          text.count   = end - start;
          file->documentation = string_copy(file->arena, text);          
        }
        else
        {
          String text = {0};
          text.cstring = token->value.cstring + start;
          text.count = (start < end) ? (end - start) : 0;

          // arena-backed array push
          Intsp_Code_Tag *slot;
          arena_array_push(slot, file->code_tags, file->code_tags_count, file->code_tags_capacity);

          slot->kind = tag;
          slot->file = file;
          slot->line = token->l0;
          if (text.count > 0) 
          {
            slot->text = string_copy(file->arena, text);
          }
        }

        break; // one tag per comment is enough
      }

      lexer_eat_token(&comment_lexer);
    }

    scratch_end(&scratch);
    lexer_eat_token(lexer);
  }
}

function String
_intsp_get_next_comment_or_line_break(Lexer *lexer, Intsp_File *file)
{
  Token *token = _intsp_peek_token(lexer, file);
  
  while (token->kind != Token_Comment_Line &&  token->kind != Token_Comment_Block &&  token->kind != Token_Line_Break &&  token->kind != Token_End_Of_File)
  {
    lexer_eat_token(lexer);
    token = lexer_peek_token(lexer);
  }
  
  if (token->kind == Token_Comment_Line || token->kind == Token_Comment_Block)
  {
    String comment = string_copy(file->arena, token->value);
    return comment;
  }
  else if (token->kind == Token_Line_Break)
  {
    lexer_eat_token(lexer);
    return string_zero();
  }
  
  return string_zero();
}

#endif // INTROSPECTION_H
#ifndef INTROSPECTION_H
#define INTROSPECTION_H

#include "../Base.h"
#include "Lexer.h"

#define MAX_FUNCTION_KEYWORDS 4 
#define MAX_FUNCTION_ARGUMENTS 16
#define MAX_FUNCTIONS 1024

#define MAX_ENUMS 256
#define MAX_TYPEDEFS 256
#define MAX_STRUCTS 512

#define MAX_META_COMMANDS 128

typedef struct Source_Location Source_Location;
struct Source_Location 
{
  String file;
  u32 line;
};

typedef struct Data_Type Data_Type;
struct Data_Type
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

typedef struct Typedef_Object Typedef_Object; /* For when the user typedefs something that isn't a struct or enum. E.g. "typedef unsigned char c8", unsigned char is data_type, c8 is identifier */
struct Typedef_Object
{
  Data_Type data_type;
  String identifier;
};

typedef struct Function_Argument Function_Argument;
struct Function_Argument
{
  b32 is_var_args; /* If true, overrides data_type and identifier, since they lose meaning. */
  Data_Type data_type;
  String identifier;
};

typedef struct Function_Object Function_Object;
struct Function_Object
{
  String* keywords; /* Things like 'function', 'inline', 'CALLBACK', 'WINAPI' etc... */
  u32 keywords_count;
  u32 keywords_capacity;

  Data_Type return_type;
  String identifier;

  Function_Argument* arguments;
  u32 arguments_count;
  u32 arguments_capacity;

  String body;
  String documentation;

  Source_Location declaration;
  Source_Location implementation;
};

typedef struct Struct_Member Struct_Member;
struct Struct_Member
{
  Data_Type data_type;
  String identifier;
  String documentation;
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
typedef struct Struct_Object Struct_Object;
struct Struct_Object
{
  String identifier;

  Struct_Member* members;
  u32 members_count;
  u32 members_capacity;

  String body;
  String documentation;

  Source_Location forward_declaration;
  Source_Location definition;
};

typedef struct Enum_Member Enum_Member;
struct Enum_Member
{
  String identifier;
  String documentation;
  b32 is_explicit_bit_flag; /* In case the value is something like Trivia_Line_Break = (1 << 5) ... */
  b32 is_one_or_more_enum_flags; /* In case the value is something like Trivia_All = Trivia_Whitespace|Trivia_Carriage_Return|Trivia_Tab|Trivia_Form_Feed|Trivia_Vertical_Tab|Trivia_Line_Break ... */
  s64 value;
};

typedef struct Enum_Object Enum_Object;
struct Enum_Object 
{
  String identifier;
  String type;

  Enum_Member* members;
  u32 members_count;
  u32 members_capacity;

  Source_Location type_casted_at;
  Source_Location definition;
};

typedef struct Global_Variable Global_Variable;
struct Global_Variable
{
  Data_Type data_type;
  String identifier;
  String initialization; // If it's initialized, this string will contain the value;
};

typedef enum
{
  Meta_Command_Error = 0,
  Meta_Command_Enum_To_String,
  Meta_Command_Forward_Declare_Enum, /* It's not really typedefing an enum, since that's not really possible, but typedefs a integer to the enum. */
} Meta_Command_Kind;

typedef struct Meta_Command Meta_Command;
struct Meta_Command
{
  Meta_Command_Kind kind;
  String file; /* File that called this command */

  union
  {
    Enum_Object* enum_object;
  } payload;
};

typedef struct Introspection Introspection;
struct Introspection
{
  Arena* functions_arena;
  Function_Object* functions;
  u32 functions_capacity;
  u32 functions_count;

  Arena* structs_arena;
  Struct_Object* structs;
  u32 structs_capacity;
  u32 structs_count;

  Arena* enums_arena;
  Enum_Object* enums;
  u32 enums_capacity;
  u32 enums_count;

  Arena* typedefs_arena;
  Typedef_Object* typedefs;
  u32 typedefs_capacity;
  u32 typedefs_count;

  Arena* commands_arena;
  Meta_Command* commands;
  u32 commands_capacity;
  u32 commands_count;
};

function Introspection get_introspection(String source_directory, b32 introspect_base_library); /* Parses a project directory. */

function u32  parse_enum_members(Arena* arena, Lexer* lexer, Enum_Member* temp_members, u32 max_members); /* Parses the members of an enum. Anything after { and before }. Returns how many members were added */
function void parse_enum(Lexer* lexer, Introspection* introspection, String file_path); /* Parses enums */
function void parse_typedef(Lexer* lexer, Introspection* introspection, String file_path); /* Parses typedefs that are not structs nor enums */
function void parse_function(Lexer* lexer, Introspection* introspection, String file_path);
function void parse_structs(Lexer* lexer, Introspection* introspection, String file_path);
function void eat_trivia(Lexer* lexer);
function Data_Type        parse_data_type(Arena* arena, Token* tokens, u32 start, u32 end); /* Parses a data type from a view into a token array */
function Function_Object* find_function(Introspection* introspection, String function_name);
function Enum_Object*     find_enum(Introspection* introspection, String enum_name);
function Struct_Object*   find_struct(Introspection* introspection, String struct_name);
function Typedef_Object*  find_typedef(Introspection* introspection, String typedef_name);


function Introspection
get_introspection(String source_directory, b32 introspect_base_library)
{
  Scratch scratch = scratch_begin(0,0);

  Introspection result;
  memory_zero_struct(&result);

  result.functions_arena    = arena_alloc();
  result.functions_capacity = MAX_FUNCTIONS;
  result.functions          = push_array(result.functions_arena, Function_Object, result.functions_capacity);

  result.enums_arena    = arena_alloc();
  result.enums_capacity = MAX_ENUMS;
  result.enums          = push_array(result.enums_arena, Enum_Object, result.enums_capacity);

  result.typedefs_arena    = arena_alloc();
  result.typedefs_capacity = MAX_TYPEDEFS;
  result.typedefs          = push_array(result.typedefs_arena, Typedef_Object, result.enums_capacity);

  result.structs_arena    = arena_alloc();
  result.structs_capacity = MAX_STRUCTS;
  result.structs          = push_array(result.structs_arena, Struct_Object, result.structs_capacity);
  
  result.commands_arena    = arena_alloc();
  result.commands_capacity = MAX_META_COMMANDS;
  result.commands          = push_array(result.commands_arena, Meta_Command, result.commands_capacity);

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
          parse_function(&lexer, &result, file_being_lexed);
          at_line_start = true;
          continue;
        }
        else if (string_equals(token->value, S("enum"), true))
        {
          parse_enum(&lexer, &result, file_being_lexed);
          at_line_start = true;
          continue;
        }
        else if (string_equals(token->value, S("struct"), true))
        {
          lexer_eat_token(&lexer); // TODO: real struct parsing
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
          eat_trivia(&lexer);
          token = lexer_peek_token(&lexer);

          if (string_equals(token->value, S("enum"), true))
          {
            // @TODO(fz): Better logging system
            printf("In our codebase, please don't do typedef enum. Instead do:\n"
            "enum META_ENUM_LINK(Name_Of_Enum, type)\n"
            "{\n"
            "  ...\n"
            "};\n");
            lexer_eat_token(&lexer);
          }
          else if (string_equals(token->value, S("struct"), true))
          {
            parse_structs(&lexer, &result, file_being_lexed);
          }
          else
          {
            parse_typedef(&lexer, &result, file_being_lexed);
          }

          at_line_start = true;
          continue;
        }

        // @NOTE(fz): Global scope meta commands
        else if (string_equals(token->value, S("META_ENUM_TO_STRING"), true))
        {
          lexer_eat_token(&lexer);
          token = lexer_peek_token(&lexer);
          assert(token->kind == Token_Open_Parentheses);

          lexer_eat_token(&lexer);
          token = lexer_peek_token(&lexer);
          assert(token->kind == Token_Identifier);

          Enum_Object* enum_object = find_enum(&result, token->value);
          Meta_Command* command = &result.commands[result.commands_count++];
          command->kind = Meta_Command_Enum_To_String;
          command->payload.enum_object = enum_object; 
          command->file = string_copy(result.commands_arena, file_being_lexed);

          lexer_eat_token(&lexer);
          token = lexer_peek_token(&lexer);
          assert(token->kind == Token_Close_Parentheses);

          lexer_eat_token(&lexer);
          token = lexer_peek_token(&lexer);
          assert(token->kind == Token_Semicolon);
        }
      }

      lexer_eat_token(&lexer);
      at_line_start = false;
    }
  }

  scratch_end(&scratch);
  return result;
}

function Enum_Object*
find_enum(Introspection* introspection, String enum_name)
{
  Enum_Object* result = NULL;
  for (u32 i = 0; i < introspection->enums_count; i += 1)
  {
    Enum_Object* enum_object = &(introspection->enums[i]);
    if (string_equals(enum_name, enum_object->identifier, true))
    {
      result = enum_object;
      break;
    }
  }
  return result;
}

function Function_Object*
find_function(Introspection* introspection, String function_name)
{
  Function_Object* result = NULL;
  for (u32 i = 0; i < introspection->functions_count; i += 1)
  {
    Function_Object* function_object = &(introspection->functions[i]);
    if (string_equals(function_name, function_object->identifier, true))
    {
      result = function_object;
      break;
    }
  }
  return result;
}

function Struct_Object*
find_struct(Introspection* introspection, String struct_name)
{
  Struct_Object* result = NULL;
  for (u32 i = 0; i < introspection->structs_count; i += 1)
  {
    Struct_Object* struct_object = &(introspection->structs[i]);
    if (string_equals(struct_name, struct_object->identifier, true))
    {
      result = struct_object;
      break;
    }
  }
  return result;
}

function Typedef_Object*
find_typedef(Introspection* introspection, String typedef_name)
{
  Typedef_Object* result = NULL;
  for (u32 i = 0; i < introspection->typedefs_count; i += 1)
  {
    Typedef_Object* typedef_object = &(introspection->typedefs[i]);
    if (string_equals(typedef_name, typedef_object->identifier, true))
    {
      result = typedef_object;
      break;
    }
  }
  return result;
}

function Data_Type
parse_data_type(Arena* arena, Token* tokens, u32 start, u32 end)
{
  Data_Type type;
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
parse_enum_members(Arena* arena, Lexer* lexer, Enum_Member* temp_members, u32 max_members)
{
  Token* token = lexer_peek_token(lexer);
  assert(token->kind == Token_Open_Brace);

  lexer_eat_token(lexer);
  eat_trivia(lexer);

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

    Enum_Member* member = &temp_members[members_count++];
    memory_zero_struct(member);

    member->identifier = string_copy(arena, token->value);

    lexer_eat_token(lexer);
    eat_trivia(lexer);

    token = lexer_peek_token(lexer);

    if(token->kind == Token_Equal)
    {
      lexer_eat_token(lexer);
      eat_trivia(lexer);

      token = lexer_peek_token(lexer);

      // Flag: (1 << N)
      if(token->kind == Token_Open_Parentheses)
      {
        member->is_explicit_bit_flag = true;

        lexer_eat_token(lexer);
        eat_trivia(lexer);

        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Number && token->value.cstring[0] == '1');
        lexer_eat_token(lexer);
        eat_trivia(lexer);

        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Less_Less);
        lexer_eat_token(lexer);
        eat_trivia(lexer);

        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Number);

        s64 shift;
        b32 ok = s64_from_string(token->value, &shift);
        assert(ok);

        member->value = ((s64)1 << shift);

        lexer_eat_token(lexer);
        eat_trivia(lexer);

        token = lexer_peek_token(lexer);
        assert(token->kind == Token_Close_Parentheses);

        lexer_eat_token(lexer);
        eat_trivia(lexer);
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
          eat_trivia(lexer);

          token = lexer_peek_token(lexer);
          if(token->kind != Token_Or) break;

          lexer_eat_token(lexer);
          eat_trivia(lexer);

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
        eat_trivia(lexer);
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
      eat_trivia(lexer);
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
      eat_trivia(lexer);
    }

    token = lexer_peek_token(lexer);
    assert(token->kind == Token_Identifier || token->kind == Token_Close_Brace);
  }

  return members_count;
}

function void
parse_typedef(Lexer* lexer, Introspection* introspection, String file_path)
{
  Scratch scratch = scratch_begin(0,0);

  Typedef_Object* typedef_datatype = &(introspection->typedefs[introspection->typedefs_count++]);
  memory_zero_struct(typedef_datatype);

  u32 tokens_capacity = 32;
  u32 tokens_count = 0;
  Token* tokens = push_array(scratch.arena, Token, tokens_capacity);

  // Collect tokens until semicolon
  for (;;)
  {
    Token* token = lexer_peek_token(lexer);
    lexer_eat_token(lexer);
    eat_trivia(lexer);

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
    typedef_datatype->data_type  = parse_data_type(introspection->typedefs_arena, tokens, 0, name_index);
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

    typedef_datatype->data_type = parse_data_type(introspection->typedefs_arena, tokens, 0, base_type_end);

    String declaration_string = string_zero();
    for (u32 i = base_type_end; i < tokens_count; i++)
    {
      declaration_string = string_join(scratch.arena, declaration_string, tokens[i].value);
    }

    typedef_datatype->identifier = string_copy(introspection->typedefs_arena, declaration_string);
  }

  scratch_end(&scratch);
}

function void
parse_structs(Lexer* lexer, Introspection* introspection, String file_path)
{
  Token* token = lexer_peek_token(lexer);
  assert(string_equals(token->value, S("struct"), true));

  lexer_eat_token(lexer);
  eat_trivia(lexer);

  Token* identifier = lexer_peek_token(lexer);

  Struct_Object* struct_object = NULL;
  struct_object = find_struct(introspection, identifier->value);

  if (struct_object == NULL)
  {
    struct_object = &(introspection->structs[introspection->structs_count++]);
    memory_zero_struct(struct_object);

    struct_object->identifier = string_copy(introspection->structs_arena, identifier->value);
  }

  lexer_eat_token(lexer);
  eat_trivia(lexer);
  Token* next = lexer_peek_token(lexer);

  if (next->kind == Token_Identifier)
  {
    // Forwawrd declaration
    if (!string_equals(identifier->value, next->value, true))
    {
      // @TODO(fz): Error. In a struct fwd declaration, both names have to be the same.
      assert(0);
    }

    lexer_eat_token(lexer);
    eat_trivia(lexer);
    next = lexer_peek_token(lexer);

    assert(next->kind == Token_Semicolon);

    lexer_eat_token(lexer);
    eat_trivia(lexer);
    next = lexer_peek_token(lexer);

    if (next->kind == Token_Comment_Block || next->kind == Token_Comment_Line)
    {
      String comment = string_slice(introspection->structs_arena, next->value, 2, next->value.count - 2);
      comment = string_trim(introspection->structs_arena, comment);
      struct_object->documentation = string_copy(introspection->structs_arena, comment);
      lexer_eat_token(lexer);
    }
  }
  else if (next->kind == Token_Open_Brace)
  {
    // Definition
    lexer_eat_token(lexer); // @TODO(fz): Just to skip
  }
  else
  {
    // @TODO(fz): Error not handled
    assert(0);
  }
}

function void
parse_enum(Lexer* lexer, Introspection* introspection, String file_path)
{
  // These are anonymous enums. They don't have an identifier because they are likely type-casted elsewhere.
  Token* token = lexer_peek_token(lexer);
  assert(string_equals(token->value, S("enum"), true));

  Enum_Object* enum_object = &(introspection->enums[introspection->enums_count++]);
  memory_zero_struct(enum_object);

  enum_object->definition.file = string_copy(introspection->enums_arena, file_path);
  enum_object->definition.line = token->l0;

  lexer_eat_token(lexer);
  eat_trivia(lexer);

  token = lexer_peek_token(lexer);

  // Look for Metaprogramming tags
  if(token->kind == Token_Identifier)
  {
    if(string_equals(token->value, S("META_ENUM_LINK"), true))
    {
      lexer_eat_token(lexer);
      token = lexer_peek_token(lexer);
      assert(token->kind == Token_Open_Parentheses);

      lexer_eat_token(lexer);
      eat_trivia(lexer);
      token = lexer_peek_token(lexer);
      assert(token->kind == Token_Identifier);

      enum_object->identifier = string_copy(introspection->enums_arena, token->value);

      lexer_eat_token(lexer);
      eat_trivia(lexer);
      token = lexer_peek_token(lexer);
      assert(token->kind == Token_Comma);

      lexer_eat_token(lexer);
      eat_trivia(lexer);
      token = lexer_peek_token(lexer);
      assert(token->kind == Token_Identifier);

      enum_object->type = string_copy(introspection->enums_arena, token->value);

      lexer_eat_token(lexer);
      eat_trivia(lexer);
      token = lexer_peek_token(lexer);
      assert(token->kind == Token_Close_Parentheses);

      lexer_eat_token(lexer);
      eat_trivia(lexer);
      token = lexer_peek_token(lexer);
    }
    else
    {
      // This is not how we should set enums in the codebase, but anyway.
      // An identifier after enum that is not a metaprogram tag, it's probably the name of the enum.
      // E.g. enum Dummy_Enum { Dummy };
      enum_object->identifier = string_copy(introspection->enums_arena, token->value);
      lexer_eat_token(lexer);
      eat_trivia(lexer);
      token = lexer_peek_token(lexer);
    }
  }

  Enum_Member temp_members[MAX_ENUMS];
  u32 members_count = parse_enum_members(introspection->enums_arena, lexer, temp_members, MAX_ENUMS);

  token = lexer_peek_token(lexer);
  assert(token->kind == Token_Close_Brace);

  lexer_eat_token(lexer);
  eat_trivia(lexer);
  token = lexer_peek_token(lexer);
  
  if (token->kind == Token_Identifier)
  {
    enum_object->identifier = string_copy(introspection->enums_arena, token->value);
    lexer_eat_token(lexer);
    eat_trivia(lexer);
    token = lexer_peek_token(lexer);
  }

  assert(token->kind == Token_Semicolon);
  lexer_eat_token(lexer);

  enum_object->members_count = members_count;
  enum_object->members_capacity = members_count;
  enum_object->members = push_array(introspection->enums_arena, Enum_Member, members_count);

  for(u32 i = 0; i < members_count; ++i)
  {
    enum_object->members[i] = temp_members[i];
  }

  assert(enum_object->type.count != 0);

  // Add command to typedef the enum into an number-type
  Meta_Command* command = &introspection->commands[introspection->commands_count++];
  command->kind = Meta_Command_Forward_Declare_Enum;
  command->payload.enum_object = enum_object; 
  command->file = string_copy(introspection->commands_arena, file_path);
}

function void
parse_function(Lexer* lexer, Introspection* introspection, String file_path)
{
  Scratch scratch = scratch_begin(0,0);
  
  u32 tokens_max   = 32;
  u32 tokens_count = 0;
  Token* tokens    = push_array(scratch.arena, Token, tokens_max);

  s64 line = -1;

  b32 is_declaration = false;

  for (;;)
  {
    Token* token = lexer_peek_token(lexer);
    if (line == -1) line = token->l0;

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

  Data_Type return_type;
  memory_zero_struct(&return_type);

  String function_name = string_zero();
  Function_Object* function_object = NULL;

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

  function_object = find_function(introspection, function_name);

  if (function_object == NULL)
  {
    function_object = &(introspection->functions[introspection->functions_count++]);
    memory_zero_struct(function_object);
    function_object->identifier = string_copy(introspection->functions_arena, function_name);

    function_object->keywords_count = 0;
    function_object->keywords_capacity = MAX_FUNCTION_KEYWORDS;
    function_object->keywords = push_array(introspection->functions_arena, String, function_object->keywords_capacity);

    function_object->arguments_count = 0;
    function_object->arguments_capacity = MAX_FUNCTION_ARGUMENTS;
    function_object->arguments = push_array(introspection->functions_arena, Function_Argument, function_object->arguments_capacity);

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

    function_object->return_type = parse_data_type(introspection->functions_arena, tokens, (u32)return_type_start, name_index);

    assert(tokens[name_index + 1].kind == Token_Open_Parentheses);
    assert(tokens[tokens_count - 1].kind == Token_Close_Parentheses);

    for (u32 i = name_index + 2; i < tokens_count - 1; )
    {
      Function_Argument* argument = &function_object->arguments[function_object->arguments_count++];
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
      argument->data_type = parse_data_type(introspection->functions_arena, tokens, type_start, name_index_arg);
      argument->identifier = string_copy(introspection->functions_arena, tokens[name_index_arg].value);

      // Advance index past identifier
      i = name_index_arg + 1;

      // Optional array suffix (already reflected in Data_Type)
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
    eat_trivia(lexer);
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
eat_trivia(Lexer* lexer)
{
  for (;;)
  {
    Token* token = lexer_peek_token(lexer);
    if (!token_is_trivia(token)) break;
    lexer_eat_token(lexer);
  }
}

#endif // INTROSPECTION_H
#ifndef LEXER_H
#define LEXER_H

/*
  Example usage:

  Lexer* lexer;
  lexer_init_with_single_file_path(&lexer, PATH, Trivia_Line_Break|Trivia_Whitespace, Emit_String_Literals|Emit_Block_Comments);

  for (;;)
  {
    Token* token = lexer_peek_token(lexer);
    if (token->kind == Token_End_Of_File) break;
    printf("Token: (%d:%d) %s '"S_FMT"' \n", token->l0, token->c0, token_to_string(token->kind), S_ARG(value));
    lexer_eat_token(lexer);
  }
*/

#define MAX_LEXER_SCRATCH_BUFFER_SIZE 512
#define MAX_LOOKAHEAD_TOKENS 8

// @TODO(fz): Fix static asserts
// static_assert(is_power_of_two(MAX_CONCURRENT_TOKENS), "MAX_CONCURRENT_TOKENS Must be a power of two");

#define TOKEN_KIND                     \
  X(Token_End_Of_File)                 \
                                       \
  X(Token_Identifier)                  \
  X(Token_Number)                      \
                                       \
  X(Token_String_Literal)              \
  X(Token_Character_Literal)           \
                                       \
  X(Token_Open_Parentheses)            /* ( */ \
  X(Token_Close_Parentheses)           /* ) */ \
  X(Token_Open_Bracket)                /* [ */ \
  X(Token_Close_Bracket)               /* ] */ \
  X(Token_Open_Brace)                  /* { */ \
  X(Token_Close_Brace)                 /* } */ \
  X(Token_Semicolon)                   /* ; */ \
  X(Token_Comma)                       /* , */ \
  X(Token_Dot)                         /* . */ \
  X(Token_Colon)                       /* : */ \
  X(Token_Plus)                        /* + */ \
  X(Token_Minus)                       /* - */ \
  X(Token_Asterisk)                    /* * */ \
  X(Token_Slash)                       /* / */ \
  X(Token_Backslash)                   /* \ */ \
  X(Token_Percent)                     /* % */ \
  X(Token_Equal)                       /* = */ \
  X(Token_Less)                        /* < */ \
  X(Token_Greater)                     /* > */ \
  X(Token_Bang)                        /* ! */ \
  X(Token_And)                         /* & */ \
  X(Token_Or)                          /* | */ \
  X(Token_Caret)                       /* ^ */ \
  X(Token_Tilde)                       /* ~ */ \
  X(Token_Question)                    /* ? */ \
  X(Token_Hash)                        /* # */ \
  X(Token_Double_Quote)                /* " @NOTE(fz): Not used in Mimir. We parse string and char literals immediately. */ \
  X(Token_Single_Quote)                /* ' @NOTE(fz): Not used in Mimir. We parse string and char literals immediately. */ \
                                               \
  X(Token_Plus_Plus)                   /* ++ */  \
  X(Token_Plus_Equal)                  /* += */  \
  X(Token_Minus_Minus)                 /* -- */  \
  X(Token_Minus_Equal)                 /* -= */  \
  X(Token_Asterisk_Equal)              /* *= */  \
  X(Token_Slash_Equal)                 /* /= */  \
  X(Token_Percent_Equal)               /* %= */  \
  X(Token_Equal_Equal)                 /* == */  \
  X(Token_Bang_Equal)                  /* != */  \
  X(Token_Less_Less)                   /* << */  \
  X(Token_Less_Less_Equal)             /* <<= */ \
  X(Token_Greater_Greater)             /* >> */  \
  X(Token_Greater_Greater_Equal)       /* >>= */ \
  X(Token_And_And)                     /* && */  \
  X(Token_And_Equal)                   /* &= */  \
  X(Token_Or_Or)                       /* || */  \
  X(Token_Or_Equal)                    /* |= */  \
  X(Token_Caret_Equal)                 /* ^= */  \
                                       \
  X(Token_Comment_Line)                \
  X(Token_Comment_Block)               \
                                       \
  X(Token_Whitespace)                  \
  X(Token_Tab)                         \
  X(Token_Vertical_Tab)                \
  X(Token_Line_Break)                  \
  X(Token_Carriage_Return)             /* This is here for the sake of completion. By design, this lexer skips all \r found in the text. */ \
  X(Token_Form_Feed)

typedef enum
{
  Token_Error = 0,

#define X(name) name,
  TOKEN_KIND
#undef X
  
  Token_Count

} Token_Kind;

function const char* token_to_string(Token_Kind kind); /* For debug purposes, converts enum to a string */

typedef struct Token Token;
struct Token
{
  Token_Kind kind;

  s32 l0, c0; /* Starting line and column of token */
  s32 l1, c1; /* Ending line and column of token */

  String value; /* Value of token */
};

typedef u8 Trivia_Flags;
enum
{
  Trivia_None = 0,

  Trivia_Whitespace       = (1 << 0),
  Trivia_Carriage_Return  = (1 << 1), /* @NOTE(fz): Here for the sake of completion. But this lexer skips it. */
  Trivia_Tab              = (1 << 2),
  Trivia_Form_Feed        = (1 << 3),
  Trivia_Vertical_Tab     = (1 << 4),
  Trivia_Line_Break       = (1 << 5),

  Trivia_All = Trivia_Whitespace|Trivia_Carriage_Return|Trivia_Tab|Trivia_Form_Feed|Trivia_Vertical_Tab|Trivia_Line_Break,
};

typedef u8 Emit_Structures;
enum
{
  Emit_None = 0,

  Emit_String_Literals    = (1 << 0), /* Emits a Token_String_Literal like "This is a string literal", instead of individual tokens like "\"", "This", "is" ... */
  Emit_Character_Literals = (1 << 1), /* Emits a Token_Character_Literal like 'c', instead of 3 tokens like "'", "c", "'" */
  Emit_Line_Comments      = (1 << 2), /* Emits a line comment made made with '//' as a single token like: Token_Line_Comment "// This is a whole line comment" */
  Emit_Block_Comments     = (1 << 3), /* Emits a block comment made with made with /* */

  Emit_All = Emit_String_Literals|Emit_Character_Literals|Emit_Line_Comments|Emit_Block_Comments,
};

typedef struct Lexer Lexer;
struct Lexer
{
  Arena* arena;
  u8 scratch_buffer[MAX_LEXER_SCRATCH_BUFFER_SIZE]; /* Scratch buffer for storing identifier values and parsing tokens */

  // Character stream
  String source; /* Source text to do lexical analysis on */
  s32    source_cursor; /* Cursor for indexing source */
  s32 current_line_number;     /* Relative to the beginning of the current line */
  s32 current_character_index; /* Relative to the beginning of the current line */

  // Token stream
  Token incoming_tokens[MAX_LOOKAHEAD_TOKENS]; /* Circular buffer of incoming (not yet eaten) tokens */
  s32   incoming_tokens_head;   /* index of next token to consume */
  s32   incoming_tokens_count;  /* number of valid tokens */

  // Settings
  Trivia_Flags trivia_flags; /* What trivia tokens should be lexed. E.g. whitespace, line breaks... */
  Emit_Structures emit_structures; /* Predefined structures that this lexer can emit. E.g. String literals, block comments ... */
};

function s32    lexer_token_index(Lexer* lexer, s32 lookahead); /* Ensure we get a valid index into the circular buffer */
function Token* lexer_reserve_token_slot(Lexer* lexer); /* Add a new incoming token to buffer */
function void   lexer_dump_tokens(Lexer* lexer, String path, Trivia_Flags trivia_tokens, Emit_Structures emit_structures);

function b32    lexer_init_with_single_file_path(Lexer* lexer, String path, Trivia_Flags trivia_flags, Emit_Structures emit_structures); /* Attaches a file to the lexer for it to parse */
function void   lexer_set_emit_structures(Lexer* lexer, b32 string_literals, b32 character_literals, b32 line_comments, b32 block_comments); /* Configs lexer to emit some pre defined commonly used tokens */
function Token* lexer_make_new_token(Lexer* lexer); /* Returns a new token */
function Token* lexer_peek_token(Lexer* lexer); /* Creates and puts a new token into incoming tokens */
function s16    lexer_peek_character(Lexer* lexer); /* Returns the next character without advancing the lexer */
function s16    lexer_peek_nth_character(Lexer* lexer, u32 nth); /* Returns the nth character without advancing the lexer. 0 is current character, 1 is next character, etc... */
function void   lexer_rewind_token(Lexer* lexer, u32 count); /* Used to undo a call to lexer_eat_token */
function void   lexer_eat_character(Lexer* lexer); /* Advances the lexer by 1 character */
function void   lexer_eat_token(Lexer* lexer); /* Advances lexer by 1 token */
function void   lexer_eat_spaces(Lexer* lexer); /* Advances over all spaces */

// @Section: Parsing helpers
function void lexer_parse_single_character_token(Lexer* lexer, Token* token, Token_Kind kind); /* Parses a token that is a single character long */
function void lexer_parse_identifier(Lexer* lexer, Token* token); /* Parses the next token as an identifier */
function void lexer_parse_number(Lexer* lexer, Token* token); /* Parses the next token as a number */
function void lexer_parse_preprocessor_directive(Lexer* lexer, Token* token); /* Parses the next token as a preprocessor directive */
function void lexer_parse_string_literal(Lexer* lexer, Token* token); /* Parses a string literal that is in between double quotes */
function void lexer_parse_character_literal(Lexer* lexer, Token* token); /* Parses a character literal that is in between single ticks */
function void lexer_parse_trivia(Lexer* lexer, Token* token);

// @Section: Token_helpers
function b32  token_is_space(Token* token);

// @Section: Implementation

function const char*
token_to_string(Token_Kind kind)
{
  switch (kind)
  {
    #define X(name) case name: return #name;
    TOKEN_KIND
    #undef X
    default: return "Unknown Token_Kind";
  }
}

function s32
lexer_token_index(Lexer* lexer, s32 lookahead)
{
  assert(lookahead >= 0);
  assert(lookahead < MAX_LOOKAHEAD_TOKENS);

  s32 result = (lexer->incoming_tokens_head + lookahead) & (MAX_LOOKAHEAD_TOKENS - 1);
  return result;
}

function Token_Kind
lexer_classify_trivia(u8 c)
{
  switch (c)
  {
    case ' ':  return Token_Whitespace;
    case '\r': return Token_Carriage_Return;
    case '\t': return Token_Tab;
    case '\f': return Token_Form_Feed;
    case '\v': return Token_Vertical_Tab;
    case '\n': return Token_Line_Break;
  }
  return Token_Error;
}

function b32
lexer_should_emit_trivia(Lexer* lexer, Token_Kind kind)
{
  switch (kind)
  {
    case Token_Whitespace:      return lexer->trivia_flags & Trivia_Whitespace;
    case Token_Carriage_Return: return lexer->trivia_flags & Trivia_Carriage_Return;
    case Token_Tab:             return lexer->trivia_flags & Trivia_Tab;
    case Token_Form_Feed:       return lexer->trivia_flags & Trivia_Form_Feed;
    case Token_Vertical_Tab:    return lexer->trivia_flags & Trivia_Vertical_Tab;
    case Token_Line_Break:      return lexer->trivia_flags & Trivia_Line_Break;
    default: return true;
  }
}

function Token*
lexer_reserve_token_slot(Lexer* lexer)
{
  assert(lexer->incoming_tokens_count < MAX_LOOKAHEAD_TOKENS);

  s32 index = lexer_token_index(lexer, lexer->incoming_tokens_count);
  Token* result = &lexer->incoming_tokens[index];

  result->l0 = lexer->current_line_number;
  result->c0 = lexer->current_character_index;
  result->l1 = -1;
  result->c1 = -1;
  result->kind  = Token_Error;
  result->value = S("");

  return result;
}

function void
lexer_dump_tokens(Lexer* lexer, String path, Trivia_Flags trivia_tokens, Emit_Structures emit_structures)
{
  lexer_init_with_single_file_path(lexer, path, trivia_tokens, emit_structures);

  for (;;)
  {
    Token* token = lexer_peek_token(lexer);
    if (token->kind == Token_End_Of_File) break;

    String value = token->value;
    switch (token->kind)
    {
      case Token_Carriage_Return: value = S("\\r"); break;
      case Token_Tab:             value = S("\\t"); break;
      case Token_Form_Feed:       value = S("\\f"); break;
      case Token_Vertical_Tab:    value = S("\\v"); break;
      case Token_Line_Break:      value = S("\\n"); break;
   }

    printf("Token: (%d:%d) %s '"S_FMT"' \n", token->l0, token->c0, token_to_string(token->kind), S_ARG(value));
    lexer_eat_token(lexer);
  }
}

function b32
lexer_init_with_single_file_path(Lexer* lexer, String path, Trivia_Flags trivia_flags, Emit_Structures emit_structures)
{
  if (path.count == 0) return false;
  memory_zero_struct(lexer);

  lexer->arena  = arena_alloc();
  lexer->source = file_load(lexer->arena, path);
  lexer->current_line_number     = 1;
  lexer->current_character_index = 1;
  lexer->trivia_flags            = trivia_flags;
  lexer->emit_structures         = emit_structures;

  if (lexer->source.count == 0)
  {
    arena_free(lexer->arena);
    return false;
  }

  return true;
}

function s16
lexer_peek_nth_character(Lexer* lexer, u32 nth)
{
  s16 result = -1;
  if (lexer->source_cursor < (lexer->source.count + nth))
  {
    result = (s16)lexer->source.cstring[lexer->source_cursor + nth];
  }
  return result;
}

function s16
lexer_peek_character(Lexer* lexer)
{
  s16 result = -1;
  for (;;)
  {
    if (lexer->source_cursor >= lexer->source.count)
    {
      return result;
    }
    if (lexer->source_cursor < (lexer->source.count))
    {
      result = (s16)lexer->source.cstring[lexer->source_cursor];
      if ((u8)result != '\r')
      {
        break;
      }
      else
      {
        lexer_eat_character(lexer);
      }
    }
  }
  return result;
}

function Token*
lexer_peek_token(Lexer* lexer)
{
  if (lexer->incoming_tokens_count == 0)
  {
    Token* token = lexer_make_new_token(lexer);
    lexer->incoming_tokens_count += 1;
    return token;
  }

  return &lexer->incoming_tokens[lexer->incoming_tokens_head];
}

function void
lexer_eat_character(Lexer* lexer)
{
  u8 c = lexer->source.cstring[lexer->source_cursor];
  lexer->source_cursor += 1;

  if (c == '\n')
  {
    lexer->current_line_number    += 1;
    lexer->current_character_index = 1;
  }
  else
  {
    lexer->current_character_index += 1;
  }
}

function void
lexer_eat_token(Lexer* lexer)
{
  assert(lexer->incoming_tokens_count > 0);

  lexer->incoming_tokens_head   = (lexer->incoming_tokens_head + 1) & (MAX_LOOKAHEAD_TOKENS - 1);
  lexer->incoming_tokens_count -= 1;
}

function void
lexer_parse_single_character_token(Lexer* lexer, Token* token, Token_Kind kind)
{
  s16 c_s16 = lexer_peek_character(lexer);
  if (c_s16 == -1)
  {
    token->kind = Token_End_Of_File;
    token->l1 = lexer->current_line_number;
    token->c1 = lexer->current_character_index;
    return;
  }

  u8 c = (u8)c_s16;
  lexer_eat_character(lexer);
  lexer->scratch_buffer[0] = c;
  lexer->scratch_buffer[1] = '\0';

  token->value = string_copy(lexer->arena, (String){ 1, lexer->scratch_buffer});
  token->kind  = kind;
  token->l1    = lexer->current_line_number;
  token->c1    = lexer->current_character_index;
}

function void
lexer_parse_identifier(Lexer* lexer, Token* token)
{
  u32 scratch_position = 0;

  for (;;)
  {
    s16 c_s16 = lexer_peek_character(lexer);
    if (c_s16 == -1) break;

    u8 c = (u8)c_s16;

    if (!(char8_is_alpha(c) || char8_is_digit(c) || c == '_')) break;
    if (scratch_position + 1 < MAX_LEXER_SCRATCH_BUFFER_SIZE)
    {
      lexer->scratch_buffer[scratch_position] = c;
      scratch_position += 1;
    }

    lexer_eat_character(lexer);
  }

  lexer->scratch_buffer[scratch_position] = '\0';
  token->value = string_copy(lexer->arena, (String){scratch_position, lexer->scratch_buffer});
  token->kind  = Token_Identifier;
  token->l1 = lexer->current_line_number;
  token->c1 = lexer->current_character_index;
}

function void
lexer_parse_number(Lexer* lexer, Token* token)
{
  // @TODO(fz):
  // Handle Hex: 0xFF, Oct: 0123, Exp: 1e10, Suffixed: 10U, 3.14f

  u32 scratch_position = 0;

  for (;;)
  {
    s16 c_s16 = lexer_peek_character(lexer);
    if (c_s16 == -1) break;

    u8 c = (u8)c_s16;

    if (!(char8_is_digit(c) || c == '\'' || c == '.')) break;

    if (scratch_position + 1 < MAX_LEXER_SCRATCH_BUFFER_SIZE)
    {
      lexer->scratch_buffer[scratch_position] = c;
      scratch_position += 1;
    }

    lexer_eat_character(lexer);
  }

  lexer->scratch_buffer[scratch_position] = '\0';
  token->value = string_copy(lexer->arena, (String){scratch_position, lexer->scratch_buffer});
  token->kind  = Token_Number;
  token->l1    = lexer->current_line_number;
  token->c1    = lexer->current_character_index;
}

function void
lexer_eat_spaces(Lexer* lexer)
{
  for (;;)
  {
    s16 c_s16 = lexer_peek_character(lexer);
    if (c_s16 == -1) break;
    u8 c = (u8)c_s16;
    if (!char8_is_space(c)) break;
    lexer_eat_character(lexer); // fallback if you don't want tokens
  }
}

function void
lexer_parse_trivia(Lexer* lexer, Token* token)
{
  s16 c_s16 = lexer_peek_character(lexer);
  if (c_s16 == -1)
  {
    token->kind = Token_End_Of_File;
    token->l1 = lexer->current_line_number;
    token->c1 = lexer->current_character_index;
    return;
  }

  u8 c = (u8)c_s16;
  Token_Kind kind = lexer_classify_trivia(c);
  lexer_parse_single_character_token(lexer, token, kind);
}

function void
lexer_parse_string_literal(Lexer* lexer, Token* token)
{
  // @TODO(fz): We have to change this to use a dynamic buffer, since it's reasonable to think strings could be larget than MAX_LEXER_SCRATCH_BUFFER_SIZE

  u32 scratch_position = 0;
  lexer_eat_character(lexer);
  lexer->scratch_buffer[scratch_position++] = '"';

  for (;;)
  {
    s16 c_s16 = lexer_peek_character(lexer);
    if (c_s16 == -1) break;
    u8 c = (u8)c_s16;

    lexer_eat_character(lexer);

    if (scratch_position + 1 < MAX_LEXER_SCRATCH_BUFFER_SIZE) lexer->scratch_buffer[scratch_position++] = c;
    if (c == '"') break;

    if (c == '\\')
    {
      s16 esc = lexer_peek_character(lexer);
      if (esc != -1)
      {
        lexer_eat_character(lexer);
        if (scratch_position + 1 < MAX_LEXER_SCRATCH_BUFFER_SIZE) lexer->scratch_buffer[scratch_position++] = (u8)esc;
      }
    }
  }

  lexer->scratch_buffer[scratch_position] = '\0';
  token->value = string_copy(lexer->arena, (String){scratch_position, lexer->scratch_buffer});
  token->kind  = Token_String_Literal;
  token->l1    = lexer->current_line_number;
  token->c1    = lexer->current_character_index;
}

function void
lexer_parse_character_literal(Lexer* lexer, Token* token)
{
  u32 scratch_position = 0;
  lexer_eat_character(lexer);
  lexer->scratch_buffer[scratch_position++] = '\'';

  s16 c_s16 = lexer_peek_character(lexer);
  if (c_s16 == -1) return;
  u8 char_val = (u8)c_s16;

  lexer_eat_character(lexer);

  if (scratch_position + 1 < MAX_LEXER_SCRATCH_BUFFER_SIZE) lexer->scratch_buffer[scratch_position++] = char_val;

  if (char_val == '\\')
  {
    s16 esc = lexer_peek_character(lexer);
    if (esc != -1)
    {
      lexer_eat_character(lexer);
      if (scratch_position + 1 < MAX_LEXER_SCRATCH_BUFFER_SIZE) lexer->scratch_buffer[scratch_position++] = (u8)esc;
    }
  }

  s16 closing = lexer_peek_character(lexer);
  if (closing == '\'')
  {
    lexer_eat_character(lexer);
    if (scratch_position + 1 < MAX_LEXER_SCRATCH_BUFFER_SIZE) lexer->scratch_buffer[scratch_position++] = '\'';
  }

  lexer->scratch_buffer[scratch_position] = '\0';
  token->value = string_copy(lexer->arena, (String){scratch_position, lexer->scratch_buffer});
  token->kind  = Token_Character_Literal;
  token->l1    = lexer->current_line_number;
  token->c1    = lexer->current_character_index;
}

function void
lexer_parse_line_comment(Lexer* lexer, Token* token)
{
  lexer_eat_character(lexer);
  lexer_eat_character(lexer);

  u32 scratch_position = 0;
  lexer->scratch_buffer[scratch_position++] = '/';
  lexer->scratch_buffer[scratch_position++] = '/';

  for (;;)
  {
    s16 c_s16 = lexer_peek_character(lexer);
    if (c_s16 == -1 || c_s16 == '\n') break;
    if (scratch_position + 1 < MAX_LEXER_SCRATCH_BUFFER_SIZE) lexer->scratch_buffer[scratch_position++] = (u8)c_s16;
    lexer_eat_character(lexer);
  }

  lexer->scratch_buffer[scratch_position] = '\0';
  token->value = string_copy(lexer->arena, (String){scratch_position, lexer->scratch_buffer});
  token->kind  = Token_Comment_Line;
  token->l1    = lexer->current_line_number;
  token->c1    = lexer->current_character_index;
}

function Token*
lexer_make_token_from_next_n_characters(Lexer* lexer, Token* token, Token_Kind kind, u32 count)
{
  u32 scratch_position = 0;
  for (u32 i = 0; i < count; i += 1)
  {
    s16 c = lexer_peek_character(lexer);
    if (c == -1) break;
    if (scratch_position + 1 < MAX_LEXER_SCRATCH_BUFFER_SIZE) lexer->scratch_buffer[scratch_position++] = (u8)c;
    lexer_eat_character(lexer);
  }

  lexer->scratch_buffer[scratch_position] = '\0';
  token->value = string_copy(lexer->arena, (String){scratch_position, lexer->scratch_buffer});
  token->kind  = kind;
  token->l1    = lexer->current_line_number;
  token->c1    = lexer->current_character_index;

  return token;
}

function void
lexer_parse_block_comment(Lexer* lexer, Token* token)
{
  lexer_eat_character(lexer);
  lexer_eat_character(lexer);

  String_Buffer buffer;
  defer_loop(string_buffer_init(&buffer, &MallocAllocator, 512), string_buffer_free(&buffer)) // @TODO(fz): Replace Malloc allocator with scratch arena
  {
    string_buffer_push(&buffer, "/*"); // '/*' have already been eaten
    for (;;)
    {
      s16 c_s16 = lexer_peek_character(lexer);
      if (c_s16 == -1) break;
      u8 c = (u8)c_s16;

      string_buffer_push(&buffer, "%c", c);
      lexer_eat_character(lexer);

      s16 next_c = lexer_peek_character(lexer);

      if (c == '*' && next_c == '/')
      {
        lexer_eat_character(lexer); // eat next_c
        string_buffer_push(&buffer, "/");
        break;
      }
    }

    token->value = string_copy(lexer->arena, (String){buffer.count, buffer.data});
    token->kind  = Token_Comment_Block;
    token->l1    = lexer->current_line_number;
    token->c1    = lexer->current_character_index;
  }
}

function Token*
lexer_make_new_token(Lexer* lexer)
{
  for (;;)
  {
    Token* token = lexer_reserve_token_slot(lexer);

    s16 c_s16 = lexer_peek_character(lexer);
    if (c_s16 == -1)
    {
      token->kind = Token_End_Of_File;
      token->l1   = lexer->current_line_number;
      token->c1   = lexer->current_character_index;
      return token;
    }

    u8 c = (u8)c_s16;

    if (char8_is_space(c))
    {
      lexer_parse_trivia(lexer, token);

      if (lexer_should_emit_trivia(lexer, token->kind))
      {
        return token;
      }

      // Discard token slot
      continue;
    }

    if (char8_is_alpha(c) || c == '_') // Is this character a valid identifier starting point?
    {
      lexer_parse_identifier(lexer, token);
    }
    else if (char8_is_digit(c))
    {
      lexer_parse_number(lexer, token);
    }
    else if (lexer->trivia_flags > 0 && char8_is_space(c))
    {
      lexer_parse_trivia(lexer, token);
    }
    else switch (c)
    {
      case '#': { lexer_parse_single_character_token(lexer, token, Token_Hash); }              break;
      case '(': { lexer_parse_single_character_token(lexer, token, Token_Open_Parentheses); }  break;
      case ')': { lexer_parse_single_character_token(lexer, token, Token_Close_Parentheses); } break;
      case '[': { lexer_parse_single_character_token(lexer, token, Token_Open_Bracket); }      break;
      case ']': { lexer_parse_single_character_token(lexer, token, Token_Close_Bracket); }     break;
      case '{': { lexer_parse_single_character_token(lexer, token, Token_Open_Brace); }        break;
      case '}': { lexer_parse_single_character_token(lexer, token, Token_Close_Brace); }       break;
      case ';': { lexer_parse_single_character_token(lexer, token, Token_Semicolon); }         break;
      case ',': { lexer_parse_single_character_token(lexer, token, Token_Comma); }             break;
      case '.': { lexer_parse_single_character_token(lexer, token, Token_Dot); }               break;
      case ':': { lexer_parse_single_character_token(lexer, token, Token_Colon); }             break;
      case '\\':{ lexer_parse_single_character_token(lexer, token, Token_Backslash); }         break;
      case '~': { lexer_parse_single_character_token(lexer, token, Token_Tilde); }             break;
      case '?': { lexer_parse_single_character_token(lexer, token, Token_Question); }          break;

      case '\'':
      { 
        if (has_flags(lexer->emit_structures, Emit_Character_Literals))
        {
          lexer_parse_character_literal(lexer, token);
        }
        else
        {
          lexer_parse_single_character_token(lexer, token, Token_Single_Quote);
        }
      }
      break;

      case '"':
      {
        if (has_flags(lexer->emit_structures, Emit_String_Literals))
        {
          lexer_parse_string_literal(lexer, token);
        }
        else
        {
          lexer_parse_single_character_token(lexer, token, Token_Double_Quote);
        }
      }
      break;

      case '+':
      {
        s16 n = lexer_peek_nth_character(lexer, 1);
        if (n == '+')      { lexer_make_token_from_next_n_characters(lexer, token, Token_Plus_Plus, 2); }
        else if (n == '=') { lexer_make_token_from_next_n_characters(lexer, token, Token_Plus_Equal, 2); }
        else               { lexer_parse_single_character_token(lexer, token, Token_Plus); }
      }
      break;

      case '-':
      {
        s16 n = lexer_peek_nth_character(lexer, 1);
        if (n == '-')      { lexer_make_token_from_next_n_characters(lexer, token, Token_Minus_Minus, 2); }
        else if (n == '=') { lexer_make_token_from_next_n_characters(lexer, token, Token_Minus_Equal, 2); }
        else               { lexer_parse_single_character_token(lexer, token, Token_Minus); }
      }
      break;

      case '*':
      {
        s16 n = lexer_peek_nth_character(lexer, 1);
        if (n == '=') { lexer_make_token_from_next_n_characters(lexer, token, Token_Asterisk_Equal, 2); }
        else          { lexer_parse_single_character_token(lexer, token, Token_Asterisk); }
      }
      break;

      case '/':
      {
        s16 n = lexer_peek_nth_character(lexer, 1);
        if (n == '=')
        {
          lexer_make_token_from_next_n_characters(lexer, token, Token_Slash_Equal, 2);
        }
        else if (has_flags(lexer->emit_structures, Emit_Line_Comments) && (n == '/' || n == '*'))
        {
          if      (n == '/') { lexer_parse_line_comment(lexer, token); }
          else if (n == '*') { lexer_parse_block_comment(lexer, token); }
        }
        else
        {
          lexer_parse_single_character_token(lexer, token, Token_Slash);
        }
      }
      break;

      case '%':
      {
        s16 n = lexer_peek_nth_character(lexer, 1);
        if (n == '=') { lexer_make_token_from_next_n_characters(lexer, token, Token_Percent_Equal, 2); }
        else          { lexer_parse_single_character_token(lexer, token, Token_Percent); }
      }
      break;

      case '=':
      {
        s16 n = lexer_peek_nth_character(lexer, 1);
        if (n == '=') { lexer_make_token_from_next_n_characters(lexer, token, Token_Equal_Equal, 2); }
        else          { lexer_parse_single_character_token(lexer, token, Token_Equal); }
      }
      break;

      case '!':
      {
        s16 n = lexer_peek_nth_character(lexer, 1);
        if (n == '=') { lexer_make_token_from_next_n_characters(lexer, token, Token_Bang_Equal, 2); }
        else          { lexer_parse_single_character_token(lexer, token, Token_Bang); }
      }
      break;

      case '<':
      {
        s16 n = lexer_peek_nth_character(lexer, 1);
        if (n == '<')
        {
          s16 n2 = lexer_peek_nth_character(lexer, 2);
          if (n2 == '=') { lexer_make_token_from_next_n_characters(lexer, token, Token_Less_Less_Equal, 3); }
          else           { lexer_make_token_from_next_n_characters(lexer, token, Token_Less_Less, 2); }
        }
        else 
        {
          lexer_parse_single_character_token(lexer, token, Token_Less);
        }
      break;
      }

      case '>':
      {
        s16 n = lexer_peek_nth_character(lexer, 1);
        if (n == '<')
        {
          s16 n2 = lexer_peek_nth_character(lexer, 2);
          if (n2 == '=') { lexer_make_token_from_next_n_characters(lexer, token, Token_Greater_Greater_Equal, 3); }
          else           { lexer_make_token_from_next_n_characters(lexer, token, Token_Greater_Greater, 2); }
        }
        else 
        {
          lexer_parse_single_character_token(lexer, token, Token_Greater);
        }
      }
      break;

      case '&':
      {
        s16 n = lexer_peek_nth_character(lexer, 1);
        if (n == '&')      { lexer_make_token_from_next_n_characters(lexer, token, Token_And_And, 2); }
        else if (n == '=') { lexer_make_token_from_next_n_characters(lexer, token, Token_And_Equal, 2); }
        else               { lexer_parse_single_character_token(lexer, token, Token_And); } 
      }
      break;

      case '|':
      {
        s16 n = lexer_peek_nth_character(lexer, 1);
        if (n == '|')      { lexer_make_token_from_next_n_characters(lexer, token, Token_Or_Or, 2); }
        else if (n == '=') { lexer_make_token_from_next_n_characters(lexer, token, Token_Or_Equal, 2); }
        else               { lexer_parse_single_character_token(lexer, token, Token_Or); } 
      }
      break;

      case '^':
      {
        s16 n = lexer_peek_nth_character(lexer, 1);
        if (n == '=') { lexer_make_token_from_next_n_characters(lexer, token, Token_Caret_Equal, 2); }
        else          { lexer_parse_single_character_token(lexer, token, Token_Caret); }
      }
      break;

      default:
      {
        token->kind = Token_Error;
        lexer_eat_character(lexer);
        token->l1 = lexer->current_line_number;
        token->c1 = lexer->current_character_index;
      }
    }

    return token;

  }
}

function void
lexer_rewind_token(Lexer* lexer, u32 count)
{
  assert(count <= MAX_LOOKAHEAD_TOKENS); // Cannot rewind more tokens than the buffer can  hold
  assert(count <= (u32)(MAX_LOOKAHEAD_TOKENS - lexer->incoming_tokens_count)); // Cannot rewind beyond what has already been eaten

  for (u32 i = 0; i < count; ++i)
  {
    lexer->incoming_tokens_head -= 1;
    if (lexer->incoming_tokens_head < 0)
    {
      lexer->incoming_tokens_head = MAX_LOOKAHEAD_TOKENS - 1;
    }
    lexer->incoming_tokens_count += 1;
  }
}

function b32
token_is_space(Token* token)
{
  b32 result = false;
  switch (token->kind)
  {
    case Token_Whitespace:
    case Token_Tab:
    case Token_Vertical_Tab:
    case Token_Line_Break:
    case Token_Carriage_Return:
    case Token_Form_Feed:
    {
      result = true;
    }
    break;
  }
  return result;
}

#endif // LEXER_H

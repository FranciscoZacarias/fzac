#ifndef LEXER_H
#define LEXER_H

#include "../fzac.h"

const char* token_kind_string[] = {
  "Token_Unknown",
  "Token_EOF",
  "Token_Space",
  "Token_Tab",
  "Token_Newline",
  "Token_Carriage_Return",
  "Token_Null",
  "Token_Bell",
  "Token_Backspace",
  "Token_Vertical_Tab",
  "Token_Form_Feed",
  "Token_Escape",
  "Token_Delete",
  "Token_String_Identifier",
  "Token_Number",
  "Token_Exclamation",
  "Token_Quote",
  "Token_Apostrophe",
  "Token_Hash",
  "Token_Dollar",
  "Token_Percent",
  "Token_Ampersand",
  "Token_Paren_Open",
  "Token_Paren_Close",
  "Token_Asterisk",
  "Token_Plus",
  "Token_Comma",
  "Token_Minus",
  "Token_Dot",
  "Token_Slash",
  "Token_Colon",
  "Token_Semicolon",
  "Token_Less",
  "Token_Equals",
  "Token_Greater",
  "Token_Question",
  "Token_At",
  "Token_Bracket_Open",
  "Token_Bracket_Close",
  "Token_Backslash",
  "Token_Caret",
  "Token_Underscore",
  "Token_Backtick",
  "Token_Brace_Open",
  "Token_Brace_Close",
  "Token_Pipe",
  "Token_Tilde",
  "Token_Count",
};

typedef u64 Token_Kind;
enum
{
  Token_Unknown = 0,
  Token_EOF,

  Token_Space,               /* ' ' */
  Token_Tab,                 /* '\t' */
  Token_Newline,             /* '\n' */
  Token_Carriage_Return,     /* '\r' */
  Token_Null,                /* '\0' */
  Token_Bell,                /* '\a' */
  Token_Backspace,           /* '\b' */
  Token_Vertical_Tab,        /* '\v' */
  Token_Form_Feed,           /* '\f' */
  Token_Escape,              /* '\x1B' */
  Token_Delete,              /* 0x7F */

  Token_String_Identifier,   /* sequence of [A-Za-z0-9_] */
  Token_Number,              /* sequence of digits */

  Token_Exclamation,         /* ! */
  Token_Quote,               /* " */
  Token_Apostrophe,          /* ' */
  Token_Hash,                /* # */
  Token_Dollar,              /* $ */
  Token_Percent,             /* % */
  Token_Ampersand,           /* & */
  Token_Paren_Open,          /* ( */
  Token_Paren_Close,         /* ) */
  Token_Asterisk,            /* * */
  Token_Plus,                /* + */
  Token_Comma,               /* , */
  Token_Minus,               /* - */
  Token_Dot,                 /* . */
  Token_Slash,               /* / */
  Token_Colon,               /* : */
  Token_Semicolon,           /* ; */
  Token_Less,                /* < */
  Token_Equals,              /* = */
  Token_Greater,             /* > */
  Token_Question,            /* ? */
  Token_At,                  /* @ */
  Token_Bracket_Open,        /* [ */
  Token_Bracket_Close,       /* ] */
  Token_Backslash,           /* \ */
  Token_Caret,               /* ^ */
  Token_Underscore,          /* _ */
  Token_Backtick,            /* ` */
  Token_Brace_Open,          /* { */
  Token_Brace_Close,         /* } */
  Token_Pipe,                /* | */
  Token_Tilde,               /* ~ */

  Token_Count,
};

typedef struct Token Token;
struct Token
{
  Token_Kind kind;
  String value;
  u32 start_offset;
  u32 end_offset;
  u32 line;
  u32 column;
};

typedef struct Token_Array Token_Array;
struct Token_Array
{
  Token* tokens;
  u64 count;
};

#endif // LEXER_H
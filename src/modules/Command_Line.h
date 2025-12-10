#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

// The acceptable commands are:
// --command "this is my value" -no-string
// key: "command", value: "this is my value", is-flag: false
// -no-string
// key: "no-string", value: "no-string", is-flag: true

#define MAX_COMMAND_LINE_ARGS 16
#define TEMP_BUFFER_SIZE 2048
#define PARSED_BUFFER_SIZE 4096

typedef struct Command_Line_Arg Command_Line_Arg;
typedef struct Command_Line Command_Line;

struct Command_Line_Arg {
  b32     is_flag;
  String key;
  String value;
};

struct Command_Line 
{
  String executable;
  String raw_args;
  Command_Line_Arg args[MAX_COMMAND_LINE_ARGS];
  u32              args_count;
};

function Command_Line_Arg command_line_arg_new(String key, String value, b32 is_flag);
function Command_Line     command_line_parse(String input); /* Parses a command line type from a String */
function Command_Line     command_line_parse_from_argc_argv(s32 argc, u8** argv); /* Parses a Command line from a c style argc/argv argument */
function String          _command_line_parse_token(u8** cursor);
function void            _command_line_skip_whitespace(u8** cursor);
function String          _command_line_strip_quotes(String in);
function String          _command_line_strip_leading_dashes(String in);

function Command_Line_Arg
command_line_arg_new(String key, String value, b32 is_flag)
{
  Command_Line_Arg result = (Command_Line_Arg)
  {
    .is_flag = is_flag,
    .key     = key,
    .value   = value  
  };
  return result;
}

function void
_command_line_skip_whitespace(u8** cursor)
{
  while (char8_is_space(**cursor)) (*cursor)++;
}

function String
_command_line_strip_quotes(String in)
{
  if (in.size >= 2 && in.cstring[0] == '"' && in.cstring[in.size - 1] == '"')
  {
    return string_new(in.size - 2,  in.cstring + 1);
  }
  return in;
}

function String
_command_line_strip_leading_dashes(String in)
{
  u64 offset = 0;
  while (offset < in.size && in.cstring[offset] == '-')
  {
    offset++;
  }
  return string_new(in.size - offset, in.cstring + offset);
}

function String
_command_line_parse_token(u8** cursor)
{
  _command_line_skip_whitespace(cursor);
  if (**cursor == 0)
  {
    return (String){0};
  }

  u8* start = *cursor;
  u8* end = start;

  if (*start == '"')
  {
    start++; // skip opening quote
    end = start;
    while (*end && *end != '"') end++;
    *cursor = (*end == '"') ? end + 1 : end;
  }
  else
  {
    while (*end && !char8_is_space(*end)) end++;
    *cursor = end;
  }

  String result = string_new((u64)(end - start), start);
  return result;
}

function Command_Line
command_line_parse_from_argc_argv(s32 argc, u8** argv)
{
  Command_Line result = {0};
  Scratch scratch = scratch_begin(0,0);
  if (argc > 0)
  {
    String first_arg = string_new(cstring_length(argv[0]), argv[0]);
    String_List arg_list = string_list_new(scratch.arena, first_arg);
    for (s32 idx = 1; idx < argc; idx += 1)
    {
      String arg = string_new(cstring_length(argv[idx]), argv[idx]);
      string_list_push(scratch.arena, &arg_list, arg);
    }
    String args = string_list_join(scratch.arena, &arg_list);
    result = command_line_parse(args);
  }
  scratch_end(&scratch);
  return result;
}

function Command_Line
command_line_parse(String input)
{
  Command_Line result = {0};

  static u8 exe_buffer[MAX_PATH];
  DWORD exe_len = GetModuleFileNameA(0, exe_buffer, MAX_PATH);
  result.executable = (String){ exe_len, exe_buffer };

  // Copy input into stable memory
  static u8 temp_buffer[TEMP_BUFFER_SIZE];
  static u8 parsed_buffer[PARSED_BUFFER_SIZE];
  u64 parsed_cursor = 0;

  u64 len = input.size;
  if (len >= sizeof(temp_buffer)) len = sizeof(temp_buffer) - 1;
  MemoryCopy(temp_buffer, input.cstring, len);
  temp_buffer[len] = 0;

  result.raw_args = (String){ len, temp_buffer };

  u8* cursor = temp_buffer;

  while (*cursor && result.args_count < MAX_COMMAND_LINE_ARGS)
  {
    _command_line_skip_whitespace(&cursor);
    if (*cursor == 0) break;

    String token = _command_line_parse_token(&cursor);
    if (token.size == 0) break;

    if (token.cstring[0] == '-')
    {
      String key = _command_line_strip_leading_dashes(token);

      // Copy key to stable memory
      String key_copy = {
        .size = key.size,
        .cstring = parsed_buffer + parsed_cursor
      };
      MemoryCopy(key_copy.cstring, key.cstring, key.size);
      parsed_cursor += key.size;

      // Peek for value
      _command_line_skip_whitespace(&cursor);
      if (*cursor == 0 || *cursor == '-')
      {
        // Flag
        result.args[result.args_count++] = command_line_arg_new(key_copy, key_copy, true);
      }
      else
      {
        String val = _command_line_parse_token(&cursor);
        val = _command_line_strip_quotes(val);

        // Copy value to stable memory
        String val_copy = {
          .size = val.size,
          .cstring = parsed_buffer + parsed_cursor
        };
        MemoryCopy(val_copy.cstring, val.cstring, val.size);
        parsed_cursor += val.size;

        result.args[result.args_count++] = command_line_arg_new(key_copy, val_copy, false);
      }
    }
  }

  return result;
}

#endif // COMMAND_LINE_H
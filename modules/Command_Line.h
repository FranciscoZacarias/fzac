#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

// The acceptable commands are:
// --key "this is my value" -no-string
// key: "key", value: "this is my value", is-flag: false
// -no-string
// key: "no-string", value: "no-string", is-flag: true

#define MAX_COMMAND_LINE_ARGS 16
#define TEMP_BUFFER_SIZE 2048
#define PARSED_BUFFER_SIZE 4096

typedef struct Command_Line_Arg Command_Line_Arg;
typedef struct Command_Line Command_Line;

struct Command_Line_Arg {
  b32    is_flag; /* This just means: Is this a standalone value, or is this a key/value pair? */
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

fz_internal Command_Line_Arg command_line_arg_new(String key, String value, b32 is_flag);
fz_internal Command_Line     command_line_parse(String input); /* Parses a command line type from a String */
fz_internal Command_Line     command_line_parse_from_argc_argv(s32 argc, u8** argv); /* Parses a Command line from a c style argc/argv argument */
fz_internal String          _command_line_parse_token(u8** cursor);
fz_internal void            _command_line_skip_whitespace(u8** cursor);
fz_internal String          _command_line_strip_quotes(String in);
fz_internal String          _command_line_strip_leading_dashes(String in);

fz_internal Command_Line_Arg
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

fz_internal void
_command_line_skip_whitespace(u8** cursor)
{
  while (char_is_space(**cursor)) (*cursor)++;
}

fz_internal String
_command_line_strip_quotes(String in)
{
  if (in.count >= 2 && in.cstring[0] == '"' && in.cstring[in.count - 1] == '"')
  {
    return string_new(in.count - 2,  in.cstring + 1);
  }
  return in;
}

fz_internal String
_command_line_strip_leading_dashes(String in)
{
  u64 offset = 0;
  while (offset < in.count && in.cstring[offset] == '-')
  {
    offset++;
  }
  return string_new(in.count - offset, in.cstring + offset);
}

fz_internal String
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
    while (*end && !char_is_space(*end)) end++;
    *cursor = end;
  }

  String result = string_new((u64)(end - start), start);
  return result;
}

fz_internal Command_Line
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

fz_internal Command_Line
command_line_parse(String input)
{
  Command_Line result = {0};
  if (input.count <= 1)
  {
    return result;
  }

  fz_local_persist u8 exe_buffer[MAX_PATH];
  DWORD exe_len = GetModuleFileNameA(0, (LPSTR)exe_buffer, MAX_PATH);
  result.executable = (String){ exe_len, exe_buffer };

  // Copy input into stable memory
  fz_local_persist u8 temp_buffer[TEMP_BUFFER_SIZE];
  fz_local_persist u8 parsed_buffer[PARSED_BUFFER_SIZE];
  u64 parsed_cursor = 0;

  u64 len = input.count;
  if (len >= sizeof(temp_buffer)) len = sizeof(temp_buffer) - 1;
  memory_copy(temp_buffer, input.cstring, len);
  temp_buffer[len] = 0;

  result.raw_args = (String){ len, temp_buffer };

  u8* cursor = temp_buffer;

  while (*cursor && result.args_count < MAX_COMMAND_LINE_ARGS)
  {
    _command_line_skip_whitespace(&cursor);
    if (*cursor == 0) break;

    String token = _command_line_parse_token(&cursor);
    if (token.count == 0) break;

    if (token.cstring[0] == '-')
    {
      String key = _command_line_strip_leading_dashes(token);

      // Copy key to stable memory
      String key_copy = {
        .count = key.count,
        .cstring = parsed_buffer + parsed_cursor
      };
      memory_copy(key_copy.cstring, key.cstring, key.count);
      parsed_cursor += key.count;

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
          .count = val.count,
          .cstring = parsed_buffer + parsed_cursor
        };
        memory_copy(val_copy.cstring, val.cstring, val.count);
        parsed_cursor += val.count;

        result.args[result.args_count++] = command_line_arg_new(key_copy, val_copy, false);
      }
    }
  }

  return result;
}

#endif // COMMAND_LINE_H
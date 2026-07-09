#ifndef FZ_LOGGING_H
#define FZ_LOGGING_H

#include <stdarg.h>

#include "Files.h"

#define LOG_FILE_BUFFER_SIZE megabytes(1)
#define LOG_MAX_HOOKS_COUNT  8

typedef enum Log_Level Log_Level;
enum Log_Level
{
  Log_Level_Trace = 0,
  Log_Level_Info  = 1,
  Log_Level_Warn  = 2,
  Log_Level_Error = 3,
  Log_Level_Fatal = 4,
  LogLevel_Count,
};

typedef void log_hook_function(Log_Level level, const char* file, int line, String msg);

typedef struct Log_Hook Log_Hook;
struct Log_Hook
{
  log_hook_function* hook_function;
};

typedef struct Log_State Log_State;
struct Log_State
{
  String    file_path;
  u8        file_buf[LOG_FILE_BUFFER_SIZE];
  u64       file_head;
  u64       file_total;
  b32       file_enabled;
  Log_Level min_level;
  Log_Hook  hooks[LOG_MAX_HOOKS_COUNT];
  u32       hook_count;
  b8        inited;
};

global Log_State GlobalLogger = {0};

function inline const char*
log_level_str(Log_Level level)
{
  switch(level)
  {
    case Log_Level_Trace: return "TRACE";
    case Log_Level_Info:  return "INFO";
    case Log_Level_Warn:  return "WARN";
    case Log_Level_Error: return "ERROR";
    case Log_Level_Fatal: return "FATAL";
    default:              return "?????";
  }
}

function inline const char*
log_level_color(Log_Level level)
{
  switch(level)
  {
    case Log_Level_Trace: return "\x1b[90m";
    case Log_Level_Info:  return "\x1b[32m";
    case Log_Level_Warn:  return "\x1b[33m";
    case Log_Level_Error: return "\x1b[31m";
    case Log_Level_Fatal: return "\x1b[35m";
    default:              return "";
  }
}

function inline void
log_flush_to_file(String line)
{
  if(!GlobalLogger.file_enabled || line.count == 0)
  {
    return;
  }
  u64 len   = line.count;
  u64 space = (u64)LOG_FILE_BUFFER_SIZE - GlobalLogger.file_head;
  if(len <= space)
  {
    memory_copy(GlobalLogger.file_buf + GlobalLogger.file_head, line.cstring, len);
    GlobalLogger.file_head += len;
  }
  else
  {
    memory_copy(GlobalLogger.file_buf + GlobalLogger.file_head, line.cstring, space);
    memory_copy(GlobalLogger.file_buf, line.cstring + space, len - space);
    GlobalLogger.file_head = len - space;
  }
  GlobalLogger.file_total += len;
  u64 written = (GlobalLogger.file_total < (u64)LOG_FILE_BUFFER_SIZE) ? GlobalLogger.file_total : (u64)LOG_FILE_BUFFER_SIZE;
  file_write(GlobalLogger.file_path, GlobalLogger.file_buf, written);
}

function inline void
log_fatal_popup(String msg, const char* src_file, int src_line)
{
  Scratch scratch = scratch_begin(0, 0);
  String  file    = string_new(cstring_length((u8*)src_file), (u8*)src_file);
  String  body    = Sf(scratch.arena, "Fatal Error\n" S_FMT "\n\nat " S_FMT ":%d", S_ARG(msg), S_ARG(file), src_line);
  message_box_with_debugger(S("Fatal Error"), body, file, (u32)src_line);
  scratch_end(&scratch);
}

function void
log_write(Log_Level level, const char* src_file, int src_line, const char* fmt, ...)
{
  if(level < GlobalLogger.min_level || !GlobalLogger.inited)
  {
    return;
  }
  Scratch scratch = scratch_begin(0, 0);
  va_list args;
  va_start(args, fmt);
  String msg = string_from_format_va(scratch.arena, fmt, args);
  va_end(args);
  Date_Time date_time   = datetime_now();
  String    time_string = datetime_to_string(scratch.arena, date_time, false);
  String line      = Sf(scratch.arena, "%s" S_FMT " [%s] %s:%d: " S_FMT "\x1b[0m\n", log_level_color(level), S_ARG(time_string), log_level_str(level), src_file, src_line, S_ARG(msg));
  string_print(line);
  String file_line = Sf(scratch.arena, S_FMT " [%s] %s:%d: " S_FMT "\n", S_ARG(time_string), log_level_str(level), src_file, src_line, S_ARG(msg));
  log_flush_to_file(file_line);
  for(u32 i = 0; i < GlobalLogger.hook_count; i++)
  {
    if(GlobalLogger.hooks[i].hook_function)
    {
      GlobalLogger.hooks[i].hook_function(level, src_file, src_line, msg);
    }
  }
  if(level == Log_Level_Fatal)
  {
    log_fatal_popup(msg, src_file, src_line);
    assert(0);
  }
  scratch_end(&scratch);
}

function void
log_write_string(Log_Level level, const char* src_file, int src_line, String msg)
{
  if(level < GlobalLogger.min_level || !GlobalLogger.inited)
  {
    return;
  }
  Scratch scratch = scratch_begin(0, 0);
  Date_Time date_time   = datetime_now();
  String    time_string = datetime_to_string(scratch.arena, date_time, false);
  String line      = Sf(scratch.arena, "%s" S_FMT " [%s] %s:%d: " S_FMT "\x1b[0m\n", log_level_color(level), S_ARG(time_string), log_level_str(level), src_file, src_line, S_ARG(msg));
  string_print(line);
  String file_line = Sf(scratch.arena, S_FMT " [%s] %s:%d: " S_FMT "\n", S_ARG(time_string), log_level_str(level), src_file, src_line, S_ARG(msg));
  log_flush_to_file(file_line);
  for(u32 i = 0; i < GlobalLogger.hook_count; i++)
  {
    if(GlobalLogger.hooks[i].hook_function)
    {
      GlobalLogger.hooks[i].hook_function(level, src_file, src_line, msg);
    }
  }
  if(level == Log_Level_Fatal)
  {
    log_fatal_popup(msg, src_file, src_line);
    assert(0);
  }
  scratch_end(&scratch);
}

function b32
logging_init(Log_Level min_level, String file_path)
{
  if(!TimingInited)
  {
    time_init();
  }
  memory_zero_struct(&GlobalLogger);
  GlobalLogger.min_level = min_level;
  if(file_path.count > 0)
  {
    if(!file_exists(file_path))
    {
      file_create(file_path);
    }
    else
    {
      file_wipe(file_path);
    }
    GlobalLogger.file_path    = file_path;
    GlobalLogger.file_enabled = true;
  }
  GlobalLogger.inited = true;
  return true;
}

function void
logging_free(void)
{
  if(GlobalLogger.file_enabled && GlobalLogger.file_total > 0)
  {
    u64 written = (GlobalLogger.file_total < (u64)LOG_FILE_BUFFER_SIZE) ? GlobalLogger.file_total : (u64)LOG_FILE_BUFFER_SIZE;
    file_write(GlobalLogger.file_path, GlobalLogger.file_buf, written);
  }
  memory_zero_struct(&GlobalLogger);
}

function void
logging_set_level(Log_Level level)
{
  GlobalLogger.min_level = level;
}

function b8
logging_add_hook(log_hook_function* hook_function)
{
  b8 result = false;
  if(GlobalLogger.hook_count < LOG_MAX_HOOKS_COUNT) 
  { 
    GlobalLogger.hooks[GlobalLogger.hook_count].hook_function = hook_function;
    GlobalLogger.hook_count += 1;
    result = true;
  }
  return result;
}

function void
logging_remove_hook(s32 idx)
{
  if(idx < 0 || (u32)idx >= GlobalLogger.hook_count)
  {
    return;
  }
  GlobalLogger.hooks[idx] = GlobalLogger.hooks[--GlobalLogger.hook_count];
}

#define log_trace(fmt, ...) log_write(Log_Level_Trace, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...)  log_write(Log_Level_Info,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...)  log_write(Log_Level_Warn,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) log_write(Log_Level_Error, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...) log_write(Log_Level_Fatal, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define log_trace_str(str) log_write_string(Log_Level_Trace, __FILE__, __LINE__, str)
#define log_info_str(str)  log_write_string(Log_Level_Info,  __FILE__, __LINE__, str)
#define log_warn_str(str)  log_write_string(Log_Level_Warn,  __FILE__, __LINE__, str)
#define log_error_str(str) log_write_string(Log_Level_Error, __FILE__, __LINE__, str)
#define log_fatal_str(str) log_write_string(Log_Level_Fatal, __FILE__, __LINE__, str)

#define log_trace_if(cond, fmt, ...) do { if(cond) { log_write(Log_Level_Trace, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } } while(0)
#define log_info_if (cond, fmt, ...) do { if(cond) { log_write(Log_Level_Info, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } } while(0)
#define log_warn_if (cond, fmt, ...) do { if(cond) { log_write(Log_Level_Warn, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } } while(0)
#define log_error_if(cond, fmt, ...) do { if(cond) { log_write(Log_Level_Error, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } } while(0)
#define log_fatal_if(cond, fmt, ...) do { if(cond) { log_write(Log_Level_Fatal, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } } while(0)

#define log_trace_if_str(cond, str)  do { if(cond) { log_write_string(Log_Level_Trace, __FILE__, __LINE__, str); } } while(0)
#define log_info_if_str (cond, str)  do { if(cond) { log_write_string(Log_Level_Info, __FILE__, __LINE__, str); } } while(0)
#define log_warn_if_str (cond, str)  do { if(cond) { log_write_string(Log_Level_Warn, __FILE__, __LINE__, str); } } while(0)
#define log_error_if_str(cond, str)  do { if(cond) { log_write_string(Log_Level_Error, __FILE__, __LINE__, str); } } while(0)
#define log_fatal_if_str(cond, str)  do { if(cond) { log_write_string(Log_Level_Fatal, __FILE__, __LINE__, str); } } while(0)

#endif // FZ_LOGGING_H
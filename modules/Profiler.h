#ifndef PROFILER_H
#define PROFILER_H

#include "Timing.h"

#define PROFILER_OUTPUT_FILE S("../profile.txt")

#define PROFILER_MAX_WRITE_CHARACTERS thousand(10)

#define PROFILER_RECORDINGS_CAPACITY 32
#define PROFILER_EVENTS_CAPACITY million(1)

typedef enum
{
  Profiler_Event_Zone_Begin,
  Profiler_Event_Zone_End,
  Profiler_Event_Frame,
  Profiler_Event_Plot,
} Profiler_Event_Kind;

typedef enum
{
  Profiler_Event_Plot_F64,
  Profiler_Event_Plot_U64,
  Profiler_Event_Plot_S64,
} Profiler_Event_Plot_Kind;

typedef struct Profiler_Event Profiler_Event;
struct Profiler_Event
{
  Profiler_Event_Kind kind;
  u64 timestamp_us;

  union
  {
    struct
    {
      String name;
    } zone_begin;

    struct
    {
      Profiler_Event_Plot_Kind kind;
      String name;
      union
      {
        f64 f64;
        u64 u64;
        s64 s64;
      } value;
    } plot;
  };
};

typedef struct Profiler_Recording Profiler_Recording;
struct Profiler_Recording
{
  Arena *arena;

  Array(events, Profiler_Event);
  
  u64 time_us_start;
  u64 time_us_end;
  u64 time_us_total;

  String text;
};

typedef struct Profiler_Context Profiler_Context;
struct Profiler_Context
{
  Arena *arena;

  // Frame
  u64 total_frames;
  Profiler_Event *previous_frame_event;

  // File
  String file_output;

  // Recordings
  Array(recordings, Profiler_Recording);
  Profiler_Recording *active_recording; // NULL if it's not recording
};

global Profiler_Context ProfileContext;

// @Section: API

#ifdef USE_PROFILER

#define profiler_init()  _profiler_init()
#define profiler_end()   _profiler_end()

#define profiler_record()    defer_loop(_profiler_recording_start(), _profiler_recording_stop())
#define profiler_recording_start() _profiler_recording_start()
#define profiler_recording_stop()  _profiler_recording_stop()

#define profiler_frame() _profiler_frame()

#define profiler_function_begin() _profiler_zone_begin(S(__func__))
#define profiler_function_end()   _profiler_zone_end()

#define profiler_zone(name)       defer_loop(_profiler_zone_begin(name), _profiler_zone_end())
#define profiler_zone_begin(name) _profiler_zone_begin(name)
#define profiler_zone_end()       _profiler_zone_end()

#define profiler_thread(name) _profiler_thread(name)

#define profiler_plot_s64(name, value) _profiler_plot_s64(name, value)
#define profiler_plot_u64(name, value) _profiler_plot_u64(name, value)
#define profiler_plot_f64(name, value) _profiler_plot_f64(name, value)

#else
#define profiler_init()
#define profiler_end()
#define profiler_record_scope()
#define profiler_recording_start()
#define profiler_recording_stop()
#define profiler_frame()
#define profiler_function_begin()
#define profiler_function_end()
#define profiler_zone(name)
#define profiler_zone_begin(name)
#define profiler_zone_end()
#define profiler_thread(name)
#define profiler_plot_s64(name, value)
#define profiler_plot_u64(name, value)
#define profiler_plot_f64(name, value)
#endif // USE_PROFILER

// @Section: Internal

function void _profiler_init();
function void _profiler_end();

function void _profiler_recording_start();
function void _profiler_recording_stop();

function void _profiler_frame();

function void _profiler_zone_begin(String name);
function void _profiler_zone_end();

function void _profiler_thread(String name);

function void _profiler_plot_s64(String name, s64 value);
function void _profiler_plot_u64(String name, u64 value);
function void _profiler_plot_f64(String name, f64 value);

// Helpers
function Profiler_Event* _profiler_push_event_to_recording(Profiler_Recording *recording, Profiler_Event_Kind kind);
function String         _profiler_format_us(Arena *arena, u64 us);

// @Section: Implementation
function void
_profiler_init()
{
  memory_zero_struct(&ProfileContext);
  ProfileContext.arena = arena_alloc();
  time_init();

  ProfileContext.file_output  = PROFILER_OUTPUT_FILE;
  
  array_init(ProfileContext.arena, ProfileContext.recordings, Profiler_Recording, PROFILER_RECORDINGS_CAPACITY);
}

function void
_profiler_end()
{
}

function void 
_profiler_recording_start()
{
  Profiler_Recording *rec = array_add(ProfileContext.recordings);
  ProfileContext.active_recording = rec;

  memory_zero_struct(rec);
  rec->arena = arena_alloc();
  array_init(rec->arena, rec->events, Profiler_Event, PROFILER_EVENTS_CAPACITY);
  rec->time_us_start = time_microseconds();

  // Output file
  if (file_exists(ProfileContext.file_output))
  {
    file_delete(ProfileContext.file_output);
  }
  file_create(ProfileContext.file_output);
}

function void 
_profiler_recording_stop()
{
  Scratch scratch = scratch_begin(0,0);

  ProfileContext.active_recording->time_us_end   = time_microseconds();
  ProfileContext.active_recording->time_us_total = ProfileContext.active_recording->time_us_end - ProfileContext.active_recording->time_us_start; 
 
  String_Builder builder = string_builder_init(kilobytes(64));
  string_builder_pushf(&builder, "[Recording %u - "S_FMT"]\n\n", ProfileContext.recordings_count, S_ARG(_profiler_format_us(scratch.arena, ProfileContext.active_recording->time_us_end - ProfileContext.active_recording->time_us_start)));

  Array(event_stack, Profiler_Event);
  array_init(scratch.arena, event_stack, Profiler_Event, ProfileContext.active_recording->events_count);

  u64 nests = 0;
  u64 previous_frame_timestamp = 0;

  for (u64 recording_idx = 0; recording_idx < ProfileContext.recordings_count; recording_idx += 1)
  {
    Profiler_Recording *rec = &ProfileContext.recordings[recording_idx];
    for (u64 events_idx = 0; events_idx < rec->events_count; events_idx += 1)
    {
      Profiler_Event *event = &rec->events[events_idx];
      switch (event->kind)
      {
        case Profiler_Event_Zone_Begin:
        {
          for (u32 i = 0; i < nests; i += 1) string_builder_push(&builder, "  ");
          string_builder_pushf(&builder, S_FMT "\n", S_ARG(event->zone_begin.name));
          for (u32 i = 0; i < nests; i += 1) string_builder_push(&builder, "  ");
          string_builder_push(&builder, "{\n");

          Profiler_Event *stack_event = array_add(event_stack);
          *stack_event = *event;

          nests += 1;
        } break;
      
        case Profiler_Event_Zone_End:
        {
          nests -= 1;

          Profiler_Event *begin_event;
          array_pop(begin_event, event_stack);
          assert(begin_event->kind == Profiler_Event_Zone_Begin);
          u64 total_time = event->timestamp_us - begin_event->timestamp_us;
          for (u32 i = 0; i < nests; i += 1) string_builder_push(&builder, "  ");
          string_builder_pushf(&builder,"} // " S_FMT "\n", S_ARG(_profiler_format_us(scratch.arena, total_time)));
        } break;
      
        case Profiler_Event_Frame:
        {
          if (previous_frame_timestamp == 0)
          {
            string_builder_push(&builder, "---- First Frame ----\n");
          }
          else
          {
            u64 frame_time_us = event->timestamp_us - previous_frame_timestamp;
            string_builder_pushf(&builder, "---- Frame " S_FMT " ----\n", S_ARG(_profiler_format_us(scratch.arena, frame_time_us)));
          }

          previous_frame_timestamp = event->timestamp_us;
        } break;
      
        case Profiler_Event_Plot:
        {
          for (u32 i = 0; i < nests; i += 1) string_builder_push(&builder, "  ");
          string_builder_pushf(&builder, "Plot: " S_FMT " %.2f \n", S_ARG(event->plot.name), event->plot.value.f64);
        } break;
      
        default: 
        {
          assert(0);
        };
      }
    }
  }

  String result = string_builder_to_string(scratch.arena, &builder);
  file_write(ProfileContext.file_output, result.cstring, result.count > PROFILER_MAX_WRITE_CHARACTERS  ? PROFILER_MAX_WRITE_CHARACTERS : result.count);

  string_builder_free(&builder);

  ProfileContext.active_recording = NULL;
  scratch_end(&scratch);
}

function void 
_profiler_zone_begin(String name)
{
  if (!ProfileContext.active_recording)
  {
    return;
  }
  Profiler_Event *event = _profiler_push_event_to_recording(ProfileContext.active_recording, Profiler_Event_Zone_Begin);
  event->zone_begin.name = string_copy(ProfileContext.active_recording->arena, name);
}

function void 
_profiler_zone_end()
{
  if (!ProfileContext.active_recording)
  {
    return;
  }
  _profiler_push_event_to_recording(ProfileContext.active_recording, Profiler_Event_Zone_End);
}

function void
_profiler_frame()
{
  ProfileContext.total_frames += 1;

  if (!ProfileContext.active_recording)
  {
    return;
  }

  ProfileContext.previous_frame_event =_profiler_push_event_to_recording(ProfileContext.active_recording, Profiler_Event_Frame);
  ProfileContext.previous_frame_event->timestamp_us = time_microseconds();
}

function void 
_profiler_plot_s64(String name, s64 value)
{
  if (!ProfileContext.active_recording)
  {
    return;
  }
  Profiler_Event *event = _profiler_push_event_to_recording(ProfileContext.active_recording, Profiler_Event_Plot);
  event->plot.kind = Profiler_Event_Plot_S64;
  event->plot.name = string_copy(ProfileContext.active_recording->arena, name);
  event->plot.value.s64 = value;
}

function void 
_profiler_plot_u64(String name, u64 value)
{
  if (!ProfileContext.active_recording)
  {
    return;
  }
  Profiler_Event *event = _profiler_push_event_to_recording(ProfileContext.active_recording, Profiler_Event_Plot);
  event->plot.kind = Profiler_Event_Plot_U64;
  event->plot.name = string_copy(ProfileContext.active_recording->arena, name);
  event->plot.value.u64 = value;
}

function void 
_profiler_plot_f64(String name, f64 value)
{
  if (!ProfileContext.active_recording)
  {
    return;
  }
  Profiler_Event *event = _profiler_push_event_to_recording(ProfileContext.active_recording, Profiler_Event_Plot);
  event->plot.kind = Profiler_Event_Plot_F64;
  event->plot.name = string_copy(ProfileContext.active_recording->arena, name);
  event->plot.value.f64 = value;
}

function Profiler_Event*
_profiler_push_event_to_recording(Profiler_Recording *recording, Profiler_Event_Kind kind)
{
  if (!ProfileContext.active_recording)
  {
    return NULL;
  }
  Profiler_Event *result = array_add(recording->events);

  result->kind = kind;
  result->timestamp_us = time_microseconds();

  return result;
}

function String
_profiler_format_us(Arena *arena, u64 us)
{
  String result;
  f64 value;
  char *unit;

  if (us < 1000)
  {
    value = (f64)us;
    unit = "us";
  }
  else if (us < 1000 * 1000)
  {
    value = (f64)us / 1000.0;
    unit = "ms";
  }
  else
  {
    value = (f64)us / 1000000.0;
    unit = "s";
  }

  result = Sf(arena, "%.3f %s", value, unit);
  return result;
}

#endif // PROFILER_H
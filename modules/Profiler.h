#ifndef PROFILER_H
#define PROFILER_H

#include "Timing.h"

#define PROFILER_OUTPUT_FILE S("../profile.txt")

#define PROFILER_RECORDINGS_CAPACITY 32
#define PROFILER_EVENTS_CAPACITY million(1)

typedef enum
{
  Profile_Event_Zone_Begin,
  Profile_Event_Zone_End,
  Profile_Event_Frame,
  Profile_Event_Plot,
} Profile_Event_Kind;

typedef enum
{
  Profile_Event_Plot_F64,
  Profile_Event_Plot_U64,
  Profile_Event_Plot_S64,
} Profile_Event_Plot_Kind;

typedef struct Profile_Event Profile_Event;
struct Profile_Event
{
  Profile_Event_Kind kind;
  u64 timestamp_us;

  union
  {
    struct
    {
      String name;
    } zone_begin;

    struct
    {
      u64 time_us;
    } frame;

    struct
    {
      Profile_Event_Plot_Kind kind;
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

typedef struct Profile_Recording Profile_Recording;
struct Profile_Recording
{
  Arena *arena;

  Profile_Event *events;
  u64 events_capacity;
  u64 events_count;  
  
  u64 time_us_start;
  u64 time_us_end;
  u64 time_us_total;

  String text; // @TODO(Fz): This is kind of a debug thing? I don't think the ideal interface of a profiler should be a txt.
};

typedef struct Profile_Context Profile_Context;
struct Profile_Context
{
  Arena *arena;

  u64 total_frames; // Even if it's not recording, we increment this just to be syncronized with the actual program

  String file_output;

  Profile_Recording *recordings;
  u64 recordings_capacity;
  u64 recordings_count;

  Profile_Recording *active_recording; // NULL if it's not recording
};

global Profile_Context ProfileContext;

// @Section: API

#ifdef USE_PROFILER

#define profile_init()  _profile_init()
#define profile_end()   _profile_end()

#define profile_record_scope()    defer_loop(_profile_recording_start(), _profile_recording_stop())
#define profile_recording_start() _profile_recording_start()
#define profile_recording_stop()  _profile_recording_stop()

#define profile_frame() _profile_frame()

#define profile_function_begin() _profile_zone_begin(S(__func__))
#define profile_function_end()   _profile_zone_end()

#define profile_zone(name)       defer_loop(_profile_zone_begin(name), _profile_zone_end())
#define profile_zone_begin(name) _profile_zone_begin(name)
#define profile_zone_end()       _profile_zone_end()

#define profile_thread(name) _profile_thread(name)

#define profile_plot_s64(name, value) _profile_plot_s64(name, value)
#define profile_plot_u64(name, value) _profile_plot_u64(name, value)
#define profile_plot_f64(name, value) _profile_plot_f64(name, value)

#else

#define profile_init()
#define profile_end()
#define profile_record_scope()
#define profile_recording_start()
#define profile_recording_stop()
#define profile_frame()
#define profile_function_begin()
#define profile_function_end()
#define profile_zone(name)
#define profile_zone_begin(name)
#define profile_zone_end()
#define profile_thread(name)
#define profile_plot_s64(name, value)
#define profile_plot_u64(name, value)
#define profile_plot_f64(name, value)

#endif // USE_PROFILER

// @Section: Internal

function void _profile_init();
function void _profile_end();

function void _profile_recording_start();
function void _profile_recording_stop();

function void _profile_frame();

function void _profile_zone_begin(String name);
function void _profile_zone_end();

function void _profile_thread(String name);

function void _profile_plot_s64(String name, s64 value);
function void _profile_plot_u64(String name, u64 value);
function void _profile_plot_f64(String name, f64 value);

// Helpers
function Profile_Event* _profile_push_event_to_recording(Profile_Recording *recording, Profile_Event_Kind kind);
function String         _profile_format_us(Arena *arena, u64 us);

// @Section: Implementation
function void
_profile_init()
{
  memory_zero_struct(&ProfileContext);
  ProfileContext.arena = arena_alloc();
  time_init();

  ProfileContext.file_output  = PROFILER_OUTPUT_FILE;
  
  arena_array_init(ProfileContext.arena, ProfileContext.recordings, Profile_Recording, PROFILER_RECORDINGS_CAPACITY);
}

function void
_profile_end()
{
}

function void 
_profile_recording_start()
{
  Profile_Recording *rec = NULL;
  arena_array_push(rec, ProfileContext.recordings);
  ProfileContext.active_recording = rec;

  memory_zero_struct(rec);
  rec->arena = arena_alloc();
  arena_array_init(rec->arena, rec->events, Profile_Event, PROFILER_EVENTS_CAPACITY);
  rec->time_us_start = time_microseconds();

  // Output file
  if (file_exists(ProfileContext.file_output))
  {
    file_delete(ProfileContext.file_output);
  }
  file_create(ProfileContext.file_output);
}

function void 
_profile_recording_stop()
{
  Scratch scratch = scratch_begin(0,0);

  ProfileContext.active_recording->time_us_end   = time_microseconds();
  ProfileContext.active_recording->time_us_total = ProfileContext.active_recording->time_us_end - ProfileContext.active_recording->time_us_start; 
 
  String_Builder builder = string_builder_init(kilobytes(64));
  string_builder_pushf(&builder, "Recording %u - "S_FMT"\n{\n", ProfileContext.recordings_count, S_ARG(_profile_format_us(scratch.arena, ProfileContext.active_recording->time_us_end - ProfileContext.active_recording->time_us_start)));

  Profile_Event *event_stack = NULL;
  u64 event_stack_count    = 0;
  u64 event_stack_capacity = 0;
  arena_array_init(scratch.arena, event_stack, Profile_Event, ProfileContext.active_recording->events_count);

  u64 nests = 0;

  for (u64 recording_idx = 0; recording_idx < ProfileContext.recordings_count; recording_idx += 1)
  {
    Profile_Recording *rec = &ProfileContext.recordings[recording_idx];
    for (u64 events_idx = 0; events_idx < rec->events_count; events_idx += 1)
    {
      Profile_Event *event = &rec->events[events_idx];
      switch (event->kind)
      {
        case Profile_Event_Zone_Begin:
        {
          for (u32 i = 0; i < nests; i += 1) string_builder_push(&builder, "  ");
          string_builder_pushf(&builder, "  Zone Begin: "S_FMT"\n", S_ARG(event->zone_begin.name));

          Profile_Event *stack_event;
          arena_array_push(stack_event, event_stack);
          *stack_event = *event;

          nests += 1; // Keep at the end
        } break;
        case Profile_Event_Zone_End:
        {
          nests -= 1; // Keep at the start

          Profile_Event *poped_event;
          arena_array_pop(poped_event, event_stack);
          assert(poped_event->kind == Profile_Event_Zone_Begin);
          u64 total_time = event->timestamp_us - poped_event->timestamp_us;

          for (u32 i = 0; i < nests; i += 1) string_builder_push(&builder, "  ");
          string_builder_pushf(&builder, "  Zone End "S_FMT"\n", S_ARG(_profile_format_us(scratch.arena, total_time)));
        } break;
        case Profile_Event_Frame:
        {
          string_builder_push(&builder, "  Frame --------------------------------------------------------------- \n");
        
        } break;
        case Profile_Event_Plot:
        {
          string_builder_pushf(&builder, "  Plot "S_FMT"\n", S_ARG(event->plot.name));
          
        } break;
        default: 
        {
          assert(0);
        };
      }
    }
  }

  string_builder_push(&builder, "}\n");

  String result = string_builder_to_string(scratch.arena, &builder);
  file_append(ProfileContext.file_output, result.cstring, result.count);

  string_builder_free(&builder);

  ProfileContext.active_recording = NULL;
  scratch_end(&scratch);
}

function void 
_profile_zone_begin(String name)
{
  if (!ProfileContext.active_recording)
  {
    return;
  }
  Profile_Event *event = _profile_push_event_to_recording(ProfileContext.active_recording, Profile_Event_Zone_Begin);
  event->zone_begin.name = string_copy(ProfileContext.active_recording->arena, name);
}

function void 
_profile_zone_end()
{
  if (!ProfileContext.active_recording)
  {
    return;
  }
  _profile_push_event_to_recording(ProfileContext.active_recording, Profile_Event_Zone_End);
}

function void
_profile_frame()
{
  ProfileContext.total_frames += 1;
  if (!ProfileContext.active_recording)
  {
    return;
  }
  Profile_Event *event = _profile_push_event_to_recording(ProfileContext.active_recording, Profile_Event_Frame);
  event->frame.time_us = time_milliseconds();
}

function void 
_profile_plot_s64(String name, s64 value)
{
  Profile_Event *event = _profile_push_event_to_recording(ProfileContext.active_recording, Profile_Event_Plot);
  event->plot.kind = Profile_Event_Plot_S64;
  event->plot.value.s64 = value;
}

function void 
_profile_plot_u64(String name, u64 value)
{
  Profile_Event *event = _profile_push_event_to_recording(ProfileContext.active_recording, Profile_Event_Plot);
  event->plot.kind = Profile_Event_Plot_U64;
  event->plot.value.u64 = value;
}

function void 
_profile_plot_f64(String name, f64 value)
{
  Profile_Event *event = _profile_push_event_to_recording(ProfileContext.active_recording, Profile_Event_Plot);
  event->plot.kind = Profile_Event_Plot_F64;
  event->plot.value.f64 = value;
}

function Profile_Event*
_profile_push_event_to_recording(Profile_Recording *recording, Profile_Event_Kind kind)
{
  Profile_Event *result;
  arena_array_push(result, recording->events);

  result->kind = kind;
  result->timestamp_us = time_microseconds();

  return result;
}

function String
_profile_format_us(Arena *arena, u64 us)
{
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

  String result = Sf(arena, "%.3f %s", value, unit);
  return result;
}

#endif // PROFILER_H
#ifndef TIMING_H
#define TIMING_H

typedef struct Timer Timer;
struct Timer
{
  u64 opaque[2];
};

typedef struct Date_Time Date_Time;
struct Date_Time
{
  u16 year;
  u8  month, day, hour, minute, second;
  u16 millisecond;
};

fz_global b8 TimingInited = false;

fz_internal void     time_init(void); /* Initializes timer module */
fz_internal u64      time_microseconds(void); /* Time in microseconds */
fz_internal u64      time_milliseconds(void); /* Time in milliseconds */
fz_internal f64      time_seconds(void); /* Time in seconds */
fz_internal u64      get_epoch_microseconds(void); /* Wall clock time since unix epoch (1970-01-01) in microseconds */
fz_internal Date_Time datetime_now(void); /* Current local date and time */
fz_internal String   datetime_to_string(Arena* arena, Date_Time date, b32 include_ms); /* Returns a verbose datetime string */

fz_internal Timer timer_start(void); /* Returns a started timer */
fz_internal u64   timer_microseconds(Timer *timer); /* Returns timer's elapsed time in microseconds */
fz_internal u64   timer_milliseconds(Timer *timer); /* Returns timer's elapsed time in milliseconds */
fz_internal f64   timer_seconds(Timer *timer); /* Returns timer's elapsed time in seconds */
fz_internal void  timer_reset(Timer *timer); /* Resets a timer */

fz_internal f64 milliseconds_from_microseconds(u64 us);
fz_internal f64 seconds_from_microseconds(u64 us);

fz_internal f64
milliseconds_from_microseconds(u64 us)
{
  f64 result = (f64)(us) / 1000.0;
  return result;
}

fz_internal f64
seconds_from_microseconds(u64 us)
{
  return (f64)us / 1000000.0;
}

#if OS_WINDOWS

fz_global LARGE_INTEGER Win32PerformanceFrequency;

fz_internal void
time_init(void)
{
  if (!TimingInited)
  {
    QueryPerformanceFrequency(&Win32PerformanceFrequency);
    TimingInited = true;
  }
}

fz_internal u64
time_microseconds(void)
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (counter.QuadPart * 1000000) / Win32PerformanceFrequency.QuadPart;
}

fz_internal u64
time_milliseconds(void)
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (counter.QuadPart * 1000) / Win32PerformanceFrequency.QuadPart;
}

fz_internal f64
time_seconds(void)
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (f64)counter.QuadPart / (f64)Win32PerformanceFrequency.QuadPart;
}

fz_internal u64
get_epoch_microseconds(void)
{
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  ULARGE_INTEGER uli = {ft.dwLowDateTime, ft.dwHighDateTime};
  return (uli.QuadPart - 116444736000000000ULL) / 10;
}

fz_internal Date_Time
datetime_now(void)
{
  SYSTEMTIME st;
  GetLocalTime(&st);
  Date_Time result =
  {
    (u16)st.wYear, 
    (u8)st.wMonth, 
    (u8)st.wDay,
    (u8)st.wHour, 
    (u8)st.wMinute, 
    (u8)st.wSecond,
    (u16)st.wMilliseconds
  };
  return result;
}

fz_internal Timer
timer_start(void)
{
  Timer timer;
  QueryPerformanceCounter((LARGE_INTEGER*)&timer.opaque[0]);
  return timer;
}

fz_internal u64
timer_microseconds(Timer *timer)
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  u64 start = timer->opaque[0];
  return ((now.QuadPart - start) * 1000000) / Win32PerformanceFrequency.QuadPart;
}

fz_internal u64
timer_milliseconds(Timer *timer)
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  u64 start = timer->opaque[0];
  return ((now.QuadPart - start) * 1000) / Win32PerformanceFrequency.QuadPart;
}

fz_internal f64
timer_seconds(Timer *timer)
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  u64 start = timer->opaque[0];
  return (f64)(now.QuadPart - start) / (f64)Win32PerformanceFrequency.QuadPart;
}

fz_internal void
timer_reset(Timer *timer)
{
  QueryPerformanceCounter((LARGE_INTEGER*)&timer->opaque[0]);
}

#elif OS_LINUX

#include <time.h>
#include <sys/time.h>

fz_internal void
time_init(void)
{
  TimingInited = true;
}

fz_internal u64
time_microseconds(void)
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  return ((u64)ts.tv_sec * 1000000ULL) + ((u64)ts.tv_nsec / 1000ULL);
}

fz_internal u64
time_milliseconds(void)
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  return ((u64)ts.tv_sec * 1000ULL) + ((u64)ts.tv_nsec / 1000000ULL);
}

fz_internal f64
time_seconds(void)
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  return (f64)ts.tv_sec + ((f64)ts.tv_nsec / 1e9);
}

fz_internal u64
get_epoch_microseconds(void)
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return ((u64)ts.tv_sec * 1000000ULL) + ((u64)ts.tv_nsec / 1000ULL);
}

fz_internal Date_Time
datetime_now(void)
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);

  struct tm t;
  localtime_r(&ts.tv_sec, &t);

  Date_Time result =
  {
    (u16)(t.tm_year + 1900),
    (u8)(t.tm_mon + 1),
    (u8)t.tm_mday,
    (u8)t.tm_hour,
    (u8)t.tm_min,
    (u8)t.tm_sec,
    (u16)(ts.tv_nsec / 1000000ULL)
  };
  return result;
}

fz_internal Timer
timer_start(void)
{
  Timer timer = {0};
  timer.opaque[0] = time_microseconds();
  return timer;
}

fz_internal u64
timer_microseconds(Timer *timer)
{
  u64 now = time_microseconds();
  return now - timer->opaque[0];
}

fz_internal u64
timer_milliseconds(Timer *timer)
{
  u64 now = time_microseconds();
  return (now - timer->opaque[0]) / 1000ULL;
}

fz_internal f64
timer_seconds(Timer *timer)
{
  u64 now = time_microseconds();
  return (f64)(now - timer->opaque[0]) / 1000000.0;
}

fz_internal void
timer_reset(Timer *timer)
{
  timer->opaque[0] = time_microseconds();
}

#else
# error Operating System not supported
#endif

// Cross-platform helper functions
fz_internal String
datetime_to_string(Arena *arena, Date_Time dt, b32 include_ms)
{
  fz_local_persist const char* months[] = 
  {
    "January", "February", "March", 
    "April",   "May",      "June",
    "July",    "August",   "September", 
    "October", "November", "December"
  };
  
  u8 safe_month = (dt.month >= 1 && dt.month <= 12) ? dt.month : 1;
  String month_name;
  month_name.cstring = (u8*)months[safe_month - 1];
  month_name.count   = cstring_length(month_name.cstring);
  
  String result;
  if (include_ms)
  {
    result = Sf(arena, "%04u-" S_FMT "-%02u %02u:%02u:%02u.%03u", dt.year, S_ARG(month_name), dt.day, dt.hour, dt.minute, dt.second, dt.millisecond);
  }
  else
  {
    result = Sf(arena, "%04u-" S_FMT "-%02u %02u:%02u:%02u", dt.year, S_ARG(month_name), dt.day, dt.hour, dt.minute, dt.second);
  }
  
  return result;
}

#endif // TIMING_H
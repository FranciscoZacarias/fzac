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

global LARGE_INTEGER Win32PerformanceFrequency;

function void      time_init(); /* Initializes timer module */
function u64       time_microseconds(); /* Time since boot in microseconds */
function u64       time_milliseconds(); /* Time since boot in milliseconds */
function f64       time_seconds(); /* Time since boot in seconds */
function u64       get_epoch_microseconds(); /* Wall clock time since unix epoch (1970-01-01) in microseconds */
function Date_Time datetime_now(); /* Current local date and time */
function String    datetime_to_string8(Arena* arena, Date_Time date, b32 include_ms); /* Returns a verbose datetime string */

function Timer timer_start(); /* Returns a started timer */
function u64   timer_microseconds(Timer *timer); /* Returns timer's elapsed time in microseconds */
function u64   timer_milliseconds(Timer *timer); /* Returns timer's elapsed time in milliseconds */
function f64   timer_seconds(Timer *timer); /* Returns timer's elapsed time in seconds */
function void  timer_reset(Timer *timer); /* Resets a timer */

#if OS_WINDOWS

function void
time_init()
{
  QueryPerformanceFrequency(&Win32PerformanceFrequency);
}

function u64
time_microseconds()
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (counter.QuadPart * 1000000) / Win32PerformanceFrequency.QuadPart;
}

function u64
time_milliseconds()
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (counter.QuadPart * 1000) / Win32PerformanceFrequency.QuadPart;
}

function f64
time_seconds()
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (f64)counter.QuadPart / (f64)Win32PerformanceFrequency.QuadPart;
}

function u64
get_epoch_microseconds()
{
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  ULARGE_INTEGER uli = {ft.dwLowDateTime, ft.dwHighDateTime};
  return (uli.QuadPart - 116444736000000000ULL) / 10;
}

function Date_Time
datetime_now()
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

function String
datetime_to_string8(Arena *arena, Date_Time dt, b32 include_ms)
{
  local_persist const char* months[] = 
  {
    "January", "February", "March", 
    "April",   "May",      "June",
    "July",    "August",   "September", 
    "October", "November", "December"
  };
 
  String month_name;
  month_name.cstring = (u8*)months[dt.month - 1];
  month_name.count   = strlen((const char*)month_name.cstring);
  
  String result;
  if (include_ms)
  {
    result = Sf(arena, "%04u-" S_FMT "-%02u %02u:%02u:%02u.%03u",
      dt.year, month_name.count, month_name.cstring, dt.day,dt.hour, dt.minute, dt.second, dt.millisecond);
  }
  else
  {
    result = Sf(arena, "%04u-" S_FMT "-%02u %02u:%02u:%02u",
      dt.year, month_name.count, month_name.cstring, dt.day,dt.hour, dt.minute, dt.second);
  }
  
  return result;
}

function Timer
timer_start()
{
  Timer timer;
  QueryPerformanceCounter((LARGE_INTEGER*)&timer.opaque[0]);
  return timer;
}

function u64
timer_microseconds(Timer *timer)
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  u64 start = timer->opaque[0];
  return ((now.QuadPart - start) * 1000000) / Win32PerformanceFrequency.QuadPart;
}

function u64
timer_milliseconds(Timer *timer)
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  u64 start = timer->opaque[0];
  return ((now.QuadPart - start) * 1000) / Win32PerformanceFrequency.QuadPart;
}

function f64
timer_seconds(Timer *timer)
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  u64 start = timer->opaque[0];
  return (f64)(now.QuadPart - start) / (f64)Win32PerformanceFrequency.QuadPart;
}

function void
timer_reset(Timer *timer)
{
  QueryPerformanceCounter((LARGE_INTEGER*)&timer->opaque[0]);
}

#else
# error Operating System not supported
#endif


#endif // TIMING_H
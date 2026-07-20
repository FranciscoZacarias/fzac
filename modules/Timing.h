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

b8 TimingInited = false;

fz_function void      time_init(); /* Initializes timer module */
fz_function u64       time_microseconds(); /* Time in microseconds */
fz_function u64       time_milliseconds(); /* Time in milliseconds */
fz_function f64       time_seconds(); /* Time inseconds */
fz_function u64       get_epoch_microseconds(); /* Wall clock time since unix epoch (1970-01-01) in microseconds */
fz_function Date_Time datetime_now(); /* Current local date and time */
fz_function String    datetime_to_string(Arena* arena, Date_Time date, b32 include_ms); /* Returns a verbose datetime string */

fz_function Timer timer_start(); /* Returns a started timer */
fz_function u64   timer_microseconds(Timer *timer); /* Returns timer's elapsed time in microseconds */
fz_function u64   timer_milliseconds(Timer *timer); /* Returns timer's elapsed time in milliseconds */
fz_function f64   timer_seconds(Timer *timer); /* Returns timer's elapsed time in seconds */
fz_function void  timer_reset(Timer *timer); /* Resets a timer */

fz_function f64 milliseconds_from_microseconds(u64 us);
fz_function f64 seconds_from_microseconds(u64 us);

fz_function f64
milliseconds_from_microseconds(u64 us)
{
  f64 result = (f64)(us) / 1000.0;
  return result;
}

fz_function f64
seconds_from_microseconds(u64 us)
{
  return (f64)us / 1000000.0;
}

#if OS_WINDOWS

fz_global LARGE_INTEGER Win32PerformanceFrequency;

fz_function void
time_init() // @TODO(Fz): This should just be inited by default?
{
  if (!TimingInited)
  {
    QueryPerformanceFrequency(&Win32PerformanceFrequency);
    TimingInited = true;
  }
}

fz_function u64
time_microseconds()
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (counter.QuadPart * 1000000) / Win32PerformanceFrequency.QuadPart;
}

fz_function u64
time_milliseconds()
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (counter.QuadPart * 1000) / Win32PerformanceFrequency.QuadPart;
}

fz_function f64
time_seconds()
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (f64)counter.QuadPart / (f64)Win32PerformanceFrequency.QuadPart;
}

fz_function u64
get_epoch_microseconds()
{
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  ULARGE_INTEGER uli = {ft.dwLowDateTime, ft.dwHighDateTime};
  return (uli.QuadPart - 116444736000000000ULL) / 10;
}

fz_function Date_Time
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

fz_function String
datetime_to_string(Arena *arena, Date_Time dt, b32 include_ms)
{
  fz_local_persist const char* months[] = 
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
    result = Sf(arena, "%04u-" S_FMT "-%02u %02u:%02u:%02u.%03u", dt.year, month_name.count, month_name.cstring, dt.day,dt.hour, dt.minute, dt.second, dt.millisecond);
  }
  else
  {
    result = Sf(arena, "%04u-" S_FMT "-%02u %02u:%02u:%02u", dt.year, month_name.count, month_name.cstring, dt.day,dt.hour, dt.minute, dt.second);
  }
  
  return result;
}

fz_function Timer
timer_start()
{
  Timer timer;
  QueryPerformanceCounter((LARGE_INTEGER*)&timer.opaque[0]);
  return timer;
}

fz_function u64
timer_microseconds(Timer *timer)
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  u64 start = timer->opaque[0];
  return ((now.QuadPart - start) * 1000000) / Win32PerformanceFrequency.QuadPart;
}

fz_function u64
timer_milliseconds(Timer *timer)
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  u64 start = timer->opaque[0];
  return ((now.QuadPart - start) * 1000) / Win32PerformanceFrequency.QuadPart;
}

fz_function f64
timer_seconds(Timer *timer)
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  u64 start = timer->opaque[0];
  return (f64)(now.QuadPart - start) / (f64)Win32PerformanceFrequency.QuadPart;
}

fz_function void
timer_reset(Timer *timer)
{
  QueryPerformanceCounter((LARGE_INTEGER*)&timer->opaque[0]);
}

#else
# error Operating System not supported
#endif


#endif // TIMING_H
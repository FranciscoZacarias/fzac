#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

// @Section: Dynamic String Buffer
#include <stdarg.h>
#define STB_SPRINTF_IMPLEMENTATION
#include "Extern/stb_sprintf.h"

typedef struct String_Buffer String_Buffer;
struct String_Buffer
{
  u8* data;
  u64 count;
  u64 capacity;
  Allocator *allocator;
};

function void string_buffer_init(String_Buffer *buffer, Allocator *allocator, u64 initial_capacity); /* Initializes the buffer */
function void string_buffer_push(String_Buffer *buffer, const char *fmt, ...); /* Adds data to the buffer */
function void string_buffer_free(String_Buffer *buffer); /* Frees the buffer */
function String string_buffer_to_string(Arena* arena, String_Buffer *buffer); /* Copies the contents of buffer into a string */

function void
string_buffer_init(String_Buffer *buffer, Allocator *allocator, u64 initial_capacity)
{
  buffer->allocator = allocator;
  buffer->count     = 0;
  buffer->capacity  = initial_capacity;

  if (initial_capacity > 0)
  {
    buffer->data = (u8*)allocator->alloc_no_zero(initial_capacity, allocator->context);
  }
  else
  {
    buffer->data = NULL;
  }
}

function void
string_buffer_push(String_Buffer *buffer, const char* fmt, ...)
{
  if (buffer->data == NULL)
  {
    // @TODO(Fz): Log error
    printf("Buffer not initialized");
    return;
  }

  for (;;)
  {
    u64 available = buffer->capacity - buffer->count;

    va_list args;
    va_start(args, fmt);
    int written = stbsp_vsnprintf((char*)(buffer->data + buffer->count), (int)available, fmt, args);
    va_end(args);

    if (written < 0) return;
    if ((u64)written < available)
    {
      buffer->count += (u64)written;
      return;
    }

    u64 new_capacity = buffer->capacity > 0 ? buffer->capacity * 2 : 64;
    while (new_capacity < buffer->count + (u64)written) new_capacity *= 2;

    u8* new_data = (u8*)buffer->allocator->alloc_no_zero(new_capacity, buffer->allocator->context);
    if (buffer->data)
    {
      memory_copy(new_data, buffer->data, buffer->count); // copy old data
      buffer->allocator->free(buffer->capacity, buffer->data, buffer->allocator->context);
    }

    buffer->data     = new_data;
    buffer->capacity = new_capacity;
  }
}

function void
string_buffer_free(String_Buffer *buffer)
{
  if (buffer->data)
  {
    buffer->allocator->free(buffer->capacity, buffer->data, buffer->allocator->context);
    buffer->data = NULL;
  }

  buffer->count    = 0;
  buffer->capacity = 0;
}

function String
string_buffer_to_string(Arena* arena, String_Buffer *buffer)
{
  String result = {0};

  if (buffer == 0 || buffer->count == 0) return result;

  u8* data = arena_push(arena, u8, buffer->count + 1);
  memory_copy(data, buffer->data, buffer->count);
  data[buffer->count] = 0;

  result.count   = buffer->count;
  result.cstring = data;

  return result;
}

#endif // DATA_STRUCTURES_H
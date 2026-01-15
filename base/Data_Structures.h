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
  Allocator* allocator;
};

function void string_buffer_init(String_Buffer* buffer, Allocator* allocator, u64 initial_capacity); /* Initializes the buffer */
function void string_buffer_push(String_Buffer* buffer, const char* fmt, ...); /* Adds data to the buffer */
function void string_buffer_free(String_Buffer* buffer); /* Frees the buffer */
function String string_buffer_to_string(Arena* arena, String_Buffer* buffer); /* Copies the contents of buffer into a string */

function void
string_buffer_init(String_Buffer* buffer, Allocator* allocator, u64 initial_capacity)
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
string_buffer_push(String_Buffer* buffer, const char* fmt, ...)
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
string_buffer_free(String_Buffer* buffer)
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
string_buffer_to_string(Arena* arena, String_Buffer* buffer)
{
  String result = {0};

  if (buffer == 0 || buffer->count == 0) return result;

  u8* data = push_array(arena, u8, buffer->count + 1);
  memory_copy(data, buffer->data, buffer->count);
  data[buffer->count] = 0;

  result.count   = buffer->count;
  result.cstring = data;

  return result;
}

// @Section: STDLIB backed array

#define ArrayType(T) T##_Array
#define Make_Array_Type(T)                  \
  typedef struct ArrayType(T) ArrayType(T); \
  raddbg_type_view(T##_Array, data, count); \
  struct ArrayType(T) {                     \
    T* data;                                \
    u64 count;                              \
    u64 capacity;                           \
  } 

#define array_make(type, cap)                   \
  (ArrayType(type)) {                           \
    .data = (type*)calloc((cap), sizeof(type)), \
    .count = 0,                                 \
    .capacity = cap                             \
  }

#define array_make_no_zero(type, cap)            \
  (ArrayType(type)){                             \
    .data = (type*)malloc(sizeof(type) * (cap)), \
    .count = 0,                                  \
    .capacity = cap                              \
  }

#define array_free(arr_pointer)    \
  statement(                       \
    if ((arr_pointer)->data) {     \
      free((arr_pointer)->data);   \
      (arr_pointer)->data = NULL;  \
      (arr_pointer)->count = 0;    \
      (arr_pointer)->capacity = 0; \
    }                              \
  )

#define array_ensure_capacity(arr_pointer, type)                                    \
  statement(                                                                        \
    if ((arr_pointer)->count >= (arr_pointer)->capacity) {                          \
      u64 new_cap = (arr_pointer)->capacity == 0 ? 8 : (arr_pointer)->capacity * 2; \
      type* new_data = (type*)realloc((arr_pointer)->data, sizeof(type) * new_cap); \
      if (new_data) {                                                               \
        (arr_pointer)->data = new_data;                                             \
        (arr_pointer)->capacity = new_cap;                                          \
      }                                                                             \
    }                                                                               \
  )

#define array_push(arr_pointer, type, value)                                  \
  statement(                                                                  \
    array_ensure_capacity(arr_pointer, type);                                 \
    if ((arr_pointer)->count < (arr_pointer)->capacity) {                     \
      (arr_pointer)->data[(arr_pointer)->count++] = (value);                  \
    }                                                                         \
  )

#define array_get(arr_pointer, index)              ((arr_pointer)->data[index])
#define array_get_safe(arr_pointer, index)         ((index) < (arr_pointer)->count ? &(arr_pointer)->data[index] : NULL)
#define array_unordered_remove(arr_pointer, index) ((index) < (arr_pointer)->count ? ((arr_pointer)->data[index] = (arr_pointer)->data[--(arr_pointer)->count], 1) : 0)

#define array_reserve(arr_pointer, type, new_cap)                                     \
  statement(                                                                          \
    if ((new_cap) > (arr_pointer)->capacity) {                                        \
      type* new_data = (type*)realloc((arr_pointer)->data, sizeof(type) * (new_cap)); \
      if (new_data) {                                                                 \
        (arr_pointer)->data = new_data;                                               \
        (arr_pointer)->capacity = new_cap;                                            \
      }                                                                               \
    }                                                                                 \
  )

#define array_get_next(arr_pointer, type, out_ptr)              \
  statement(                                                    \
  array_ensure_capacity(arr_pointer, type);                     \
  if ((arr_pointer)->count < (arr_pointer)->capacity) {         \
    (out_ptr) = &((arr_pointer)->data[(arr_pointer)->count++]); \
  } else {                                                      \
    (out_ptr) = NULL;                                           \
  })

#endif // DATA_STRUCTURES_H
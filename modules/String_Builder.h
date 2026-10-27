#ifndef FZAC_STRING_BUILDER_H
#define FZAC_STRING_BUILDER_H

raddbg_type_view(String_Builder, (data,count));
typedef struct String_Builder String_Builder;
struct String_Builder
{
  Arena* arena;
  u8*    data;
  u64    count;
  u64    capacity;
};

fz_internal String_Builder string_builder_init(Arena* arena, u64 initial_capacity);           /* Initializes a builder that allocates from the caller's arena. Memory lives until the caller releases the arena */
fz_internal void           string_builder_pushf(String_Builder* builder, const char* fmt, ...); /* Appends a formatted cstring */
fz_internal void           string_builder_push(String_Builder* builder, const char* text);      /* Appends a literal cstring */
fz_internal void           string_builder_push_string(String_Builder* builder, String str);     /* Appends a String */
fz_internal void           string_builder_push_char(String_Builder* builder, u8 c);             /* Appends a single character */
fz_internal void           string_builder_clear(String_Builder* builder);                       /* Resets write position, keeps the current block */
fz_internal String         string_builder_to_string(Arena* arena, String_Builder* builder);     /* Copies builder contents into a null-terminated string on arena */

// @Section: Implementation

/* Grows the builder to fit at least <needed> bytes of capacity.
   Pushes a new block into the builder's arena and copies existing content.
   The old block is abandoned and is reclaimed when the caller releases the arena. */
fz_internal void
_string_builder_grow(String_Builder* builder, u64 needed)
{
  u64 new_capacity = builder->capacity > 0 ? builder->capacity : 64;
  while (new_capacity < needed) { new_capacity *= 2; }

  u8* new_data = push_array_no_zero(builder->arena, u8, new_capacity);
  if (builder->data && builder->count > 0)
  {
    memory_copy(new_data, builder->data, builder->count);
  }

  builder->data     = new_data;
  builder->capacity = new_capacity;
}

fz_internal String_Builder
string_builder_init(Arena* arena, u64 initial_capacity)
{
  assert(arena);

  String_Builder builder = {0};
  builder.arena = arena;
  if (initial_capacity > 0)
  {
    builder.data     = push_array_no_zero(arena, u8, initial_capacity);
    builder.capacity = initial_capacity;
  }
  return builder;
}

fz_internal void
string_builder_pushf(String_Builder* builder, const char* fmt, ...)
{
  for (;;)
  {
    if (builder->count >= builder->capacity)
    {
      _string_builder_grow(builder, builder->count + 1);
    }

    u64 available = builder->capacity - builder->count;

    va_list args;
    va_start(args, fmt);
    int written = stbsp_vsnprintf((char*)(builder->data + builder->count), (int)available, fmt, args);
    va_end(args);

    if (written < 0) { return; }

    if ((u64)written < available)
    {
      builder->count += (u64)written;
      return;
    }

    _string_builder_grow(builder, builder->count + (u64)written + 1);
  }
}

fz_internal void
string_builder_push(String_Builder* builder, const char* text)
{
  u64 len = strlen(text);
  if (len == 0) { return; }

  if (builder->count + len > builder->capacity)
  {
    _string_builder_grow(builder, builder->count + len);
  }

  memory_copy(builder->data + builder->count, text, len);
  builder->count += len;
}

fz_internal void
string_builder_push_string(String_Builder* builder, String str)
{
  if (str.count == 0) { return; }

  if (builder->count + str.count > builder->capacity)
  {
    _string_builder_grow(builder, builder->count + str.count);
  }

  memory_copy(builder->data + builder->count, str.cstring, str.count);
  builder->count += str.count;
}

fz_internal void
string_builder_push_char(String_Builder* builder, u8 c)
{
  if (builder->count + 1 > builder->capacity)
  {
    _string_builder_grow(builder, builder->count + 1);
  }

  builder->data[builder->count] = c;
  builder->count += 1;
}

fz_internal void
string_builder_clear(String_Builder* builder)
{
  builder->count = 0;
}

fz_internal String
string_builder_to_string(Arena* arena, String_Builder* builder)
{
  u8* data = push_array_no_zero(arena, u8, builder->count + 1);
  memory_copy(data, builder->data, builder->count);
  data[builder->count] = 0;

  String result = { builder->count, data };
  return result;
}

#endif // FZAC_STRING_BUILDER_H

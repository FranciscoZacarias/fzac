#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

/* TODO(fz): This should not own an arena, it should receive an arena from the user. */

raddbg_type_view(String_Builder, (data,count));
typedef struct String_Builder String_Builder;
struct String_Builder
{
  Arena* arena;
  u8*    data;
  u64    count;
  u64    capacity;
};

fz_function String_Builder string_builder_init(u64 initial_capacity);                        /* Allocates an internal arena and initializes the builder */
fz_function void           string_builder_pushf(String_Builder* builder, const char* fmt, ...); /* Appends a formatted cstring */
fz_function void           string_builder_push(String_Builder* builder, const char* text);      /* Appends a literal cstring */
fz_function void           string_builder_push_string(String_Builder* builder, String str);     /* Appends a String */
fz_function void           string_builder_push_char(String_Builder* builder, u8 c);             /* Appends a single character */
fz_function void           string_builder_clear(String_Builder* builder);                       /* Resets write position, keeps allocated memory */
fz_function void           string_builder_free(String_Builder* builder);                        /* Frees the internal arena, invalidates all builder memory */
fz_function String         string_builder_to_string(Arena* arena, String_Builder* builder);     /* Copies builder contents into a permanent string on arena */

// @Section: Implementation

/* Grows the builder to fit at least <needed> bytes of capacity.
   Pushes a new block into the internal arena and copies existing content.
   The old block is abandoned — arena_clear on string_builder_free reclaims it. */
fz_function b8
_string_builder_grow(String_Builder* builder, u64 needed)
{
  u64 new_capacity = builder->capacity > 0 ? builder->capacity : 64;
  while (new_capacity < needed) { new_capacity *= 2; }

  u8* new_data = push_array_no_zero(builder->arena, u8, new_capacity);
  if (!new_data)
  {
    assert(!"_string_builder_grow: internal arena out of memory");
    return 0;
  }

  if (builder->data && builder->count > 0)
  {
    memory_copy(new_data, builder->data, builder->count);
  }

  builder->data     = new_data;
  builder->capacity = new_capacity;
  return 1;
}

fz_function String_Builder
string_builder_init(u64 initial_capacity)
{
  String_Builder builder = {0};

  builder.arena = arena_alloc();
  if (!builder.arena)
  {
    assert(!"string_builder_init: failed to allocate internal arena");
    return builder;
  }

  builder.count    = 0;
  builder.capacity = initial_capacity;
  builder.data     = NULL;

  if (initial_capacity > 0)
  {
    builder.data = push_array_no_zero(builder.arena, u8, initial_capacity);
    if (!builder.data)
    {
      assert(!"string_builder_init: failed to allocate initial block");
      arena_free(builder.arena);
      builder.arena    = NULL;
      builder.capacity = 0;
    }
  }

  return builder;
}

fz_function void
string_builder_pushf(String_Builder* builder, const char* fmt, ...)
{
  if (!builder || !builder->arena || !fmt) { return; }

  for (;;)
  {
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

    if (!_string_builder_grow(builder, builder->count + (u64)written + 1)) { return; }
  }
}

fz_function void
string_builder_push(String_Builder* builder, const char* text)
{
  if (!builder || !builder->arena || !text) { return; }

  u64 len = strlen(text);
  if (len == 0) { return; }

  if (builder->count + len > builder->capacity)
  {
    if (!_string_builder_grow(builder, builder->count + len)) { return; }
  }

  memory_copy(builder->data + builder->count, text, len);
  builder->count += len;
}

fz_function void
string_builder_push_string(String_Builder* builder, String str)
{
  if (!builder || !builder->arena || !str.cstring || str.count == 0) { return; }

  if (builder->count + str.count > builder->capacity)
  {
    if (!_string_builder_grow(builder, builder->count + str.count)) { return; }
  }

  memory_copy(builder->data + builder->count, str.cstring, str.count);
  builder->count += str.count;
}

fz_function void
string_builder_push_char(String_Builder* builder, u8 c)
{
  if (!builder || !builder->arena) { return; }

  if (builder->count + 1 > builder->capacity)
  {
    if (!_string_builder_grow(builder, builder->count + 1)) { return; }
  }

  builder->data[builder->count] = c;
  builder->count += 1;
}

fz_function void
string_builder_clear(String_Builder* builder)
{
  if (!builder || !builder->arena) { return; }

  /* Reset the write position and clear the internal arena, reclaiming any
     blocks abandoned during growth. The next push will allocate fresh. */
  builder->count    = 0;
  builder->data     = NULL;
  builder->capacity = 0;
  arena_clear(builder->arena);
}

fz_function void
string_builder_free(String_Builder* builder)
{
  if (!builder || !builder->arena) { return; }

  arena_free(builder->arena);

  builder->arena    = NULL;
  builder->data     = NULL;
  builder->count    = 0;
  builder->capacity = 0;
}

fz_function String
string_builder_to_string(Arena* arena, String_Builder* builder)
{
  if (!builder || builder->count == 0)
  {
    return S("");
  }

  String result = {0};

  if (!builder || !builder->arena || builder->count == 0) { return result; }
  if (!arena)
  {
    assert(!"string_builder_to_string: arena is NULL");
    return result;
  }

  u8* data = push_array(arena, u8, builder->count + 1);
  if (!data)
  {
    assert(!"string_builder_to_string: failed to allocate string from arena");
    return result;
  }

  memory_copy(data, builder->data, builder->count);
  data[builder->count] = 0;

  result.count   = builder->count;
  result.cstring = data;
  return result;
}

#endif // STRING_BUILDER_H
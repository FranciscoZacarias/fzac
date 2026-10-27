#ifndef FZAC_ARRAY_H
#define FZAC_ARRAY_H

/*
  Static_Array(Entity) entities;
  static_array_init_arena(&entities, arena, 128);
  static_array_add(&entities, entity);
  s64 index = static_array_find(&entities, &entity);
  static_array_remove_unordered(&entities, index);

  Dynamic_Array(u32) numbers;
  dynamic_array_init(&numbers);
  dynamic_array_add(&numbers, 42);
  dynamic_array_free(&numbers);

  find and add_if_unique take a pointer to the item and compare its bytes.
*/

// Static Array ------------------------------------------------------------------------

#define Static_Array(T)  \
struct                   \
{                        \
  T*  data;              \
  u64 count;             \
  u64 capacity;          \
}

typedef struct Static_Array_Base Static_Array_Base;
struct Static_Array_Base
{
  void* data;
  u64 count;
  u64 capacity;
};

#define _static_array_base(array) ((Static_Array_Base*)(array))
#define _array_element_size(array) sizeof(*(array)->data)

#define static_array_init(array, memory, cap)        static_array_init_impl(_static_array_base(array), (memory), (cap))
#define static_array_init_arena(array, arena, cap)   static_array_init_impl(_static_array_base(array), _push_array_no_zero((arena), _array_element_size(array) * (cap)), (cap))
#define static_array_add(array, value)               (_static_array_push_impl(_static_array_base(array)), (array)->data[(array)->count - 1] = (value))
#define static_array_add_if_unique(array, item_ptr)  _static_array_add_if_unique_impl(_static_array_base(array), _array_element_size(array), (item_ptr))
#define static_array_remove(array, index)            _array_remove_impl((array)->data, &(array)->count, _array_element_size(array), (index))
#define static_array_remove_unordered(array, index)  _array_remove_unordered_impl((array)->data, &(array)->count, _array_element_size(array), (index))
#define static_array_insert_at(array, index, value)  (ArrayInsertIndex = _static_array_insert_at_impl(_static_array_base(array), _array_element_size(array), (index)), (array)->data[ArrayInsertIndex] = (value))
#define static_array_find(array, item_ptr)           _array_find_impl((array)->data, (array)->count, _array_element_size(array), (item_ptr))
#define static_array_reset(array)                    ((array)->count = 0)

// Dynamic Array ------------------------------------------------------------------------

#define Dynamic_Array(T) \
struct                   \
{                        \
  T* data;               \
  u64 count;             \
  u64 capacity;          \
  Allocator allocator;   \
}

typedef struct Dynamic_Array_Base Dynamic_Array_Base;
struct Dynamic_Array_Base
{
  void* data;
  u64 count;
  u64 capacity;
  Allocator allocator;
};

#define _dynamic_array_base(array) ((Dynamic_Array_Base*)(array))

#define dynamic_array_init(array)                      dynamic_array_init_impl(_dynamic_array_base(array), MallocAllocator)
#define dynamic_array_init_allocator(array, allocator) dynamic_array_init_impl(_dynamic_array_base(array), (allocator))
#define dynamic_array_free(array)                      dynamic_array_free_impl(_dynamic_array_base(array), _array_element_size(array))
#define dynamic_array_add(array, value)                (dynamic_array_maybe_grow_impl(_dynamic_array_base(array), _array_element_size(array), 1), (array)->data[(array)->count++] = (value))
#define dynamic_array_add_if_unique(array, item_ptr)   _dynamic_array_add_if_unique_impl(_dynamic_array_base(array), _array_element_size(array), (item_ptr))
#define dynamic_array_remove(array, index)             _array_remove_impl((array)->data, &(array)->count, _array_element_size(array), (index))
#define dynamic_array_remove_unordered(array, index)   _array_remove_unordered_impl((array)->data, &(array)->count, _array_element_size(array), (index))
#define dynamic_array_insert_at(array, index, value)   (ArrayInsertIndex = _dynamic_array_insert_at_impl(_dynamic_array_base(array), _array_element_size(array), (index)), (array)->data[ArrayInsertIndex] = (value))
#define dynamic_array_find(array, item_ptr)            _array_find_impl((array)->data, (array)->count, _array_element_size(array), (item_ptr))
#define dynamic_array_reset(array)                     ((array)->count = 0)
#define dynamic_array_reserve(array, capacity)         dynamic_array_reserve_impl(_dynamic_array_base(array), _array_element_size(array), (capacity))
#define dynamic_array_maybe_grow(array, additional)    dynamic_array_maybe_grow_impl(_dynamic_array_base(array), _array_element_size(array), (additional))

// Shared ------------------------------------------------------------------------------

C_LINKAGE thread_static u64 ArrayInsertIndex = 0; /* Lets insert_at evaluate its index argument once */

fz_internal s64
_array_find_impl(void* data, u64 count, u64 element_size, void* item)
{
  for (u64 i = 0; i < count; i += 1)
  {
    if (memory_equals((u8*)data + i * element_size, item, element_size))
    {
      return (s64)i;
    }
  }
  return -1;
}

fz_internal void
_array_check_index(u64 index, u64 count)
{
  if (index >= count)
  {
    char message[128];
    stbsp_snprintf(message, sizeof(message), "Array index out of bounds: index=%llu count=%llu", index, count);
    fatal_error(message);
  }
}

fz_internal void
_array_remove_impl(void* data, u64* count, u64 element_size, u64 index)
{
  _array_check_index(index, *count);
  u8* element = (u8*)data + index * element_size;
  memory_move(element, element + element_size, (*count - index - 1) * element_size);
  *count -= 1;
}

fz_internal void
_array_remove_unordered_impl(void* data, u64* count, u64 element_size, u64 index)
{
  _array_check_index(index, *count);
  u64 last = *count - 1;
  if (index != last)
  {
    memory_copy((u8*)data + index * element_size, (u8*)data + last * element_size, element_size);
  }
  *count -= 1;
}

// Static Array implementation ---------------------------------------------------------

fz_internal void
static_array_init_impl(Static_Array_Base* array, void* memory, u64 capacity)
{
  array->data     = memory;
  array->count    = 0;
  array->capacity = capacity;
}

fz_internal void
_static_array_push_impl(Static_Array_Base* array)
{
  if (array->count >= array->capacity)
  {
    char message[128];
    stbsp_snprintf(message, sizeof(message), "Static_Array overflow: capacity=%llu", array->capacity);
    fatal_error(message);
  }
  array->count += 1;
}

fz_internal s64
_static_array_add_if_unique_impl(Static_Array_Base* array, u64 element_size, void* item)
{
  s64 index = _array_find_impl(array->data, array->count, element_size, item);
  if (index < 0)
  {
    _static_array_push_impl(array);
    index = (s64)array->count - 1;
    memory_copy((u8*)array->data + (u64)index * element_size, item, element_size);
  }
  return index;
}

fz_internal u64
_static_array_insert_at_impl(Static_Array_Base* array, u64 element_size, u64 index)
{
  _array_check_index(index, array->count + 1);
  _static_array_push_impl(array);
  u8* element = (u8*)array->data + index * element_size;
  memory_move(element + element_size, element, (array->count - 1 - index) * element_size);
  return index;
}

// Dynamic Array implementation --------------------------------------------------------

fz_internal void
dynamic_array_init_impl(Dynamic_Array_Base* array, Allocator allocator)
{
  array->data      = NULL;
  array->count     = 0;
  array->capacity  = 0;
  array->allocator = allocator;
}

fz_internal void
dynamic_array_free_impl(Dynamic_Array_Base* array, u64 element_size)
{
  if (array->data)
  {
    array->allocator.free(array->capacity * element_size, array->data, array->allocator.context);
  }

  array->data = NULL;
  array->count = 0;
  array->capacity = 0;
}

fz_internal void
dynamic_array_reserve_impl(Dynamic_Array_Base* array, u64 element_size, u64 capacity)
{
  if (capacity <= array->capacity)
  {
    return;
  }

  u64 bytes = capacity * element_size;
  void* new_memory = array->allocator.alloc_no_zero(bytes, array->allocator.context);
  if (!new_memory)
  {
    char message[128];
    stbsp_snprintf(message, sizeof(message), "Dynamic_Array failed to allocate %llu bytes", bytes);
    fatal_error(message);
  }

  if (array->data)
  {
    memory_copy(new_memory, array->data, array->count * element_size);
    array->allocator.free(array->capacity * element_size, array->data, array->allocator.context);
  }

  array->data = new_memory;
  array->capacity = capacity;
}

fz_internal void
dynamic_array_maybe_grow_impl(Dynamic_Array_Base* array, u64 element_size, u64 additional_elements)
{
  u64 required = array->count + additional_elements;
  if (required <= array->capacity)
  {
    return;
  }

  u64 new_capacity = (array->capacity == 0) ? 8 : array->capacity;
  while (new_capacity < required)
  {
    new_capacity *= 2;
  }

  dynamic_array_reserve_impl(array, element_size, new_capacity);
}

fz_internal s64
_dynamic_array_add_if_unique_impl(Dynamic_Array_Base* array, u64 element_size, void* item)
{
  s64 index = _array_find_impl(array->data, array->count, element_size, item);
  if (index < 0)
  {
    dynamic_array_maybe_grow_impl(array, element_size, 1);
    index = (s64)array->count;
    memory_copy((u8*)array->data + (u64)index * element_size, item, element_size);
    array->count += 1;
  }
  return index;
}

fz_internal u64
_dynamic_array_insert_at_impl(Dynamic_Array_Base* array, u64 element_size, u64 index)
{
  _array_check_index(index, array->count + 1);
  dynamic_array_maybe_grow_impl(array, element_size, 1);
  u8* element = (u8*)array->data + index * element_size;
  memory_move(element + element_size, element, (array->count - index) * element_size);
  array->count += 1;
  return index;
}

#endif // FZAC_ARRAY_H

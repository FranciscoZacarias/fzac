#ifndef FZ_ARRAY_H
#define FZ_ARRAY_H

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

#define static_array_init(array, memory, cap) static_array_init_impl((Static_Array_Base*)(array), (memory), (cap))
#define static_array_add()
#define static_array_add_if_unique()
#define static_array_remove()
#define static_array_remove_unordered()
#define static_array_insert_at()
#define static_array_find()
#define static_array_reset()

fz_function void
static_array_init_impl(Static_Array_Base* array, void* memory, u64 capacity)
{
  array->data     = memory;
  array->count    = 0;
  array->capacity = capacity;
}

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

#define dynamic_array_init(array)                      dynamic_array_init_impl((Dynamic_Array_Base*)(array), MallocAllocator)
#define dynamic_array_init_allocator(array, allocator) dynamic_array_init_impl((Dynamic_Array_Base*)(array), (allocator))
#define dynamic_array_free(array, element_size)        dynamic_array_free_impl((Dynamic_Array_Base*)(array), (element_size))
#define dynamic_array_add()
#define dynamic_array_add_if_unique()
#define dynamic_array_remove()
#define dynamic_array_remove_unordered()
#define dynamic_array_insert_at()
#define dynamic_array_find()
#define dynamic_array_reset(array)                                dynamic_array_reset_impl((Dynamic_Array_Base*)(array))
#define dynamic_array_reserve(array, element_size, capacity)      dynamic_array_reserve_impl((Dynamic_Array_Base*)(array), (element_size), (capacity))
#define dynamic_array_maybe_grow(array, element_size, additional) dynamic_array_maybe_grow_impl((Dynamic_Array_Base*)(array), (element_size), (additional))

fz_function void
dynamic_array_init_impl(Dynamic_Array_Base* array, Allocator allocator)
{
  array->data      = NULL;
  array->count     = 0;
  array->capacity  = 0;
  array->allocator = allocator;
}

fz_function void
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

fz_function void
dynamic_array_reset_impl(Dynamic_Array_Base* array)
{
  array->count = 0;
}

fz_function b32
dynamic_array_reserve_impl(Dynamic_Array_Base* array, u64 element_size, u64 capacity)
{
  if (capacity <= array->capacity)
  {
    return true;
  }

  u64 bytes = capacity * element_size;
  void* new_memory = array->allocator.alloc(bytes, array->allocator.context);
  if (!new_memory)
  {
    return false;
  }

  if (array->data)
  {
    memory_copy(new_memory, array->data, array->count * element_size);
    array->allocator.free(array->capacity * element_size, array->data, array->allocator.context);
  }

  array->data = new_memory;
  array->capacity = capacity;

  return true;
}

fz_function b32
dynamic_array_maybe_grow_impl(Dynamic_Array_Base* array, u64 element_size, u64 additional_elements)
{
  u64 required = array->count + additional_elements;

  if (required <= array->capacity)
  {
    return true;
  }

  u64 new_capacity = (array->capacity == 0) ? 8 : array->capacity;

  while (new_capacity < required)
  {
    new_capacity *= 2;
  }

  return dynamic_array_reserve_impl(array, element_size, new_capacity);
}



#endif // FZ_ARRAY_H

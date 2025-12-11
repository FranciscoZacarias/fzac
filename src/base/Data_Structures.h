#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

// @Section: Array

#define ARRAY_INITIAL_CAPACITY 4

typedef struct Array_Header Array_Header;
struct Array_Header
{
  u64 size;
  u64 capacity;
  Allocator* allocator;
};

#define array(type, allocator)                 _array_init_implementation(sizeof(type), ARRAY_INITIAL_CAPACITY, allocator)
#define array_sized(type, allocator, capacity) _array_init_implementation(sizeof(type), capacity, allocator)
#define array_add(array, value) ( \
  (array) = _array_ensure_capacity(array, 1, sizeof(value)), \
  (array)[array_header(array)->size] = (value), \
  &(array)[array_header(array)->size++])

#define array_header(a)  ((Array_Header *)(a) - 1)
#define array_size(a)     (array_header(a)->size)
#define array_capacity(a) (array_header(a)->capacity)

function void*
_array_init_implementation(u64 size, u64 capacity, Allocator* allocator)
{
  void* ptr = NULL;
  u64 bytes = sizeof(Array_Header) + (size * capacity);
  Array_Header* header = allocator->alloc(bytes, allocator->context);
  if (header)
  {
    header->size      = 0;
    header->capacity  = capacity;
    header->allocator = allocator;
    ptr = header + 1;
  }
  else
  {
    // @TODO(fz): Handle failed memory allocation
  }
  return ptr;
}

function void* 
_array_ensure_capacity(void* array, u64 item_count, u64 item_size)
{
  Array_Header* header = array_header(array);
  u64 desired_capacity = header->size + item_count;

  if (header->capacity < desired_capacity)
  {
    u64 new_capacity = header->capacity * 2;
    while (new_capacity < desired_capacity) new_capacity *= 2;

    u64 new_size = sizeof(Array_Header) + (new_capacity * item_size);
    Array_Header* new_header = header->allocator->alloc(new_size, header->allocator->context);

    if (new_header)
    {
      u64 old_size = sizeof(*header) + header->size * item_size;
      memory_copy(new_header, header, old_size);

      if (header->allocator->free)
      {
        header->allocator->free(old_size, header, header->allocator->context);
      }

      new_header->capacity = new_capacity;
      header = new_header + 1;
    } else {
      header = 0;
    }
  }
  else
  {
    header += 1;
  }

  return header;
}

function void
array_print(void* array)
{
  if (!array)
  {
    printf("array: NULL\n");
    return;
  }

  Array_Header* header = (Array_Header*)array - 1;

  printf("array: size=%llu, capacity=%llu\n",
         (unsigned long long)header->size,
         (unsigned long long)header->capacity);
}

#endif // DATA_STRUCTURES_H
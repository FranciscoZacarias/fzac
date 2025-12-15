#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

// @Section: Array

#define ARRAY_INITIAL_CAPACITY 16

typedef struct Array_Header Array_Header;
struct Array_Header
{
  u64 length;             /* Total elements in the array */
  u64 capacity;         /* Max allocated */
  u64 item_size;        /* Size of the data type. We need this because if we have an implicit cast to a different sized data type, the array breaks. I.e. if we have array of u64, doing array_add(a,1); '1' will cast to s32, with half the size that is actually expected. */
  Allocator* allocator; /* Allocator */
};

#define array_header(a)  ((Array_Header *)(a) - 1)
#define array_size(a)     (array_header(a)->length)
#define array_capacity(a) (array_header(a)->capacity)

#define array_new(type, allocator)                 _array_init_implementation(sizeof(type), ARRAY_INITIAL_CAPACITY, allocator)
#define array_new_sized(type, allocator, capacity) _array_init_implementation(sizeof(type), capacity, allocator)
#define array_free(arr)                            _array_free(arr);
#define array_add(arr, value)             ((arr) = _array_ensure_capacity(arr, 1), (arr)[array_header(arr)->length] = (value), &(arr)[array_header((arr))->length++])
#define array_print(arr)                           printf("array: size=%llu, capacity=%llu\n", (u64)((Array_Header*)arr - 1)->length, (u64)((Array_Header*)arr - 1)->capacity);

function void*
_array_init_implementation(u64 size, u64 capacity, Allocator* allocator)
{
  void* ptr = NULL;
  u64 bytes = sizeof(Array_Header) + (size * capacity);
  Array_Header* header = allocator->alloc(bytes, allocator->context);
  if (header)
  {
    header->length     = 0;
    header->capacity  = capacity;
    header->item_size = size;
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
_array_ensure_capacity(void* array, u64 item_count)
{
  Array_Header* header = array_header(array);
  u64 desired_capacity = header->length + item_count;

  if (header->capacity < desired_capacity)
  {
    u64 new_capacity = header->capacity * 2;
    while (new_capacity < desired_capacity) new_capacity *= 2;

    u64 new_size = sizeof(Array_Header) + (new_capacity * header->item_size);
    Array_Header* new_header = header->allocator->alloc(new_size, header->allocator->context);

    if (new_header)
    {
      u64 old_size = sizeof(Array_Header) + (header->length * header->item_size);
      memory_copy(new_header, header, old_size);
      if (header->allocator->free)
      {
        header->allocator->free(old_size, header, header->allocator->context);
      }
      else
      {
        // @TODO(fz): Handle not being able to free memory
      }
      new_header->capacity = new_capacity;
      header = new_header + 1;
    }
    else
    {
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
_array_free(void* array)
{
  Array_Header* header = array_header(array);
  if (header->allocator->free)
  {
    u64 size = sizeof(Array_Header) + (header->length * header->item_size);
    header->allocator->free(size, header, header->allocator->context);
  }
  else
  {
    // @TODO(fz): Handle not being able to free memory
  }
}

#endif // DATA_STRUCTURES_H
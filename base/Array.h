#ifndef ARRAY_H
#define ARRAY_H

/* @File: Array implementation backed by stdlib allocators. Not in Data_Structures because it's included before everything, as it is a more fundamental type. */

#define Array(T) T##_Array

#define Make_Array_Type(T)                  \
  typedef struct Array(T) Array(T);         \
  raddbg_type_view(T##_Array, data, count); \
  struct Array(T) {                         \
    T* data;                                \
    u64 count;                              \
    u64 capacity;                           \
  } 

#define array_make(type, cap)                   \
  (Array(type)) {                               \
    .data = (type*)calloc((cap), sizeof(type)), \
    .count = 0,                                 \
    .capacity = cap                             \
  }

#define array_make_no_zero(type, cap)            \
  (Array(type)){                                 \
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

#define array_get(arr_pointer, index) ((index) < (arr_pointer)->count ? &(arr_pointer)->data[index] : NULL)
#define array_get_next(arr_pointer, type, out_ptr)                \
  statement(                                                      \
    array_ensure_capacity(arr_pointer, type);                     \
    if ((arr_pointer)->count < (arr_pointer)->capacity) {         \
      (out_ptr) = &((arr_pointer)->data[(arr_pointer)->count++]); \
    } else {                                                      \
      (out_ptr) = NULL;                                           \
    }                                                             \
  )

#endif // ARRAY_H
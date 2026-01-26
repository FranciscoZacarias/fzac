#ifndef ARRAY_H
#define ARRAY_H

/* @File: Array implementation backed by stdlib allocator and one with arena allocator.
Not in Data_Structures because it's included before everything, as it is a more fundamental type. */

// @Section: Arena backed array 

#define ArenaArray(T, name) \
  Arena *name##_arena;      \
  T *name;                  \
  u32 name##_count;         \
  u32 name##_capacity

#define arena_array_init(ctx, T, name, initial_cap)                     \
  statement(                                                            \
    (ctx)->name##_arena = arena_alloc();                                \
    (ctx)->name = arena_push((ctx)->name##_arena, T, (initial_cap));    \
    (ctx)->name##_count = 0;                                            \
    (ctx)->name##_capacity = (initial_cap);                             \
  )

#define arena_array_ensure(ctx, T, name, scratch)                             \
  statement(                                                                  \
    if ((ctx)->name##_count + 1 >= (ctx)->name##_capacity)                    \
    {                                                                         \
      u32 new_cap = (ctx)->name##_capacity ? (ctx)->name##_capacity * 2 : 2;  \
      T *tmp = arena_push(scratch.arena, T, (ctx)->name##_count);             \
      memory_copy(tmp, (ctx)->name, sizeof(T) * (ctx)->name##_count);         \
      arena_clear((ctx)->name##_arena);                                       \
     (ctx)->name = arena_push((ctx)->name##_arena, T, new_cap);               \
     (ctx)->name##_capacity = new_cap;                                        \
      memory_copy((ctx)->name, tmp, sizeof(T) * (ctx)->name##_count);         \
    }                                                                         \
  )

#define arena_array_push(ctx, T, name, value)        \
  statement(                                         \
    arena_array_ensure(ctx, T, name);                \
    (ctx)->name[(ctx)->name##_count++] = (value);    \
  )

#define arena_array_get_next(ctx, T, name, out_ptr, scratch) \
  statement(                                                 \
    arena_array_ensure(ctx, T, name, scratch);               \
    (out_ptr) = &((ctx)->name[(ctx)->name##_count++]);       \
  )

#define arena_array_reset(ctx, name) \
  ((ctx)->name##_count = 0)

// @Section: Stdlib backed Array

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
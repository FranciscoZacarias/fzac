#ifndef FZAC_HASH_MAP_H
#define FZAC_HASH_MAP_H

/*
  Open addressing hash map backed by an arena. Grows by allocating new tables from the arena.
  Values are copied in by size. Keys are u64 or String (String keys are copied into the arena).

  Hash_Map map = hash_map_init(arena, 64, sizeof(Entity));
  hash_map_put_string(&map, S("player"), &player);
  Entity* found = hash_map_get_string(&map, S("player"));
  hash_map_remove_string(&map, S("player"));

  for (Hash_Map_Iterator it = hash_map_iterate(&map); hash_map_next(&it);)
  {
    Entity* entity = it.value;
  }
*/

typedef enum Hash_Map_Slot_State
{
  Hash_Map_Slot_Empty = 0,
  Hash_Map_Slot_Used,
  Hash_Map_Slot_Removed,
} Hash_Map_Slot_State;

typedef struct Hash_Map_Slot Hash_Map_Slot;
struct Hash_Map_Slot
{
  u64    hash;
  u64    key;
  String key_string;
  u8     state;
  b8     is_string;
};

typedef struct Hash_Map Hash_Map;
struct Hash_Map
{
  Arena*         arena;
  Hash_Map_Slot* slots;
  u8*            values;
  u64            value_size;
  u64            capacity;
  u64            count;
  u64            removed_count;
};

typedef struct Hash_Map_Iterator Hash_Map_Iterator;
struct Hash_Map_Iterator
{
  Hash_Map* map;
  u64       index;
  u64       key;
  String    key_string;
  void*     value;
};

fz_internal Hash_Map hash_map_init(Arena* arena, u64 initial_capacity, u64 value_size); /* Capacity is rounded up to a power of two */
fz_internal void*    hash_map_put_u64(Hash_Map* map, u64 key, void* value);      /* Inserts or overwrites. Returns the stored value. NULL value stores zeroes */
fz_internal void*    hash_map_get_u64(Hash_Map* map, u64 key);                   /* Returns the stored value or NULL */
fz_internal b32      hash_map_remove_u64(Hash_Map* map, u64 key);
fz_internal void*    hash_map_put_string(Hash_Map* map, String key, void* value); /* Inserts or overwrites. Returns the stored value. NULL value stores zeroes */
fz_internal void*    hash_map_get_string(Hash_Map* map, String key);             /* Returns the stored value or NULL */
fz_internal b32      hash_map_remove_string(Hash_Map* map, String key);
fz_internal void     hash_map_clear(Hash_Map* map);                               /* Removes every entry, keeps the current table */
fz_internal Hash_Map_Iterator hash_map_iterate(Hash_Map* map);
fz_internal b32               hash_map_next(Hash_Map_Iterator* iterator);         /* Advances to the next entry, false when done */

// @Section: Implementation

fz_internal u64
_hash_map_mix(u64 value)
{
  value ^= value >> 33;
  value *= 0xff51afd7ed558ccdull;
  value ^= value >> 33;
  value *= 0xc4ceb9fe1a85ec53ull;
  value ^= value >> 33;
  return value;
}

fz_internal void
_hash_map_allocate(Hash_Map* map, u64 capacity)
{
  map->slots         = push_array(map->arena, Hash_Map_Slot, capacity);
  map->values        = push_array(map->arena, u8, capacity * map->value_size);
  map->capacity      = capacity;
  map->count         = 0;
  map->removed_count = 0;
}

fz_internal Hash_Map
hash_map_init(Arena* arena, u64 initial_capacity, u64 value_size)
{
  assert(arena);
  assert(value_size > 0);

  u64 capacity = 8;
  while (capacity < initial_capacity)
  {
    capacity *= 2;
  }

  Hash_Map map = {0};
  map.arena      = arena;
  map.value_size = value_size;
  _hash_map_allocate(&map, capacity);
  return map;
}

fz_internal b32
_hash_map_slot_matches(Hash_Map_Slot* slot, u64 hash, b32 is_string, u64 key, String key_string)
{
  if (slot->state != Hash_Map_Slot_Used || slot->hash != hash || slot->is_string != is_string)
  {
    return false;
  }
  if (is_string)
  {
    return string_equals(slot->key_string, key_string, true);
  }
  return slot->key == key;
}

fz_internal s64
_hash_map_find_index(Hash_Map* map, u64 hash, b32 is_string, u64 key, String key_string)
{
  u64 mask = map->capacity - 1;
  for (u64 probe = 0; probe < map->capacity; probe += 1)
  {
    u64 index = (hash + probe) & mask;
    Hash_Map_Slot* slot = &map->slots[index];
    if (slot->state == Hash_Map_Slot_Empty)
    {
      return -1;
    }
    if (_hash_map_slot_matches(slot, hash, is_string, key, key_string))
    {
      return (s64)index;
    }
  }
  return -1;
}

fz_internal void* _hash_map_put(Hash_Map* map, u64 hash, b32 is_string, u64 key, String key_string, void* value);

fz_internal void
_hash_map_grow(Hash_Map* map)
{
  Hash_Map_Slot* old_slots    = map->slots;
  u8*            old_values   = map->values;
  u64            old_capacity = map->capacity;
  u64            new_capacity = (map->count * 2 >= old_capacity) ? old_capacity * 2 : old_capacity;

  _hash_map_allocate(map, new_capacity);
  for (u64 i = 0; i < old_capacity; i += 1)
  {
    Hash_Map_Slot* slot = &old_slots[i];
    if (slot->state == Hash_Map_Slot_Used)
    {
      _hash_map_put(map, slot->hash, slot->is_string, slot->key, slot->key_string, old_values + i * map->value_size);
    }
  }
}

fz_internal void*
_hash_map_put(Hash_Map* map, u64 hash, b32 is_string, u64 key, String key_string, void* value)
{
  s64 existing = _hash_map_find_index(map, hash, is_string, key, key_string);
  if (existing < 0)
  {
    if ((map->count + map->removed_count + 1) * 4 > map->capacity * 3)
    {
      _hash_map_grow(map);
    }

    u64 mask  = map->capacity - 1;
    u64 index = hash & mask;
    while (map->slots[index].state == Hash_Map_Slot_Used)
    {
      index = (index + 1) & mask;
    }

    Hash_Map_Slot* slot = &map->slots[index];
    if (slot->state == Hash_Map_Slot_Removed)
    {
      map->removed_count -= 1;
    }
    slot->state      = Hash_Map_Slot_Used;
    slot->hash       = hash;
    slot->is_string  = (b8)is_string;
    slot->key        = key;
    slot->key_string = key_string;
    map->count += 1;
    existing = (s64)index;
  }

  void* destination = map->values + (u64)existing * map->value_size;
  if (value)
  {
    memory_copy(destination, value, map->value_size);
  }
  else
  {
    memory_zero(destination, map->value_size);
  }
  return destination;
}

fz_internal b32
_hash_map_remove(Hash_Map* map, u64 hash, b32 is_string, u64 key, String key_string)
{
  s64 index = _hash_map_find_index(map, hash, is_string, key, key_string);
  if (index < 0)
  {
    return false;
  }
  map->slots[index].state = Hash_Map_Slot_Removed;
  map->count         -= 1;
  map->removed_count += 1;
  return true;
}

fz_internal void*
hash_map_put_u64(Hash_Map* map, u64 key, void* value)
{
  return _hash_map_put(map, _hash_map_mix(key), false, key, string_zero(), value);
}

fz_internal void*
hash_map_get_u64(Hash_Map* map, u64 key)
{
  s64 index = _hash_map_find_index(map, _hash_map_mix(key), false, key, string_zero());
  return (index < 0) ? NULL : map->values + (u64)index * map->value_size;
}

fz_internal b32
hash_map_remove_u64(Hash_Map* map, u64 key)
{
  return _hash_map_remove(map, _hash_map_mix(key), false, key, string_zero());
}

fz_internal void*
hash_map_put_string(Hash_Map* map, String key, void* value)
{
  u64 hash = _hash_map_mix(string_hash(key));
  if (_hash_map_find_index(map, hash, true, 0, key) < 0)
  {
    key = string_copy(map->arena, key);
  }
  return _hash_map_put(map, hash, true, 0, key, value);
}

fz_internal void*
hash_map_get_string(Hash_Map* map, String key)
{
  s64 index = _hash_map_find_index(map, _hash_map_mix(string_hash(key)), true, 0, key);
  return (index < 0) ? NULL : map->values + (u64)index * map->value_size;
}

fz_internal b32
hash_map_remove_string(Hash_Map* map, String key)
{
  return _hash_map_remove(map, _hash_map_mix(string_hash(key)), true, 0, key);
}

fz_internal void
hash_map_clear(Hash_Map* map)
{
  memory_zero(map->slots, sizeof(Hash_Map_Slot) * map->capacity);
  map->count         = 0;
  map->removed_count = 0;
}

fz_internal Hash_Map_Iterator
hash_map_iterate(Hash_Map* map)
{
  Hash_Map_Iterator iterator = {0};
  iterator.map   = map;
  iterator.index = U64_MAX;
  return iterator;
}

fz_internal b32
hash_map_next(Hash_Map_Iterator* iterator)
{
  Hash_Map* map = iterator->map;
  for (iterator->index += 1; iterator->index < map->capacity; iterator->index += 1)
  {
    Hash_Map_Slot* slot = &map->slots[iterator->index];
    if (slot->state == Hash_Map_Slot_Used)
    {
      iterator->key        = slot->key;
      iterator->key_string = slot->key_string;
      iterator->value      = map->values + iterator->index * map->value_size;
      return true;
    }
  }
  return false;
}

#endif // FZAC_HASH_MAP_H

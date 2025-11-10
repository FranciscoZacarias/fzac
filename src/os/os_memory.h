#ifndef OS_MEMORY_H
#define OS_MEMORY_H

typedef struct OS_Memory_Stats OS_Memory_Stats;
struct OS_Memory_Stats
{
  u64 total_physical;
  u64 available_physical;
  u64 total_virtual;
  u64 used_virtual;
};

function OS_Memory_Stats os_memory_stats(); /* Returns the current memory usage statistics of the system and process */
function void*           os_memory_reserve(u64 size); /* Reserves a block of virtual address space without committing physical memory */
function b32             os_memory_commit(void *ptr, u64 size); /* Commits physical memory to a reserved region */
function void            os_memory_decommit(void *ptr, u64 size); /* Decommits physical memory from a region, keeping the address space reserved */
function void            os_memory_release(void *ptr, u64 size); /* Releases previously reserved memory, making the address space reusable */
function u64             os_memory_get_page_size(); /* Returns the operating system's memory page size */

#endif // OS_MEMORY_H
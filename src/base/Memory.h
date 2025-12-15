#ifndef MEMORY_H
#define MEMORY_H

function void* os_memory_reserve(u64 size); /* Reserves a block of virtual address space without committing physical memory */
function b32   os_memory_commit(void *ptr, u64 size); /* Commits physical memory to a reserved region */
function void  os_memory_decommit(void *ptr, u64 size); /* Decommits physical memory from a region, keeping the address space reserved */
function void  os_memory_free(void *ptr, u64 size); /* Releases previously reserved memory, making the address space reusable */
function u64   os_memory_get_page_size(); /* Returns the operating system's memory page size */


#if OS_WINDOWS 

function void*
os_memory_reserve(u64 size)
{
  void *result = VirtualAlloc(0, (size_t)size, MEM_RESERVE, PAGE_READWRITE);
  return result;
}

function b32
os_memory_commit(void *ptr, u64 size)
{
  b32 result = (VirtualAlloc(ptr, (size_t)size, MEM_COMMIT, PAGE_READWRITE) != 0);
  return result;
}

function void
os_memory_decommit(void *ptr, u64 size)
{
  VirtualFree(ptr, (size_t)size, MEM_DECOMMIT);
}

function void
os_memory_free(void *ptr, u64 size)
{
  VirtualFree(ptr, 0, MEM_RELEASE);
}

function u64
os_memory_get_page_size()
{
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  u64 result = (u64)info.dwPageSize;
  return result;
}

#endif // OS_WINDOWS

#endif // MEMORY_H
#ifndef MEMORY_H
#define MEMORY_H

fz_internal void* memory_reserve(u64 size); /* Reserves a block of virtual address space without committing physical memory */
fz_internal b32   memory_commit(void *ptr, u64 size); /* Commits physical memory to a reserved region */
fz_internal void  memory_decommit(void *ptr, u64 size); /* Decommits physical memory from a region, keeping the address space reserved */
fz_internal void  memory_free(void *ptr, u64 size); /* Releases previously reserved memory, making the address space reusable */
fz_internal u64   memory_get_page_size(); /* Returns the operating system's memory page size */


#if OS_WINDOWS 

fz_internal void*
memory_reserve(u64 size)
{
  void *result = VirtualAlloc(0, (size_t)size, MEM_RESERVE, PAGE_READWRITE);
  return result;
}

fz_internal b32
memory_commit(void *ptr, u64 size)
{
  b32 result = (VirtualAlloc(ptr, (size_t)size, MEM_COMMIT, PAGE_READWRITE) != 0);
  return result;
}

fz_internal void
memory_decommit(void *ptr, u64 size)
{
  VirtualFree(ptr, (size_t)size, MEM_DECOMMIT);
}

fz_internal void
memory_free(void *ptr, u64 size)
{
  VirtualFree(ptr, 0, MEM_RELEASE);
}

fz_internal u64
memory_get_page_size()
{
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  u64 result = (u64)info.dwPageSize;
  return result;
}

#endif // OS_WINDOWS

#endif // MEMORY_H
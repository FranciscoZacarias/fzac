function OS_Memory_Stats
os_memory_stats()
{
  OS_Memory_Stats result = {0};

  MEMORYSTATUSEX status = {0};
  status.dwLength = sizeof(status);

  if(GlobalMemoryStatusEx(&status))
  {
    result.total_physical     = (u64)status.ullTotalPhys;
    result.available_physical = (u64)status.ullAvailPhys;
    result.total_virtual      = (u64)status.ullTotalPageFile;
    result.used_virtual       = (u64)(status.ullTotalPageFile - status.ullAvailPageFile);
  }

  return result;
}

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
os_memory_release(void *ptr, u64 size)
{
  ignore_unused(size); // NOTE(fz): Size not needed for windows. Though required for other OS's.
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
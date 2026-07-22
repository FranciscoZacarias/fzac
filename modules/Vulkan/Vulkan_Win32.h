#ifndef FZ_VULKAN_WIN32_H
#define FZ_VULKAN_WIN32_H

static HMODULE vulkan_library = NULL;

fz_function b32
vulkan_init()
{
  b32 result = false;

  vulkan_library = GetModuleHandleA("vulkan-1.dll");

  if (!vulkan_library)
  {
    vulkan_library = LoadLibraryA("vulkan-1.dll");
  }

  if (!vulkan_library)
  {
    message_box(S("Vulkan"), S("Unable to load vulkan-1.dll"), S("Vulkan"), 0);
    return result;
  }

  __vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr) GetProcAddress(vulkan_library, "vkGetInstanceProcAddr");
  if (!__vkGetInstanceProcAddr)
  {
    message_box(S("Vulkan"), S("Unable to load vkGetInstanceProcAddr"), S("Vulkan"), 0);
    return result;
  }

  __vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)GetProcAddress( vulkan_library, "vkGetDeviceProcAddr");
  if (!__vkGetDeviceProcAddr)
  {
    message_box(S("Vulkan"), S("Unable to load vkGetDeviceProcAddr"), S("Vulkan"), 0);
    return result;
  }

  result = vulkan_load_global_functions();

  return result;
}

#endif // FZ_VULKAN_WIN32_H
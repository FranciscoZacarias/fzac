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

  __vkGetDeviceProcAddr =(PFN_vkGetDeviceProcAddr) __vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkGetDeviceProcAddr");
  result = _vulkan_load_global_functions();
  return result;
}

fz_function b32
vulkan_instance_init(VkInstance instance)
{
  b32 result = _vulkan_load_instance_functions(instance);
  return result;
}

fz_function b32
vulkan_device_init(VkDevice device)
{
  b32 result = _vulkan_load_device_functions(device);
  return result;
}


#endif // FZ_VULKAN_WIN32_H
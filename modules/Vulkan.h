#ifndef FZ_VULKAN_H
#define FZ_VULKAN_H

#define VK_NO_PROTOTYPES
#include "Vulkan\sdk\vulkan.h"

PFN_vkGetInstanceProcAddr __vkGetInstanceProcAddr = NULL;
PFN_vkGetDeviceProcAddr   __vkGetDeviceProcAddr   = NULL;

fz_function b32 vulkan_init();
fz_function b32 vulkan_instance_init(VkInstance instance);
fz_function b32 vulkan_device_init(VkDevice device);

// @Section: Implementation
#include "Vulkan/generated/Vulkan.cgen.h"
#include "Vulkan/generated/Vulkan.cgen.c"

#if OS_WINDOWS
# include "Vulkan\Vulkan_Win32.h"
#else
# error Operating System not supported
#endif

#endif // FZ_VULKAN_H

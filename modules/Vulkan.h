#ifndef FZ_VULKAN_H
#define FZ_VULKAN_H

#if OS_WINDOWS
  #define VK_USE_PLATFORM_WIN32_KHR
#elif OS_LINUX
  #define VK_USE_PLATFORM_XLIB_KHR
  /* or VK_USE_PLATFORM_WAYLAND_KHR */
#elif OS_MACOS
  #define VK_USE_PLATFORM_METAL_EXT
#endif

#define VK_NO_PROTOTYPES
#include "Vulkan\sdk\vulkan.h"

PFN_vkGetInstanceProcAddr __vkGetInstanceProcAddr = NULL;
PFN_vkGetDeviceProcAddr   __vkGetDeviceProcAddr   = NULL;

fz_internal b32 vulkan_init();
VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *data, void *user);

// @Section: Implementation
#include "Vulkan/generated/Vulkan.cgen.h"
#include "Vulkan/generated/Vulkan.cgen.c"

#if OS_WINDOWS
# include "Vulkan\Vulkan_Win32.h"
#else
# error Operating System not supported
#endif

VKAPI_ATTR VkBool32 VKAPI_CALL
vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *data, void *user)
{
  const char *severity_str = "Unknown";
  Log_Level log_level = Log_Level_Trace;

  if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
  {
    severity_str = "Error";
    log_level = Log_Level_Fatal;
  }
  else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
  {
    severity_str = "Warning";
    log_level = Log_Level_Warn;
  }
  else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
  {
    severity_str = "Info";
    log_level = Log_Level_Info;
  }
  else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
  {
    severity_str = "Verbose";
    log_level = Log_Level_Trace;
  }

  const char *type_str = "Unknown";
  if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
  {
    type_str = "Validation";
  }
  else if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
  {
    type_str = "Performance";
  }
  else if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
  {
    type_str = "General";
  }

  log_write(log_level, __FILE__, __LINE__, "Vulkan [%s] [%s]\n%s", severity_str, type_str, data->pMessage);
  return VK_FALSE;
}


#endif // FZ_VULKAN_H

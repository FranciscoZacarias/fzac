/* Generated code */

#ifndef VULKAN_FUNCTIONS_H
#define VULKAN_FUNCTIONS_H

#define check_vk_result(result, message) if (result != VK_SUCCESS) { log_error("Error: %s, code %x", message, result); }

fz_function b32 vulkan_load_global_functions();
fz_function b32 vulkan_load_instance_functions(VkInstance instance);
fz_function b32 vulkan_load_device_functions(VkDevice device);

fz_global PFN_vkCreateInstance __vkCreateInstance = NULL;
fz_function VkResult
vkCreateInstance (const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
  VkResult result = __vkCreateInstance (pCreateInfo, pAllocator, pInstance);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateInstance");
#endif
  return result;
}

fz_global PFN_vkEnumerateInstanceExtensionProperties __vkEnumerateInstanceExtensionProperties = NULL;
fz_function VkResult
vkEnumerateInstanceExtensionProperties (const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
  VkResult result = __vkEnumerateInstanceExtensionProperties (pLayerName, pPropertyCount, pProperties);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkEnumerateInstanceExtensionProperties");
#endif
  return result;
}

fz_global PFN_vkEnumerateInstanceLayerProperties __vkEnumerateInstanceLayerProperties = NULL;
fz_function VkResult
vkEnumerateInstanceLayerProperties (uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
  VkResult result = __vkEnumerateInstanceLayerProperties (pPropertyCount, pProperties);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkEnumerateInstanceLayerProperties");
#endif
  return result;
}

fz_global PFN_vkEnumerateInstanceVersion __vkEnumerateInstanceVersion = NULL;
fz_function VkResult
vkEnumerateInstanceVersion (uint32_t* pApiVersion)
{
  VkResult result = __vkEnumerateInstanceVersion (pApiVersion);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkEnumerateInstanceVersion");
#endif
  return result;
}

fz_global PFN_vkEnumeratePhysicalDevices __vkEnumeratePhysicalDevices = NULL;
fz_function VkResult
vkEnumeratePhysicalDevices (VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices)
{
  VkResult result = __vkEnumeratePhysicalDevices (instance, pPhysicalDeviceCount, pPhysicalDevices);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkEnumeratePhysicalDevices");
#endif
  return result;
}

fz_global PFN_vkEnumeratePhysicalDeviceGroups __vkEnumeratePhysicalDeviceGroups = NULL;
fz_function VkResult
vkEnumeratePhysicalDeviceGroups (VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
  VkResult result = __vkEnumeratePhysicalDeviceGroups (instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkEnumeratePhysicalDeviceGroups");
#endif
  return result;
}

fz_global PFN_vkCreateDevice __vkCreateDevice = NULL;
fz_function VkResult
vkCreateDevice (VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
  VkResult result = __vkCreateDevice (physicalDevice, pCreateInfo, pAllocator, pDevice);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateDevice");
#endif
  return result;
}

fz_global PFN_vkEnumerateDeviceExtensionProperties __vkEnumerateDeviceExtensionProperties = NULL;
fz_function VkResult
vkEnumerateDeviceExtensionProperties (VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
  VkResult result = __vkEnumerateDeviceExtensionProperties (physicalDevice, pLayerName, pPropertyCount, pProperties);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkEnumerateDeviceExtensionProperties");
#endif
  return result;
}

fz_global PFN_vkEnumerateDeviceLayerProperties __vkEnumerateDeviceLayerProperties = NULL;
fz_function VkResult
vkEnumerateDeviceLayerProperties (VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
  VkResult result = __vkEnumerateDeviceLayerProperties (physicalDevice, pPropertyCount, pProperties);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkEnumerateDeviceLayerProperties");
#endif
  return result;
}

fz_global PFN_vkGetPhysicalDeviceSurfaceSupportKHR __vkGetPhysicalDeviceSurfaceSupportKHR = NULL;
fz_function VkResult
vkGetPhysicalDeviceSurfaceSupportKHR (VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
  VkResult result = __vkGetPhysicalDeviceSurfaceSupportKHR (physicalDevice, queueFamilyIndex, surface, pSupported);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkGetPhysicalDeviceSurfaceSupportKHR");
#endif
  return result;
}

fz_global PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR __vkGetPhysicalDeviceSurfaceCapabilitiesKHR = NULL;
fz_function VkResult
vkGetPhysicalDeviceSurfaceCapabilitiesKHR (VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
  VkResult result = __vkGetPhysicalDeviceSurfaceCapabilitiesKHR (physicalDevice, surface, pSurfaceCapabilities);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
#endif
  return result;
}

fz_global PFN_vkGetPhysicalDeviceSurfaceFormatsKHR __vkGetPhysicalDeviceSurfaceFormatsKHR = NULL;
fz_function VkResult
vkGetPhysicalDeviceSurfaceFormatsKHR (VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats)
{
  VkResult result = __vkGetPhysicalDeviceSurfaceFormatsKHR (physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkGetPhysicalDeviceSurfaceFormatsKHR");
#endif
  return result;
}

fz_global PFN_vkGetPhysicalDeviceSurfacePresentModesKHR __vkGetPhysicalDeviceSurfacePresentModesKHR = NULL;
fz_function VkResult
vkGetPhysicalDeviceSurfacePresentModesKHR (VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
  VkResult result = __vkGetPhysicalDeviceSurfacePresentModesKHR (physicalDevice, surface, pPresentModeCount, pPresentModes);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkGetPhysicalDeviceSurfacePresentModesKHR");
#endif
  return result;
}

fz_global PFN_vkCreateWin32SurfaceKHR __vkCreateWin32SurfaceKHR = NULL;
fz_function VkResult
vkCreateWin32SurfaceKHR (VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
  VkResult result = __vkCreateWin32SurfaceKHR (instance, pCreateInfo, pAllocator, pSurface);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateWin32SurfaceKHR");
#endif
  return result;
}

fz_global PFN_vkGetPhysicalDeviceToolPropertiesEXT __vkGetPhysicalDeviceToolPropertiesEXT = NULL;
fz_function VkResult
vkGetPhysicalDeviceToolPropertiesEXT (VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties)
{
  VkResult result = __vkGetPhysicalDeviceToolPropertiesEXT (physicalDevice, pToolCount, pToolProperties);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkGetPhysicalDeviceToolPropertiesEXT");
#endif
  return result;
}

fz_global PFN_vkCreateDebugUtilsMessengerEXT __vkCreateDebugUtilsMessengerEXT = NULL;
fz_function VkResult
vkCreateDebugUtilsMessengerEXT (VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{
  VkResult result = __vkCreateDebugUtilsMessengerEXT (instance, pCreateInfo, pAllocator, pMessenger);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateDebugUtilsMessengerEXT");
#endif
  return result;
}

fz_global PFN_vkDestroyInstance __vkDestroyInstance = NULL;
fz_function void
vkDestroyInstance (VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyInstance (instance, pAllocator);
}

fz_global PFN_vkDestroySurfaceKHR __vkDestroySurfaceKHR = NULL;
fz_function void
vkDestroySurfaceKHR (VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroySurfaceKHR (instance, surface, pAllocator);
}

fz_global PFN_vkDestroyDebugUtilsMessengerEXT __vkDestroyDebugUtilsMessengerEXT = NULL;
fz_function void
vkDestroyDebugUtilsMessengerEXT (VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyDebugUtilsMessengerEXT (instance, messenger, pAllocator);
}

fz_global PFN_vkSubmitDebugUtilsMessageEXT __vkSubmitDebugUtilsMessageEXT = NULL;
fz_function void
vkSubmitDebugUtilsMessageEXT (VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
  __vkSubmitDebugUtilsMessageEXT (instance, messageSeverity, messageTypes, pCallbackData);
}

fz_global PFN_vkGetPhysicalDeviceFeatures __vkGetPhysicalDeviceFeatures = NULL;
fz_function void
vkGetPhysicalDeviceFeatures (VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
{
  __vkGetPhysicalDeviceFeatures (physicalDevice, pFeatures);
}

fz_global PFN_vkGetPhysicalDeviceFeatures2 __vkGetPhysicalDeviceFeatures2 = NULL;
fz_function void
vkGetPhysicalDeviceFeatures2 (VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
  __vkGetPhysicalDeviceFeatures2 (physicalDevice, pFeatures);
}

fz_global PFN_vkGetPhysicalDeviceProperties __vkGetPhysicalDeviceProperties = NULL;
fz_function void
vkGetPhysicalDeviceProperties (VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
{
  __vkGetPhysicalDeviceProperties (physicalDevice, pProperties);
}

fz_global PFN_vkGetPhysicalDeviceProperties2 __vkGetPhysicalDeviceProperties2 = NULL;
fz_function void
vkGetPhysicalDeviceProperties2 (VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
  __vkGetPhysicalDeviceProperties2 (physicalDevice, pProperties);
}

fz_global PFN_vkGetPhysicalDeviceMemoryProperties __vkGetPhysicalDeviceMemoryProperties = NULL;
fz_function void
vkGetPhysicalDeviceMemoryProperties (VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
  __vkGetPhysicalDeviceMemoryProperties (physicalDevice, pMemoryProperties);
}

fz_global PFN_vkGetPhysicalDeviceQueueFamilyProperties __vkGetPhysicalDeviceQueueFamilyProperties = NULL;
fz_function void
vkGetPhysicalDeviceQueueFamilyProperties (VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties)
{
  __vkGetPhysicalDeviceQueueFamilyProperties (physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

fz_global PFN_vkGetPhysicalDeviceFormatProperties __vkGetPhysicalDeviceFormatProperties = NULL;
fz_function void
vkGetPhysicalDeviceFormatProperties (VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties)
{
  __vkGetPhysicalDeviceFormatProperties (physicalDevice, format, pFormatProperties);
}

fz_global PFN_vkGetPhysicalDeviceImageFormatProperties __vkGetPhysicalDeviceImageFormatProperties = NULL;
fz_function void
vkGetPhysicalDeviceImageFormatProperties (VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties)
{
  __vkGetPhysicalDeviceImageFormatProperties (physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}

fz_global PFN_vkGetPhysicalDeviceFormatProperties2 __vkGetPhysicalDeviceFormatProperties2 = NULL;
fz_function void
vkGetPhysicalDeviceFormatProperties2 (VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
  __vkGetPhysicalDeviceFormatProperties2 (physicalDevice, format, pFormatProperties);
}

fz_global PFN_vkGetPhysicalDeviceImageFormatProperties2 __vkGetPhysicalDeviceImageFormatProperties2 = NULL;
fz_function void
vkGetPhysicalDeviceImageFormatProperties2 (VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
  __vkGetPhysicalDeviceImageFormatProperties2 (physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

fz_global PFN_vkGetPhysicalDeviceSparseImageFormatProperties __vkGetPhysicalDeviceSparseImageFormatProperties = NULL;
fz_function void
vkGetPhysicalDeviceSparseImageFormatProperties (VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties)
{
  __vkGetPhysicalDeviceSparseImageFormatProperties (physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
}

fz_global PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 __vkGetPhysicalDeviceSparseImageFormatProperties2 = NULL;
fz_function void
vkGetPhysicalDeviceSparseImageFormatProperties2 (VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties)
{
  __vkGetPhysicalDeviceSparseImageFormatProperties2 (physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}

fz_global PFN_vkGetPhysicalDeviceExternalBufferProperties __vkGetPhysicalDeviceExternalBufferProperties = NULL;
fz_function void
vkGetPhysicalDeviceExternalBufferProperties (VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
  __vkGetPhysicalDeviceExternalBufferProperties (physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

fz_global PFN_vkGetPhysicalDeviceExternalFenceProperties __vkGetPhysicalDeviceExternalFenceProperties = NULL;
fz_function void
vkGetPhysicalDeviceExternalFenceProperties (VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
  __vkGetPhysicalDeviceExternalFenceProperties (physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

fz_global PFN_vkGetPhysicalDeviceExternalSemaphoreProperties __vkGetPhysicalDeviceExternalSemaphoreProperties = NULL;
fz_function void
vkGetPhysicalDeviceExternalSemaphoreProperties (VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
  __vkGetPhysicalDeviceExternalSemaphoreProperties (physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

fz_global PFN_vkDeviceWaitIdle __vkDeviceWaitIdle = NULL;
fz_function VkResult
vkDeviceWaitIdle (VkDevice device)
{
  VkResult result = __vkDeviceWaitIdle (device);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkDeviceWaitIdle");
#endif
  return result;
}

fz_global PFN_vkQueueSubmit __vkQueueSubmit = NULL;
fz_function VkResult
vkQueueSubmit (VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
{
  VkResult result = __vkQueueSubmit (queue, submitCount, pSubmits, fence);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkQueueSubmit");
#endif
  return result;
}

fz_global PFN_vkQueueSubmit2 __vkQueueSubmit2 = NULL;
fz_function VkResult
vkQueueSubmit2 (VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence)
{
  VkResult result = __vkQueueSubmit2 (queue, submitCount, pSubmits, fence);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkQueueSubmit2");
#endif
  return result;
}

fz_global PFN_vkQueueWaitIdle __vkQueueWaitIdle = NULL;
fz_function VkResult
vkQueueWaitIdle (VkQueue queue)
{
  VkResult result = __vkQueueWaitIdle (queue);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkQueueWaitIdle");
#endif
  return result;
}

fz_global PFN_vkCreateSwapchainKHR __vkCreateSwapchainKHR = NULL;
fz_function VkResult
vkCreateSwapchainKHR (VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
{
  VkResult result = __vkCreateSwapchainKHR (device, pCreateInfo, pAllocator, pSwapchain);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateSwapchainKHR");
#endif
  return result;
}

fz_global PFN_vkGetSwapchainImagesKHR __vkGetSwapchainImagesKHR = NULL;
fz_function VkResult
vkGetSwapchainImagesKHR (VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
{
  VkResult result = __vkGetSwapchainImagesKHR (device, swapchain, pSwapchainImageCount, pSwapchainImages);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkGetSwapchainImagesKHR");
#endif
  return result;
}

fz_global PFN_vkAcquireNextImageKHR __vkAcquireNextImageKHR = NULL;
fz_function VkResult
vkAcquireNextImageKHR (VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
  VkResult result = __vkAcquireNextImageKHR (device, swapchain, timeout, semaphore, fence, pImageIndex);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkAcquireNextImageKHR");
#endif
  return result;
}

fz_global PFN_vkCreateCommandPool __vkCreateCommandPool = NULL;
fz_function VkResult
vkCreateCommandPool (VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
  VkResult result = __vkCreateCommandPool (device, pCreateInfo, pAllocator, pCommandPool);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateCommandPool");
#endif
  return result;
}

fz_global PFN_vkAllocateCommandBuffers __vkAllocateCommandBuffers = NULL;
fz_function VkResult
vkAllocateCommandBuffers (VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
  VkResult result = __vkAllocateCommandBuffers (device, pAllocateInfo, pCommandBuffers);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkAllocateCommandBuffers");
#endif
  return result;
}

fz_global PFN_vkBeginCommandBuffer __vkBeginCommandBuffer = NULL;
fz_function VkResult
vkBeginCommandBuffer (VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo)
{
  VkResult result = __vkBeginCommandBuffer (commandBuffer, pBeginInfo);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkBeginCommandBuffer");
#endif
  return result;
}

fz_global PFN_vkEndCommandBuffer __vkEndCommandBuffer = NULL;
fz_function VkResult
vkEndCommandBuffer (VkCommandBuffer commandBuffer)
{
  VkResult result = __vkEndCommandBuffer (commandBuffer);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkEndCommandBuffer");
#endif
  return result;
}

fz_global PFN_vkCreateSemaphore __vkCreateSemaphore = NULL;
fz_function VkResult
vkCreateSemaphore (VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
{
  VkResult result = __vkCreateSemaphore (device, pCreateInfo, pAllocator, pSemaphore);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateSemaphore");
#endif
  return result;
}

fz_global PFN_vkCreateFence __vkCreateFence = NULL;
fz_function VkResult
vkCreateFence (VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
  VkResult result = __vkCreateFence (device, pCreateInfo, pAllocator, pFence);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateFence");
#endif
  return result;
}

fz_global PFN_vkWaitForFences __vkWaitForFences = NULL;
fz_function VkResult
vkWaitForFences (VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
  VkResult result = __vkWaitForFences (device, fenceCount, pFences, waitAll, timeout);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkWaitForFences");
#endif
  return result;
}

fz_global PFN_vkResetFences __vkResetFences = NULL;
fz_function VkResult
vkResetFences (VkDevice device, uint32_t fenceCount, const VkFence* pFences)
{
  VkResult result = __vkResetFences (device, fenceCount, pFences);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkResetFences");
#endif
  return result;
}

fz_global PFN_vkCreateBuffer __vkCreateBuffer = NULL;
fz_function VkResult
vkCreateBuffer (VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
  VkResult result = __vkCreateBuffer (device, pCreateInfo, pAllocator, pBuffer);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateBuffer");
#endif
  return result;
}

fz_global PFN_vkCreateImage __vkCreateImage = NULL;
fz_function VkResult
vkCreateImage (VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
  VkResult result = __vkCreateImage (device, pCreateInfo, pAllocator, pImage);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateImage");
#endif
  return result;
}

fz_global PFN_vkCreateImageView __vkCreateImageView = NULL;
fz_function VkResult
vkCreateImageView (VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
  VkResult result = __vkCreateImageView (device, pCreateInfo, pAllocator, pView);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateImageView");
#endif
  return result;
}

fz_global PFN_vkCreateSampler __vkCreateSampler = NULL;
fz_function VkResult
vkCreateSampler (VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
{
  VkResult result = __vkCreateSampler (device, pCreateInfo, pAllocator, pSampler);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateSampler");
#endif
  return result;
}

fz_global PFN_vkAllocateMemory __vkAllocateMemory = NULL;
fz_function VkResult
vkAllocateMemory (VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
  VkResult result = __vkAllocateMemory (device, pAllocateInfo, pAllocator, pMemory);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkAllocateMemory");
#endif
  return result;
}

fz_global PFN_vkCreateShaderModule __vkCreateShaderModule = NULL;
fz_function VkResult
vkCreateShaderModule (VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
  VkResult result = __vkCreateShaderModule (device, pCreateInfo, pAllocator, pShaderModule);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateShaderModule");
#endif
  return result;
}

fz_global PFN_vkCreatePipelineLayout __vkCreatePipelineLayout = NULL;
fz_function VkResult
vkCreatePipelineLayout (VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
{
  VkResult result = __vkCreatePipelineLayout (device, pCreateInfo, pAllocator, pPipelineLayout);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreatePipelineLayout");
#endif
  return result;
}

fz_global PFN_vkCreateGraphicsPipelines __vkCreateGraphicsPipelines = NULL;
fz_function VkResult
vkCreateGraphicsPipelines (VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
  VkResult result = __vkCreateGraphicsPipelines (device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateGraphicsPipelines");
#endif
  return result;
}

fz_global PFN_vkCreateComputePipelines __vkCreateComputePipelines = NULL;
fz_function VkResult
vkCreateComputePipelines (VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
  VkResult result = __vkCreateComputePipelines (device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateComputePipelines");
#endif
  return result;
}

fz_global PFN_vkCreateDescriptorSetLayout __vkCreateDescriptorSetLayout = NULL;
fz_function VkResult
vkCreateDescriptorSetLayout (VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
  VkResult result = __vkCreateDescriptorSetLayout (device, pCreateInfo, pAllocator, pSetLayout);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateDescriptorSetLayout");
#endif
  return result;
}

fz_global PFN_vkCreateDescriptorPool __vkCreateDescriptorPool = NULL;
fz_function VkResult
vkCreateDescriptorPool (VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
{
  VkResult result = __vkCreateDescriptorPool (device, pCreateInfo, pAllocator, pDescriptorPool);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateDescriptorPool");
#endif
  return result;
}

fz_global PFN_vkAllocateDescriptorSets __vkAllocateDescriptorSets = NULL;
fz_function VkResult
vkAllocateDescriptorSets (VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
  VkResult result = __vkAllocateDescriptorSets (device, pAllocateInfo, pDescriptorSets);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkAllocateDescriptorSets");
#endif
  return result;
}

fz_global PFN_vkCreateRenderPass __vkCreateRenderPass = NULL;
fz_function VkResult
vkCreateRenderPass (VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
  VkResult result = __vkCreateRenderPass (device, pCreateInfo, pAllocator, pRenderPass);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateRenderPass");
#endif
  return result;
}

fz_global PFN_vkCreateRenderPass2 __vkCreateRenderPass2 = NULL;
fz_function VkResult
vkCreateRenderPass2 (VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
  VkResult result = __vkCreateRenderPass2 (device, pCreateInfo, pAllocator, pRenderPass);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateRenderPass2");
#endif
  return result;
}

fz_global PFN_vkCreateFramebuffer __vkCreateFramebuffer = NULL;
fz_function VkResult
vkCreateFramebuffer (VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
{
  VkResult result = __vkCreateFramebuffer (device, pCreateInfo, pAllocator, pFramebuffer);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateFramebuffer");
#endif
  return result;
}

fz_global PFN_vkCreateQueryPool __vkCreateQueryPool = NULL;
fz_function VkResult
vkCreateQueryPool (VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
{
  VkResult result = __vkCreateQueryPool (device, pCreateInfo, pAllocator, pQueryPool);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkCreateQueryPool");
#endif
  return result;
}

fz_global PFN_vkGetQueryPoolResults __vkGetQueryPoolResults = NULL;
fz_function VkResult
vkGetQueryPoolResults (VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
{
  VkResult result = __vkGetQueryPoolResults (device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
#if DEBUG_VULKAN_CHECK_RESULTS
  check_vk_result(result, "vkGetQueryPoolResults");
#endif
  return result;
}

fz_global PFN_vkDestroyDevice __vkDestroyDevice = NULL;
fz_function void
vkDestroyDevice (VkDevice device, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyDevice (device, pAllocator);
}

fz_global PFN_vkGetDeviceQueue __vkGetDeviceQueue = NULL;
fz_function void
vkGetDeviceQueue (VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
  __vkGetDeviceQueue (device, queueFamilyIndex, queueIndex, pQueue);
}

fz_global PFN_vkGetDeviceQueue2 __vkGetDeviceQueue2 = NULL;
fz_function void
vkGetDeviceQueue2 (VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
{
  __vkGetDeviceQueue2 (device, pQueueInfo, pQueue);
}

fz_global PFN_vkDestroySwapchainKHR __vkDestroySwapchainKHR = NULL;
fz_function void
vkDestroySwapchainKHR (VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroySwapchainKHR (device, swapchain, pAllocator);
}

fz_global PFN_vkDestroyBuffer __vkDestroyBuffer = NULL;
fz_function void
vkDestroyBuffer (VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyBuffer (device, buffer, pAllocator);
}

fz_global PFN_vkDestroyImage __vkDestroyImage = NULL;
fz_function void
vkDestroyImage (VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyImage (device, image, pAllocator);
}

fz_global PFN_vkDestroyImageView __vkDestroyImageView = NULL;
fz_function void
vkDestroyImageView (VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyImageView (device, imageView, pAllocator);
}

fz_global PFN_vkDestroySampler __vkDestroySampler = NULL;
fz_function void
vkDestroySampler (VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroySampler (device, sampler, pAllocator);
}

fz_global PFN_vkFreeMemory __vkFreeMemory = NULL;
fz_function void
vkFreeMemory (VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
{
  __vkFreeMemory (device, memory, pAllocator);
}

fz_global PFN_vkDestroyShaderModule __vkDestroyShaderModule = NULL;
fz_function void
vkDestroyShaderModule (VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyShaderModule (device, shaderModule, pAllocator);
}

fz_global PFN_vkDestroyPipeline __vkDestroyPipeline = NULL;
fz_function void
vkDestroyPipeline (VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyPipeline (device, pipeline, pAllocator);
}

fz_global PFN_vkDestroyPipelineLayout __vkDestroyPipelineLayout = NULL;
fz_function void
vkDestroyPipelineLayout (VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyPipelineLayout (device, pipelineLayout, pAllocator);
}

fz_global PFN_vkDestroyDescriptorSetLayout __vkDestroyDescriptorSetLayout = NULL;
fz_function void
vkDestroyDescriptorSetLayout (VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyDescriptorSetLayout (device, descriptorSetLayout, pAllocator);
}

fz_global PFN_vkDestroyDescriptorPool __vkDestroyDescriptorPool = NULL;
fz_function void
vkDestroyDescriptorPool (VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyDescriptorPool (device, descriptorPool, pAllocator);
}

fz_global PFN_vkFreeDescriptorSets __vkFreeDescriptorSets = NULL;
fz_function void
vkFreeDescriptorSets (VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
{
  __vkFreeDescriptorSets (device, descriptorPool, descriptorSetCount, pDescriptorSets);
}

fz_global PFN_vkDestroyRenderPass __vkDestroyRenderPass = NULL;
fz_function void
vkDestroyRenderPass (VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyRenderPass (device, renderPass, pAllocator);
}

fz_global PFN_vkDestroyFramebuffer __vkDestroyFramebuffer = NULL;
fz_function void
vkDestroyFramebuffer (VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyFramebuffer (device, framebuffer, pAllocator);
}

fz_global PFN_vkDestroyQueryPool __vkDestroyQueryPool = NULL;
fz_function void
vkDestroyQueryPool (VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyQueryPool (device, queryPool, pAllocator);
}

fz_global PFN_vkDestroySemaphore __vkDestroySemaphore = NULL;
fz_function void
vkDestroySemaphore (VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroySemaphore (device, semaphore, pAllocator);
}

fz_global PFN_vkDestroyFence __vkDestroyFence = NULL;
fz_function void
vkDestroyFence (VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyFence (device, fence, pAllocator);
}

fz_global PFN_vkResetCommandPool __vkResetCommandPool = NULL;
fz_function void
vkResetCommandPool (VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags)
{
  __vkResetCommandPool (device, commandPool, flags);
}

fz_global PFN_vkDestroyCommandPool __vkDestroyCommandPool = NULL;
fz_function void
vkDestroyCommandPool (VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyCommandPool (device, commandPool, pAllocator);
}

fz_global PFN_vkFreeCommandBuffers __vkFreeCommandBuffers = NULL;
fz_function void
vkFreeCommandBuffers (VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
  __vkFreeCommandBuffers (device, commandPool, commandBufferCount, pCommandBuffers);
}

fz_global PFN_vkCmdBeginRenderPass __vkCmdBeginRenderPass = NULL;
fz_function void
vkCmdBeginRenderPass (VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
{
  __vkCmdBeginRenderPass (commandBuffer, pRenderPassBegin, contents);
}

fz_global PFN_vkCmdEndRenderPass __vkCmdEndRenderPass = NULL;
fz_function void
vkCmdEndRenderPass (VkCommandBuffer commandBuffer)
{
  __vkCmdEndRenderPass (commandBuffer);
}

fz_global PFN_vkCmdBeginRenderPass2 __vkCmdBeginRenderPass2 = NULL;
fz_function void
vkCmdBeginRenderPass2 (VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo)
{
  __vkCmdBeginRenderPass2 (commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

fz_global PFN_vkCmdEndRenderPass2 __vkCmdEndRenderPass2 = NULL;
fz_function void
vkCmdEndRenderPass2 (VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo)
{
  __vkCmdEndRenderPass2 (commandBuffer, pSubpassEndInfo);
}

fz_global PFN_vkCmdBindPipeline __vkCmdBindPipeline = NULL;
fz_function void
vkCmdBindPipeline (VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
  __vkCmdBindPipeline (commandBuffer, pipelineBindPoint, pipeline);
}

fz_global PFN_vkCmdBindDescriptorSets __vkCmdBindDescriptorSets = NULL;
fz_function void
vkCmdBindDescriptorSets (VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
{
  __vkCmdBindDescriptorSets (commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

fz_global PFN_vkCmdBindVertexBuffers __vkCmdBindVertexBuffers = NULL;
fz_function void
vkCmdBindVertexBuffers (VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
{
  __vkCmdBindVertexBuffers (commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

fz_global PFN_vkCmdBindIndexBuffer __vkCmdBindIndexBuffer = NULL;
fz_function void
vkCmdBindIndexBuffer (VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
  __vkCmdBindIndexBuffer (commandBuffer, buffer, offset, indexType);
}

fz_global PFN_vkCmdDraw __vkCmdDraw = NULL;
fz_function void
vkCmdDraw (VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
  __vkCmdDraw (commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

fz_global PFN_vkCmdDrawIndexed __vkCmdDrawIndexed = NULL;
fz_function void
vkCmdDrawIndexed (VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
  __vkCmdDrawIndexed (commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

fz_global PFN_vkCmdDispatch __vkCmdDispatch = NULL;
fz_function void
vkCmdDispatch (VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
  __vkCmdDispatch (commandBuffer, groupCountX, groupCountY, groupCountZ);
}

fz_global PFN_vkCmdCopyBuffer __vkCmdCopyBuffer = NULL;
fz_function void
vkCmdCopyBuffer (VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
{
  __vkCmdCopyBuffer (commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

fz_global PFN_vkCmdCopyImage __vkCmdCopyImage = NULL;
fz_function void
vkCmdCopyImage (VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
{
  __vkCmdCopyImage (commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

fz_global PFN_vkCmdBlitImage __vkCmdBlitImage = NULL;
fz_function void
vkCmdBlitImage (VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
{
  __vkCmdBlitImage (commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

fz_global PFN_vkCmdCopyBufferToImage __vkCmdCopyBufferToImage = NULL;
fz_function void
vkCmdCopyBufferToImage (VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
  __vkCmdCopyBufferToImage (commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

fz_global PFN_vkCmdCopyImageToBuffer __vkCmdCopyImageToBuffer = NULL;
fz_function void
vkCmdCopyImageToBuffer (VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
  __vkCmdCopyImageToBuffer (commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

fz_global PFN_vkCmdPipelineBarrier __vkCmdPipelineBarrier = NULL;
fz_function void
vkCmdPipelineBarrier (VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
  __vkCmdPipelineBarrier (commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

fz_global PFN_vkCmdPushConstants __vkCmdPushConstants = NULL;
fz_function void
vkCmdPushConstants (VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
  __vkCmdPushConstants (commandBuffer, layout, stageFlags, offset, size, pValues);
}

fz_global PFN_vkCmdSetViewport __vkCmdSetViewport = NULL;
fz_function void
vkCmdSetViewport (VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
{
  __vkCmdSetViewport (commandBuffer, firstViewport, viewportCount, pViewports);
}

fz_global PFN_vkCmdSetScissor __vkCmdSetScissor = NULL;
fz_function void
vkCmdSetScissor (VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
{
  __vkCmdSetScissor (commandBuffer, firstScissor, scissorCount, pScissors);
}

fz_global PFN_vkCmdSetLineWidth __vkCmdSetLineWidth = NULL;
fz_function void
vkCmdSetLineWidth (VkCommandBuffer commandBuffer, float lineWidth)
{
  __vkCmdSetLineWidth (commandBuffer, lineWidth);
}

fz_global PFN_vkCmdSetDepthBias __vkCmdSetDepthBias = NULL;
fz_function void
vkCmdSetDepthBias (VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
  __vkCmdSetDepthBias (commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

fz_global PFN_vkCmdSetBlendConstants __vkCmdSetBlendConstants = NULL;
fz_function void
vkCmdSetBlendConstants (VkCommandBuffer commandBuffer, const float blendConstants[4])
{
  __vkCmdSetBlendConstants (commandBuffer, blendConstants);
}

fz_global PFN_vkCmdSetDepthBounds __vkCmdSetDepthBounds = NULL;
fz_function void
vkCmdSetDepthBounds (VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
  __vkCmdSetDepthBounds (commandBuffer, minDepthBounds, maxDepthBounds);
}

fz_global PFN_vkCmdSetStencilCompareMask __vkCmdSetStencilCompareMask = NULL;
fz_function void
vkCmdSetStencilCompareMask (VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
{
  __vkCmdSetStencilCompareMask (commandBuffer, faceMask, compareMask);
}

fz_global PFN_vkCmdSetStencilWriteMask __vkCmdSetStencilWriteMask = NULL;
fz_function void
vkCmdSetStencilWriteMask (VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
{
  __vkCmdSetStencilWriteMask (commandBuffer, faceMask, writeMask);
}

fz_global PFN_vkCmdSetStencilReference __vkCmdSetStencilReference = NULL;
fz_function void
vkCmdSetStencilReference (VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
{
  __vkCmdSetStencilReference (commandBuffer, faceMask, reference);
}

#endif // VULKAN_FUNCTIONS_H
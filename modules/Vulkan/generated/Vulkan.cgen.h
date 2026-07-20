/* Generated code */

#ifndef VULKAN_FUNCTIONS_H
#define VULKAN_FUNCTIONS_H

fz_function b32 _vulkan_load_global_functions();
fz_function b32 _vulkan_load_instance_functions(VkInstance instance);
fz_function b32 _vulkan_load_device_functions(VkDevice device);

fz_global PFN_vkCreateInstance __vkCreateInstance = NULL;
fz_function VkResult
vkCreateInstance (const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
  VkResult result = __vkCreateInstance (pCreateInfo, pAllocator, pInstance);
  return result;
}

fz_global PFN_vkEnumerateInstanceExtensionProperties __vkEnumerateInstanceExtensionProperties = NULL;
fz_function VkResult
vkEnumerateInstanceExtensionProperties (const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
  VkResult result = __vkEnumerateInstanceExtensionProperties (pLayerName, pPropertyCount, pProperties);
  return result;
}

fz_global PFN_vkEnumerateInstanceLayerProperties __vkEnumerateInstanceLayerProperties = NULL;
fz_function VkResult
vkEnumerateInstanceLayerProperties (uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
  VkResult result = __vkEnumerateInstanceLayerProperties (pPropertyCount, pProperties);
  return result;
}

fz_global PFN_vkEnumerateInstanceVersion __vkEnumerateInstanceVersion = NULL;
fz_function VkResult
vkEnumerateInstanceVersion (uint32_t* pApiVersion)
{
  VkResult result = __vkEnumerateInstanceVersion (pApiVersion);
  return result;
}

fz_global PFN_vkEnumeratePhysicalDevices __vkEnumeratePhysicalDevices = NULL;
fz_function VkResult
vkEnumeratePhysicalDevices (VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices)
{
  VkResult result = __vkEnumeratePhysicalDevices (instance, pPhysicalDeviceCount, pPhysicalDevices);
  return result;
}

fz_global PFN_vkCreateDevice __vkCreateDevice = NULL;
fz_function VkResult
vkCreateDevice (VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
  VkResult result = __vkCreateDevice (physicalDevice, pCreateInfo, pAllocator, pDevice);
  return result;
}

fz_global PFN_vkGetPhysicalDeviceSurfaceSupportKHR __vkGetPhysicalDeviceSurfaceSupportKHR = NULL;
fz_function VkResult
vkGetPhysicalDeviceSurfaceSupportKHR (VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
  VkResult result = __vkGetPhysicalDeviceSurfaceSupportKHR (physicalDevice, queueFamilyIndex, surface, pSupported);
  return result;
}

fz_global PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR __vkGetPhysicalDeviceSurfaceCapabilitiesKHR = NULL;
fz_function VkResult
vkGetPhysicalDeviceSurfaceCapabilitiesKHR (VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
  VkResult result = __vkGetPhysicalDeviceSurfaceCapabilitiesKHR (physicalDevice, surface, pSurfaceCapabilities);
  return result;
}

fz_global PFN_vkGetPhysicalDeviceSurfaceFormatsKHR __vkGetPhysicalDeviceSurfaceFormatsKHR = NULL;
fz_function VkResult
vkGetPhysicalDeviceSurfaceFormatsKHR (VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats)
{
  VkResult result = __vkGetPhysicalDeviceSurfaceFormatsKHR (physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
  return result;
}

fz_global PFN_vkGetPhysicalDeviceSurfacePresentModesKHR __vkGetPhysicalDeviceSurfacePresentModesKHR = NULL;
fz_function VkResult
vkGetPhysicalDeviceSurfacePresentModesKHR (VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
  VkResult result = __vkGetPhysicalDeviceSurfacePresentModesKHR (physicalDevice, surface, pPresentModeCount, pPresentModes);
  return result;
}

fz_global PFN_vkDestroyInstance __vkDestroyInstance = NULL;
fz_function void
vkDestroyInstance (VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroyInstance (instance, pAllocator);
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

fz_global PFN_vkDeviceWaitIdle __vkDeviceWaitIdle = NULL;
fz_function VkResult
vkDeviceWaitIdle (VkDevice device)
{
  VkResult result = __vkDeviceWaitIdle (device);
  return result;
}

fz_global PFN_vkQueueSubmit __vkQueueSubmit = NULL;
fz_function VkResult
vkQueueSubmit (VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
{
  VkResult result = __vkQueueSubmit (queue, submitCount, pSubmits, fence);
  return result;
}

fz_global PFN_vkQueueWaitIdle __vkQueueWaitIdle = NULL;
fz_function VkResult
vkQueueWaitIdle (VkQueue queue)
{
  VkResult result = __vkQueueWaitIdle (queue);
  return result;
}

fz_global PFN_vkCreateSwapchainKHR __vkCreateSwapchainKHR = NULL;
fz_function VkResult
vkCreateSwapchainKHR (VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
{
  VkResult result = __vkCreateSwapchainKHR (device, pCreateInfo, pAllocator, pSwapchain);
  return result;
}

fz_global PFN_vkGetSwapchainImagesKHR __vkGetSwapchainImagesKHR = NULL;
fz_function VkResult
vkGetSwapchainImagesKHR (VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
{
  VkResult result = __vkGetSwapchainImagesKHR (device, swapchain, pSwapchainImageCount, pSwapchainImages);
  return result;
}

fz_global PFN_vkAcquireNextImageKHR __vkAcquireNextImageKHR = NULL;
fz_function VkResult
vkAcquireNextImageKHR (VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
  VkResult result = __vkAcquireNextImageKHR (device, swapchain, timeout, semaphore, fence, pImageIndex);
  return result;
}

fz_global PFN_vkQueuePresentKHR __vkQueuePresentKHR = NULL;
fz_function VkResult
vkQueuePresentKHR (VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
  VkResult result = __vkQueuePresentKHR (queue, pPresentInfo);
  return result;
}

fz_global PFN_vkCreateCommandPool __vkCreateCommandPool = NULL;
fz_function VkResult
vkCreateCommandPool (VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
  VkResult result = __vkCreateCommandPool (device, pCreateInfo, pAllocator, pCommandPool);
  return result;
}

fz_global PFN_vkAllocateCommandBuffers __vkAllocateCommandBuffers = NULL;
fz_function VkResult
vkAllocateCommandBuffers (VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
  VkResult result = __vkAllocateCommandBuffers (device, pAllocateInfo, pCommandBuffers);
  return result;
}

fz_global PFN_vkBeginCommandBuffer __vkBeginCommandBuffer = NULL;
fz_function VkResult
vkBeginCommandBuffer (VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo)
{
  VkResult result = __vkBeginCommandBuffer (commandBuffer, pBeginInfo);
  return result;
}

fz_global PFN_vkEndCommandBuffer __vkEndCommandBuffer = NULL;
fz_function VkResult
vkEndCommandBuffer (VkCommandBuffer commandBuffer)
{
  VkResult result = __vkEndCommandBuffer (commandBuffer);
  return result;
}

fz_global PFN_vkCreateSemaphore __vkCreateSemaphore = NULL;
fz_function VkResult
vkCreateSemaphore (VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
{
  VkResult result = __vkCreateSemaphore (device, pCreateInfo, pAllocator, pSemaphore);
  return result;
}

fz_global PFN_vkCreateFence __vkCreateFence = NULL;
fz_function VkResult
vkCreateFence (VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
  VkResult result = __vkCreateFence (device, pCreateInfo, pAllocator, pFence);
  return result;
}

fz_global PFN_vkWaitForFences __vkWaitForFences = NULL;
fz_function VkResult
vkWaitForFences (VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
  VkResult result = __vkWaitForFences (device, fenceCount, pFences, waitAll, timeout);
  return result;
}

fz_global PFN_vkResetFences __vkResetFences = NULL;
fz_function VkResult
vkResetFences (VkDevice device, uint32_t fenceCount, const VkFence* pFences)
{
  VkResult result = __vkResetFences (device, fenceCount, pFences);
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

fz_global PFN_vkDestroySwapchainKHR __vkDestroySwapchainKHR = NULL;
fz_function void
vkDestroySwapchainKHR (VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
  __vkDestroySwapchainKHR (device, swapchain, pAllocator);
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

fz_global PFN_vkCmdBindPipeline __vkCmdBindPipeline = NULL;
fz_function void
vkCmdBindPipeline (VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
  __vkCmdBindPipeline (commandBuffer, pipelineBindPoint, pipeline);
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

#endif // VULKAN_FUNCTIONS_H
/* Generated code */

fz_function b32
_vulkan_load_global_functions()
{
  __vkCreateInstance = (PFN_vkCreateInstance)__vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkCreateInstance");
  if (!__vkCreateInstance)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCreateInstance"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)__vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceExtensionProperties");
  if (!__vkEnumerateInstanceExtensionProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkEnumerateInstanceExtensionProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)__vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceLayerProperties");
  if (!__vkEnumerateInstanceLayerProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkEnumerateInstanceLayerProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)__vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
  if (!__vkEnumerateInstanceVersion)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkEnumerateInstanceVersion"), S("Vulkan.cgen"), 0);
    return false;
  }

  return true;
}

fz_function b32
_vulkan_load_instance_functions(VkInstance instance)
{
  __vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices) __vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDevices");
  if (!__vkEnumeratePhysicalDevices)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkEnumeratePhysicalDevices"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateDevice = (PFN_vkCreateDevice) __vkGetInstanceProcAddr(instance, "vkCreateDevice");
  if (!__vkCreateDevice)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCreateDevice"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
  if (!__vkGetPhysicalDeviceSurfaceSupportKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkGetPhysicalDeviceSurfaceSupportKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
  if (!__vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkGetPhysicalDeviceSurfaceCapabilitiesKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
  if (!__vkGetPhysicalDeviceSurfaceFormatsKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkGetPhysicalDeviceSurfaceFormatsKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
  if (!__vkGetPhysicalDeviceSurfacePresentModesKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkGetPhysicalDeviceSurfacePresentModesKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyInstance = (PFN_vkDestroyInstance) __vkGetInstanceProcAddr(instance, "vkDestroyInstance");
  if (!__vkDestroyInstance)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkDestroyInstance"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures");
  if (!__vkGetPhysicalDeviceFeatures)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkGetPhysicalDeviceFeatures"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceFeatures2 = (PFN_vkGetPhysicalDeviceFeatures2) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures2");
  if (!__vkGetPhysicalDeviceFeatures2)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkGetPhysicalDeviceFeatures2"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties");
  if (!__vkGetPhysicalDeviceProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkGetPhysicalDeviceProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceProperties2 = (PFN_vkGetPhysicalDeviceProperties2) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties2");
  if (!__vkGetPhysicalDeviceProperties2)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkGetPhysicalDeviceProperties2"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties");
  if (!__vkGetPhysicalDeviceMemoryProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkGetPhysicalDeviceMemoryProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties");
  if (!__vkGetPhysicalDeviceQueueFamilyProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkGetPhysicalDeviceQueueFamilyProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  return true;
}

fz_function b32
_vulkan_load_device_functions(VkDevice device)
{
  __vkDeviceWaitIdle = (PFN_vkDeviceWaitIdle) __vkGetDeviceProcAddr(device, "vkDeviceWaitIdle");
  if (!__vkDeviceWaitIdle)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkDeviceWaitIdle"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkQueueSubmit = (PFN_vkQueueSubmit) __vkGetDeviceProcAddr(device, "vkQueueSubmit");
  if (!__vkQueueSubmit)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkQueueSubmit"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkQueueWaitIdle = (PFN_vkQueueWaitIdle) __vkGetDeviceProcAddr(device, "vkQueueWaitIdle");
  if (!__vkQueueWaitIdle)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkQueueWaitIdle"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR) __vkGetDeviceProcAddr(device, "vkCreateSwapchainKHR");
  if (!__vkCreateSwapchainKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCreateSwapchainKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR) __vkGetDeviceProcAddr(device, "vkGetSwapchainImagesKHR");
  if (!__vkGetSwapchainImagesKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkGetSwapchainImagesKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR) __vkGetDeviceProcAddr(device, "vkAcquireNextImageKHR");
  if (!__vkAcquireNextImageKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkAcquireNextImageKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkQueuePresentKHR = (PFN_vkQueuePresentKHR) __vkGetDeviceProcAddr(device, "vkQueuePresentKHR");
  if (!__vkQueuePresentKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkQueuePresentKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateCommandPool = (PFN_vkCreateCommandPool) __vkGetDeviceProcAddr(device, "vkCreateCommandPool");
  if (!__vkCreateCommandPool)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCreateCommandPool"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers) __vkGetDeviceProcAddr(device, "vkAllocateCommandBuffers");
  if (!__vkAllocateCommandBuffers)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkAllocateCommandBuffers"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer) __vkGetDeviceProcAddr(device, "vkBeginCommandBuffer");
  if (!__vkBeginCommandBuffer)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkBeginCommandBuffer"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkEndCommandBuffer = (PFN_vkEndCommandBuffer) __vkGetDeviceProcAddr(device, "vkEndCommandBuffer");
  if (!__vkEndCommandBuffer)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkEndCommandBuffer"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateSemaphore = (PFN_vkCreateSemaphore) __vkGetDeviceProcAddr(device, "vkCreateSemaphore");
  if (!__vkCreateSemaphore)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCreateSemaphore"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateFence = (PFN_vkCreateFence) __vkGetDeviceProcAddr(device, "vkCreateFence");
  if (!__vkCreateFence)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCreateFence"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkWaitForFences = (PFN_vkWaitForFences) __vkGetDeviceProcAddr(device, "vkWaitForFences");
  if (!__vkWaitForFences)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkWaitForFences"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkResetFences = (PFN_vkResetFences) __vkGetDeviceProcAddr(device, "vkResetFences");
  if (!__vkResetFences)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkResetFences"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyDevice = (PFN_vkDestroyDevice) __vkGetDeviceProcAddr(device, "vkDestroyDevice");
  if (!__vkDestroyDevice)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkDestroyDevice"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetDeviceQueue = (PFN_vkGetDeviceQueue) __vkGetDeviceProcAddr(device, "vkGetDeviceQueue");
  if (!__vkGetDeviceQueue)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkGetDeviceQueue"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR) __vkGetDeviceProcAddr(device, "vkDestroySwapchainKHR");
  if (!__vkDestroySwapchainKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkDestroySwapchainKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyCommandPool = (PFN_vkDestroyCommandPool) __vkGetDeviceProcAddr(device, "vkDestroyCommandPool");
  if (!__vkDestroyCommandPool)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkDestroyCommandPool"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkFreeCommandBuffers = (PFN_vkFreeCommandBuffers) __vkGetDeviceProcAddr(device, "vkFreeCommandBuffers");
  if (!__vkFreeCommandBuffers)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkFreeCommandBuffers"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroySemaphore = (PFN_vkDestroySemaphore) __vkGetDeviceProcAddr(device, "vkDestroySemaphore");
  if (!__vkDestroySemaphore)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkDestroySemaphore"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyFence = (PFN_vkDestroyFence) __vkGetDeviceProcAddr(device, "vkDestroyFence");
  if (!__vkDestroyFence)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkDestroyFence"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdBeginRenderPass = (PFN_vkCmdBeginRenderPass) __vkGetDeviceProcAddr(device, "vkCmdBeginRenderPass");
  if (!__vkCmdBeginRenderPass)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCmdBeginRenderPass"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdEndRenderPass = (PFN_vkCmdEndRenderPass) __vkGetDeviceProcAddr(device, "vkCmdEndRenderPass");
  if (!__vkCmdEndRenderPass)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCmdEndRenderPass"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdBindPipeline = (PFN_vkCmdBindPipeline) __vkGetDeviceProcAddr(device, "vkCmdBindPipeline");
  if (!__vkCmdBindPipeline)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCmdBindPipeline"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers) __vkGetDeviceProcAddr(device, "vkCmdBindVertexBuffers");
  if (!__vkCmdBindVertexBuffers)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCmdBindVertexBuffers"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer) __vkGetDeviceProcAddr(device, "vkCmdBindIndexBuffer");
  if (!__vkCmdBindIndexBuffer)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCmdBindIndexBuffer"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdDraw = (PFN_vkCmdDraw) __vkGetDeviceProcAddr(device, "vkCmdDraw");
  if (!__vkCmdDraw)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCmdDraw"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdDrawIndexed = (PFN_vkCmdDrawIndexed) __vkGetDeviceProcAddr(device, "vkCmdDrawIndexed");
  if (!__vkCmdDrawIndexed)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCmdDrawIndexed"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdDispatch = (PFN_vkCmdDispatch) __vkGetDeviceProcAddr(device, "vkCmdDispatch");
  if (!__vkCmdDispatch)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan function: vkCmdDispatch"), S("Vulkan.cgen"), 0);
    return false;
  }

  return true;
}

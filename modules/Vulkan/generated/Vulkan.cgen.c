/* Generated code */

fz_function b32
vulkan_load_global_functions()
{
  __vkCreateInstance = (PFN_vkCreateInstance)__vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkCreateInstance");
  if (!__vkCreateInstance)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateInstance"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)__vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceExtensionProperties");
  if (!__vkEnumerateInstanceExtensionProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkEnumerateInstanceExtensionProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)__vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceLayerProperties");
  if (!__vkEnumerateInstanceLayerProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkEnumerateInstanceLayerProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)__vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
  if (!__vkEnumerateInstanceVersion)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkEnumerateInstanceVersion"), S("Vulkan.cgen"), 0);
    return false;
  }

  return true;
}

fz_function b32
vulkan_load_instance_functions(VkInstance instance)
{
  __vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices) __vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDevices");
  if (!__vkEnumeratePhysicalDevices)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkEnumeratePhysicalDevices"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkEnumeratePhysicalDeviceGroups = (PFN_vkEnumeratePhysicalDeviceGroups) __vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDeviceGroups");
  if (!__vkEnumeratePhysicalDeviceGroups)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkEnumeratePhysicalDeviceGroups"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateDevice = (PFN_vkCreateDevice) __vkGetInstanceProcAddr(instance, "vkCreateDevice");
  if (!__vkCreateDevice)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateDevice"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkEnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties) __vkGetInstanceProcAddr(instance, "vkEnumerateDeviceExtensionProperties");
  if (!__vkEnumerateDeviceExtensionProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkEnumerateDeviceExtensionProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkEnumerateDeviceLayerProperties = (PFN_vkEnumerateDeviceLayerProperties) __vkGetInstanceProcAddr(instance, "vkEnumerateDeviceLayerProperties");
  if (!__vkEnumerateDeviceLayerProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkEnumerateDeviceLayerProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
  if (!__vkGetPhysicalDeviceSurfaceSupportKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceSurfaceSupportKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
  if (!__vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceSurfaceCapabilitiesKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
  if (!__vkGetPhysicalDeviceSurfaceFormatsKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceSurfaceFormatsKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
  if (!__vkGetPhysicalDeviceSurfacePresentModesKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceSurfacePresentModesKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR) __vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
  if (!__vkCreateWin32SurfaceKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateWin32SurfaceKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceToolPropertiesEXT = (PFN_vkGetPhysicalDeviceToolPropertiesEXT) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceToolPropertiesEXT");
  if (!__vkGetPhysicalDeviceToolPropertiesEXT)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceToolPropertiesEXT"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) __vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (!__vkCreateDebugUtilsMessengerEXT)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateDebugUtilsMessengerEXT"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyInstance = (PFN_vkDestroyInstance) __vkGetInstanceProcAddr(instance, "vkDestroyInstance");
  if (!__vkDestroyInstance)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyInstance"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR) __vkGetInstanceProcAddr(instance, "vkDestroySurfaceKHR");
  if (!__vkDestroySurfaceKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroySurfaceKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) __vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (!__vkDestroyDebugUtilsMessengerEXT)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyDebugUtilsMessengerEXT"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkSubmitDebugUtilsMessageEXT = (PFN_vkSubmitDebugUtilsMessageEXT) __vkGetInstanceProcAddr(instance, "vkSubmitDebugUtilsMessageEXT");
  if (!__vkSubmitDebugUtilsMessageEXT)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkSubmitDebugUtilsMessageEXT"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures");
  if (!__vkGetPhysicalDeviceFeatures)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceFeatures"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceFeatures2 = (PFN_vkGetPhysicalDeviceFeatures2) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures2");
  if (!__vkGetPhysicalDeviceFeatures2)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceFeatures2"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties");
  if (!__vkGetPhysicalDeviceProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceProperties2 = (PFN_vkGetPhysicalDeviceProperties2) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties2");
  if (!__vkGetPhysicalDeviceProperties2)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceProperties2"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties");
  if (!__vkGetPhysicalDeviceMemoryProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceMemoryProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties");
  if (!__vkGetPhysicalDeviceQueueFamilyProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceQueueFamilyProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties");
  if (!__vkGetPhysicalDeviceFormatProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceFormatProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceImageFormatProperties = (PFN_vkGetPhysicalDeviceImageFormatProperties) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceImageFormatProperties");
  if (!__vkGetPhysicalDeviceImageFormatProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceImageFormatProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceFormatProperties2 = (PFN_vkGetPhysicalDeviceFormatProperties2) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties2");
  if (!__vkGetPhysicalDeviceFormatProperties2)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceFormatProperties2"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceImageFormatProperties2 = (PFN_vkGetPhysicalDeviceImageFormatProperties2) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceImageFormatProperties2");
  if (!__vkGetPhysicalDeviceImageFormatProperties2)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceImageFormatProperties2"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceSparseImageFormatProperties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSparseImageFormatProperties");
  if (!__vkGetPhysicalDeviceSparseImageFormatProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceSparseImageFormatProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceSparseImageFormatProperties2 = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSparseImageFormatProperties2");
  if (!__vkGetPhysicalDeviceSparseImageFormatProperties2)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceSparseImageFormatProperties2"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceExternalBufferProperties = (PFN_vkGetPhysicalDeviceExternalBufferProperties) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceExternalBufferProperties");
  if (!__vkGetPhysicalDeviceExternalBufferProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceExternalBufferProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceExternalFenceProperties = (PFN_vkGetPhysicalDeviceExternalFenceProperties) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceExternalFenceProperties");
  if (!__vkGetPhysicalDeviceExternalFenceProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceExternalFenceProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetPhysicalDeviceExternalSemaphoreProperties = (PFN_vkGetPhysicalDeviceExternalSemaphoreProperties) __vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceExternalSemaphoreProperties");
  if (!__vkGetPhysicalDeviceExternalSemaphoreProperties)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetPhysicalDeviceExternalSemaphoreProperties"), S("Vulkan.cgen"), 0);
    return false;
  }

  return true;
}

fz_function b32
vulkan_load_device_functions(VkDevice device)
{
  __vkDeviceWaitIdle = (PFN_vkDeviceWaitIdle) __vkGetDeviceProcAddr(device, "vkDeviceWaitIdle");
  if (!__vkDeviceWaitIdle)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDeviceWaitIdle"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkQueueSubmit = (PFN_vkQueueSubmit) __vkGetDeviceProcAddr(device, "vkQueueSubmit");
  if (!__vkQueueSubmit)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkQueueSubmit"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkQueueSubmit2 = (PFN_vkQueueSubmit2) __vkGetDeviceProcAddr(device, "vkQueueSubmit2");
  if (!__vkQueueSubmit2)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkQueueSubmit2"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkQueueWaitIdle = (PFN_vkQueueWaitIdle) __vkGetDeviceProcAddr(device, "vkQueueWaitIdle");
  if (!__vkQueueWaitIdle)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkQueueWaitIdle"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR) __vkGetDeviceProcAddr(device, "vkCreateSwapchainKHR");
  if (!__vkCreateSwapchainKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateSwapchainKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR) __vkGetDeviceProcAddr(device, "vkGetSwapchainImagesKHR");
  if (!__vkGetSwapchainImagesKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetSwapchainImagesKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR) __vkGetDeviceProcAddr(device, "vkAcquireNextImageKHR");
  if (!__vkAcquireNextImageKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkAcquireNextImageKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateCommandPool = (PFN_vkCreateCommandPool) __vkGetDeviceProcAddr(device, "vkCreateCommandPool");
  if (!__vkCreateCommandPool)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateCommandPool"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers) __vkGetDeviceProcAddr(device, "vkAllocateCommandBuffers");
  if (!__vkAllocateCommandBuffers)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkAllocateCommandBuffers"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer) __vkGetDeviceProcAddr(device, "vkBeginCommandBuffer");
  if (!__vkBeginCommandBuffer)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkBeginCommandBuffer"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkEndCommandBuffer = (PFN_vkEndCommandBuffer) __vkGetDeviceProcAddr(device, "vkEndCommandBuffer");
  if (!__vkEndCommandBuffer)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkEndCommandBuffer"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateSemaphore = (PFN_vkCreateSemaphore) __vkGetDeviceProcAddr(device, "vkCreateSemaphore");
  if (!__vkCreateSemaphore)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateSemaphore"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateFence = (PFN_vkCreateFence) __vkGetDeviceProcAddr(device, "vkCreateFence");
  if (!__vkCreateFence)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateFence"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkWaitForFences = (PFN_vkWaitForFences) __vkGetDeviceProcAddr(device, "vkWaitForFences");
  if (!__vkWaitForFences)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkWaitForFences"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkResetFences = (PFN_vkResetFences) __vkGetDeviceProcAddr(device, "vkResetFences");
  if (!__vkResetFences)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkResetFences"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateBuffer = (PFN_vkCreateBuffer) __vkGetDeviceProcAddr(device, "vkCreateBuffer");
  if (!__vkCreateBuffer)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateBuffer"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateImage = (PFN_vkCreateImage) __vkGetDeviceProcAddr(device, "vkCreateImage");
  if (!__vkCreateImage)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateImage"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateImageView = (PFN_vkCreateImageView) __vkGetDeviceProcAddr(device, "vkCreateImageView");
  if (!__vkCreateImageView)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateImageView"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateSampler = (PFN_vkCreateSampler) __vkGetDeviceProcAddr(device, "vkCreateSampler");
  if (!__vkCreateSampler)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateSampler"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkAllocateMemory = (PFN_vkAllocateMemory) __vkGetDeviceProcAddr(device, "vkAllocateMemory");
  if (!__vkAllocateMemory)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkAllocateMemory"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateShaderModule = (PFN_vkCreateShaderModule) __vkGetDeviceProcAddr(device, "vkCreateShaderModule");
  if (!__vkCreateShaderModule)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateShaderModule"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreatePipelineLayout = (PFN_vkCreatePipelineLayout) __vkGetDeviceProcAddr(device, "vkCreatePipelineLayout");
  if (!__vkCreatePipelineLayout)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreatePipelineLayout"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines) __vkGetDeviceProcAddr(device, "vkCreateGraphicsPipelines");
  if (!__vkCreateGraphicsPipelines)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateGraphicsPipelines"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateComputePipelines = (PFN_vkCreateComputePipelines) __vkGetDeviceProcAddr(device, "vkCreateComputePipelines");
  if (!__vkCreateComputePipelines)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateComputePipelines"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout) __vkGetDeviceProcAddr(device, "vkCreateDescriptorSetLayout");
  if (!__vkCreateDescriptorSetLayout)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateDescriptorSetLayout"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateDescriptorPool = (PFN_vkCreateDescriptorPool) __vkGetDeviceProcAddr(device, "vkCreateDescriptorPool");
  if (!__vkCreateDescriptorPool)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateDescriptorPool"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkAllocateDescriptorSets = (PFN_vkAllocateDescriptorSets) __vkGetDeviceProcAddr(device, "vkAllocateDescriptorSets");
  if (!__vkAllocateDescriptorSets)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkAllocateDescriptorSets"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateRenderPass = (PFN_vkCreateRenderPass) __vkGetDeviceProcAddr(device, "vkCreateRenderPass");
  if (!__vkCreateRenderPass)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateRenderPass"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateRenderPass2 = (PFN_vkCreateRenderPass2) __vkGetDeviceProcAddr(device, "vkCreateRenderPass2");
  if (!__vkCreateRenderPass2)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateRenderPass2"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateFramebuffer = (PFN_vkCreateFramebuffer) __vkGetDeviceProcAddr(device, "vkCreateFramebuffer");
  if (!__vkCreateFramebuffer)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateFramebuffer"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCreateQueryPool = (PFN_vkCreateQueryPool) __vkGetDeviceProcAddr(device, "vkCreateQueryPool");
  if (!__vkCreateQueryPool)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCreateQueryPool"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetQueryPoolResults = (PFN_vkGetQueryPoolResults) __vkGetDeviceProcAddr(device, "vkGetQueryPoolResults");
  if (!__vkGetQueryPoolResults)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetQueryPoolResults"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyDevice = (PFN_vkDestroyDevice) __vkGetDeviceProcAddr(device, "vkDestroyDevice");
  if (!__vkDestroyDevice)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyDevice"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetDeviceQueue = (PFN_vkGetDeviceQueue) __vkGetDeviceProcAddr(device, "vkGetDeviceQueue");
  if (!__vkGetDeviceQueue)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetDeviceQueue"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkGetDeviceQueue2 = (PFN_vkGetDeviceQueue2) __vkGetDeviceProcAddr(device, "vkGetDeviceQueue2");
  if (!__vkGetDeviceQueue2)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkGetDeviceQueue2"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR) __vkGetDeviceProcAddr(device, "vkDestroySwapchainKHR");
  if (!__vkDestroySwapchainKHR)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroySwapchainKHR"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyBuffer = (PFN_vkDestroyBuffer) __vkGetDeviceProcAddr(device, "vkDestroyBuffer");
  if (!__vkDestroyBuffer)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyBuffer"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyImage = (PFN_vkDestroyImage) __vkGetDeviceProcAddr(device, "vkDestroyImage");
  if (!__vkDestroyImage)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyImage"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyImageView = (PFN_vkDestroyImageView) __vkGetDeviceProcAddr(device, "vkDestroyImageView");
  if (!__vkDestroyImageView)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyImageView"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroySampler = (PFN_vkDestroySampler) __vkGetDeviceProcAddr(device, "vkDestroySampler");
  if (!__vkDestroySampler)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroySampler"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkFreeMemory = (PFN_vkFreeMemory) __vkGetDeviceProcAddr(device, "vkFreeMemory");
  if (!__vkFreeMemory)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkFreeMemory"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyShaderModule = (PFN_vkDestroyShaderModule) __vkGetDeviceProcAddr(device, "vkDestroyShaderModule");
  if (!__vkDestroyShaderModule)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyShaderModule"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyPipeline = (PFN_vkDestroyPipeline) __vkGetDeviceProcAddr(device, "vkDestroyPipeline");
  if (!__vkDestroyPipeline)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyPipeline"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyPipelineLayout = (PFN_vkDestroyPipelineLayout) __vkGetDeviceProcAddr(device, "vkDestroyPipelineLayout");
  if (!__vkDestroyPipelineLayout)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyPipelineLayout"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout) __vkGetDeviceProcAddr(device, "vkDestroyDescriptorSetLayout");
  if (!__vkDestroyDescriptorSetLayout)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyDescriptorSetLayout"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyDescriptorPool = (PFN_vkDestroyDescriptorPool) __vkGetDeviceProcAddr(device, "vkDestroyDescriptorPool");
  if (!__vkDestroyDescriptorPool)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyDescriptorPool"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkFreeDescriptorSets = (PFN_vkFreeDescriptorSets) __vkGetDeviceProcAddr(device, "vkFreeDescriptorSets");
  if (!__vkFreeDescriptorSets)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkFreeDescriptorSets"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyRenderPass = (PFN_vkDestroyRenderPass) __vkGetDeviceProcAddr(device, "vkDestroyRenderPass");
  if (!__vkDestroyRenderPass)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyRenderPass"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyFramebuffer = (PFN_vkDestroyFramebuffer) __vkGetDeviceProcAddr(device, "vkDestroyFramebuffer");
  if (!__vkDestroyFramebuffer)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyFramebuffer"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyQueryPool = (PFN_vkDestroyQueryPool) __vkGetDeviceProcAddr(device, "vkDestroyQueryPool");
  if (!__vkDestroyQueryPool)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyQueryPool"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroySemaphore = (PFN_vkDestroySemaphore) __vkGetDeviceProcAddr(device, "vkDestroySemaphore");
  if (!__vkDestroySemaphore)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroySemaphore"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyFence = (PFN_vkDestroyFence) __vkGetDeviceProcAddr(device, "vkDestroyFence");
  if (!__vkDestroyFence)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyFence"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkResetCommandPool = (PFN_vkResetCommandPool) __vkGetDeviceProcAddr(device, "vkResetCommandPool");
  if (!__vkResetCommandPool)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkResetCommandPool"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkDestroyCommandPool = (PFN_vkDestroyCommandPool) __vkGetDeviceProcAddr(device, "vkDestroyCommandPool");
  if (!__vkDestroyCommandPool)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkDestroyCommandPool"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkFreeCommandBuffers = (PFN_vkFreeCommandBuffers) __vkGetDeviceProcAddr(device, "vkFreeCommandBuffers");
  if (!__vkFreeCommandBuffers)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkFreeCommandBuffers"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdBeginRenderPass = (PFN_vkCmdBeginRenderPass) __vkGetDeviceProcAddr(device, "vkCmdBeginRenderPass");
  if (!__vkCmdBeginRenderPass)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdBeginRenderPass"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdEndRenderPass = (PFN_vkCmdEndRenderPass) __vkGetDeviceProcAddr(device, "vkCmdEndRenderPass");
  if (!__vkCmdEndRenderPass)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdEndRenderPass"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdBeginRenderPass2 = (PFN_vkCmdBeginRenderPass2) __vkGetDeviceProcAddr(device, "vkCmdBeginRenderPass2");
  if (!__vkCmdBeginRenderPass2)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdBeginRenderPass2"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdEndRenderPass2 = (PFN_vkCmdEndRenderPass2) __vkGetDeviceProcAddr(device, "vkCmdEndRenderPass2");
  if (!__vkCmdEndRenderPass2)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdEndRenderPass2"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdBindPipeline = (PFN_vkCmdBindPipeline) __vkGetDeviceProcAddr(device, "vkCmdBindPipeline");
  if (!__vkCmdBindPipeline)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdBindPipeline"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets) __vkGetDeviceProcAddr(device, "vkCmdBindDescriptorSets");
  if (!__vkCmdBindDescriptorSets)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdBindDescriptorSets"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers) __vkGetDeviceProcAddr(device, "vkCmdBindVertexBuffers");
  if (!__vkCmdBindVertexBuffers)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdBindVertexBuffers"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer) __vkGetDeviceProcAddr(device, "vkCmdBindIndexBuffer");
  if (!__vkCmdBindIndexBuffer)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdBindIndexBuffer"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdDraw = (PFN_vkCmdDraw) __vkGetDeviceProcAddr(device, "vkCmdDraw");
  if (!__vkCmdDraw)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdDraw"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdDrawIndexed = (PFN_vkCmdDrawIndexed) __vkGetDeviceProcAddr(device, "vkCmdDrawIndexed");
  if (!__vkCmdDrawIndexed)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdDrawIndexed"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdDispatch = (PFN_vkCmdDispatch) __vkGetDeviceProcAddr(device, "vkCmdDispatch");
  if (!__vkCmdDispatch)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdDispatch"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdCopyBuffer = (PFN_vkCmdCopyBuffer) __vkGetDeviceProcAddr(device, "vkCmdCopyBuffer");
  if (!__vkCmdCopyBuffer)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdCopyBuffer"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdCopyImage = (PFN_vkCmdCopyImage) __vkGetDeviceProcAddr(device, "vkCmdCopyImage");
  if (!__vkCmdCopyImage)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdCopyImage"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdBlitImage = (PFN_vkCmdBlitImage) __vkGetDeviceProcAddr(device, "vkCmdBlitImage");
  if (!__vkCmdBlitImage)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdBlitImage"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdCopyBufferToImage = (PFN_vkCmdCopyBufferToImage) __vkGetDeviceProcAddr(device, "vkCmdCopyBufferToImage");
  if (!__vkCmdCopyBufferToImage)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdCopyBufferToImage"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdCopyImageToBuffer = (PFN_vkCmdCopyImageToBuffer) __vkGetDeviceProcAddr(device, "vkCmdCopyImageToBuffer");
  if (!__vkCmdCopyImageToBuffer)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdCopyImageToBuffer"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdPipelineBarrier = (PFN_vkCmdPipelineBarrier) __vkGetDeviceProcAddr(device, "vkCmdPipelineBarrier");
  if (!__vkCmdPipelineBarrier)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdPipelineBarrier"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdPushConstants = (PFN_vkCmdPushConstants) __vkGetDeviceProcAddr(device, "vkCmdPushConstants");
  if (!__vkCmdPushConstants)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdPushConstants"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdSetViewport = (PFN_vkCmdSetViewport) __vkGetDeviceProcAddr(device, "vkCmdSetViewport");
  if (!__vkCmdSetViewport)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdSetViewport"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdSetScissor = (PFN_vkCmdSetScissor) __vkGetDeviceProcAddr(device, "vkCmdSetScissor");
  if (!__vkCmdSetScissor)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdSetScissor"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdSetLineWidth = (PFN_vkCmdSetLineWidth) __vkGetDeviceProcAddr(device, "vkCmdSetLineWidth");
  if (!__vkCmdSetLineWidth)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdSetLineWidth"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdSetDepthBias = (PFN_vkCmdSetDepthBias) __vkGetDeviceProcAddr(device, "vkCmdSetDepthBias");
  if (!__vkCmdSetDepthBias)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdSetDepthBias"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdSetBlendConstants = (PFN_vkCmdSetBlendConstants) __vkGetDeviceProcAddr(device, "vkCmdSetBlendConstants");
  if (!__vkCmdSetBlendConstants)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdSetBlendConstants"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdSetDepthBounds = (PFN_vkCmdSetDepthBounds) __vkGetDeviceProcAddr(device, "vkCmdSetDepthBounds");
  if (!__vkCmdSetDepthBounds)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdSetDepthBounds"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdSetStencilCompareMask = (PFN_vkCmdSetStencilCompareMask) __vkGetDeviceProcAddr(device, "vkCmdSetStencilCompareMask");
  if (!__vkCmdSetStencilCompareMask)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdSetStencilCompareMask"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdSetStencilWriteMask = (PFN_vkCmdSetStencilWriteMask) __vkGetDeviceProcAddr(device, "vkCmdSetStencilWriteMask");
  if (!__vkCmdSetStencilWriteMask)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdSetStencilWriteMask"), S("Vulkan.cgen"), 0);
    return false;
  }

  __vkCmdSetStencilReference = (PFN_vkCmdSetStencilReference) __vkGetDeviceProcAddr(device, "vkCmdSetStencilReference");
  if (!__vkCmdSetStencilReference)
  {
    message_box(S("Vulkan"), S("Unable to load Vulkan fz_function: vkCmdSetStencilReference"), S("Vulkan.cgen"), 0);
    return false;
  }

  return true;
}

#include "VulkanFunctions.hpp"

namespace TestFramework {

	void VulkanLoader::LoadExtensionFunctions(VkDevice InDevice, VkInstance InInstance)
	{
		evkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)(vkGetInstanceProcAddr(InInstance, "vkDestroySurfaceKHR"));
		evkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT) (vkGetDeviceProcAddr(InDevice, "vkSetDebugUtilsObjectNameEXT"));
		evkCmdPipelineBarrier2KHR = (PFN_vkCmdPipelineBarrier2KHR)(vkGetDeviceProcAddr(InDevice, "vkCmdPipelineBarrier2KHR"));
		evkCmdBeginRenderingKHR = (PFN_vkCmdBeginRenderingKHR) (vkGetDeviceProcAddr(InDevice, "vkCmdBeginRenderingKHR"));
		evkCmdEndRenderingKHR = (PFN_vkCmdEndRenderingKHR) (vkGetDeviceProcAddr(InDevice, "vkCmdEndRenderingKHR"));
	}
}

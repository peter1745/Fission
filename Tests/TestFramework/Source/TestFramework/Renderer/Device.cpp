#include "Device.hpp"
#include "VulkanFunctions.hpp"
#include "CommandBuffer.hpp"

#include <iostream>

namespace TestFramework {

	static const std::vector<const char*> c_DeviceExtensions = { "VK_KHR_synchronization2", "VK_KHR_dynamic_rendering" };

	Device::Device(VkInstance InInstance, VkSurfaceKHR InSurface)
	{
		FindSuitablePhysicalDevice(InInstance, InSurface);
		CreateLogicalDevice();

		VulkanLoader::LoadExtensionFunctions(m_Device, InInstance);
	}

	void Device::Wait() const
	{
		vkDeviceWaitIdle(m_Device);
	}

	void Device::Release()
	{
		vkDeviceWaitIdle(m_Device);
		vkDestroyDevice(m_Device, nullptr);

		m_Device = VK_NULL_HANDLE;
		m_PhysicalDevice = VK_NULL_HANDLE;
	}

	SurfaceProperties Device::GetSurfaceProperties(VkSurfaceKHR InSurface) const
	{
		SurfaceProperties Result = {};

		uint32_t NumFormats = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, InSurface, &NumFormats, nullptr);
		Result.Formats.resize(NumFormats);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, InSurface, &NumFormats, Result.Formats.data());

		uint32_t NumPresentModes = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, InSurface, &NumPresentModes, nullptr);
		Result.PresentModes.resize(NumPresentModes);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, InSurface, &NumPresentModes, Result.PresentModes.data());

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, InSurface, &Result.Capabilities);

		return Result;
	}

	void Device::FindSuitablePhysicalDevice(VkInstance InInstance, VkSurfaceKHR InSurface)
	{
		uint32_t NumPhysicalDevices = 0;
		vkEnumeratePhysicalDevices(InInstance, &NumPhysicalDevices, nullptr);
		std::vector<VkPhysicalDevice> AvailableDevices(NumPhysicalDevices);
		vkEnumeratePhysicalDevices(InInstance, &NumPhysicalDevices, AvailableDevices.data());

		uint32_t MaxScore = 0;

		for (auto PhysicalDevice : AvailableDevices)
		{
			VkPhysicalDeviceProperties DeviceProperties;
			vkGetPhysicalDeviceProperties(PhysicalDevice, &DeviceProperties);

			VkPhysicalDeviceFeatures DeviceFeatures;
			vkGetPhysicalDeviceFeatures(PhysicalDevice, &DeviceFeatures);

			uint32_t Score = 0;

			// Give score based on GPU type
			switch (DeviceProperties.deviceType)
			{
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			{
				Score += 3;
				break;
			}
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			{
				Score += 4;
				break;
			}
			case VK_PHYSICAL_DEVICE_TYPE_CPU:
			{
				Score += 2;
				break;
			}
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			{
				Score += 1;
				break;
			}
			case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			{
				Score = 0;
				break;
			}
			case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
			{
				break;
			}
			}

			// Queue Families
			{
				auto DeviceQueues = GetQueues(PhysicalDevice, InSurface);

				for (const auto& QueueInfo : DeviceQueues)
				{
					if (QueueInfo.SupportsPresent && QueueInfo.SupportsGraphics)
						Score += 10;
				}
			}

			if (Score > MaxScore)
			{
				MaxScore = Score;
				m_PhysicalDevice = PhysicalDevice;
			}
		}

		// Found suitable GPU, now determine Graphics + Present Queue
		auto DeviceQueues = GetQueues(m_PhysicalDevice, InSurface);

		for (const auto& QueueInfo : DeviceQueues)
		{
			if (QueueInfo.SupportsPresent && QueueInfo.SupportsGraphics)
			{
				// Always prefer queues that have BOTH present and graphics capabilities
				m_Queue = QueueInfo;
				break;
			}
		}

		// Print GPU info
		VkPhysicalDeviceProperties DeviceProperties;
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &DeviceProperties);
		std::cout << "Selected GPU: " << DeviceProperties.deviceName << "\n";
	}

	void Device::CreateLogicalDevice()
	{
		float QueuePriority = 1.0f;
		VkDeviceQueueCreateInfo QueueInfo = {};
		QueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		QueueInfo.queueFamilyIndex = m_Queue.QueueFamily;
		QueueInfo.queueCount = 1;
		QueueInfo.pQueuePriorities = &QueuePriority;

		VkPhysicalDeviceVulkan13Features Vulkan13Features = {};
		Vulkan13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		Vulkan13Features.dynamicRendering = VK_TRUE;
		Vulkan13Features.synchronization2 = VK_TRUE;

		VkPhysicalDeviceFeatures2 DeviceFeatures2 = {};
		DeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		DeviceFeatures2.pNext = &Vulkan13Features;

		VkDeviceCreateInfo DeviceInfo = {};
		DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		DeviceInfo.pQueueCreateInfos = &QueueInfo;
		DeviceInfo.queueCreateInfoCount = 1;
		DeviceInfo.pNext = &DeviceFeatures2;

		std::vector<const char*> RequiredExtensions(c_DeviceExtensions);
		RequiredExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		DeviceInfo.ppEnabledExtensionNames = RequiredExtensions.data();
		DeviceInfo.enabledExtensionCount = RequiredExtensions.size();

		vkCreateDevice(m_PhysicalDevice, &DeviceInfo, nullptr, &m_Device);
		vkGetDeviceQueue(m_Device, m_Queue.QueueFamily, 0, &m_Queue.Queue);
	}

	std::vector<QueueInfo> Device::GetQueues(VkPhysicalDevice InPhysicalDevice, VkSurfaceKHR InSurface)
	{
		uint32_t NumQueues = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(InPhysicalDevice, &NumQueues, nullptr);
		std::vector<VkQueueFamilyProperties> QueueProperties(NumQueues);
		vkGetPhysicalDeviceQueueFamilyProperties(InPhysicalDevice, &NumQueues, QueueProperties.data());

		std::vector<QueueInfo> Result(NumQueues);

		for (uint32_t QueueIdx = 0; QueueIdx < NumQueues; QueueIdx++)
		{
			auto& QueueInfo = Result[QueueIdx];
			QueueInfo.QueueFamily = QueueIdx;

			// Check for present support
			vkGetPhysicalDeviceSurfaceSupportKHR(InPhysicalDevice, QueueIdx, InSurface, reinterpret_cast<VkBool32*>(&QueueInfo.SupportsPresent));

			QueueInfo.SupportsGraphics = QueueProperties[QueueIdx].queueFlags & VK_QUEUE_GRAPHICS_BIT;
			QueueInfo.SupportCompute = QueueProperties[QueueIdx].queueFlags & VK_QUEUE_COMPUTE_BIT;
		}

		return Result;
	}

}
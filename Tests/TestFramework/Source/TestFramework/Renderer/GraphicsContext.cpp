#include "GraphicsContext.hpp"
#include "SwapChain.hpp"
#include "VulkanFunctions.hpp"

#include "TestFramework/Core/Application.hpp"

#include <queue>

#include <GLFW/glfw3.h>

namespace TestFramework {

#ifdef FUSION_DEBUG
	static const std::vector<const char*> s_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
#endif

	static const std::vector<const char*> c_InstanceExtensions = { "VK_KHR_get_physical_device_properties2", "VK_EXT_debug_utils" };

	static GraphicsContext* s_Instance = nullptr;

	GraphicsContext::GraphicsContext()
	{
		VkApplicationInfo AppInfo = {};
		AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		AppInfo.pApplicationName = "TestFramework Vulkan App";
		AppInfo.pEngineName = "TestFramework";
		AppInfo.apiVersion = VK_API_VERSION_1_3;

		// Create Vulkan Instance
		{
			VkInstanceCreateInfo InstanceInfo = {};
			InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			InstanceInfo.pApplicationInfo = &AppInfo;

			// Instance extensions
			uint32_t NumGLFWExtensions = 0;
			const char** GLFWExtensions = glfwGetRequiredInstanceExtensions(&NumGLFWExtensions);
			std::vector<const char*> InstanceExtensions(NumGLFWExtensions);
			for (uint32_t Idx = 0; Idx < NumGLFWExtensions; Idx++)
				InstanceExtensions[Idx] = GLFWExtensions[Idx];

			for (auto Ext : c_InstanceExtensions)
				InstanceExtensions.push_back(Ext);

			InstanceInfo.enabledExtensionCount = InstanceExtensions.size();
			InstanceInfo.ppEnabledExtensionNames = InstanceExtensions.data();

#ifdef FUSION_DEBUG
			InstanceInfo.enabledLayerCount = s_ValidationLayers.size();
			InstanceInfo.ppEnabledLayerNames = s_ValidationLayers.data();
#endif

			vkCreateInstance(&InstanceInfo, nullptr, &m_Instance);
		}

		// Create Window Surface via GLFW
		{
			auto* NativeWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetWindowHandle());
			glfwCreateWindowSurface(m_Instance, NativeWindow, nullptr, &m_Surface);
		}

		m_Device = std::make_unique<Device>(m_Instance, m_Surface);

		CommandPoolInfo AllocatorInfo = {};
		AllocatorInfo.InitialListCount = 1;
		m_TemporaryCommandAllocator = std::make_unique<CommandPool>(m_Device.get(), AllocatorInfo);

		m_Allocator = MakeUnique<VulkanAllocator>(m_Instance, m_Device.get());

		s_Instance = this;
	}

	GraphicsContext::~GraphicsContext()
	{
	}

	void GraphicsContext::Release()
	{
		m_TemporaryCommandAllocator->Release();
		m_TemporaryCommandAllocator = nullptr;

		m_Device->Release();
		m_Device = nullptr;

		evkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
	}

	GraphicsContext* GraphicsContext::Get() { return s_Instance; }

}

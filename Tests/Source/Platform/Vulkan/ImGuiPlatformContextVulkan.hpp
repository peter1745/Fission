#pragma once

#include "UI/ImGuiPlatformContext.hpp"

#include <TestFramework/Renderer/SwapChain.hpp>
#include <FTL/UUID.hpp>

#include <vulkan/vulkan.h>

namespace FissionTests {

	class ImGuiPlatformContextVulkan : public ImGuiPlatformContext
	{
	public:
		virtual void InitPlatform(const TestFramework::Unique<TestFramework::Window>& InWindow, TestFramework::GraphicsContext* InContext, TestFramework::SwapChain* InSwapChain) override;
		virtual void BeginFramePlatform(TestFramework::CommandBuffer* InCommandList) override;
		virtual void EndFramePlatform(TestFramework::CommandBuffer* InCommandList) override;
		virtual void ShutdownPlatform() override;

	private:
		void OnSwapChainInvalidated(const TestFramework::SwapChain& InSwapChain);

	private:
		TestFramework::SwapChain* m_SwapChain = nullptr;
		VkRenderPass m_RenderPass = VK_NULL_HANDLE;
		std::vector<VkFramebuffer> m_FrameBuffers;
		VkDescriptorPool m_FontDescriptorPool = VK_NULL_HANDLE;
		FTL::UUID m_SwapChainCallbackHandle = 0;
	};
}

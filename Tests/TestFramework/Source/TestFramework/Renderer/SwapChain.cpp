#include "SwapChain.hpp"
#include "VulkanFunctions.hpp"

#include "TestFramework/Core/Application.hpp"

#include <GLFW/glfw3.h>

namespace TestFramework {

	SwapChain::SwapChain(GraphicsContext* InContext, const SwapChainInfo& InCreateInfo)
	    : m_Context(InContext), m_CreateInfo(InCreateInfo)
	{
		Create(false);
	}

	void SwapChain::Bind(CommandBuffer* InCommandList)
	{
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = float(m_CreateInfo.Width);
		viewport.height = float(m_CreateInfo.Height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 0.0f;
		vkCmdSetViewport(InCommandList->GetBuffer(), 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = m_ImageExtent;
		vkCmdSetScissor(InCommandList->GetBuffer(), 0, 1, &scissor);

		ImageTransitionInfo transitionInfo = {};
		transitionInfo.Image = m_Images[m_CurrentImage];
		transitionInfo.OldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		transitionInfo.NewLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		transitionInfo.SrcAccessFlag = 0;
		transitionInfo.DstAccessFlag = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		transitionInfo.SrcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		transitionInfo.DstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		transitionInfo.AspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		TransitionImage(InCommandList->GetBuffer(), transitionInfo);

		ImageTransitionInfo depthTransitionInfo = {};
		depthTransitionInfo.Image = m_DepthImage;
		depthTransitionInfo.OldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthTransitionInfo.NewLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		depthTransitionInfo.SrcAccessFlag = 0;
		depthTransitionInfo.DstAccessFlag = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		depthTransitionInfo.SrcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		depthTransitionInfo.DstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		depthTransitionInfo.AspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		TransitionImage(InCommandList->GetBuffer(), depthTransitionInfo);

		VkClearValue clearValue = {};
		clearValue.color.float32[0] = 0.0f;
		clearValue.color.float32[1] = 0.0f;
		clearValue.color.float32[2] = 0.0f;
		clearValue.color.float32[3] = 1.0f;

		VkRenderingAttachmentInfoKHR colorAttachment = {};
		colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
		colorAttachment.imageView = m_ImageViews[m_CurrentImage];
		colorAttachment.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.clearValue = clearValue;

		VkClearValue depthClearValue = {};
		depthClearValue.depthStencil.depth = 1.0f;

		VkRenderingAttachmentInfoKHR depthAttachment = {};
		depthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
		depthAttachment.imageView = m_DepthImageView;
		depthAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		depthAttachment.clearValue = depthClearValue;

		VkRenderingInfoKHR renderingInfo = {};
		renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
		renderingInfo.renderArea.offset = { 0, 0 };
		renderingInfo.renderArea.extent = m_ImageExtent;
		renderingInfo.layerCount = 1;
		renderingInfo.viewMask = 0;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.pColorAttachments = &colorAttachment;
		renderingInfo.pDepthAttachment = &depthAttachment;
		evkCmdBeginRenderingKHR(InCommandList->GetBuffer(), &renderingInfo);
	}

	void SwapChain::Clear(CommandBuffer* InCommandList) {}

	void SwapChain::Present(VkSemaphore InFinishedSemaphore)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &InFinishedSemaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_SwapChain;
		presentInfo.pImageIndices = &m_CurrentImage;
		VkResult result = vkQueuePresentKHR(m_Context->GetDevice()->GetQueueInfo().Queue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			Invalidate();
	}

	void SwapChain::Unbind(CommandBuffer* InCommandList)
	{
		auto cmdList = dynamic_cast<CommandBuffer*>(InCommandList)->GetBuffer();
		evkCmdEndRenderingKHR(cmdList);
	}

	void SwapChain::Resize(uint32_t InWidth, uint32_t InHeight)
	{
	}

	void SwapChain::Create(bool InWasInvalidated)
	{
		SurfaceProperties surfaceProps = m_Context->GetDevice()->GetSurfaceProperties(m_Context->GetSurface());

		VkSurfaceFormatKHR format = surfaceProps.Formats[0];
		if (format.format != VK_FORMAT_B8G8R8A8_UNORM || format.colorSpace != VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			// Find suitable format

			for (uint32_t Idx = 1; Idx < surfaceProps.Formats.size(); Idx++)
			{
				format = surfaceProps.Formats[Idx];

				if (format.format == VK_FORMAT_B8G8R8A8_UNORM)
					break;
			}

			if (format.format != VK_FORMAT_B8G8R8A8_UNORM)
				format = surfaceProps.Formats[0]; // Just pick the first if the desired one isn't found
		}

		m_ImageFormat = format.format;

		// Find suitable present mode
		VkPresentModeKHR presentMode = surfaceProps.PresentModes[0];
		if (presentMode != VK_PRESENT_MODE_MAILBOX_KHR)
		{
			for (uint32_t i = 1; i < surfaceProps.PresentModes.size(); i++)
			{
				presentMode = surfaceProps.PresentModes[i];
				if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
					break;
			}

			// Look for FIFO if mailbox isn't supported
			if (presentMode != VK_PRESENT_MODE_MAILBOX_KHR)
			{
				for (uint32_t i = 1; i < surfaceProps.PresentModes.size(); i++)
				{
					presentMode = surfaceProps.PresentModes[i];
					if (presentMode == VK_PRESENT_MODE_FIFO_KHR)
						break;
				}
			}
		}

		if (surfaceProps.Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			m_ImageExtent = surfaceProps.Capabilities.currentExtent;
		}
		else
		{
			auto* nativeWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetWindowHandle());
			int32_t imageWidth = 0, imageHeight = 0;
			glfwGetFramebufferSize(nativeWindow, &imageWidth, &imageHeight);
			m_ImageExtent.width = std::clamp(uint32_t(imageWidth), surfaceProps.Capabilities.minImageExtent.width, surfaceProps.Capabilities.maxImageExtent.width);
			m_ImageExtent.height = std::clamp(uint32_t(imageHeight), surfaceProps.Capabilities.minImageExtent.height, surfaceProps.Capabilities.maxImageExtent.height);
		}

		m_ImageCount = std::min(surfaceProps.Capabilities.minImageCount + 1, surfaceProps.Capabilities.maxImageCount);

		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = m_Context->GetSurface();
		swapchainInfo.minImageCount = m_ImageCount;
		swapchainInfo.imageFormat = format.format;
		swapchainInfo.imageColorSpace = format.colorSpace;
		swapchainInfo.imageExtent = m_ImageExtent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = 0;
		swapchainInfo.pQueueFamilyIndices = nullptr;
		swapchainInfo.preTransform = surfaceProps.Capabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = presentMode;
		swapchainInfo.clipped = VK_TRUE;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

		vkCreateSwapchainKHR(m_Context->GetDevice()->GetLogicalDevice(), &swapchainInfo, nullptr, &m_SwapChain);

		// Get SwapChain images and create image views
		{
			vkGetSwapchainImagesKHR(m_Context->GetDevice()->GetLogicalDevice(), m_SwapChain, &m_ImageCount, nullptr);
			m_Images.resize(m_ImageCount);
			m_ImageViews.resize(m_ImageCount);
			vkGetSwapchainImagesKHR(m_Context->GetDevice()->GetLogicalDevice(), m_SwapChain, &m_ImageCount, m_Images.data());

			for (uint32_t i = 0; i < m_ImageCount; i++)
			{
				VkImageViewCreateInfo imageViewInfo = {};
				imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				imageViewInfo.image = m_Images[i];
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				imageViewInfo.format = format.format;

				imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageViewInfo.subresourceRange.levelCount = 1;
				imageViewInfo.subresourceRange.baseMipLevel = 0;
				imageViewInfo.subresourceRange.layerCount = 1;
				imageViewInfo.subresourceRange.baseArrayLayer = 0;

				vkCreateImageView(m_Context->GetDevice()->GetLogicalDevice(), &imageViewInfo, nullptr, &m_ImageViews[i]);
			}

			// Create Depth Image
			{
				VkImageCreateInfo depthInfo = {};
				depthInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				depthInfo.imageType = VK_IMAGE_TYPE_2D;
				depthInfo.format = VK_FORMAT_D24_UNORM_S8_UINT;
				depthInfo.extent.width = m_ImageExtent.width;
				depthInfo.extent.height = m_ImageExtent.height;
				depthInfo.extent.depth = 1;
				depthInfo.mipLevels = 1;
				depthInfo.arrayLayers = 1;
				depthInfo.samples = VK_SAMPLE_COUNT_1_BIT;
				depthInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				depthInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				depthInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				m_DepthAllocation = m_Context->GetAllocator()->CreateImage(depthInfo, &m_DepthImage);

				VkImageViewCreateInfo imageViewInfo = {};
				imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				imageViewInfo.image = m_DepthImage;
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				imageViewInfo.format = VK_FORMAT_D24_UNORM_S8_UINT;
				imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
				imageViewInfo.subresourceRange.levelCount = 1;
				imageViewInfo.subresourceRange.baseMipLevel = 0;
				imageViewInfo.subresourceRange.layerCount = 1;
				imageViewInfo.subresourceRange.baseArrayLayer = 0;

				vkCreateImageView(m_Context->GetDevice()->GetLogicalDevice(), &imageViewInfo, nullptr, &m_DepthImageView);
			}
		}

		if (InWasInvalidated)
			m_OnInvalidatedCallbacks.Invoke(*this);
	}

	void SwapChain::Invalidate()
	{
		m_Context->GetDevice()->Wait();

		vkDestroyImageView(m_Context->GetDevice()->GetLogicalDevice(), m_DepthImageView, nullptr);
		m_Context->GetAllocator()->DestroyImage(m_DepthAllocation, m_DepthImage);

		for (auto& view : m_ImageViews)
			vkDestroyImageView(m_Context->GetDevice()->GetLogicalDevice(), view, nullptr);

		vkDestroySwapchainKHR(m_Context->GetDevice()->GetLogicalDevice(), m_SwapChain, nullptr);

		m_CurrentImage = 0;

		Create(true);
	}

	bool SwapChain::AcquireNextImage(VkDevice InDevice, VkSemaphore InImageAvailableSemaphore)
	{
		VkResult result = vkAcquireNextImageKHR(InDevice, m_SwapChain, UINT64_MAX, InImageAvailableSemaphore, VK_NULL_HANDLE, &m_CurrentImage);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			Invalidate();
			return false;
		}

		return true;
	}

	void SwapChain::Release()
	{
		// Ensure the queue isn't busy before we try to destroy the resources
		vkQueueWaitIdle(m_Context->GetDevice()->GetQueueInfo().Queue);

		vkDestroyImageView(m_Context->GetDevice()->GetLogicalDevice(), m_DepthImageView, nullptr);
		m_Context->GetAllocator()->DestroyImage(m_DepthAllocation, m_DepthImage);

		for (auto& view : m_ImageViews)
			vkDestroyImageView(m_Context->GetDevice()->GetLogicalDevice(), view, nullptr);

		vkDestroySwapchainKHR(m_Context->GetDevice()->GetLogicalDevice(), m_SwapChain, nullptr);
	}

}
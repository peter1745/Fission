#include "RenderTexture.hpp"
#include "GraphicsContext.hpp"
#include "VulkanFunctions.hpp"

#include "TestFramework/Renderer/Renderer.hpp"

#include <vulkan/vulkan.h>

namespace TestFramework {

	RenderTexture::RenderTexture(const RenderTextureInfo& InCreateInfo)
	    : m_CreateInfo(InCreateInfo)
	{
		Invalidate();
	}

	void RenderTexture::Bind(CommandBuffer* InCommandList)
	{
		VkExtent2D imageExtent = { m_CreateInfo.Width, m_CreateInfo.Height };

		for (uint32_t i = 0; i < m_CreateInfo.ColorAttachments.size(); i++)
		{
			ImageTransitionInfo transitionInfo = {};
			transitionInfo.Image = m_Images[i]->GetImage();
			transitionInfo.OldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			transitionInfo.NewLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			transitionInfo.SrcAccessFlag = 0;
			transitionInfo.DstAccessFlag = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			transitionInfo.SrcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			transitionInfo.DstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			transitionInfo.AspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

			TransitionImage(InCommandList->GetBuffer(), transitionInfo);

			m_Images[i]->m_State = EImageState::ColorAttachmentOptimal;
		}

		if (IsDepthFormat(m_CreateInfo.DepthAttachment.Format))
		{
			ImageTransitionInfo transitionInfo = {};
			transitionInfo.Image = m_Images[m_Images.size() - 1]->GetImage();
			transitionInfo.OldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			transitionInfo.NewLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			transitionInfo.SrcAccessFlag = 0;
			transitionInfo.DstAccessFlag = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			transitionInfo.SrcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			transitionInfo.DstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			transitionInfo.AspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
			TransitionImage(InCommandList->GetBuffer(), transitionInfo);
		}

		std::vector<VkRenderingAttachmentInfoKHR> colorAttachments(m_CreateInfo.ColorAttachments.size());
		for (size_t i = 0; i < m_CreateInfo.ColorAttachments.size(); i++)
		{
			auto& attachment = colorAttachments[i];
			attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
			attachment.imageView = m_ImageViews[i];
			attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachment.clearValue = m_ClearValues[i];
		}

		VkRenderingAttachmentInfoKHR depthAttachment = {};
		depthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
		
		bool hasDepthAttachment = IsDepthFormat(m_CreateInfo.DepthAttachment.Format);
		if (hasDepthAttachment)
		{
			depthAttachment.imageView = m_ImageViews[m_ImageViews.size() - 1];
			depthAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			depthAttachment.clearValue = m_ClearValues[m_ClearValues.size() - 1];
		}

		VkRenderingInfoKHR renderingInfo = {};
		renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
		renderingInfo.renderArea.offset = { 0, 0 };
		renderingInfo.renderArea.extent = imageExtent;
		renderingInfo.layerCount = 1;
		renderingInfo.viewMask = 0;
		renderingInfo.colorAttachmentCount = colorAttachments.size();
		renderingInfo.pColorAttachments = colorAttachments.data();
		renderingInfo.pDepthAttachment = hasDepthAttachment ? &depthAttachment : nullptr;
		evkCmdBeginRenderingKHR(InCommandList->GetBuffer(), &renderingInfo);
	}

	void RenderTexture::Unbind(CommandBuffer* InCommandList)
	{
		evkCmdEndRenderingKHR(InCommandList->GetBuffer());

		for (uint32_t i = 0; i < m_CreateInfo.ColorAttachments.size(); i++)
		{
			ImageTransitionInfo transitionInfo = {};
			transitionInfo.Image = m_Images[i]->GetImage();
			transitionInfo.OldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			transitionInfo.NewLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			transitionInfo.SrcAccessFlag = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			transitionInfo.DstAccessFlag = 0;
			transitionInfo.SrcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			transitionInfo.DstStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			transitionInfo.AspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			TransitionImage(InCommandList->GetBuffer(), transitionInfo);

			m_Images[i]->m_State = EImageState::PixelShaderResource;
		}
	}

	void RenderTexture::Clear() {}

	void RenderTexture::Resize(uint32_t InAttachmentIndex, const ImageSize& InSize)
	{
		if (InSize.Width == 0 || InSize.Height == 0)
			return;

		m_CreateInfo.Width = InSize.Width;
		m_CreateInfo.Height = InSize.Height;

		Invalidate();
	}

	void RenderTexture::TransitionImages(CommandBuffer* InCommandList, EImageState InColorAttachmentState, EImageState InDepthStencilState)
	{
		//m_Images[0]->Transition(InCommandList, InColorAttachmentState);
		//m_Images[m_Images.size() - 1]->Transition(InCommandList, InDepthStencilState);
	}

	void RenderTexture::Invalidate()
	{
		Release();

		uint32_t attachmentCount = m_CreateInfo.ColorAttachments.size() + (IsDepthFormat(m_CreateInfo.DepthAttachment.Format) ? 1 : 0);

		m_Images.resize(attachmentCount);
		m_ImageViews.resize(attachmentCount);

		m_ClearValues.resize(attachmentCount);

		for (uint32_t attachmentIdx = 0; attachmentIdx < m_CreateInfo.ColorAttachments.size(); attachmentIdx++)
		{
			const auto& attachmentInfo = m_CreateInfo.ColorAttachments[attachmentIdx];

			Image2DInfo imageInfo = {};
			imageInfo.Size = { m_CreateInfo.Width, m_CreateInfo.Height };
			imageInfo.Usage = EImageUsage::Attachment;
			imageInfo.Format = attachmentInfo.Format;
			imageInfo.Flags = attachmentInfo.Flags;
			imageInfo.InitialState = EImageState::Undefined;
			memcpy(imageInfo.ClearColor, attachmentInfo.ClearColor, sizeof(imageInfo.ClearColor));

			if (attachmentInfo.IsMappable)
				imageInfo.Usage |= EImageUsage::CopySource;

			std::unique_ptr<Image2D> image = std::make_unique<Image2D>(imageInfo);

			VkImageViewCreateInfo viewInfo = {};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = image->GetImage();
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = EFormatToVkFormat(attachmentInfo.Format);
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.layerCount = 1;
			viewInfo.subresourceRange.levelCount = 1;

			vkCreateImageView(GraphicsContext::Get()->GetDevice()->GetLogicalDevice(), &viewInfo, nullptr, &m_ImageViews[attachmentIdx]);

			m_Images[attachmentIdx] = std::move(image);

			memcpy(m_ClearValues[attachmentIdx].color.float32, attachmentInfo.ClearColor, sizeof(m_ClearValues[attachmentIdx].color.float32));
		}

		if (IsDepthFormat(m_CreateInfo.DepthAttachment.Format))
		{
			Image2DInfo imageInfo = {};
			imageInfo.Size = { m_CreateInfo.Width, m_CreateInfo.Height };
			imageInfo.Usage = EImageUsage::Attachment;
			imageInfo.Format = m_CreateInfo.DepthAttachment.Format;
			imageInfo.Flags = m_CreateInfo.DepthAttachment.Flags;
			imageInfo.InitialState = EImageState::Undefined;
			memcpy(imageInfo.ClearColor, m_CreateInfo.DepthAttachment.ClearColor, sizeof(imageInfo.ClearColor));

			std::unique_ptr<Image2D> image = std::make_unique<Image2D>(imageInfo);

			VkImageViewCreateInfo viewInfo = {};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = image->GetImage();
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = EFormatToVkFormat(m_CreateInfo.DepthAttachment.Format);
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.layerCount = 1;
			viewInfo.subresourceRange.levelCount = 1;

			vkCreateImageView(GraphicsContext::Get()->GetDevice()->GetLogicalDevice(), &viewInfo, nullptr, &m_ImageViews[m_ImageViews.size() - 1]);

			m_Images.emplace_back(std::move(image));

			m_ClearValues[attachmentCount - 1].depthStencil.depth = 1.0f;
		}
	}

	void RenderTexture::Release()
	{
		/*Renderer::GetCurrent().SubmitResourceForDestruction(3, [InImageViews = m_ImageViews]() {
			auto Device = GraphicsContext::Get<GraphicsContext>()->GetDevice().As<Device>();

			for (auto& ImageView : InImageViews)
				vkDestroyImageView(Device->GetLogicalDevice(), ImageView, nullptr);
		});*/

		for (auto& image : m_Images)
		{
			if (!image)
				continue;

			image->Release();
		}
	}
}
#include "Image.hpp"
#include "Device.hpp"
#include "GraphicsContext.hpp"

#include "TestFramework/Renderer/Renderer.hpp"

namespace TestFramework {

	Image2D::Image2D(const Image2DInfo& InCreateInfo)
	    : m_CreateInfo(InCreateInfo)
	{
		m_State = m_CreateInfo.InitialState;

		Invalidate();
	}

	void Image2D::Transition(CommandBuffer* InCmdList, EImageState InState)
	{
		if (m_State == InState)
			return;

		ImageTransitionInfo transitionInfo = {};
		transitionInfo.Image = m_Image;
		transitionInfo.OldLayout = ImageStatesToVkImageLayout(m_State);
		transitionInfo.NewLayout = ImageStatesToVkImageLayout(InState);
		transitionInfo.SrcAccessFlag = 0;
		transitionInfo.DstAccessFlag = 0;
		transitionInfo.SrcStage = 0;
		transitionInfo.DstStage = 0;
		transitionInfo.AspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		TransitionImage(InCmdList->GetBuffer(), transitionInfo);

		m_State = InState;
	}

	void Image2D::CopyTo(CommandBuffer* InCmdList, const ImageRegion& InRegion, Buffer* InBuffer)
	{
		VkBufferImageCopy copyRegion;
		copyRegion.bufferOffset = 0;
		copyRegion.bufferRowLength = 0;
		copyRegion.bufferImageHeight = 0;
		copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copyRegion.imageSubresource.layerCount = 1;
		copyRegion.imageSubresource.baseArrayLayer = 0;
		copyRegion.imageSubresource.mipLevel = 0;
		copyRegion.imageOffset = { InRegion.X, InRegion.Y, 0 };
		copyRegion.imageExtent.width = InRegion.Width;
		copyRegion.imageExtent.height = InRegion.Height;
		copyRegion.imageExtent.depth = 1;

		vkCmdCopyImageToBuffer(InCmdList->GetBuffer(), m_Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, InBuffer->GetBuffer(), 1, &copyRegion);
	}

	void Image2D::CopyFrom(CommandBuffer* InCmdList, const ImageRegion& InRegion, Buffer* InBuffer)
	{
		VkBufferImageCopy copyRegion;
		copyRegion.bufferOffset = 0;
		copyRegion.bufferRowLength = 0;
		copyRegion.bufferImageHeight = 0;
		copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copyRegion.imageSubresource.layerCount = 1;
		copyRegion.imageSubresource.baseArrayLayer = 0;
		copyRegion.imageSubresource.mipLevel = 0;
		copyRegion.imageOffset = { InRegion.X, InRegion.Y, 0 };
		copyRegion.imageExtent.width = InRegion.Width;
		copyRegion.imageExtent.height = InRegion.Height;
		copyRegion.imageExtent.depth = 1;

		vkCmdCopyBufferToImage(InCmdList->GetBuffer(), InBuffer->GetBuffer(), m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
	}

	void Image2D::Invalidate()
	{
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.extent.width = m_CreateInfo.Size.Width;
		imageCreateInfo.extent.height = m_CreateInfo.Size.Height;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.format = EFormatToVkFormat(m_CreateInfo.Format);
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.usage = GetImageUsageFlags(m_CreateInfo.Usage, m_CreateInfo.Format);
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.flags = 0;

		m_Allocation = GraphicsContext::Get()->GetAllocator()->CreateImage(imageCreateInfo, &m_Image);
	}

	void Image2D::Release()
	{
		/*Renderer::GetCurrent().SubmitResourceForDestruction(3, [InAllocation = m_Allocation, InImage = m_Image]()
	    {
		    auto* Allocator = GraphicsContext::Get<GraphicsContext>()->GetAllocator();
		    Allocator->DestroyImage(InAllocation, InImage);
	    });*/
	}

}

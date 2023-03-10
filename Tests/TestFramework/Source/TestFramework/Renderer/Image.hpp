#pragma once

#include "Common.hpp"
#include "Device.hpp"
#include "CommandBuffer.hpp"
#include "Buffer.hpp"

#include <VMA/vk_mem_alloc.h>

namespace TestFramework {

	struct ImageSize
	{
		uint32_t Width;
		uint32_t Height;
	};

	struct Image2DInfo
	{
		ImageSize Size;

		EImageUsage Usage;
		EFormat Format;
		EImageFlag Flags;
		EImageState InitialState;

		// Only used for render targets and depth stencils
		float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		uint32_t Channels = 0;

		bool IsMappable = false;
	};

	struct ImageRegion
	{
		int32_t X;
		int32_t Y;
		int32_t Width;
		int32_t Height;
	};

	class Image2D
	{
	public:
		Image2D(const Image2DInfo& InCreateInfo);

		const ImageSize& GetSize() const { return m_CreateInfo.Size; }

		void Resize(const ImageSize& InSize)
		{
			m_CreateInfo.Size = InSize;
			Invalidate();
		}

		void Transition(CommandBuffer* InCmdList, EImageState InState);

		void CopyTo(CommandBuffer* InCmdList, const ImageRegion& InRegion, Buffer* InBuffer);
		void CopyFrom(CommandBuffer* InCmdList, const ImageRegion& InRegion, Buffer* InBuffer);

		EImageState GetState() const { return m_State; }

		const Image2DInfo& GetInfo() const { return m_CreateInfo; }

		void Release();

		VkImage GetImage() { return m_Image; }

	private:
		void Invalidate();

	private:
		Image2DInfo m_CreateInfo;

		VkImage m_Image = VK_NULL_HANDLE;
		VmaAllocation m_Allocation;
		EImageState m_State;

		friend class RenderTexture;
	};

}

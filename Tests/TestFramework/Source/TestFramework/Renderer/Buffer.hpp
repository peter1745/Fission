#pragma once

#include "Common.hpp"
#include "VulkanAllocator.hpp"

namespace TestFramework {

	struct BufferInfo
	{
		EBufferUsage Usage;
		uint64_t Size;

		void* InitialData = nullptr;
	};

	class CommandBuffer;

	class Buffer
	{
	public:
		Buffer(const BufferInfo& InInfo);
		~Buffer();

		void* Map();
		void Unmap();

		//void Transition(CommandList* InCmdList, EBufferState InState);

		void SetData(CommandBuffer* InCmdList, const void* InData, Buffer* InUploadBuffer);

		const BufferInfo& GetInfo() const { return m_Info; }
		uint64_t GetSize() const { return m_Info.Size; }

		VkBuffer GetBuffer() const { return m_Buffer; }

	private:
		BufferInfo m_Info;

		VkBuffer m_Buffer = VK_NULL_HANDLE;
		VmaAllocation m_Allocation;
	};

}

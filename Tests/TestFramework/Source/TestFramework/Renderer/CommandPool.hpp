#pragma once

#include "Device.hpp"
#include "CommandBuffer.hpp"

namespace TestFramework {

	struct CommandPoolInfo
	{
		size_t InitialListCount = 0;
	};

	class CommandPool
	{
	public:
		CommandPool(Device* InDevice, const CommandPoolInfo& InCreateInfo);
		~CommandPool();

		CommandBuffer* AllocateCommandBuffer();
		std::vector<CommandBuffer*> AllocateCommandBuffers(size_t InCount);

		void DestroyCommandBuffer(CommandBuffer* InCommandBuffer);

		CommandBuffer* GetCommandBuffer(size_t InIndex) const
		{
			return m_CommandBuffers[InIndex].get();
		}

		void Reset();

		void Release();

	private:
		CommandPoolInfo m_CreateInfo;
		Device* m_Device = nullptr;
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
		std::vector<Unique<CommandBuffer>> m_CommandBuffers;
	};

}

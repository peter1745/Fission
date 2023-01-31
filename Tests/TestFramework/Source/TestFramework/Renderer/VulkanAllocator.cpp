#include "VulkanAllocator.hpp"

namespace TestFramework {

	VulkanAllocator::VulkanAllocator(VkInstance InInstance, Device* InDevice)
	{
		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.instance = InInstance;
		allocatorInfo.physicalDevice = InDevice->GetPhysicalDevice();
		allocatorInfo.device = InDevice->GetLogicalDevice();
		allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_3;
		allocatorInfo.preferredLargeHeapBlockSize = 0;
		vmaCreateAllocator(&allocatorInfo, &m_Allocator);
	}

	VulkanAllocator::~VulkanAllocator()
	{
	}

	VmaAllocation VulkanAllocator::CreateImage(const VkImageCreateInfo& InCreateInfo, VkImage* OutImage)
	{
		VmaAllocationCreateInfo allocationInfo = {};
		allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;

		VmaAllocation allocation;
		vmaCreateImage(m_Allocator, &InCreateInfo, &allocationInfo, OutImage, &allocation, nullptr);

		return allocation;
	}

	void VulkanAllocator::DestroyImage(VmaAllocation InAllocation, VkImage InImage)
	{
		vmaDestroyImage(m_Allocator, InImage, InAllocation);
	}

	VmaAllocation VulkanAllocator::CreateBuffer(const VkBufferCreateInfo& InCreateInfo, VkBuffer* OutBuffer)
	{
		VmaAllocationCreateInfo allocationInfo = {};
		allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;
		allocationInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

		VmaAllocation allocation;
		vmaCreateBuffer(m_Allocator, &InCreateInfo, &allocationInfo, OutBuffer, &allocation, nullptr);

		return allocation;
	}

	void* VulkanAllocator::MapMemory(VmaAllocation InAllocation) const
	{
		void* mappedMemory;
		vmaMapMemory(m_Allocator, InAllocation, &mappedMemory);
		return mappedMemory;
	}

	void VulkanAllocator::UnmapMemory(VmaAllocation InAllocation) const
	{
		vmaUnmapMemory(m_Allocator, InAllocation);
	}

	VmaAllocationInfo VulkanAllocator::GetAllocationInfo(VmaAllocation InAllocation) const
	{
		VmaAllocationInfo allocationInfo;
		vmaGetAllocationInfo(m_Allocator, InAllocation, &allocationInfo);
		return allocationInfo;
	}

}

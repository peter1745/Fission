#include "Renderer.hpp"
#include "CommandBuffer.hpp"
#include "GraphicsContext.hpp"

//#include <tracy/Tracy.hpp>

#include <algorithm>

namespace TestFramework {

	static Renderer* s_Instance = nullptr;

	Renderer::Renderer(GraphicsContext* InContext, const RendererInfo& InInfo)
	    : m_Context(InContext), m_Info(InInfo)
	{
		m_FramesInFlight = std::min(m_FramesInFlight, InInfo.TargetSwapChain->GetImageCount());

		// Create frame semaphores and fences
		{
			m_ImageAvailableSemaphores.resize(m_FramesInFlight);
			m_RenderFinishedSemaphores.resize(m_FramesInFlight);
			m_ImageFences.resize(m_FramesInFlight);

			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VkFenceCreateInfo fenceInfo = {};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			for (uint32_t i = 0; i < m_FramesInFlight; i++)
			{
				vkCreateSemaphore(InContext->GetDevice()->GetLogicalDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]);
				vkCreateSemaphore(InContext->GetDevice()->GetLogicalDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]);
				vkCreateFence(InContext->GetDevice()->GetLogicalDevice(), &fenceInfo, nullptr, &m_ImageFences[i]);
			}
		}

		// Create the command allocators
		{
			CommandPoolInfo allocatorInfo = {};
			allocatorInfo.InitialListCount = 1;

			for (uint32_t i = 0; i < m_FramesInFlight; i++)
				m_CommandAllocators.push_back(std::make_unique<CommandPool>(InContext->GetDevice(), allocatorInfo));
		}

		s_Instance = this;
	}

	void Renderer::BeginFrame()
	{
		//ZoneScoped;

		vkWaitForFences(m_Context->GetDevice()->GetLogicalDevice(), 1, &m_ImageFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex = m_Info.TargetSwapChain->AcquireNextImage(m_Context->GetDevice()->GetLogicalDevice(), m_ImageAvailableSemaphores[m_CurrentFrame]);

		if (imageIndex == UINT32_MAX)
		{
			// Failed to acquire next image, possibly because we need to invalidate the swap chain images

			for (auto& imageSemaphores : m_ImageAvailableSemaphores)
				vkDestroySemaphore(m_Context->GetDevice()->GetLogicalDevice(), imageSemaphores, nullptr);

			for (auto& renderSemaphores : m_RenderFinishedSemaphores)
				vkDestroySemaphore(m_Context->GetDevice()->GetLogicalDevice(), renderSemaphores, nullptr);

			for (auto& imageFence : m_ImageFences)
				vkDestroyFence(m_Context->GetDevice()->GetLogicalDevice(), imageFence, nullptr);

			return;
		}

		vkResetFences(m_Context->GetDevice()->GetLogicalDevice(), 1, &m_ImageFences[m_CurrentFrame]);

		for (auto it = m_DestroyQueue.begin(); it != m_DestroyQueue.end();)
		{
			if (it->FrameIndex == m_CurrentFrame)
			{
				it->DestroyFunc();
				it = m_DestroyQueue.erase(it);
			}
			else
			{
				it++;
			}
		}

		m_CommandAllocators[m_CurrentFrame]->Reset();
		m_CommandAllocators[m_CurrentFrame]->GetCommandBuffer(0)->BeginRecording();
	}

	void Renderer::EndFrame()
	{
		//ZoneScoped;

		auto* commandList = m_CommandAllocators[m_CurrentFrame]->GetCommandBuffer(0);

		ImageTransitionInfo transitionInfo = {};
		transitionInfo.Image = m_Info.TargetSwapChain->GetImage(m_Info.TargetSwapChain->GetCurrentImage());
		transitionInfo.OldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		transitionInfo.NewLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		transitionInfo.SrcAccessFlag = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		transitionInfo.DstAccessFlag = 0;
		transitionInfo.SrcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		transitionInfo.DstStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		transitionInfo.AspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		TransitionImage(commandList->GetBuffer(), transitionInfo);

		commandList->EndRecording();

		// Submit command list (Deliberately not using ExecuteCommandLists)
		{
			VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
			VkPipelineStageFlags waitStageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };

			VkCommandBuffer commandBuffer = commandList->GetBuffer();

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStageFlags;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			const auto& queueInfo = m_Context->GetDevice()->GetQueueInfo();
			vkQueueSubmit(queueInfo.Queue, 1, &submitInfo, m_ImageFences[m_CurrentFrame]);
		}

		m_Info.TargetSwapChain->Present(m_RenderFinishedSemaphores[m_CurrentFrame]);

		m_CurrentFrame = (m_CurrentFrame + 1) % m_FramesInFlight;
	}

	void Renderer::ExecuteCommandLists(const std::vector<CommandBuffer*>& InCommandLists)
	{
		ExecuteCommandLists(InCommandLists, true);
	}

	void Renderer::ExecuteCommandLists(const std::vector<CommandBuffer*>& InCommandLists, bool InShouldSignal)
	{
		//ZoneScoped;

		std::vector<VkCommandBuffer> commandBuffers;
		commandBuffers.reserve(InCommandLists.size());
		for (auto* commandList : InCommandLists)
			commandBuffers.push_back(dynamic_cast<CommandBuffer*>(commandList)->GetBuffer());

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		if (InShouldSignal)
		{
			submitInfo.waitSemaphoreCount = 1;
			VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
			VkPipelineStageFlags waitStageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStageFlags;
		}

		submitInfo.commandBufferCount = InCommandLists.size();
		submitInfo.pCommandBuffers = commandBuffers.data();

		if (InShouldSignal)
		{
			VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;
		}

		VkFence fence = InShouldSignal ? m_ImageFences[m_CurrentFrame] : VK_NULL_HANDLE;

		const auto& queueInfo = m_Context->GetDevice()->GetQueueInfo();
		vkQueueSubmit(queueInfo.Queue, 1, &submitInfo, fence);
	}

	void Renderer::ExecuteImmediate(const std::function<void(CommandBuffer*)>& InFunc, bool InWaitBeforeExit)
	{
		auto commandAllocator = m_Context->GetTemporaryCommandAllocator();
		auto* commandList = commandAllocator->AllocateCommandBuffer();
		commandList->Reset();
		commandList->BeginRecording();

		InFunc(commandList);

		commandList->EndRecording();

		const auto& queueInfo = m_Context->GetDevice()->GetQueueInfo();
		VkCommandBuffer nativeBuffer = commandList->GetBuffer();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &nativeBuffer;
		vkQueueSubmit(queueInfo.Queue, 1, &submitInfo, VK_NULL_HANDLE);

		if (InWaitBeforeExit)
			vkQueueWaitIdle(queueInfo.Queue);

		SubmitResourceForDestruction(2, [commandAllocator, commandList]() mutable
		{
			commandAllocator->DestroyCommandBuffer(commandList);
		});
	}

	void Renderer::Release()
	{
		for (auto& imageSemaphores : m_ImageAvailableSemaphores)
			vkDestroySemaphore(m_Context->GetDevice()->GetLogicalDevice(), imageSemaphores, nullptr);

		for (auto& renderSemaphores : m_RenderFinishedSemaphores)
			vkDestroySemaphore(m_Context->GetDevice()->GetLogicalDevice(), renderSemaphores, nullptr);

		for (auto& imageFence : m_ImageFences)
			vkDestroyFence(m_Context->GetDevice()->GetLogicalDevice(), imageFence, nullptr);

		for (auto& commandAllocator : m_CommandAllocators)
			commandAllocator->Release();
		m_CommandAllocators.clear();
	}

	Renderer& Renderer::GetCurrent() { return *s_Instance; }

	CommandQueue::CommandQueue()
	{
		m_CommandBuffer = new uint8_t[10 * 1024 * 1024];
		m_CommandBufferPtr = m_CommandBuffer;
		memset(m_CommandBuffer, 0, 10 * 1024 * 1024);
	}

	CommandQueue::~CommandQueue()
	{
		delete[] m_CommandBuffer;
	}

	void* CommandQueue::AllocateCommand(CommandQueue::CommandFunc InFunc, uint32_t InSize)
	{
		*reinterpret_cast<CommandFunc*>(m_CommandBufferPtr) = InFunc;
		m_CommandBufferPtr += sizeof(CommandFunc);

		*reinterpret_cast<uint32_t*>(m_CommandBufferPtr) = InSize;
		m_CommandBufferPtr += sizeof(uint32_t);

		void* memory = m_CommandBufferPtr;
		m_CommandBufferPtr += InSize;

		m_CommandCount++;
		return memory;
	}

	void CommandQueue::Execute(CommandBuffer* InCmdList)
	{
		uint8_t* buffer = m_CommandBuffer;

		for (uint32_t i = 0; i < m_CommandCount; i++)
		{
			CommandFunc func = *reinterpret_cast<CommandFunc*>(buffer);
			buffer += sizeof(CommandFunc);

			uint32_t size = *reinterpret_cast<uint32_t*>(buffer);
			buffer += sizeof(uint32_t);

			func(buffer, InCmdList);
			buffer += size;
		}

		m_CommandBufferPtr = m_CommandBuffer;
		m_CommandCount = 0;
	}

}
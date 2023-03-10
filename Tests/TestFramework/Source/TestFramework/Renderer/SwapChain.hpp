#pragma once

#include "Common.hpp"
#include "GraphicsContext.hpp"
#include "CommandBuffer.hpp"

#include <FTL/FunctionList.hpp>

#include <stdint.h>

namespace TestFramework {

	struct SwapChainInfo
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		bool HasDepthBuffer = false;

		float RenderTargetClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	};

	class SwapChain
	{
		using InvalidatedCallbacks = FTL::FunctionList<void(const SwapChain&)>;
	public:
		SwapChain(GraphicsContext* InContext, const SwapChainInfo& InCreateInfo);
		~SwapChain() = default;

		void Bind(CommandBuffer* InCommandList);
		void Clear(CommandBuffer* InCommandList);
		void Present(VkSemaphore InFinishedSemaphore);
		void Unbind(CommandBuffer* InCommandList);

		void Resize(uint32_t InWidth, uint32_t InHeight);

		bool AcquireNextImage(VkDevice InDevice, VkSemaphore InImageAvailableSemaphore);

		uint32_t GetImageCount() const { return m_ImageCount; }

		void Release();

		VkImageView GetImageView(uint32_t InImageIndex) const { return m_ImageViews[InImageIndex]; }
		VkFormat GetImageFormat() const { return m_ImageFormat; }
		VkExtent2D GetImageExtent() const { return m_ImageExtent; }

		VkImage GetImage(uint32_t InImageIndex) const { return m_Images[InImageIndex]; }

		uint32_t GetCurrentImage() const { return m_CurrentImage; }

		InvalidatedCallbacks::KeyType RegisterOnInvalidatedCallback(const InvalidatedCallbacks::FuncType& InCallback)
		{
			return m_OnInvalidatedCallbacks.AddFunction(InCallback);
		}

		void UnregisterOnInvalidatedCallback(const InvalidatedCallbacks::KeyType& InKey)
		{
			m_OnInvalidatedCallbacks.RemoveFunction(InKey);
		}

	private:
		void Create(bool InWasInvalidated);

		void Invalidate();

	private:
		GraphicsContext* m_Context = nullptr;
		SwapChainInfo m_CreateInfo;

		VkExtent2D m_ImageExtent = {};
		uint32_t m_ImageCount = 0;
		uint32_t m_CurrentImage = 0;

		VkFormat m_ImageFormat;

		VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;

		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;
		VkImage m_DepthImage;
		VmaAllocation m_DepthAllocation;
		VkImageView m_DepthImageView;

		InvalidatedCallbacks m_OnInvalidatedCallbacks;
	};

}

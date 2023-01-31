#pragma once

#include <TestFramework/Core/Window.hpp>
#include <TestFramework/Renderer/GraphicsContext.hpp>
#include <TestFramework/Renderer/SwapChain.hpp>
#include <TestFramework/Renderer/CommandBuffer.hpp>

#include <memory>

namespace FissionTests {

	class ImGuiPlatformContext
	{
	public:
		virtual ~ImGuiPlatformContext() = default;

		void Init(const TestFramework::Unique<TestFramework::Window>& InWindow, TestFramework::GraphicsContext* InContext, TestFramework::SwapChain* InSwapChain);
		void BeginFrame(TestFramework::CommandBuffer* InCommandList);
		void EndFrame(TestFramework::CommandBuffer* InCommandList);
		void Shutdown();

		static std::unique_ptr<ImGuiPlatformContext> Create();

	private:
		virtual void InitPlatform(const TestFramework::Unique<TestFramework::Window>& InWindow, TestFramework::GraphicsContext* InContext, TestFramework::SwapChain* InSwapChain) = 0;
		virtual void BeginFramePlatform(TestFramework::CommandBuffer* InCommandList) = 0;
		virtual void EndFramePlatform(TestFramework::CommandBuffer* InCommandList) = 0;
		virtual void ShutdownPlatform() = 0;

		void InitStyle();
	};

}

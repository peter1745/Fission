#include "ImGuiPlatformContext.hpp"

#include <TestFramework/Renderer/RenderSettings.hpp>

#include "Platform/Vulkan/ImGuiPlatformContextVulkan.hpp"

#include <imgui.h>

namespace FissionTests {

	void ImGuiPlatformContext::Init(const TestFramework::Unique<TestFramework::Window>& InWindow, TestFramework::GraphicsContext* InContext, TestFramework::SwapChain* InSwapChain)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		auto& IO = ImGui::GetIO();
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		IO.FontDefault = IO.Fonts->AddFontFromFileTTF("Resources/Fonts/Roboto-Regular.ttf", 16.0f);

		IO.ConfigWindowsMoveFromTitleBarOnly = true;

		InitStyle();
		InitPlatform(InWindow, InContext, InSwapChain);
	}

	void ImGuiPlatformContext::BeginFrame(TestFramework::CommandBuffer* InCommandList)
	{
		BeginFramePlatform(InCommandList);
		ImGui::NewFrame();
	}

	void ImGuiPlatformContext::EndFrame(TestFramework::CommandBuffer* InCommandList)
	{
		EndFramePlatform(InCommandList);
	}

	void ImGuiPlatformContext::Shutdown()
	{
		ShutdownPlatform();
		ImGui::DestroyContext();
	}

	std::unique_ptr<ImGuiPlatformContext> ImGuiPlatformContext::Create()
	{
		switch (TestFramework::RenderSettings::Get().API)
		{
		case TestFramework::ERendererAPI::None: return nullptr;
		case TestFramework::ERendererAPI::Vulkan: return std::make_unique<ImGuiPlatformContextVulkan>();
		}

		return nullptr;
	}

	void ImGuiPlatformContext::InitStyle()
	{
		auto& UIStyle = ImGui::GetStyle();
		UIStyle.WindowPadding = { 2.0f, 2.0f };
		UIStyle.WindowBorderSize = 0.0f;
		UIStyle.WindowTitleAlign = { 0.5f, 0.5f };
		UIStyle.WindowRounding = 2.0f;
		UIStyle.DisplaySafeAreaPadding = { 0.0f, 6.0f };
		UIStyle.WindowMenuButtonPosition = ImGuiDir_None;

		UIStyle.Colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.0f);
		UIStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.09f, 0.09f, 0.09f, 1.0f);
		UIStyle.Colors[ImGuiCol_Header] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		UIStyle.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.18f, 0.18f, 0.19f, 1.0f);
		UIStyle.Colors[ImGuiCol_HeaderActive] = ImVec4(0.82f, 0.75f, 0.53f, 1.0f);

		UIStyle.Colors[ImGuiCol_Tab] = ImVec4(0.13f, 0.13f, 0.13f, 1.0f);
		UIStyle.Colors[ImGuiCol_TabActive] = ImVec4(0.2f, 0.2f, 0.19f, 1.0f);
		UIStyle.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.09f, 0.09f, 0.08f, 1.0f);
		UIStyle.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.13f, 1.0f);
		UIStyle.Colors[ImGuiCol_TabHovered] = ImVec4(0.22f, 0.37f, 1.0f, 0.58f);
		UIStyle.TabRounding = 2.0f;

		UIStyle.Colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.0f);
		UIStyle.PopupRounding = 2.0f;

		UIStyle.FramePadding = { 16.0f, 8.0f };
		UIStyle.FrameRounding = 2.0f;
		UIStyle.FrameBorderSize = 0.0f;

		UIStyle.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.0f);

		UIStyle.ItemInnerSpacing = { 6.0f, 4.0f };
		UIStyle.IndentSpacing = 16.0f;

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			UIStyle.WindowRounding = 0.0f;
			UIStyle.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
	}

}

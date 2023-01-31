#pragma once

#include <TestFramework/Renderer/RenderTexture.hpp>

#include <imgui.h>

namespace FissionTests {

	class ImGuiRenderTextureImage
	{
	public:
		virtual ~ImGuiRenderTextureImage() = default;

		virtual void DrawImage(const ImVec2& InSize) = 0;

		virtual void Resize(uint32_t InWidth, uint32_t InHeight) = 0;

	public:
		static TestFramework::Unique<ImGuiRenderTextureImage> Create(const std::vector<TestFramework::RenderTexture*>& InRenderTextures);
	};

}

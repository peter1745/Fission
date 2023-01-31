#include "ImGuiRenderTextureImage.hpp"

#include <TestFramework/Renderer/RenderSettings.hpp>

#include "Platform/Vulkan/ImGuiRenderTextureImageVulkan.hpp"

namespace FissionTests {

	TestFramework::Unique<ImGuiRenderTextureImage> ImGuiRenderTextureImage::Create(const std::vector<TestFramework::RenderTexture*>& InRenderTextures)
	{
		switch (TestFramework::RenderSettings::Get().API)
		{
		case TestFramework::ERendererAPI::Vulkan: return TestFramework::MakeUnique<ImGuiRenderTextureImageVulkan>(InRenderTextures);
		}

		return nullptr;
	}

}

#pragma once

#include "UI/ImGuiRenderTextureImage.hpp"

#include <vulkan/vulkan.h>

namespace FissionTests {

	class ImGuiRenderTextureImageVulkan : public ImGuiRenderTextureImage
	{
	public:
		ImGuiRenderTextureImageVulkan(const std::vector<TestFramework::RenderTexture*>& InRenderTextures);

		void DrawImage(const ImVec2& InSize) override;
		void Resize(uint32_t InWidth, uint32_t InHeight) override;

	private:
		std::vector<TestFramework::RenderTexture*> m_TargetTextures;
		std::vector<VkDescriptorSet> m_DescriptorSets;
		VkSampler m_Sampler = VK_NULL_HANDLE;
	};

}

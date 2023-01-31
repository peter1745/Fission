#include "Shader.hpp"
#include "GraphicsContext.hpp"

#include <ranges>

namespace TestFramework {

	Shader::Shader(Device* InDevice, const CompiledShaderData& InData)
	    : m_CompiledData(InData)
	{
		for (const auto& [ShaderStage, ByteCode] : InData.ModuleByteCodes)
		{
			VkShaderModuleCreateInfo moduleCreateInfo = {};
			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.codeSize = ByteCode.size() * sizeof(uint32_t);
			moduleCreateInfo.pCode = ByteCode.data();
			vkCreateShaderModule(InDevice->GetLogicalDevice(), &moduleCreateInfo, nullptr, &m_ShaderModules[ShaderStage]);
		}
	}

	void Shader::Bind()
	{
	}

	void Shader::Unbind()
	{
	}
}
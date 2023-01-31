#pragma once

#include "Common.hpp"
#include "ShaderCompiler.hpp"

namespace TestFramework {

	class Shader
	{
	public:
		explicit Shader(Device* InDevice, const CompiledShaderData& InData);

		void Bind();
		void Unbind();

		auto GetByteCode(EShaderStage InModuleType)
		{
			if (m_CompiledData.ModuleByteCodes.find(InModuleType) == m_CompiledData.ModuleByteCodes.end())
				return std::vector<uint32_t>();

			return m_CompiledData.ModuleByteCodes[InModuleType];
		}

		const auto& GetShaderData() const { return m_CompiledData; }

		const auto& GetShaderModules() const { return m_ShaderModules; }

	private:
		CompiledShaderData m_CompiledData;
		std::unordered_map<EShaderStage, VkShaderModule> m_ShaderModules;
	};

}

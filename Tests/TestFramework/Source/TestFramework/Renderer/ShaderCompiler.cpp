#include "ShaderCompiler.hpp"
#include "Shader.hpp"
#include "GraphicsContext.hpp"

#include "TestFramework/IO/FileIO.hpp"

#include <spirv_cross/spirv_glsl.hpp>

#include <ranges>
#include <iostream>

namespace TestFramework {

	static std::unordered_map<EShaderStage, const char*> s_ShaderNames = {
		{ EShaderStage::Vertex, "Vertex" },
		{ EShaderStage::Pixel, "Pixel" }
	};

	static std::unordered_map<EShaderStage, shaderc_shader_kind> s_ShaderTypeToShaderCShaderKind = {
		{ EShaderStage::Vertex, shaderc_vertex_shader },
		{ EShaderStage::Pixel, shaderc_fragment_shader }
	};

	static std::unordered_map<EShaderResourceType, const char*> s_ShaderResourceTypeNames = {
		{ EShaderResourceType::UniformBuffer, "UniformBuffer" },
		{ EShaderResourceType::CombinedImageSampler, "CombinedImageSampler" }
	};

	static std::unordered_map<spirv_cross::SPIRType::BaseType, std::string> s_SPIRTypeName = {
		{ spirv_cross::SPIRType::BaseType::Unknown, "Unknown" },
		{ spirv_cross::SPIRType::BaseType::Void, "Void" },
		{ spirv_cross::SPIRType::BaseType::Boolean, "Boolean" },
		{ spirv_cross::SPIRType::BaseType::SByte, "SByte" },
		{ spirv_cross::SPIRType::BaseType::UByte, "UByte" },
		{ spirv_cross::SPIRType::BaseType::Short, "Short" },
		{ spirv_cross::SPIRType::BaseType::UShort, "UShort" },
		{ spirv_cross::SPIRType::BaseType::Int, "Int" },
		{ spirv_cross::SPIRType::BaseType::UInt, "UInt" },
		{ spirv_cross::SPIRType::BaseType::Int64, "Int64" },
		{ spirv_cross::SPIRType::BaseType::UInt64, "UInt64" },
		{ spirv_cross::SPIRType::BaseType::AtomicCounter, "AtomicCounter" },
		{ spirv_cross::SPIRType::BaseType::Half, "Half" },
		{ spirv_cross::SPIRType::BaseType::Float, "Float" },
		{ spirv_cross::SPIRType::BaseType::Double, "Double" },
		{ spirv_cross::SPIRType::BaseType::Struct, "Struct" },
		{ spirv_cross::SPIRType::BaseType::Image, "Image" },
		{ spirv_cross::SPIRType::BaseType::SampledImage, "SampledImage" },
		{ spirv_cross::SPIRType::BaseType::Sampler, "Sampler" },
		{ spirv_cross::SPIRType::BaseType::AccelerationStructure, "AccelerationStructure" },
		{ spirv_cross::SPIRType::BaseType::RayQuery, "RayQuery" },
	};

	static EFormat GetResourceFormat(spirv_cross::SPIRType InType)
	{
		switch (InType.basetype)
		{
		case spirv_cross::SPIRType::Unknown: return EFormat::Unknown;
		case spirv_cross::SPIRType::Void: return EFormat::Unknown;
		case spirv_cross::SPIRType::UByte:
		case spirv_cross::SPIRType::UShort:
		case spirv_cross::SPIRType::UInt:
		case spirv_cross::SPIRType::Boolean:
		{
			switch (InType.vecsize)
			{
			case 1: return EFormat::R32UInt;
			case 2: return EFormat::RG32UInt;
			case 3: return EFormat::RGB32UInt;
			case 4: return EFormat::RGBA32UInt;
			}

			break;
		}
		case spirv_cross::SPIRType::Int:
		case spirv_cross::SPIRType::Short:
		case spirv_cross::SPIRType::SByte:
		{
			switch (InType.vecsize)
			{
			case 1: return EFormat::R32SInt;
			case 2: return EFormat::RG32SInt;
			case 3: return EFormat::RGB32SInt;
			case 4: return EFormat::RGBA32SInt;
			}

			break;
		}
		case spirv_cross::SPIRType::Int64:
		{
			switch (InType.vecsize)
			{
			case 1: return EFormat::RG32SInt;
			case 2: return EFormat::RGBA32SInt;
			}

			break;
		}
		case spirv_cross::SPIRType::UInt64:
		{
			switch (InType.vecsize)
			{
			case 1: return EFormat::RG32UInt;
			case 2: return EFormat::RGBA32UInt;
			}

			break;
		}
		case spirv_cross::SPIRType::Float:
		{
			switch (InType.vecsize)
			{
			case 1: return EFormat::R32Float;
			case 2: return EFormat::RG32Float;
			case 3: return EFormat::RGB32Float;
			case 4: return EFormat::RGBA32Float;
			}

			break;
		}
		case spirv_cross::SPIRType::Double:
		{
			switch (InType.vecsize)
			{
			case 1: return EFormat::R32Float;
			case 2: return EFormat::RG32Float;
			case 3: return EFormat::RGB32Float;
			case 4: return EFormat::RGBA32Float;
			}

			break;
		}
		case spirv_cross::SPIRType::Struct:
		{
			// TODO
			break;
		}
		}

		return EFormat::Unknown;
	}

	Shader* ShaderCompiler::CompileFromFile(const std::filesystem::path& InFilePath)
	{
		CompiledShaderData shaderData = {};

		TryCompileAndReflectModule(InFilePath, EShaderStage::Vertex, shaderData);
		TryCompileAndReflectModule(InFilePath, EShaderStage::Pixel, shaderData);

		return new Shader(GraphicsContext::Get()->GetDevice(), shaderData);
	}

	void ShaderCompiler::TryCompileAndReflectModule(const std::filesystem::path& InFilePath, EShaderStage InShaderType, CompiledShaderData& OutData)
	{
		shaderc::SpvCompilationResult result = TryCompileModule(InFilePath, InShaderType);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
			return;

		OutData.ModuleByteCodes[InShaderType] = std::vector<uint32_t>(result.begin(), result.end());

		ReflectShader(InShaderType, OutData);
	}

	shaderc::SpvCompilationResult ShaderCompiler::TryCompileModule(const std::filesystem::path& InFilePath, EShaderStage InShaderType)
	{
		std::string source = "";
		if (!FileIO::ReadFileText(InFilePath, source))
			return shaderc::SpvCompilationResult(nullptr);

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

#ifdef FUSION_DEBUG
		options.SetGenerateDebugInfo();
		options.SetOptimizationLevel(shaderc_optimization_level_zero);
#else
		options.SetOptimizationLevel(shaderc_optimization_level_performance);
#endif

		switch (InShaderType)
		{
		case EShaderStage::Vertex:
		{
			options.AddMacroDefinition("FUSION_COMPILE_VERTEX");
			break;
		}
		case EShaderStage::Pixel:
		{
			options.AddMacroDefinition("FUSION_COMPILE_PIXEL");
			break;
		}
		}

		options.SetWarningsAsErrors();

		auto shaderKind = s_ShaderTypeToShaderCShaderKind.at(InShaderType);
		std::string shaderName = InFilePath.filename().string();
		shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, shaderKind, shaderName.c_str(), options);

		if (module.GetCompilationStatus() != shaderc_compilation_status_success)
			std::cout << "Failed to compile shader. Error: " << module.GetErrorMessage() << "\n";

		return module;
	}

	void ShaderCompiler::ReflectShader(EShaderStage InStage, CompiledShaderData& InCompiledData)
	{
		spirv_cross::CompilerGLSL compiler(InCompiledData.ModuleByteCodes.at(InStage));

		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		for (const auto& inputResource : resources.stage_inputs)
		{
			const auto& type = compiler.get_type(inputResource.base_type_id);

			if (type.basetype == spirv_cross::SPIRType::Struct)
			{
				auto memberCount = uint32_t(type.member_types.size());

				for (uint32_t memberIdx = 0; memberIdx < memberCount; memberIdx++)
				{
					auto& inputParam = InCompiledData.InputParameters.emplace_back();

					const auto& memberType = compiler.get_type(type.member_types[memberIdx]);
					inputParam.Stage = InStage;
					inputParam.Name = compiler.get_member_name(type.self, memberIdx);
					inputParam.Location = memberIdx;
					inputParam.Format = GetResourceFormat(memberType);
				}
			}
			else
			{
				auto& inputParam = InCompiledData.InputParameters.emplace_back();
				inputParam.Stage = InStage;
				inputParam.Name = inputResource.name;
				inputParam.Location = compiler.get_decoration(inputResource.id, spv::DecorationLocation);
				inputParam.Format = GetResourceFormat(type);
			}
		}

		for (const auto& uniformResource : resources.uniform_buffers)
		{
			uint32_t descriptorSetIndex = compiler.get_decoration(uniformResource.id, spv::DecorationDescriptorSet);

			if (descriptorSetIndex >= InCompiledData.DescriptorSets.size())
				InCompiledData.DescriptorSets.resize(descriptorSetIndex + 1);

			auto& descriptorSet = InCompiledData.DescriptorSets[descriptorSetIndex];
			descriptorSet.Index = descriptorSetIndex;

			const auto& bufferType = compiler.get_type(uniformResource.base_type_id);

			uint32_t binding = compiler.get_decoration(uniformResource.id, spv::DecorationBinding);

			if (descriptorSet.Resources.find(binding) != descriptorSet.Resources.end())
			{
				// Binding already exists, just update the stage
				auto* resource = descriptorSet.Resources[binding];
				resource->Stages |= InStage;
			}
			else
			{
				auto* resource = new UniformBufferShaderResource();
				resource->Stages = InStage;
				resource->Type = EShaderResourceType::UniformBuffer;
				resource->Name = compiler.get_name(uniformResource.id);
				resource->Binding = binding;

				auto memberCount = uint32_t(bufferType.member_types.size());

				for (uint32_t memberIdx = 0; memberIdx < memberCount; memberIdx++)
				{
					auto& uniformData = resource->Uniforms.emplace_back();
					const auto& memberType = compiler.get_type(bufferType.member_types[memberIdx]);
					uniformData.Name = compiler.get_member_name(bufferType.self, memberIdx);
					uniformData.Format = GetResourceFormat(memberType);
					uniformData.Size = compiler.get_declared_struct_member_size(bufferType, memberIdx);
					uniformData.Offset = compiler.type_struct_member_offset(bufferType, memberIdx);

					resource->Size += uniformData.Size;
				}

				descriptorSet.Resources[binding] = resource;
			}
		}

		for (const auto& samplerResource : resources.sampled_images)
		{
			uint32_t descriptorSetIndex = compiler.get_decoration(samplerResource.id, spv::DecorationDescriptorSet);

			if (descriptorSetIndex >= InCompiledData.DescriptorSets.size())
				InCompiledData.DescriptorSets.resize(descriptorSetIndex + 1);

			auto& descriptorSet = InCompiledData.DescriptorSets[descriptorSetIndex];
			descriptorSet.Index = descriptorSetIndex;
			
			const auto& baseType = compiler.get_type(samplerResource.base_type_id);

			uint32_t binding = compiler.get_decoration(samplerResource.id, spv::DecorationBinding);

			if (descriptorSet.Resources.find(binding) != descriptorSet.Resources.end())
			{
				// Binding already exists, just update the stage
				auto& resource = descriptorSet.Resources[binding];
				resource->Stages |= InStage;
			}
			else
			{
				auto* resource = new SamplerShaderResource();
				resource->Stages = InStage;
				resource->Type = EShaderResourceType::CombinedImageSampler;
				resource->Name = samplerResource.name;
				resource->Binding = binding;
				resource->Dimensions = baseType.image.dim;
				descriptorSet.Resources[binding] = resource;
			}
		}

		for (const auto& pushConstantResource : resources.push_constant_buffers)
		{
			auto& pushConstant = InCompiledData.PushConstants.emplace_back();
			const auto& bufferType = compiler.get_type(pushConstantResource.base_type_id);

			pushConstant.Stages = InStage;
			pushConstant.Size = compiler.get_declared_struct_size(bufferType);

			auto memberCount = uint32_t(bufferType.member_types.size());
			pushConstant.Uniforms.resize(memberCount);

			for (uint32_t memberIdx = 0; memberIdx < memberCount; memberIdx++)
			{
				auto& memberInfo = pushConstant.Uniforms[memberIdx];

				const auto& memberType = compiler.get_type(bufferType.member_types[memberIdx]);
				memberInfo.Name = compiler.get_member_name(bufferType.self, memberIdx);
				memberInfo.Size = compiler.get_declared_struct_member_size(bufferType, memberIdx);
				memberInfo.Offset = compiler.type_struct_member_offset(bufferType, memberIdx);
			}
		}

		for (const auto& outputResource : resources.stage_outputs)
		{
			const auto& type = compiler.get_type(outputResource.base_type_id);

			if (type.basetype == spirv_cross::SPIRType::Struct)
			{
				auto memberCount = uint32_t(type.member_types.size());

				for (uint32_t memberIdx = 0; memberIdx < memberCount; memberIdx++)
				{
					auto& outputParam = InCompiledData.OutputParameters.emplace_back();

					const auto& memberType = compiler.get_type(type.member_types[memberIdx]);
					outputParam.Stage = InStage;
					outputParam.Name = compiler.get_member_name(type.self, memberIdx);
					outputParam.Location = memberIdx;
					outputParam.Format = GetResourceFormat(memberType);
				}
			}
			else
			{
				auto& outputParam = InCompiledData.OutputParameters.emplace_back();
				outputParam.Stage = InStage;
				outputParam.Name = outputResource.name;
				outputParam.Location = compiler.get_decoration(outputResource.id, spv::DecorationLocation);
				outputParam.Format = GetResourceFormat(type);
			}
		}
	}

}
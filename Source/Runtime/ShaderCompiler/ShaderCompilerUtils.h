#pragma once
#include <Runtime/Graphics/Shader/ShaderLanguage.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Shaderc/Include/shaderc.h>
#include <spirv_reflect.h>

namespace Dream
{
	/// <summary>
	/// Utilities for ShaderCompiler
	/// </summary>
	class RUNTIME_API ShaderCompilerUtils
	{
	public:
		FORCEINLINE static shaderc_shader_kind GetShaderKind(const ShaderStage stage)
		{
			switch (stage)
			{
			case ShaderStage::Vertex:
				return shaderc_vertex_shader;
			case ShaderStage::Fragment:
				return shaderc_fragment_shader;
			case ShaderStage::Compute:
				return shaderc_compute_shader;
			default:
				return shaderc_glsl_infer_from_source;
			}
		}

		FORCEINLINE static shaderc_source_language GetLanguage(const ShaderLanguage langauge)
		{
			switch (langauge)
			{
			case ShaderLanguage::GLSL:
				return shaderc_source_language_glsl;
			case ShaderLanguage::HLSL:
				return shaderc_source_language_hlsl;
			case ShaderLanguage::MSL:
				return shaderc_source_language_hlsl;
			default:
				return shaderc_source_language_glsl;
			}
		}
	public:
		ShaderCompilerUtils() = delete;
		~ShaderCompilerUtils() = delete;
	};
}

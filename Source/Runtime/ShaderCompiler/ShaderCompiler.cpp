#include "ShaderCompiler.h"
#include <Shaderc/Include/shaderc.hpp>
#include <Spirv-Cross/Include/spirv_hlsl.hpp>
#include <Spirv-Cross/Include/spirv_glsl.hpp>
#include <Runtime/ShaderCompiler/ShaderCompilerUtils.h>

namespace Dream
{
	bool ShaderCompiler::compile_to_spirv(const std::string& source, const std::string& entryMethod, const ShaderStage stage, const ShaderLanguage language, unsigned char** ppBytesOut, unsigned int& bytesSizeOut, std::string& errorMessageOut)
	{
		shaderc::Compiler vkCompiler;
		shaderc::CompileOptions compileOptions;

		//Set options
		compileOptions.SetSourceLanguage(ShaderCompilerUtils::GetLanguage(language));
		compileOptions.SetSuppressWarnings();


		//Pre process
		const shaderc::PreprocessedSourceCompilationResult preprocessResult = vkCompiler.PreprocessGlsl(source, ShaderCompilerUtils::GetShaderKind(stage),entryMethod.c_str(), compileOptions);
		const std::string rm = preprocessResult.GetErrorMessage();
		if (preprocessResult.GetCompilationStatus() != shaderc_compilation_status_success || preprocessResult.GetNumErrors() > 0)
		{
			errorMessageOut = preprocessResult.GetErrorMessage().c_str();
			*ppBytesOut = nullptr;
			bytesSizeOut = 0;
			return false;
		}

		const shaderc::SpvCompilationResult result = vkCompiler.CompileGlslToSpv(source, ShaderCompilerUtils::GetShaderKind(stage), "", compileOptions);
		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			errorMessageOut = result.GetErrorMessage().c_str();
			*ppBytesOut = nullptr;
			bytesSizeOut = 0;
			return false;
		}

		const unsigned long long bufferSize = (unsigned char*)result.cend() - (unsigned char*)result.cbegin();
		unsigned char* pBuffer = new unsigned char[bufferSize];
		memcpy(pBuffer, result.cbegin(), bufferSize);
		*ppBytesOut = pBuffer;
		bytesSizeOut = bufferSize;

		return true;
	}
	bool ShaderCompiler::compile_spirv_to_backend(const unsigned char* pSource, const unsigned int sourceLength, const GraphicsBackend backend, unsigned char** ppBytesOut, unsigned int& bytesSizeOut, std::string& errorMessageOut)
	{
		*ppBytesOut = nullptr;

		spirv_cross::Compiler* pCompiler = nullptr;
		switch (backend)
		{
			case GraphicsBackend::Vulkan:
			{
				unsigned char* pBuffer = new unsigned char[sourceLength];
				memcpy(pBuffer, pSource, sourceLength);
				*ppBytesOut = pBuffer;
				bytesSizeOut = sourceLength;
				return true;
			}
			case GraphicsBackend::Directx12:
			{
				
				break;
			}
		}

		if (*ppBytesOut == nullptr)
		{
			DEV_LOG("ShaderCompiler", "Failed to allocate memory for SPIRV.");
			return false;
		}

		return true;
	}
}

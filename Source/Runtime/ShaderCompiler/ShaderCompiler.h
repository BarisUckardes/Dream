#pragma once
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <Runtime/Graphics/Shader/ShaderLanguage.h>
#include <Runtime/Graphics/GraphicsBackend.h>
#include <string>

namespace Dream
{
	class RUNTIME_API ShaderCompiler final
	{
	public:
		static bool CompileToSPIRV(const std::string& source, const std::string& entryMethod, const ShaderStage stage, const ShaderLanguage language, unsigned char** ppBytesOut, unsigned int& bytesSizeOut, std::string& errorMessageOut);
		static bool CompileFromSPIRV(const unsigned char* pSource, const unsigned int sourceLength, const GraphicsBackend backend, unsigned char** ppBytesOut, unsigned int& bytesSizeOut, std::string& errorMessageOut);
	public:
		ShaderCompiler() = delete;
		~ShaderCompiler() = delete;
	};
}

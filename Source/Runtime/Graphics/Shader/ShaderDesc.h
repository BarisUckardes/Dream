#pragma once
#include <Runtime/Graphics/Shader/ShaderLanguage.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <string>

namespace Dream
{
	struct RUNTIME_API ShaderDesc
	{
		ShaderStage Stage;
		std::string EntryMethod;
		unsigned char* pByteCode;
		unsigned long long ByteCodeLength;
	};
}

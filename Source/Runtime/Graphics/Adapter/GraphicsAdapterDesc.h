#pragma once
#include <Runtime/Core/Core.h>
#include <string>

namespace Dream
{
	class GraphicsInstance;
	struct RUNTIME_API GraphicsAdapterDesc
	{
		std::string ProductName;
		std::string VendorName;
		bool bSupportGeometryShader;
		bool bSupportComputeShader;
		unsigned char ColorAttachmentCount;
		unsigned int ShaderTextureCount;
		unsigned int ShaderSamplerCount;
		unsigned int ShaderBufferCount;
		unsigned int ImageWidth;
		unsigned int ImageHeight;
		unsigned int ImageDepth;
		unsigned long long Memory;
		GraphicsInstance* pInstance;
	};
}

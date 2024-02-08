#pragma once
#include <Runtime/Core/Core.h>
#include <vulkan.h>
#include <string>

namespace Dream
{
	class GraphicsInstance;
	struct RUNTIME_API GraphicsAdapterDesc
	{
		VkPhysicalDevice PhysicalDevice;
		std::string ProductName;
		std::string VendorName;
		bool bSupportGeometryShader;
		bool bSupportComputeShader;
		unsigned char ColorAttachmentCount;
		unsigned int ShaderTextureCount;
		unsigned int ShaderSamplerCount;
		unsigned int ShaderBufferCount;
		unsigned int ImageDimensions[3];
		unsigned long long DeviceMemory;
		GraphicsInstance* pInstance;
	};
}


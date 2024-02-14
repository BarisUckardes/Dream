#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>

namespace Dream
{
	struct RUNTIME_API VulkanDeviceDesc : public GraphicsDeviceDesc
	{
		std::vector<std::string> Extensions;
		std::vector<std::string> Layers;
	};
}

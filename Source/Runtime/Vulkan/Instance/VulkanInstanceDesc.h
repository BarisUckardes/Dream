#pragma once
#include <Runtime/Graphics/Instance/GraphicsInstanceDesc.h>
#include <string>
#include <vector>

namespace Dream
{
	struct RUNTIME_API VulkanInstanceDesc : public GraphicsInstanceDesc
	{
		std::vector<std::string> Extensions;
		std::vector<std::string> Layers;
		std::string ApplicationName;
		unsigned int ApplicationVersion;
		std::string EngineName;
		unsigned int APIVersion;
	};
}

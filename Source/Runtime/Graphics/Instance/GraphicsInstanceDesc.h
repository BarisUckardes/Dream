#pragma once
#include <vector>
#include <string>

namespace Dream
{
	struct RUNTIME_API GraphicsInstanceDesc
	{
		std::vector<std::string> Extensions;
		std::vector<std::string> Layers;
	};
}

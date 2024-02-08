#pragma once
#include <vector>
#include <string>

namespace Dream
{
	class GraphicsAdapter;
	struct RUNTIME_API GraphicsDeviceDesc
	{
		std::vector<std::string> Extensions;
	};
}

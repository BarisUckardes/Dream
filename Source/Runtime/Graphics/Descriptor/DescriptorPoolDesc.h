#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolSize.h>
#include <vector>
namespace Dream
{
	struct RUNTIME_API DescriptorPoolDesc
	{
		std::vector<DescriptorPoolSize> Sizes;
		unsigned long long SetCount;
	};
}

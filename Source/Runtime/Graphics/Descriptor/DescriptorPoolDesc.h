#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolSize.h>
#include <Runtime/Graphics/Descriptor/DescriptorMemoryType.h>
#include <vector>
namespace Dream
{
	struct RUNTIME_API DescriptorPoolDesc
	{
		DescriptorMemoryType Type;
		std::vector<DescriptorPoolSize> Sizes;
		unsigned long long SetCount;
	};
}

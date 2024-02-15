#pragma once
#include <Runtime/Graphics/Descriptor/DescriptorResourceType.h>

namespace Dream
{
	struct RUNTIME_API DescriptorPoolSize
	{
		DescriptorResourceType Type;
		unsigned long long Count;
	};
}

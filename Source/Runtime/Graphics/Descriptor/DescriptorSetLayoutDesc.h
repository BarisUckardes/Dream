#pragma once
#include <Runtime/Graphics/Descriptor/DescriptorSetLayoutEntry.h>
#include <vector>

namespace Dream
{
	struct RUNTIME_API DescriptorSetLayoutDesc
	{
		std::vector<DescriptorSetLayoutEntry> Entries;
	};
}

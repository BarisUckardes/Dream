#pragma once
#include <Runtime/Graphics/Common/DescriptorSetUpdateEntry.h>
#include <vector>

namespace Dream
{
	struct RUNTIME_API DescriptorSetUpdateDesc
	{
		std::vector<DescriptorSetUpdateEntry> Entries;
	};
}

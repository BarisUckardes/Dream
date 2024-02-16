#pragma once
#include <Runtime/Graphics/Common/DescriptorSetCopyEntry.h>
#include <vector>

namespace Dream
{
	struct RUNTIME_API DescriptorSetCopyDesc
	{
		std::vector<DescriptorSetCopyEntry> Entries;
	};
}

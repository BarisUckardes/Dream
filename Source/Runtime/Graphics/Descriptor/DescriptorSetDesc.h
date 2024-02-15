#pragma once
#include <Runtime/Graphics/Descriptor/DescriptorSetLayout.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>

namespace Dream
{
	struct RUNTIME_API DescriptorSetDesc
	{
		DescriptorSetLayout* pLayout;
		DescriptorPool* pPool;
	};
}

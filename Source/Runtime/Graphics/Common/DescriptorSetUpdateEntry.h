#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorResourceType.h>

namespace Dream
{
	struct RUNTIME_API DescriptorSetUpdateEntry
	{
		GraphicsDeviceObject* pResource;
		DescriptorResourceType Type;
		unsigned int Count;
		unsigned ArrayElement;
		unsigned long long BufferOffsetInBytes;
		unsigned int Binding;
	};
}

#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	enum class RUNTIME_API DescriptorResourceType : unsigned char
	{
		Sampler,
		SampledTexture,
		StorageTexture,
		ConstantBuffer,
		StorageBuffer
	};
}

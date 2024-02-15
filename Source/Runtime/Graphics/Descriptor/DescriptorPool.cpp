#include "DescriptorPool.h"

namespace Dream
{
	DescriptorPool::~DescriptorPool()
	{

	}
	DescriptorPool::DescriptorPool(const DescriptorPoolDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),mSetCount(desc.SetCount),mSizes(desc.Sizes)
	{

	}
}

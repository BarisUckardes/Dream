#include "DescriptorPool.h"

namespace Dream
{
	DescriptorPool::~DescriptorPool()
	{

	}
	DescriptorPool::DescriptorPool(const DescriptorPoolDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),mSizes(desc.Sizes),mType(desc.Type)
	{

	}
}

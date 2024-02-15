#include "DescriptorSet.h"

namespace Dream
{
	DescriptorSet::~DescriptorSet()
	{
	}
	DescriptorSet::DescriptorSet(const DescriptorSetDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),mPool(desc.pPool),mLayout(desc.pLayout)
	{

	}
}

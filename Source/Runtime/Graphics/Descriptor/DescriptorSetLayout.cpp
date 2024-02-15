#include "DescriptorSetLayout.h"

namespace Dream
{
	DescriptorSetLayout::~DescriptorSetLayout()
	{

	}
	DescriptorSetLayout::DescriptorSetLayout(const DescriptorSetLayoutDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),mEntries(desc.Entries)
	{

	}
}

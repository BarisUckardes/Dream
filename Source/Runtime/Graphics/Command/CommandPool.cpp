#include "CommandPool.h"

namespace Dream
{
	CommandPool::~CommandPool()
	{
	}
	CommandPool::CommandPool(const CommandPoolDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),mType(desc.Type)
	{

	}
}

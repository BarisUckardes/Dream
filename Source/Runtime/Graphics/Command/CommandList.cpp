#include "CommandList.h"

namespace Dream
{
	CommandList::~CommandList()
	{
	}
	CommandList::CommandList(const CommandListDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),mCmdPool(desc.pCmdPool)
	{
	}
}

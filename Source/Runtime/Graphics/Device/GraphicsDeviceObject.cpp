#include "GraphicsDeviceObject.h"
#include <Runtime/Graphics/Device/GraphicsDevice.h>

namespace Dream
{
	GraphicsDeviceObject::GraphicsDeviceObject(GraphicsDevice* pDevice) : mOwnerDevice(pDevice)
	{

	}
	GraphicsDeviceObject::~GraphicsDeviceObject()
	{
		mOwnerDevice->_OnChildObjectDeleted(this);
	}
}

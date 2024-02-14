#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceFeatures.h>
#include <Runtime/Core/Core.h>
#include <vector>
#include <string>

namespace Dream
{
	class GraphicsAdapter;
	struct RUNTIME_API GraphicsDeviceDesc
	{
		GraphicsDeviceDesc() : GraphicsQueueCount(0),ComputeQueueCount(0),TransferQueueCount(0)
		{

		}
		GraphicsAdapter* pOwnerAdapter;
		GraphicsDeviceFeatures* pRequestedFeatures;
		unsigned char GraphicsQueueCount;
		unsigned char ComputeQueueCount;
		unsigned char TransferQueueCount;
	};
}

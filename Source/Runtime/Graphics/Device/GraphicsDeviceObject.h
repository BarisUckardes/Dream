#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObjectType.h>

namespace Dream
{
	class GraphicsDevice;
	class RUNTIME_API GraphicsDeviceObject
	{
	public:
		GraphicsDeviceObject(GraphicsDevice* pDevice);
		~GraphicsDeviceObject();

		FORCEINLINE GraphicsDevice* device() const noexcept
		{
			return mDevice;
		}
		virtual GraphicsDeviceObjectType object_type() const noexcept = 0;
	private:
		GraphicsDevice* mDevice;
	};
}

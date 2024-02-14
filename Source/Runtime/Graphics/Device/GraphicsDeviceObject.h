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

		FORCEINLINE GraphicsDevice* GetDevice() const noexcept
		{
			return mDevice;
		}
		virtual GraphicsDeviceObjectType GetObjectType() const noexcept = 0;
	private:
		GraphicsDevice* mDevice;
	};
}

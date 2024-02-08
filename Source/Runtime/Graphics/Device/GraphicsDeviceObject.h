#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObjectType.h>

namespace Dream
{
	class GraphicsDevice;
	class RUNTIME_API GraphicsDeviceObject
	{
		friend class GraphicsDevice;
	public:
		GraphicsDeviceObject(GraphicsDevice* pDevice);
		~GraphicsDeviceObject();

		FORCEINLINE GraphicsDevice* GetOwnerDevice() const noexcept
		{
			return mOwnerDevice;
		}
		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept = 0;
	private:
		GraphicsDevice* mOwnerDevice;
	};
}

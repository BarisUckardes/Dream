#pragma once
#include <Runtime/Graphics/Semaphore/SemaphoreDesc.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>

namespace Dream
{
	class RUNTIME_API Semaphore : public GraphicsDeviceObject
	{
	public:
		~Semaphore();

		FORCEINLINE virtual GraphicsDeviceObjectType object_type() const noexcept
		{
			return GraphicsDeviceObjectType::Semaphore;
		}
	protected:
		Semaphore(const SemaphoreDesc& desc,GraphicsDevice* pDevice);

	};
}

#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/GraphicsBackend.h>
#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <vector>

namespace Dream
{
	class RUNTIME_API GraphicsDevice
	{
	public:
		~GraphicsDevice();

		FORCEINLINE GraphicsAdapter* GetAdapter() const noexcept
		{
			return mOwnerAdapter;
		}

		virtual GraphicsBackend GetBackend() const noexcept = 0;
		virtual bool HasQueue(const GraphicsQueueType type) const noexcept = 0;

		GraphicsQueue* CreateQueue(const GraphicsQueueDesc& desc);
	protected:
		GraphicsDevice(const GraphicsDeviceDesc* pDesc);

		void RegisterObject(GraphicsDeviceObject* pObject);
		void RemoveObject(GraphicsDeviceObject* pObject);
		virtual GraphicsQueue* CreateQueueCore(const GraphicsQueueDesc& desc) = 0;
	private:
		std::vector<GraphicsDeviceObject*> mObjects;
		GraphicsAdapter* mOwnerAdapter;
	};
}

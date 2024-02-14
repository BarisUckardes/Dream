#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Queue/GraphicsQueueDesc.h>

namespace Dream
{
	class RUNTIME_API GraphicsQueue : public GraphicsDeviceObject
	{
	public:
		~GraphicsQueue();

		FORCEINLINE GraphicsQueueType GetType() const noexcept
		{
			return mType;
		}
		virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
		{
			return GraphicsDeviceObjectType::Queue;
		}
	protected:
		GraphicsQueue(const GraphicsQueueDesc& desc,GraphicsDevice* pDevice);
	private:
		const GraphicsQueueType mType;
	};
}

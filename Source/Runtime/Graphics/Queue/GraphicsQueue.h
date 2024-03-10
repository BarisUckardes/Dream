#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Queue/GraphicsQueueDesc.h>

namespace Dream
{
	class RUNTIME_API GraphicsQueue : public GraphicsDeviceObject
	{
	public:
		~GraphicsQueue();

		FORCEINLINE GraphicsQueueFamilyType queue_type() const noexcept
		{
			return mType;
		}
		virtual GraphicsDeviceObjectType object_type() const noexcept override final
		{
			return GraphicsDeviceObjectType::Queue;
		}
	protected:
		GraphicsQueue(const GraphicsQueueDesc& desc,GraphicsDevice* pDevice);
	private:
		const GraphicsQueueFamilyType mType;
	};
}

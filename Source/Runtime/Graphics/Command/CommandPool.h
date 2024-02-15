#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Command/CommandPoolDesc.h>

namespace Dream
{
	class RUNTIME_API CommandPool : public GraphicsDeviceObject
	{
	public:
		~CommandPool();

		FORCEINLINE CommandPoolType GetType() const noexcept
		{
			return mType;
		}
		virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
		{
			return GraphicsDeviceObjectType::CommandPool;
		}
	protected:
		CommandPool(const CommandPoolDesc& desc, GraphicsDevice* pDevice);
	private:
		const CommandPoolType mType;
	};
}

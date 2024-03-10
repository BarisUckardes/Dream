#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Fence/FenceDesc.h>

namespace Dream
{
	class RUNTIME_API Fence : public GraphicsDeviceObject
	{
	public:
		~Fence();

		virtual GraphicsDeviceObjectType object_type() const noexcept override final
		{
			return GraphicsDeviceObjectType::Fence;
		}
	protected:
		Fence(const FenceDesc& desc,GraphicsDevice* pDevice);
	};
}

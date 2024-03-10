#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolDesc.h>

namespace Dream
{
	class RUNTIME_API DescriptorPool : public GraphicsDeviceObject
	{
	public:
		~DescriptorPool();

		FORCEINLINE DescriptorMemoryType memory_type() const noexcept
		{
			return mType;
		}
		
		FORCEINLINE const std::vector<DescriptorPoolSize>& sizes() const noexcept
		{
			return mSizes;
		}
		virtual GraphicsDeviceObjectType object_type() const noexcept override final
		{
			return GraphicsDeviceObjectType::DescriptorPool;
		}
	protected:
		DescriptorPool(const DescriptorPoolDesc& desc,GraphicsDevice* pDevice);
	private:
		const DescriptorMemoryType mType;
		const std::vector<DescriptorPoolSize> mSizes;
	};
}

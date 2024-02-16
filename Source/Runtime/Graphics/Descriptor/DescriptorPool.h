#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolDesc.h>

namespace Dream
{
	class RUNTIME_API DescriptorPool : public GraphicsDeviceObject
	{
	public:
		~DescriptorPool();

		FORCEINLINE DescriptorMemoryType GetMemoryType() const noexcept
		{
			return mType;
		}
		FORCEINLINE unsigned long long GetSetCount() const noexcept
		{
			return mSetCount;
		}
		FORCEINLINE const std::vector<DescriptorPoolSize>& GetSizes() const noexcept
		{
			return mSizes;
		}
		virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
		{
			return GraphicsDeviceObjectType::DescriptorPool;
		}
	protected:
		DescriptorPool(const DescriptorPoolDesc& desc,GraphicsDevice* pDevice);
	private:
		const unsigned long long mSetCount;
		const DescriptorMemoryType mType;
		const std::vector<DescriptorPoolSize> mSizes;
	};
}

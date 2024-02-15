#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetDesc.h>

namespace Dream
{
	class RUNTIME_API DescriptorSet : public GraphicsDeviceObject
	{
	public:
		~DescriptorSet();

		FORCEINLINE const DescriptorSetLayout* GetLayout() const noexcept
		{
			return mLayout;
		}
		FORCEINLINE const DescriptorPool* GetPool() const noexcept
		{
			return mPool;
		}

		virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
		{
			return GraphicsDeviceObjectType::DescriptorSet;
		}
	protected:
		DescriptorSet(const DescriptorSetDesc& desc, GraphicsDevice* pDevice);

	private:
		const DescriptorSetLayout* mLayout;
		const DescriptorPool* mPool;
	};
}

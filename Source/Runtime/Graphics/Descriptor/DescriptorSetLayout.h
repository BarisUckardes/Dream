#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetLayoutDesc.h>

namespace Dream
{
	class RUNTIME_API DescriptorSetLayout : public GraphicsDeviceObject
	{
	public:
		~DescriptorSetLayout();

		FORCEINLINE const std::vector<DescriptorSetLayoutEntry>& entries() const noexcept
		{
			return mEntries;
		}

		virtual GraphicsDeviceObjectType object_type() const noexcept override final
		{
			return GraphicsDeviceObjectType::DescriptorLayout;
		}
	protected:
		DescriptorSetLayout(const DescriptorSetLayoutDesc& desc, GraphicsDevice* pDevice);
	private:
		const std::vector<DescriptorSetLayoutEntry> mEntries;
	};
}

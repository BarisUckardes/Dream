#pragma once
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>
#include <vulkan.h>

namespace Dream
{
	class RUNTIME_API VulkanAdapter : public GraphicsAdapter
	{
	public:
		VulkanAdapter(const VkPhysicalDevice physicalDevice,const GraphicsAdapterDesc& desc);
		~VulkanAdapter();

		FORCEINLINE VkPhysicalDevice GetVkPhysicalDevice() const noexcept
		{
			return mDevice;
		}
	private:
		virtual GraphicsDevice* create_device_impl(const GraphicsDeviceDesc* pDesc) override;
	private:
		const VkPhysicalDevice mDevice;
	};
}

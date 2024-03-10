#pragma once
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanDescriptorPool : public DescriptorPool
	{
	public:
		VulkanDescriptorPool(const DescriptorPoolDesc& desc, VulkanDevice* pDevice);
		~VulkanDescriptorPool();

		FORCEINLINE VkDescriptorPool vk_pool() const noexcept
		{
			return mPool;
		}
	private:
		VkDescriptorPool mPool;
		VkDevice mLogicalDevice;
	};
}

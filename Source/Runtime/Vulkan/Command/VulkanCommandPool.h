#pragma once
#include <Runtime/Graphics/Command/CommandPool.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanCommandPool : public CommandPool
	{
	public:
		VulkanCommandPool(const CommandPoolDesc& desc, VulkanDevice* pDevice);
		~VulkanCommandPool();

		FORCEINLINE VkCommandPool GetVkPool() const noexcept
		{
			return mPool;
		}
	private:
		VkCommandPool mPool;
		VkDevice mLogicalDevice;
	};
}

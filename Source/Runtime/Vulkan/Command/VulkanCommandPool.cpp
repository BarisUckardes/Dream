#include "VulkanCommandPool.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>

namespace Dream
{
	VulkanCommandPool::VulkanCommandPool(const CommandPoolDesc& desc, VulkanDevice* pDevice) : CommandPool(desc,pDevice),mPool(VK_NULL_HANDLE),mLogicalDevice(pDevice->GetVkLogicalDevice())
	{
		//Get family index
		const unsigned char queueFamilyIndex = pDevice->vkGetQueueFamilyIndex((GraphicsQueueType)desc.Type);

		//Create command pool
		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		DEV_ASSERT(vkCreateCommandPool(mLogicalDevice, &cmdPoolInfo, nullptr, &mPool) == VK_SUCCESS, "VulkanCommandPool", "Failed to create command pool");
	}
	VulkanCommandPool::~VulkanCommandPool()
	{
		vkDestroyCommandPool(mLogicalDevice, mPool, nullptr);
	}
}

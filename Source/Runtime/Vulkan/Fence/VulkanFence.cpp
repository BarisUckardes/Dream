#include "VulkanFence.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>

namespace Dream
{
	VulkanFence::VulkanFence(const FenceDesc& desc, VulkanDevice* pDevice) : Fence(desc,pDevice),mLogicalDevice(pDevice->GetVkLogicalDevice())
	{
		VkFenceCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		info.flags = desc.bSignalled ? VK_FENCE_CREATE_SIGNALED_BIT : VkFenceCreateFlags();
		info.pNext = nullptr;

		DEV_ASSERT(vkCreateFence(mLogicalDevice, &info, nullptr, &mFence) == VK_SUCCESS, "VulkanFence", "Failed to create fence");
	}
	VulkanFence::~VulkanFence()
	{
		vkDestroyFence(mLogicalDevice, mFence, nullptr);
	}
}

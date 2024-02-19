#include "VulkanSemaphore.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>

namespace Dream
{
	VulkanSemaphore::VulkanSemaphore(const SemaphoreDesc& desc, VulkanDevice* pDevice) : Semaphore(desc,pDevice),mSemaphore(VK_NULL_HANDLE),mLogicalDevice(pDevice->GetVkLogicalDevice())
	{
		VkSemaphoreCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		info.flags = desc.bSignalled ? VK_FENCE_CREATE_SIGNALED_BIT : VkSemaphoreCreateFlags();
		info.pNext = nullptr;

		DEV_ASSERT(vkCreateSemaphore(mLogicalDevice, &info, nullptr, &mSemaphore) == VK_SUCCESS, "VulkanSemaphore", "Failed to create semaphore");
	}
	VulkanSemaphore::~VulkanSemaphore()
	{
		vkDestroySemaphore(mLogicalDevice, mSemaphore, nullptr);
	}
}

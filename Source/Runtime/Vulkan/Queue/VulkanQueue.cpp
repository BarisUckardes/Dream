#include "VulkanQueue.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>

namespace Dream
{
	VulkanQueue::VulkanQueue(const GraphicsQueueDesc& desc, VulkanDevice* pDevice) : GraphicsQueue(desc,pDevice),mQueue(VK_NULL_HANDLE)
	{
		mQueue = pDevice->vkOwnQueue(desc.Type);
	}
	VulkanQueue::~VulkanQueue()
	{
		VulkanDevice* pDevice = (VulkanDevice*)GetDevice();
		pDevice->vkReturnQueue(GetType(), mQueue);
	}
}

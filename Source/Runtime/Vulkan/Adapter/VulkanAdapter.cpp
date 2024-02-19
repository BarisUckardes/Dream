#include "VulkanAdapter.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>

namespace Dream
{
	VulkanAdapter::VulkanAdapter(const VkPhysicalDevice physicalDevice,const GraphicsAdapterDesc& desc) : GraphicsAdapter(desc),mDevice(physicalDevice)
	{

	}
	VulkanAdapter::~VulkanAdapter()
	{
		VkSemaphoreCreateInfo info = {};
		info.flags = VkSemaphoreCreateFlags();

	}
	GraphicsDevice* VulkanAdapter::CreateDeviceCore(const GraphicsDeviceDesc* pDesc)
	{
		return new VulkanDevice((VulkanDeviceDesc*)pDesc);
	}
}

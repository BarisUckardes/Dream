#include "VulkanMemory.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/Memory/VulkanMemoryUtils.h>

namespace Dream
{
	VulkanMemory::VulkanMemory(const GraphicsMemoryDesc& desc, VulkanDevice* pDevice) : GraphicsMemory(desc,pDevice),mLogicalDevice(pDevice->GetVkLogicalDevice()),mMemory(VK_NULL_HANDLE)
	{
		VkMemoryAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.allocationSize = desc.SizeInBytes;
		info.memoryTypeIndex = VulkanMemoryUtils::GetMemoryFlags(desc.Type);
		info.pNext = nullptr;

		DEV_ASSERT(vkAllocateMemory(mLogicalDevice, &info, nullptr, &mMemory) == VK_SUCCESS, "VulkanMemory", "Failed to allocate memory");
	}
	VulkanMemory::~VulkanMemory()
	{
		vkFreeMemory(mLogicalDevice, mMemory, nullptr);
	}
}

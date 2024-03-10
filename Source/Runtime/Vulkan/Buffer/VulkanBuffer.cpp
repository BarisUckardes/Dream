#include "VulkanBuffer.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/Memory/VulkanMemory.h>
#include <Runtime/Vulkan/Buffer/VulkanBufferUtils.h>

namespace Dream
{
	VulkanBuffer::VulkanBuffer(const GraphicsBufferDesc& desc, VulkanDevice* pDevice) : GraphicsBuffer(desc,pDevice),mLogicalDevice(pDevice->GetVkLogicalDevice()),mBuffer(VK_NULL_HANDLE),mMemoryOffset(0),mMemoryAlignedOffset(0)
	{
        //Get vk heap
        const VulkanMemory* pHeap = (const VulkanMemory*)desc.pMemory;

        //Create buffer
        VkBufferCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        info.flags = VkBufferCreateFlags();
        info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        info.usage = VulkanBufferUtils::GetUsages(desc.Usage);
        info.size = total_size();
        info.pNext = nullptr;

        DEV_ASSERT(vkCreateBuffer(mLogicalDevice, &info, nullptr, &mBuffer) == VK_SUCCESS, "VulkanBuffer", "Failed to create the buffer");

        //Get memory requirements
        VkMemoryRequirements requirements = {};
        vkGetBufferMemoryRequirements(mLogicalDevice, mBuffer, &requirements);

        //Get buffer memory
        const unsigned long long memoryOffset = desc.pMemory->allocate(requirements.size + requirements.alignment);
        const unsigned long long alignedMemoryOffset = memoryOffset + (memoryOffset % requirements.alignment == 0 ? 0 : (requirements.alignment - (memoryOffset % requirements.alignment)));

        //Bind memory
        DEV_ASSERT(vkBindBufferMemory(mLogicalDevice, mBuffer, pHeap->GetVkMemory(), alignedMemoryOffset) == VK_SUCCESS, "VulkanBuffer", "Failed to bind memory");

        mMemoryOffset = memoryOffset;
        mMemoryAlignedOffset = alignedMemoryOffset;
	}
	VulkanBuffer::~VulkanBuffer()
	{
        vkDestroyBuffer(mLogicalDevice, mBuffer, nullptr);
	}
}

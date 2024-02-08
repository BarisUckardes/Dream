#include "GraphicsBuffer.h"
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Buffer/VulkanBufferUtils.h>

namespace Dream
{
	GraphicsBuffer::~GraphicsBuffer()
	{
		vkDestroyBuffer(mLogicalDevice, mBuffer, nullptr);
	}
	GraphicsBuffer::GraphicsBuffer(const GraphicsBufferDesc& desc, GraphicsDevice* pDevice) :
		GraphicsDeviceObject(pDevice),mLogicalDevice(pDevice->GetLogicalDevice()),mUsages(desc.Usage),mSubItemCount(desc.SubItemCount),mSubItemSize(desc.SubItemSizeInBytes),mTotalSize(desc.SubItemCount*desc.SubItemSizeInBytes),mBuffer(VK_NULL_HANDLE),mMemoryOffset(0),mAlignedMemoryOffset(0)
	{
		//Create buffer
		VkBufferCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		info.flags = VkBufferCreateFlags();
		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.usage = VulkanBufferUtils::GetUsages(desc.Usage);
		info.size = GetTotalSize();
		info.pNext = nullptr;

		DEV_ASSERT(vkCreateBuffer(mLogicalDevice, &info, nullptr, &mBuffer) == VK_SUCCESS, "VulkanBuffer", "Failed to create buffer");

		//Get memory requirements
		VkMemoryRequirements requirements = {};
		vkGetBufferMemoryRequirements(mLogicalDevice, mBuffer, &requirements);

		//Get buffer memory
		const unsigned long long memoryOffset = desc.pHeap->Allocate(requirements.size + requirements.alignment);
		const unsigned long long alignedMemoryOffset = memoryOffset + (memoryOffset % requirements.alignment == 0 ? 0 : (requirements.alignment - (memoryOffset % requirements.alignment)));

		//Bind memory
		DEV_ASSERT(vkBindBufferMemory(mLogicalDevice, mBuffer, desc.pHeap->GetMemory(), alignedMemoryOffset) == VK_SUCCESS, "VulkanBuffer", "Failed to bind memory");

	}
}

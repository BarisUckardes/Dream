#include "GraphicsMemoryHeap.h"
#include <Runtime/Graphics/Memory/VulkanMemoryUtils.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>

namespace Dream
{
	GraphicsMemoryHeap::~GraphicsMemoryHeap()
	{
        vkFreeMemory(mLogicalDevice, mMemory, nullptr);
	}
	unsigned long long GraphicsMemoryHeap::Allocate(const unsigned long long sizeInBytes)
	{
		//Check enough space is left
		const unsigned long long spaceLeft = mCapacity - mAllocatedSize;
		if (spaceLeft < sizeInBytes)
			return uint64_max;

        //Look for an available space
        unsigned long long offset = 0;
        for (unsigned long long i = 0; i < mAllocations.size(); i++)
        {
            SubAllocation& allocation = mAllocations[i];
            if (allocation.bOwned)
            {
                offset += allocation.SizeInBytes;
                continue;
            }

            if (allocation.SizeInBytes < sizeInBytes)
            {
                offset += allocation.SizeInBytes;
                continue;
            }

            const unsigned long long allocationSizeLeft = allocation.SizeInBytes - sizeInBytes;
            allocation.SizeInBytes = allocationSizeLeft;

            SubAllocation newAllocation = {};
            newAllocation.bOwned = true;
            newAllocation.SizeInBytes = sizeInBytes;
            mAllocations.insert(mAllocations.begin() + i, newAllocation);
            mAllocatedSize += sizeInBytes;
            return offset;
        }

        return uint64_max;
	}
	void GraphicsMemoryHeap::Free(const unsigned long long offsetInBytes)
	{
        //Find and free owned memory
        unsigned long long currentOffset = 0;
        for (unsigned int i = 0; i < mAllocations.size(); i++)
        {
            SubAllocation& allocation = mAllocations[i];
            if (currentOffset != offsetInBytes)
            {
                currentOffset += allocation.SizeInBytes;
                continue;
            }

            allocation.bOwned = false;
            mAllocatedSize -= allocation.SizeInBytes;
            break;
        }

        //Compact
        for (unsigned int  i = 0; i < mAllocations.size(); i++)
        {
            SubAllocation& allocation = mAllocations[i];
            CompactReport report = GetCompactReport(i);
            if (report.Min == report.Max)
                continue;

            Compact(report);

            if (report.Min < i)
            {
                const unsigned int diff = i - report.Min;
                i -= diff;
            }
        }
	}
	GraphicsMemoryHeap::GraphicsMemoryHeap(const GraphicsMemoryHeapDesc& desc,GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),mCapacity(desc.SizeInBytes),mType(desc.Type),mAllocatedSize(0),mLogicalDevice(pDevice->GetLogicalDevice())
	{
        //Allocate memory
        VkMemoryAllocateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        info.memoryTypeIndex = VulkanMemoryUtils::GetMemoryFlags(desc.Type);
        info.allocationSize = desc.SizeInBytes;
        info.pNext = nullptr;

        DEV_ASSERT(vkAllocateMemory(pDevice->GetLogicalDevice(), &info, nullptr, &mMemory) == VK_SUCCESS, "VulkanMemoryHeap", "Failed to allocate memory");
	}
    Dream::GraphicsMemoryHeap::CompactReport GraphicsMemoryHeap::GetCompactReport(const unsigned int index)
    {
        //Check min
        unsigned int min = index;
        for (unsigned int i = index; i > 0; i--)
        {
            SubAllocation& allocation = mAllocations[i];
            if (allocation.bOwned)
                break;

            min--;
        }

        //Check max
        unsigned int max = index;
        for (unsigned int i = index; i < mAllocations.size(); i++)
        {
            SubAllocation& allocation = mAllocations[i];
            if (allocation.bOwned)
                break;

            max++;
        }

        return { min,max };
    }
    void GraphicsMemoryHeap::Compact(const CompactReport& report)
    {
        //Calculate size
        unsigned long long size = 0;
        for (unsigned int i = report.Min; i < report.Max; i++)
        {
            SubAllocation& allocation = mAllocations[i];
            size += allocation.SizeInBytes;
        }

        //Remove nodes
        const unsigned int removeCount = report.Max - report.Min;
        for (unsigned int i = 0; i < removeCount; i++)
            mAllocations.erase(mAllocations.begin() + report.Min);

        //Add single node
        SubAllocation allocation = {};
        allocation.SizeInBytes = size;
        allocation.bOwned = false;
        mAllocations.insert(mAllocations.begin() + report.Min,allocation);
    }
}

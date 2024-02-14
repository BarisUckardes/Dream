#include "GraphicsMemory.h"

namespace Dream
{
    GraphicsMemory::GraphicsMemory(const GraphicsMemoryDesc& desc,GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),mType(desc.Type),mSize(desc.SizeInBytes),mOccupiedSize(0)
    {
        //Create block
        SubAllocationBlock initialBlock = {};
        initialBlock.bOwned = false;
        initialBlock.SizeInBytes = desc.SizeInBytes;
        mBlocks.push_back(initialBlock);
    }
    unsigned long long GraphicsMemory::Allocate(const unsigned long long sizeInBytes)
    {
        //Check enough space is left
        const unsigned long long sizeLeft = mSize - mOccupiedSize;
        if (sizeLeft < sizeInBytes)
            return uint64_max;

        //Look for an available space
        unsigned long long offset = 0;
        for (unsigned int i = 0; i < mBlocks.size(); i++)
        {
            SubAllocationBlock& block = mBlocks[i];
            if (block.bOwned)
            {
                offset += block.SizeInBytes;
                continue;
            }

            if (block.SizeInBytes < sizeInBytes)
            {
                offset += block.SizeInBytes;
                continue;
            }

            const unsigned long long sizeLeft = block.SizeInBytes - sizeInBytes;
            block.SizeInBytes = sizeLeft;

            SubAllocationBlock newBlock = {};
            newBlock.bOwned = true;
            newBlock.SizeInBytes = sizeInBytes;
            mBlocks.insert(mBlocks.begin() + i, newBlock);
            mOccupiedSize += sizeInBytes;
            return offset;
        }

        return uint64_max;
    }
    void GraphicsMemory::Free(const unsigned long long offsetInBytes)
    {
        //Find and free owned memory
        unsigned long long currentOffset = 0;
        for (unsigned int i = 0; i < mBlocks.size(); i++)
        {
            SubAllocationBlock& block = mBlocks[i];
            if (currentOffset != offsetInBytes)
            {
                currentOffset += block.SizeInBytes;
                continue;
            }

            block.bOwned = false;
            mOccupiedSize -= block.SizeInBytes;
            break;
        }

        //Compact
        for (unsigned int i = 0; i < mBlocks.size(); i++)
        {
            SubAllocationBlock& block = mBlocks[i];
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
    Dream::GraphicsMemory::CompactReport GraphicsMemory::GetCompactReport(const unsigned int index)
    {
        //Check min
        unsigned int min = index;
        for (unsigned int i = index; i > 0; i--)
        {
            SubAllocationBlock& block = mBlocks[i];
            if (block.bOwned)
                break;

            min--;
        }

        //Check max
        unsigned int max = index;
        for (unsigned int i = index; i < mBlocks.size(); i++)
        {
            SubAllocationBlock& block = mBlocks[i];
            if (block.bOwned)
                break;

            max++;
        }

        return { min,max };
    }
    void GraphicsMemory::Compact(const CompactReport& report)
    {
        //Calculate size
        unsigned long long size = 0;
        for (unsigned int i = report.Min; i < report.Max; i++)
        {
            SubAllocationBlock& block = mBlocks[i];
            size += block.SizeInBytes;
        }

        //Remove nodes
        const unsigned int removeCount = report.Max - report.Min;
        for (unsigned int i = 0; i < removeCount; i++)
            mBlocks.erase(mBlocks.begin() + report.Min);

        //Add single node
        SubAllocationBlock block = {};
        block.SizeInBytes = size;
        block.bOwned = false;
        mBlocks.insert(mBlocks.begin() + report.Min,block);
    }
}

#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryHeapDesc.h>
#include <vulkan.h>
#include <vector>

namespace Dream
{
	class RUNTIME_API GraphicsMemoryHeap final : public GraphicsDeviceObject
	{
		friend class GraphicsDevice;
	private:
		struct SubAllocation
		{
			bool bOwned;
			unsigned long long SizeInBytes;
		};
		struct CompactReport
		{
			unsigned int Min;
			unsigned int Max;
		};
	public:
		~GraphicsMemoryHeap();

		FORCEINLINE unsigned long long GetCapacity() const noexcept
		{
			return mCapacity;
		}
		FORCEINLINE GraphicsMemoryType GetMemoryType() const noexcept
		{
			return mType;
		}
		FORCEINLINE unsigned long long GetAllocatedSize() const noexcept
		{
			return mAllocatedSize;
		}
		FORCEINLINE VkDeviceMemory GetMemory() const noexcept
		{
			return mMemory;
		}

		virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
		{
			return GraphicsDeviceObjectType::MemoryHeap;
		}

		unsigned long long Allocate(const unsigned long long sizeInBytes);
		void Free(const unsigned long long offsetInBytes);
	private:
		GraphicsMemoryHeap(const GraphicsMemoryHeapDesc& desc,GraphicsDevice* pDevice);

		CompactReport GetCompactReport(const unsigned int index);
		void Compact(const CompactReport& report);
	private:
		const unsigned long long mCapacity;
		const GraphicsMemoryType mType;
		std::vector<SubAllocation> mAllocations;
		unsigned long long mAllocatedSize;
		VkDevice mLogicalDevice;
		VkDeviceMemory mMemory;
	};
}

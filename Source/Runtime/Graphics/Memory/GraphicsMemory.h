#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryDesc.h>
#include <vector>

namespace Dream
{
	class RUNTIME_API GraphicsMemory : public GraphicsDeviceObject
	{
	private:
		struct SubAllocationBlock
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
		~GraphicsMemory() = default;

		FORCEINLINE unsigned long long GetHeapSize() const noexcept
		{
			return mSize;
		}
		FORCEINLINE unsigned long long GetOccupiedSize() const noexcept
		{
			return mOccupiedSize;
		}
		FORCEINLINE GraphicsMemoryType GetMemoryType() const noexcept
		{
			return mType;
		}
		virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
		{
			return GraphicsDeviceObjectType::MemoryHeap;
		}

		unsigned long long Allocate(const unsigned long long sizeInBytes);
		void Free(const unsigned long long offsetInBytes);
	protected:
		GraphicsMemory(const GraphicsMemoryDesc& desc,GraphicsDevice* pDevice);
	private:
		CompactReport GetCompactReport(const unsigned int index);
		void Compact(const CompactReport& report);
	private:
		const unsigned long long mSize;
		const GraphicsMemoryType mType;
		unsigned long long mOccupiedSize;
		std::vector<SubAllocationBlock> mBlocks;
	};
}

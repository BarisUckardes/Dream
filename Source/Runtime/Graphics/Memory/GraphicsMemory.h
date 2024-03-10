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

		FORCEINLINE unsigned long long size() const noexcept
		{
			return mSize;
		}
		FORCEINLINE unsigned long long occupied_size() const noexcept
		{
			return mOccupiedSize;
		}
		FORCEINLINE GraphicsMemoryType memory_type() const noexcept
		{
			return mType;
		}
		virtual GraphicsDeviceObjectType object_type() const noexcept override final
		{
			return GraphicsDeviceObjectType::MemoryHeap;
		}

		unsigned long long allocate(const unsigned long long sizeInBytes);
		void free(const unsigned long long offsetInBytes);
	protected:
		GraphicsMemory(const GraphicsMemoryDesc& desc,GraphicsDevice* pDevice);
	private:
		CompactReport compact_report(const unsigned int index);
		void compact(const CompactReport& report);
	private:
		const unsigned long long mSize;
		const GraphicsMemoryType mType;
		unsigned long long mOccupiedSize;
		std::vector<SubAllocationBlock> mBlocks;
	};
}

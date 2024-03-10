#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferDesc.h>

namespace Dream
{
	class RUNTIME_API GraphicsBuffer : public GraphicsDeviceObject
	{
	public:
		~GraphicsBuffer();

		FORCEINLINE GraphicsBufferUsage usages() const noexcept
		{
			return mUsages;
		}
		FORCEINLINE unsigned int sub_item_count() const noexcept
		{
			return mSubItemCount;
		}
		FORCEINLINE unsigned int sub_item_size() const noexcept
		{
			return mSubItemSize;
		}
		FORCEINLINE unsigned long long total_size() const noexcept
		{
			return mTotalSize;
		}
		FORCEINLINE const GraphicsMemory* memory() const noexcept
		{
			return mMemory;
		}

		virtual GraphicsDeviceObjectType object_type() const noexcept override final
		{
			return GraphicsDeviceObjectType::Buffer;
		}
	protected:
		GraphicsBuffer(const GraphicsBufferDesc& desc, GraphicsDevice* pDevice);

	private:
		const GraphicsBufferUsage mUsages;
		const unsigned int mSubItemCount;
		const unsigned int mSubItemSize;
		const unsigned long long mTotalSize;
		const GraphicsMemory* mMemory;
	};
}

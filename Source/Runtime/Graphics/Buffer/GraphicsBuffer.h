#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferDesc.h>

namespace Dream
{
	class RUNTIME_API GraphicsBuffer : public GraphicsDeviceObject
	{
	public:
		~GraphicsBuffer();

		FORCEINLINE GraphicsBufferUsage GetUsages() const noexcept
		{
			return mUsages;
		}
		FORCEINLINE unsigned int GetSubItemCount() const noexcept
		{
			return mSubItemCount;
		}
		FORCEINLINE unsigned int GetSubItemSize() const noexcept
		{
			return mSubItemSize;
		}
		FORCEINLINE unsigned long long GetTotalSize() const noexcept
		{
			return mTotalSize;
		}
		FORCEINLINE const GraphicsMemory* GetMemory() const noexcept
		{
			return mMemory;
		}

		virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
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

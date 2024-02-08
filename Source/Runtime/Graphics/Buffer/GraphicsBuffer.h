#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferDesc.h>
#include <vulkan.h>

namespace Dream
{
	class RUNTIME_API GraphicsBuffer : public GraphicsDeviceObject
	{
		friend class GraphicsDevice;
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
		FORCEINLINE unsigned int GetTotalSize() const noexcept
		{
			return mTotalSize;
		}
		FORCEINLINE VkBuffer GetBuffer() const noexcept
		{
			return mBuffer;
		}
		FORCEINLINE unsigned long long GetMemoryOffset() const noexcept
		{
			return mMemoryOffset;
		}
		FORCEINLINE unsigned long long GetAlignedMemoryOffset() const noexcept
		{
			return mAlignedMemoryOffset;
		}
		virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
		{
			return GraphicsDeviceObjectType::Buffer;
		}

	private:
		GraphicsBuffer(const GraphicsBufferDesc& desc, GraphicsDevice* pDevice);
	private:
		const GraphicsBufferUsage mUsages;
		const unsigned int mSubItemCount;
		const unsigned int mSubItemSize;
		const unsigned int mTotalSize;
		VkBuffer mBuffer;
		VkDevice mLogicalDevice;
		unsigned long long mMemoryOffset;
		unsigned long long mAlignedMemoryOffset;
	};
}

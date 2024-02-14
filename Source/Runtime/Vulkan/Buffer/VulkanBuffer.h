#pragma once
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanBuffer : public GraphicsBuffer
	{
	public:
		VulkanBuffer(const GraphicsBufferDesc& desc, VulkanDevice* pDevice);
		~VulkanBuffer();

		FORCEINLINE VkBuffer GetVkBuffer() const noexcept
		{
			return mBuffer;
		}
		FORCEINLINE unsigned long long GetVkMemoryOffset() const noexcept
		{
			return mMemoryOffset;
		}
		FORCEINLINE unsigned long long GetVkMemoryAlignedOffset() const noexcept
		{
			return mMemoryAlignedOffset;
		}
	private:
		VkBuffer mBuffer;
		VkDevice mLogicalDevice;
		unsigned long long mMemoryOffset;
		unsigned long long mMemoryAlignedOffset;
	};
}

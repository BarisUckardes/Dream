#pragma once
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanQueue : public GraphicsQueue
	{
	public:
		VulkanQueue(const GraphicsQueueDesc& desc,const VkQueue queue,const unsigned char familyIndex, VulkanDevice* pDevice);
		~VulkanQueue();

		FORCEINLINE VkQueue GetVkQueue() const noexcept
		{
			return mQueue;
		}
		FORCEINLINE unsigned char GetVkFamilyIndex() const noexcept
		{
			return mFamilyIndex;
		}
	private:
		const VkQueue mQueue;
		const unsigned char mFamilyIndex;
	};
}

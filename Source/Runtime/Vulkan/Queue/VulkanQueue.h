#pragma once
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanQueue : public GraphicsQueue
	{
	public:
		VulkanQueue(const GraphicsQueueDesc& desc, VulkanDevice* pDevice);
		~VulkanQueue();

		FORCEINLINE VkQueue GetVkQueue() const noexcept
		{
			return mQueue;
		}
	private:
		VkQueue mQueue;
	};
}

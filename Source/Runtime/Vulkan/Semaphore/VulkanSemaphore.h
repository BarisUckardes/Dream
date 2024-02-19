#pragma once
#include <Runtime/Graphics/Semaphore/Semaphore.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanSemaphore : public Semaphore
	{
	public:
		VulkanSemaphore(const SemaphoreDesc& desc, VulkanDevice* pDevice);
		~VulkanSemaphore();

		FORCEINLINE VkSemaphore GetVkSemaphore() const noexcept
		{
			return mSemaphore;
		}
	private:
		VkSemaphore mSemaphore;
		VkDevice mLogicalDevice;
	};
}

#pragma once
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanSwapchain : public Swapchain
	{
	public:
		VulkanSwapchain(const SwapchainDesc& desc, VulkanDevice* pDevice);
		~VulkanSwapchain();

		FORCEINLINE VkSwapchainKHR vk_swapchain() const noexcept
		{
			return mSwapchain;
		}
		FORCEINLINE VkSurfaceKHR vk_surface() const noexcept
		{
			return mSurface;
		}
	private:
		virtual void resize_impl(const unsigned int width, const unsigned int height) override;
		virtual void Present_impl(Semaphore** ppWaitSemahpores, const unsigned int waitSemaphoreCount) override;
		void delete_textures();
	private:
		VkSwapchainKHR mSwapchain;
		VkSurfaceKHR mSurface;
		VkDevice mLogicalDevice;
		VkPhysicalDevice mPhysicalDevice;
		VkInstance mInstance;
	};
}

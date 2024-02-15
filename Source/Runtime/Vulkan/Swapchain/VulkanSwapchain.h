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

		FORCEINLINE VkSwapchainKHR GetVkSwapchain() const noexcept
		{
			return mSwapchain;
		}
		FORCEINLINE VkSurfaceKHR GetVkSurface() const noexcept
		{
			return mSurface;
		}
	private:
		virtual void ResizeCore(const unsigned int width, const unsigned int height) override;
		virtual void PresentCore() override;
	private:
		VkSwapchainKHR mSwapchain;
		VkSurfaceKHR mSurface;
		VkDevice mLogicalDevice;
		VkPhysicalDevice mPhysicalDevice;
		VkInstance mInstance;
	};
}

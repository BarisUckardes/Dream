#pragma once
#include <Runtime/Graphics/Swapchain/PresentMode.h>
#include <vulkan.h>

namespace Dream
{
	class RUNTIME_API VulkanSwapchainUtils final
	{
	public:
		FORCEINLINE static VkPresentModeKHR GetPresentMode(const PresentMode mode)
		{
			switch (mode)
			{
			case Dream::PresentMode::Immediate:
			default:
				return VK_PRESENT_MODE_IMMEDIATE_KHR;
			case Dream::PresentMode::VsyncImmediate:
				return VK_PRESENT_MODE_MAILBOX_KHR;
			case Dream::PresentMode::VsyncQueued:
				return VK_PRESENT_MODE_FIFO_KHR;;
				break;
			}
		}
	public:
		VulkanSwapchainUtils() = delete;
		~VulkanSwapchainUtils() = delete;
	};
}

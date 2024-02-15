#pragma once
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(const RenderPassDesc& desc, VulkanDevice* pDevice);
		~VulkanRenderPass();

		FORCEINLINE VkRenderPass GetVkRenderPass() const noexcept
		{
			return mRenderPass;
		}
		FORCEINLINE VkFramebuffer GetVkFramebuffer() const noexcept
		{
			return mFramebuffer;
		}
	private:
		VkFramebuffer mFramebuffer;
		VkDevice mLogicalDevice;
		VkRenderPass mRenderPass;
	};
}

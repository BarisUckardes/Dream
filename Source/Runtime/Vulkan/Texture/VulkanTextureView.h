#pragma once
#include <Runtime/Graphics/Texture/TextureView.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanTextureView : public TextureView
	{
	public:
		VulkanTextureView(const TextureViewDesc& desc, const VkImageView view, VulkanDevice* pDevice);
		VulkanTextureView(const TextureViewDesc& desc, VulkanDevice* pDevice);
		~VulkanTextureView();

		FORCEINLINE VkImageView vk_view() const noexcept
		{
			return mView;
		}
	private:
		VkImageView mView;
		VkDevice mLogicalDevice;
		const bool mSwapchain;
	};
}

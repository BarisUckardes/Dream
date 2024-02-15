#pragma once
#include <Runtime/Graphics/Texture/Texture.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanTexture : public Texture
	{
	public:
		VulkanTexture(const TextureDesc& desc, const VkImage image, VulkanDevice* pDevice);
		VulkanTexture(const TextureDesc& desc, VulkanDevice* pDevice);
		~VulkanTexture();

		FORCEINLINE VkImage GetVkImage() const noexcept
		{
			return mImage;
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
		const bool mSwapchain;
		VkImage mImage;
		VkDevice mLogicalDevice;
		unsigned long long mMemoryOffset;
		unsigned long long mMemoryAlignedOffset;
	};
}

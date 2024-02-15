#include "VulkanTexture.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/Memory/VulkanMemory.h>

namespace Dream
{
	VulkanTexture::VulkanTexture(const TextureDesc& desc, const VkImage image, VulkanDevice* pDevice) : Texture(desc, pDevice), mImage(image), mLogicalDevice(pDevice->GetVkLogicalDevice()), mMemoryOffset(0), mMemoryAlignedOffset(0),mSwapchain(true)
	{

	}
	VulkanTexture::VulkanTexture(const TextureDesc& desc, VulkanDevice* pDevice) : Texture(desc,pDevice),mImage(VK_NULL_HANDLE),mLogicalDevice(pDevice->GetVkLogicalDevice()),mSwapchain(false)
	{
		const VulkanMemory* pVkMemory = (const VulkanMemory*)desc.pMemory;

		//Create image
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.extent = { desc.Width,desc.Height,desc.Depth };
		imageInfo.format = VulkanTextureUtils::GetTextureFormat(desc.Format);
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.arrayLayers = desc.ArrayLevels;
		imageInfo.mipLevels = desc.MipLevels;
		imageInfo.imageType = VulkanTextureUtils::GetImageType(desc.Type);
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.usage = VulkanTextureUtils::GetImageUsages(desc.Usages);
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = (VkSampleCountFlagBits)VulkanTextureUtils::GetSampleCount(desc.SampleCount);
		imageInfo.flags = VkImageCreateFlags();

		DEV_ASSERT(vkCreateImage(mLogicalDevice, &imageInfo, nullptr, &mImage) == VK_SUCCESS, "VulkanTexture", "Failed to create texture");

		//Get memory requirements
		VkMemoryRequirements requirements = {};
		vkGetImageMemoryRequirements(mLogicalDevice, mImage, &requirements);

		//Rent memory
		const unsigned long long memoryOffset = desc.pMemory->Allocate(requirements.size + requirements.alignment);
		const unsigned long long memoryAlignedOffset = memoryOffset + (memoryOffset == 0 ? 0 : (requirements.alignment - (memoryOffset % requirements.alignment)));

		DEV_ASSERT(vkBindImageMemory(mLogicalDevice, mImage, pVkMemory->GetVkMemory(), memoryAlignedOffset) == VK_SUCCESS, "VulkanTexture", "Failed to bind the texture memory!");

		mMemoryOffset = memoryOffset;
		mMemoryAlignedOffset = memoryAlignedOffset;
	}
	VulkanTexture::~VulkanTexture()
	{
		if(!mSwapchain)
			vkDestroyImage(mLogicalDevice, mImage, nullptr);
	}
}

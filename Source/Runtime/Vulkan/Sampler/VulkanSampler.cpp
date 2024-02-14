#include "VulkanSampler.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/Sampler/VulkanSamplerUtils.h>
#include <Runtime/Vulkan/VulkanUtils.h>

namespace Dream
{
	VulkanSampler::VulkanSampler(const SamplerDesc& desc, VulkanDevice* pDevice) : Sampler(desc,pDevice),mSampler(VK_NULL_HANDLE),mLogicalDevice(pDevice->GetVkLogicalDevice())
	{
        VkSamplerCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        createInfo.flags = VkSamplerCreateFlags();
        createInfo.minFilter = VulkanSamplerUtils::GetFilter(desc.MinFilter);
        createInfo.magFilter = VulkanSamplerUtils::GetFilter(desc.MagFilter);
        createInfo.addressModeU = VulkanSamplerUtils::GetAddressMode(desc.AddressModeU);
        createInfo.addressModeV = VulkanSamplerUtils::GetAddressMode(desc.AddressModeV);
        createInfo.addressModeW = VulkanSamplerUtils::GetAddressMode(desc.AddressModeW);
        createInfo.mipLodBias = desc.MipLodBias;
        createInfo.anisotropyEnable = desc.MaxAnisotropy != 0;
        createInfo.maxAnisotropy = desc.MaxAnisotropy;
        createInfo.compareEnable = desc.bComparisonEnabled;
        createInfo.compareOp = VulkanUtils::GetCompareOperation(desc.CompareOperation);
        createInfo.minLod = desc.MinLod;
        createInfo.maxLod = desc.MaxLod;
        createInfo.borderColor = VulkanSamplerUtils::GetBorderColor(desc.BorderColor);
        createInfo.unnormalizedCoordinates = false;

        createInfo.pNext = nullptr;

        DEV_ASSERT(vkCreateSampler(pDevice->GetVkLogicalDevice(), &createInfo, nullptr, &mSampler) == VK_SUCCESS, "VulkanSampler", "Failed to create sampler");
	}
	VulkanSampler::~VulkanSampler()
	{
        vkDestroySampler(mLogicalDevice, mSampler, nullptr);
	}
}

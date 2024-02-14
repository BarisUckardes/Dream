#pragma once
#include <Runtime/Graphics/Sampler/Sampler.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanSampler : public Sampler
	{
	public:
		VulkanSampler(const SamplerDesc& desc, VulkanDevice* pDevice);
		~VulkanSampler();

		FORCEINLINE VkSampler GetVkSampler() const noexcept
		{
			return mSampler;
		}
	private:
		VkSampler mSampler;
		VkDevice mLogicalDevice;
	};
}

#pragma once
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <vulkan.h>

namespace Dream
{
    class VulkanDevice;
    class RUNTIME_API VulkanPipeline : public Pipeline
    {
    public:
        VulkanPipeline(const GraphicsPipelineDesc& desc, VulkanDevice* pDevice);
        VulkanPipeline(const ComputePipelineDesc& desc, VulkanDevice* pDevice);
        ~VulkanPipeline();

        FORCEINLINE VkPipelineLayout GetVkPipelineLayout() const noexcept { return mLayout; }
        FORCEINLINE VkPipeline GetVkPipeline() const noexcept { return mPipeline; }
        FORCEINLINE VkPipelineBindPoint GetVkPipelinBindPoint() const noexcept { return mBindPoint; }
    private:
        VkPipelineLayout mLayout;
        VkPipeline mPipeline;
        VkPipelineBindPoint mBindPoint;
        VkDevice mLogicalDevice;
    };
}

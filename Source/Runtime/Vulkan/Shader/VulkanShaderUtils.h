#pragma once
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <vulkan.h>

namespace Dream
{
    class RUNTIME_API VulkanShaderUtils
    {
    public:
        FORCEINLINE static VkShaderStageFlags GetShaderFlags(const ShaderStage stages)
        {
            VkShaderStageFlags flags = VkShaderStageFlags();

            if (stages & ShaderStage::VertexStage)
                flags |= VK_SHADER_STAGE_VERTEX_BIT;
            if (stages & ShaderStage::FragmentStage)
                flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
            if (stages & ShaderStage::ComputeStage)
                flags |= VK_SHADER_STAGE_COMPUTE_BIT;

            return flags;
        }
    public:
        VulkanShaderUtils() = delete;
        ~VulkanShaderUtils() = delete;
    };
}

#pragma once
#include <Runtime/Graphics/Sampler/SamplerFiltering.h>
#include <Runtime/Graphics/Sampler/SamplerMapMode.h>
#include <Runtime/Graphics/Sampler/SamplerAddressMode.h>
#include <Runtime/Graphics/Sampler/SamplerBorderColor.h>
#include <vulkan.h>

namespace Dream
{
    class RUNTIME_API VulkanSamplerUtils
    {
    public:
        FORCEINLINE static VkFilter GetFilter(const SamplerFiltering filter)
        {
            switch (filter)
            {
            case SamplerFiltering::Nearest:
                return VK_FILTER_NEAREST;
            case SamplerFiltering::Linear:
                return VK_FILTER_LINEAR;
            default:
                return VK_FILTER_NEAREST;
            }
        }
        FORCEINLINE static VkSamplerMipmapMode GetMipmapMode(const SamplerMapMode mode)
        {
            switch (mode)
            {
            case SamplerMapMode::Nearest:
                return VK_SAMPLER_MIPMAP_MODE_NEAREST;
            case SamplerMapMode::Linear:
                return VK_SAMPLER_MIPMAP_MODE_LINEAR;
            default:
                return VK_SAMPLER_MIPMAP_MODE_NEAREST;
            }
        }
        FORCEINLINE static VkSamplerAddressMode GetAddressMode(const SamplerAddressMode mode)
        {
            switch (mode)
            {
            case SamplerAddressMode::Repeat:
                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            case SamplerAddressMode::MirroredRepeat:
                return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            case SamplerAddressMode::ClampToEdge:
                return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            case SamplerAddressMode::ClampToBorder:
                return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            case SamplerAddressMode::MirrorClampToEdge:
                return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
            default:
                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            }
        }
        FORCEINLINE static VkBorderColor GetBorderColor(const SamplerBorderColor color)
        {
            switch (color)
            {
            case SamplerBorderColor::TransparentBlack:
                return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
            case SamplerBorderColor::OpaqueBlack:
                return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
            case SamplerBorderColor::OpaqueWhite:
                return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
            default:
                return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
            }
        }

    public:
        VulkanSamplerUtils() = delete;
        ~VulkanSamplerUtils() = delete;

    };

}



#pragma once
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureType.h>
#include <Runtime/Graphics/Texture/TextureUsage.h>
#include <Runtime/Graphics/Texture/TextureSampleCount.h>
#include <Runtime/Graphics/Texture/TextureMemoryLayout.h>
#include <Runtime/Graphics/Texture/TextureAspectFlags.h>
#include <vulkan.h>

namespace Dream
{
    class RUNTIME_API VulkanTextureUtils
    {
    public:
        FORCEINLINE static VkFormat GetTextureFormat(const TextureFormat format) noexcept
        {
            switch (format)
            {
            case TextureFormat::None:
                return VK_FORMAT_UNDEFINED;
            case TextureFormat::R8_G8_B8_A8_UNorm:
                return VK_FORMAT_R8G8B8A8_UNORM;
            case TextureFormat::B8_G8_R8_A8_UNorm:
                return VK_FORMAT_B8G8R8A8_UNORM;
            case TextureFormat::R8_UNorm:
                return VK_FORMAT_R8_UNORM;
            case TextureFormat::R16_UNorm:
                return VK_FORMAT_R16_UNORM;
            case TextureFormat::R32_G32_B32_Float:
                return VK_FORMAT_R32G32B32_SFLOAT;
            case TextureFormat::R32_G32_B32_UInt:
                return VK_FORMAT_R32G32B32_UINT;
            case TextureFormat::R32_G32_B32_SInt:
                return VK_FORMAT_R32G32B32_SINT;
            case TextureFormat::R32_G32_B32_A32_Float:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            case TextureFormat::R32_Float:
                return VK_FORMAT_R32_SFLOAT;
            case TextureFormat::BC3_UNorm:
                return VK_FORMAT_BC3_UNORM_BLOCK;
            case TextureFormat::D32_SFloat:
                return VK_FORMAT_D32_SFLOAT;
            case TextureFormat::D24_UNorm_S8_UInt:
                return VK_FORMAT_D24_UNORM_S8_UINT;
            case TextureFormat::D32_Float_S8_UInt:
                return VK_FORMAT_D32_SFLOAT_S8_UINT;
            case TextureFormat::R32_G32_B32_A32_UInt:
                return VK_FORMAT_R32G32B32A32_UINT;
            case TextureFormat::R16G16B16_UInt:
                return VK_FORMAT_R16G16B16_UINT;
            case TextureFormat::R16G16B16_SInt:
                return VK_FORMAT_R16G16B16_SINT;
            case TextureFormat::R16G16B16_Float:
                return VK_FORMAT_R16G16B16_SFLOAT;
            case TextureFormat::R8_G8_SNorm:
                return VK_FORMAT_R8G8_SNORM;
            case TextureFormat::BC1_Rgb_UNorm:
                return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
            case TextureFormat::BC1_Rgba_UNorm:
                return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
            case TextureFormat::BC2_UNorm:
                return VK_FORMAT_BC2_UNORM_BLOCK;
            case TextureFormat::R10_G10_B10_A2_UNorm:
                return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
            case TextureFormat::R10_G10_B10_A2_UInt:
                return VK_FORMAT_A2B10G10R10_UINT_PACK32;
            case TextureFormat::R11_G11_B10_Float:
                return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
            case TextureFormat::R8_SNorm:
                return VK_FORMAT_R8_SNORM;
            case TextureFormat::R8_UInt:
                return VK_FORMAT_R8_UINT;
            case TextureFormat::R8_SInt:
                return VK_FORMAT_R8_SINT;
            case TextureFormat::R16_SNorm:
                return VK_FORMAT_R16_SNORM;
            case TextureFormat::R16_UInt:
                return VK_FORMAT_R16_UINT;
            case TextureFormat::R16_SInt:
                return VK_FORMAT_R16_SINT;
            case TextureFormat::R16_Float:
                return VK_FORMAT_R16_SFLOAT;
            case TextureFormat::R32_UInt:
                return VK_FORMAT_R32_UINT;
            case TextureFormat::R32_SInt:
                return VK_FORMAT_R32_SINT;
            case TextureFormat::R8_G8_UNorm:
                return VK_FORMAT_R8G8_UNORM;
            case TextureFormat::R8_G8_UInt:
                return VK_FORMAT_R8G8_UINT;
            case TextureFormat::R8_G8_SInt:
                return VK_FORMAT_R8G8_SINT;
            case TextureFormat::R16_G16_UNorm:
                return VK_FORMAT_R16G16_UNORM;
            case TextureFormat::R16_G16_SNorm:
                return VK_FORMAT_R16G16_SNORM;
            case TextureFormat::R16_G16_UInt:
                return VK_FORMAT_R16G16_UINT;
            case TextureFormat::R16_G16_SInt:
                return VK_FORMAT_R16G16_SINT;
            case TextureFormat::R16_G16_Float:
                return VK_FORMAT_R16G16_SFLOAT;
            case TextureFormat::R32_G32_UInt:
                return VK_FORMAT_R32G32_UINT;
            case TextureFormat::R32_G32_SInt:
                return VK_FORMAT_R32G32_SINT;
            case TextureFormat::R32_G32_Float:
                return VK_FORMAT_R32G32_SFLOAT;
            case TextureFormat::R8_G8_B8_A8_SNorm:
                return VK_FORMAT_R8G8B8A8_SNORM;
            case TextureFormat::R8_G8_B8_A8_UInt:
                return VK_FORMAT_R8G8B8A8_UINT;
            case TextureFormat::R8_G8_B8_A8_SInt:
                return VK_FORMAT_R8G8B8A8_SINT;
            case TextureFormat::R16_G16_B16_A16_UNorm:
                return VK_FORMAT_R16G16B16A16_UNORM;
            case TextureFormat::R16_G16_B16_A16_SNorm:
                return VK_FORMAT_R16G16B16A16_SNORM;
            case TextureFormat::R16_G16_B16_A16_UInt:
                return VK_FORMAT_R16G16B16A16_UINT;
            case TextureFormat::R16_G16_B16_A16_SInt:
                return VK_FORMAT_R16G16B16A16_SINT;
            case TextureFormat::R16_G16_B16_A16_Float:
                return VK_FORMAT_R16G16B16A16_SFLOAT;
            case TextureFormat::R32_G32_B32_A32_SInt:
                return VK_FORMAT_R32G32B32A32_SINT;
            case TextureFormat::R64_UInt:
                return VK_FORMAT_R64_UINT;
            case TextureFormat::R64_SInt:
                return VK_FORMAT_R64_SINT;
            case TextureFormat::R64_Float:
                return VK_FORMAT_R64_SFLOAT;
            case TextureFormat::R64_G64_UInt:
                return VK_FORMAT_R64_UINT;
            case TextureFormat::R64_G64_SInt:
                return VK_FORMAT_R64_UINT;
            case TextureFormat::R64_G64_Float:
                return VK_FORMAT_R64_SFLOAT;
            case TextureFormat::R64_G64_B64_UInt:
                return VK_FORMAT_R64G64B64_UINT;
            case TextureFormat::R64_G64_B64_SInt:
                return VK_FORMAT_R64G64B64_SINT;
            case TextureFormat::R64_G64_B64_Float:
                return VK_FORMAT_R64G64B64_SFLOAT;
            case TextureFormat::R64_G64_B64_A64_UInt:
                return VK_FORMAT_R64G64B64A64_UINT;
            case TextureFormat::R64_G64_B64_A64_SInt:
                return VK_FORMAT_R64G64B64A64_SINT;
            case TextureFormat::R64_G64_B64_A64_Float:
                return VK_FORMAT_R64G64B64A64_SFLOAT;
            case TextureFormat::ETC2_R8_G8_B8_UNorm:
                return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
            case TextureFormat::ETC2_R8_G8_B8_A1_UNorm:
                return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
            case TextureFormat::ETC2_R8_G8_B8_A8_UNorm:
                return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
            case TextureFormat::BC4_UNorm:
                return VK_FORMAT_BC4_UNORM_BLOCK;
            case TextureFormat::BC4_SNorm:
                return VK_FORMAT_BC4_SNORM_BLOCK;
            case TextureFormat::BC5_UNorm:
                return VK_FORMAT_BC5_UNORM_BLOCK;
            case TextureFormat::BC5_SNorm:
                return VK_FORMAT_BC5_SNORM_BLOCK;
            case TextureFormat::BC7_UNorm:
                return VK_FORMAT_BC7_UNORM_BLOCK;
            case TextureFormat::R8_G8_B8_A8_UNorm_SRgb:
                return VK_FORMAT_R8G8B8A8_SRGB;
            case TextureFormat::B8_G8_R8_A8_UNorm_SRgb:
                return VK_FORMAT_B8G8R8A8_SRGB;
            case TextureFormat::BC1_Rgb_UNorm_SRgb:
                return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
            case TextureFormat::BC1_Rgba_UNorm_SRgb:
                return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
            case TextureFormat::BC2_UNorm_SRgb:
                return VK_FORMAT_BC2_SRGB_BLOCK;
            case TextureFormat::BC3_UNorm_SRgb:
                return VK_FORMAT_BC3_SRGB_BLOCK;
            case TextureFormat::BC7_UNorm_SRgb:
                return VK_FORMAT_BC7_SRGB_BLOCK;
            default:
                return VK_FORMAT_UNDEFINED;
            }

        }

        FORCEINLINE static VkImageType GetImageType(const TextureType type) noexcept
        {
            switch (type)
            {
            case TextureType::Texture1D:
                return VK_IMAGE_TYPE_1D;
            case TextureType::Texture2D:
                return VK_IMAGE_TYPE_2D;
            case TextureType::Texture3D:
                return VK_IMAGE_TYPE_3D;
            default:
                return VK_IMAGE_TYPE_2D;
            }
        }
        FORCEINLINE static VkImageUsageFlags GetImageUsages(const TextureUsage usages) noexcept
        {
            VkImageUsageFlags flags = VkImageUsageFlags();

            if (usages & TextureUsage::Sampled)
                flags |= VK_IMAGE_USAGE_SAMPLED_BIT;

            if (usages & TextureUsage::ColorAttachment)
                flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            else if (usages & TextureUsage::DepthStencilAttachment)
                flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

            if (usages & TextureUsage::Storage)
                flags |= VK_IMAGE_USAGE_STORAGE_BIT;
            if (usages & TextureUsage::TransferDestination)
                flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            else if (usages & TextureUsage::TransferSource)
                flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

            return flags;
        }

        FORCEINLINE static VkSampleCountFlags GetSampleCount(const TextureSampleCount samples) noexcept
        {
            switch (samples)
            {
            case TextureSampleCount::SAMPLE_COUNT_1:
                return VK_SAMPLE_COUNT_1_BIT;
            case TextureSampleCount::SAMPLE_COUNT_2:
                return VK_SAMPLE_COUNT_2_BIT;
            case TextureSampleCount::SAMPLE_COUNT_4:
                return VK_SAMPLE_COUNT_4_BIT;
            case TextureSampleCount::SAMPLE_COUNT_8:
                return VK_SAMPLE_COUNT_8_BIT;
            case TextureSampleCount::SAMPLE_COUNT_16:
                return VK_SAMPLE_COUNT_16_BIT;
            case TextureSampleCount::SAMPLE_COUNT_32:
                return VK_SAMPLE_COUNT_32_BIT;
            case TextureSampleCount::SAMPLE_COUNT_64:
                return VK_SAMPLE_COUNT_64_BIT;
            default:
                return VK_SAMPLE_COUNT_1_BIT;
            }
        }
        FORCEINLINE static VkImageViewType GetImageViewType(const TextureType type) noexcept
        {
            switch (type)
            {
            case TextureType::Texture1D:
                return VK_IMAGE_VIEW_TYPE_1D;
            case TextureType::Texture2D:
                return VK_IMAGE_VIEW_TYPE_2D;
            case TextureType::Texture3D:
                return VK_IMAGE_VIEW_TYPE_3D;
            default:
                return VK_IMAGE_VIEW_TYPE_1D;
            }
        }
        FORCEINLINE static VkImageLayout GetImageLayout(const TextureMemoryLayout memoryLayout) noexcept
        {
            switch (memoryLayout)
            {
            case TextureMemoryLayout::Unknown:
                return VK_IMAGE_LAYOUT_UNDEFINED;
            case TextureMemoryLayout::General:
                return VK_IMAGE_LAYOUT_GENERAL;
            case TextureMemoryLayout::ColorAttachment:
                return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case TextureMemoryLayout::DepthStencilAttachment:
                return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            case TextureMemoryLayout::ShaderReadOnly:
                return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case TextureMemoryLayout::TransferSource:
                return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            case TextureMemoryLayout::TransferDestination:
                return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            case TextureMemoryLayout::DepthAttachment:
                return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case TextureMemoryLayout::Present:
                return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            default:
                return VK_IMAGE_LAYOUT_UNDEFINED;
            }
        }
        FORCEINLINE static VkImageAspectFlags GetImageAspects(const TextureAspectFlags aspects) noexcept
        {
            VkImageAspectFlags flags = VkImageAspectFlags();
            
            if (aspects & TextureAspectFlags::Color)
                flags |= VK_IMAGE_ASPECT_COLOR_BIT;
            if (aspects & TextureAspectFlags::Depth)
                flags |= VK_IMAGE_ASPECT_DEPTH_BIT;
            if (aspects & TextureAspectFlags::Stencil)
                flags |= VK_IMAGE_ASPECT_STENCIL_BIT;

            return flags;
        }
    public:
        VulkanTextureUtils() = delete;
        ~VulkanTextureUtils() = delete;
    };
}

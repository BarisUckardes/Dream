#pragma once
#include <Runtime/Graphics/Buffer/GraphicsBufferUsage.h>
#include <vulkan.h>

namespace Dream
{
    class RUNTIME_API VulkanBufferUtils
    {
    public:
        FORCEINLINE static VkBufferUsageFlags GetUsages(const GraphicsBufferUsage usages)
        {
            VkBufferUsageFlags flags = VkBufferUsageFlags();

            if (usages & GraphicsBufferUsage::VertexBuffer)
                flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            else if (usages & GraphicsBufferUsage::IndexBuffer)
                flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            else if (usages & GraphicsBufferUsage::ConstantBuffer)
                flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            else if (usages & GraphicsBufferUsage::Storage)
                flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

            if (usages & GraphicsBufferUsage::TransferDestination)
                flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            else if (usages & GraphicsBufferUsage::TransferSource)
                flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

            return flags;
        }

    public:
        VulkanBufferUtils() = delete;
        ~VulkanBufferUtils() = delete;
    };
}

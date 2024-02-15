#pragma once
#include <Runtime/Graphics/Descriptor/DescriptorResourceType.h>
#include <vulkan.h>

namespace Dream
{
    class RUNTIME_API VulkanDescriptorUtils
    {
    public:
        static VkDescriptorType GetDescriptorType(const DescriptorResourceType type);
    public:
        VulkanDescriptorUtils() = delete;
        ~VulkanDescriptorUtils() = delete;
    };
}

#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceFeatures.h>
#include <vulkan.h>

namespace Dream
{
    struct RUNTIME_API VulkanDeviceFeatures : public GraphicsDeviceFeatures
    {
        VkPhysicalDeviceFeatures Features;
    };
}

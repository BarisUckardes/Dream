#include "VulkanDescriptorPool.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorUtils.h>

namespace Dream
{
    VulkanDescriptorPool::VulkanDescriptorPool(const DescriptorPoolDesc& desc, VulkanDevice* pDevice) : DescriptorPool(desc,pDevice),mPool(VK_NULL_HANDLE),mLogicalDevice(pDevice->GetVkLogicalDevice())
    {
        //Create descriptor sizes
        VkDescriptorPoolSize sizes[5];
        for (unsigned char i = 0; i < desc.Sizes.size(); i++)
        {
            const DescriptorPoolSize& size = desc.Sizes[i];

            VkDescriptorPoolSize vkSize = {};
            vkSize.descriptorCount = size.Count;
            vkSize.type = VulkanDescriptorUtils::GetDescriptorType(size.Type);

            sizes[i] = vkSize;
        }

        //Create
        VkDescriptorPoolCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        info.maxSets = desc.SetCount;
        info.poolSizeCount = desc.Sizes.size();
        info.pPoolSizes = sizes;
        info.pNext = nullptr;
        info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        DEV_ASSERT(vkCreateDescriptorPool(mLogicalDevice, &info, nullptr, &mPool) == VK_SUCCESS, "VulkanDescriptorPool", "Failed to allocate descriptor pool!");
    }
    VulkanDescriptorPool::~VulkanDescriptorPool()
    {
        vkDestroyDescriptorPool(mLogicalDevice, mPool, nullptr);
    }
}

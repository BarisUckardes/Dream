#include "VulkanDescriptorSet.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorPool.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorSetLayout.h>

namespace Dream
{
	VulkanDescriptorSet::VulkanDescriptorSet(const DescriptorSetDesc& desc, VulkanDevice* pDevice) : DescriptorSet(desc,pDevice),mSet(VK_NULL_HANDLE),mPool(VK_NULL_HANDLE), mLogicalDevice(pDevice->GetVkLogicalDevice())
	{
		const VulkanDescriptorPool* pVkPool = (const VulkanDescriptorPool*)desc.pPool;
		const VulkanDescriptorSetLayout* pVkLayout = (const VulkanDescriptorSetLayout*)desc.pLayout;

		//Allocate
		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = pVkPool->vk_pool();
		VkDescriptorSetLayout layouts = pVkLayout->vk_layout();
		allocateInfo.pSetLayouts = &layouts;
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pNext = nullptr;

		DEV_ASSERT(vkAllocateDescriptorSets(mLogicalDevice, &allocateInfo, &mSet) == VK_SUCCESS, "VulkanDescriptorSet", "Failed to allocate descriptorset set");

		mPool = pVkPool->vk_pool();
	}
	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		vkFreeDescriptorSets(mLogicalDevice, mPool, 1, &mSet);
	}
}

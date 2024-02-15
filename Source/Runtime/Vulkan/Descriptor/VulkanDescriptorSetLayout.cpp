#include "VulkanDescriptorSetLayout.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorUtils.h>
#include <Runtime/Vulkan/Shader/VulkanShaderUtils.h>

namespace Dream
{
	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const DescriptorSetLayoutDesc& desc, VulkanDevice* pDevice) : DescriptorSetLayout(desc,pDevice),mLayout(VK_NULL_HANDLE),mLogicalDevice(pDevice->GetVkLogicalDevice())
	{
		//Create layout binding
		VkDescriptorSetLayoutBinding vkLayoutBindings[32];
		for (unsigned char i = 0; i < desc.Entries.size(); i++)
		{
			const DescriptorSetLayoutEntry& entry = desc.Entries[i];

			VkDescriptorSetLayoutBinding binding = {};
			binding.binding = entry.Binding;
			binding.descriptorCount = 1;
			binding.descriptorType = VulkanDescriptorUtils::GetDescriptorType(entry.Type);
			binding.pImmutableSamplers = nullptr;
			binding.stageFlags = VulkanShaderUtils::GetShaderFlags(entry.Stages);

			vkLayoutBindings[i] = binding;
		}

		//Create layout
		VkDescriptorSetLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.bindingCount = desc.Entries.size();
		createInfo.pBindings = vkLayoutBindings;
		createInfo.flags = VkDescriptorSetLayoutCreateFlags();
		createInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateDescriptorSetLayout(pDevice->GetVkLogicalDevice(), &createInfo, nullptr, &mLayout) == VK_SUCCESS, "VulkanDescriptorLayout", "Failed to create layout");
	}
	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(mLogicalDevice, mLayout, nullptr);
	}
}

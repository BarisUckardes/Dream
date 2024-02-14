#include "VulkanShader.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>

namespace Dream
{
	VulkanShader::VulkanShader(const ShaderDesc& desc, VulkanDevice* pDevice) : Shader(desc,pDevice),mModule(VK_NULL_HANDLE),mLogicalDevice(pDevice->GetVkLogicalDevice())
	{
		//Create module
		VkShaderModuleCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.pCode = (const uint32_t*)desc.pByteCode;
		info.codeSize = desc.ByteCodeLength;
		info.flags = VkShaderModuleCreateFlags();
	}
	VulkanShader::~VulkanShader()
	{
		vkDestroyShaderModule(mLogicalDevice, mModule, nullptr);
	}
}

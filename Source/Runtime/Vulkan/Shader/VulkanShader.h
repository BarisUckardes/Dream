#pragma once
#include <Runtime/Graphics/Shader/Shader.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanShader : public Shader
	{
	public:
		VulkanShader(const ShaderDesc& desc, VulkanDevice* pDevice);
		~VulkanShader();

		FORCEINLINE VkShaderModule GetVkModule() const noexcept
		{
			return mModule;
		}
	private:
		VkShaderModule mModule;
		VkDevice mLogicalDevice;
	};
}

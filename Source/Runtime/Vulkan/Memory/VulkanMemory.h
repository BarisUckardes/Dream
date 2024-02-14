#pragma once
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <vulkan.h>
namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanMemory : public GraphicsMemory
	{
	public:
		VulkanMemory(const GraphicsMemoryDesc& desc, VulkanDevice* pDevice);
		~VulkanMemory();

		FORCEINLINE VkDeviceMemory GetVkMemory() const noexcept
		{
			return mMemory;
		}
	private:
		VkDeviceMemory mMemory;
		VkDevice mLogicalDevice;
	};
}

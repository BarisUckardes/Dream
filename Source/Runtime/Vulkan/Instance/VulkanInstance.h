#pragma once
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Vulkan/Instance/VulkanInstanceDesc.h>
#include <vulkan.h>

namespace Dream
{
	class RUNTIME_API VulkanInstance : public GraphicsInstance
	{
	public:
		VulkanInstance(const VulkanInstanceDesc* pDesc);
		~VulkanInstance();

		FORCEINLINE VkInstance GetVkInstance() const noexcept
		{
			return mInstance;
		}
	private:
		VkInstance mInstance;
#ifdef DREAM_DEBUG
		VkDebugUtilsMessengerEXT mDebugMessenger;
#endif
	};
}

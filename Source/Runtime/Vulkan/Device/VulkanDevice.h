#pragma once
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Vulkan/Device/VulkanDeviceDesc.h>
#include <vulkan.h>

namespace Dream
{
	class RUNTIME_API VulkanDevice : public GraphicsDevice
	{
        struct DeviceQueueFamily
        {
            DeviceQueueFamily() : FamilyIndex(255), Capacity(0),RequestedCount(0)
            {

            }

			VkQueue OwnQueue()
			{
				if (FreeQueues.size() == 0)
					return VK_NULL_HANDLE;

				VkQueue queue = FreeQueues[0];
				FreeQueues.erase(FreeQueues.begin());
				return queue;
			}

			void ReturnQueue(VkQueue queue)
			{
				FreeQueues.push_back(queue);
			}

            unsigned char FamilyIndex;
            unsigned char Capacity;
			unsigned char RequestedCount;
            std::vector<VkQueue> FreeQueues;
        };
	public:
		VulkanDevice(const VulkanDeviceDesc* pDesc);
		~VulkanDevice();

		VkQueue vkOwnQueue(const GraphicsQueueType type);
		void vkReturnQueue(const GraphicsQueueType type,const VkQueue queue);
		FORCEINLINE VkDevice GetVkLogicalDevice() const noexcept
		{
			return mLogicalDevice;
		}

	private:
		virtual GraphicsBackend GetBackend() const noexcept
		{
			return GraphicsBackend::Vulkan;
		}

		virtual bool HasQueue(const GraphicsQueueType type) const noexcept override;
		virtual GraphicsQueue* CreateQueueCore(const GraphicsQueueDesc& desc) override;
	private:
		VkDevice mLogicalDevice;
        DeviceQueueFamily mGraphicsFamily;
        DeviceQueueFamily mComputeFamily;
        DeviceQueueFamily mTransferFamily;
	};
}

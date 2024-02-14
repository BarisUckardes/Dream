#include "VulkanDevice.h"
#include <Runtime/Vulkan/Adapter/VulkanAdapter.h>
#include <Runtime/Vulkan/Device/VulkanDeviceFeatures.h>
#include <Runtime/Vulkan/Queue/VulkanQueue.h>

namespace Dream
{
	VulkanDevice::VulkanDevice(const VulkanDeviceDesc* pDesc) : GraphicsDevice(pDesc)
	{
		VulkanAdapter* pVkAdapter = (VulkanAdapter*)pDesc->pOwnerAdapter;

		//Get queue family count
		unsigned int familyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(pVkAdapter->GetVkPhysicalDevice(), &familyCount, nullptr);
		DEV_ASSERT(familyCount > 0, "VulkanDevice", "Family count should be higher than 0");

		//Get queue family properties
		std::vector<VkQueueFamilyProperties> families(familyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(pVkAdapter->GetVkPhysicalDevice(), &familyCount, families.data());

		//Get queue families
		for (unsigned int i = 0; i < familyCount; i++)
		{
			const VkQueueFamilyProperties& properties = families[i];

			if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && mGraphicsFamily.FamilyIndex == 255)
			{
				mGraphicsFamily.Capacity = properties.queueCount;
				mGraphicsFamily.RequestedCount = pDesc->GraphicsQueueCount > properties.queueCount ? properties.queueCount : pDesc->GraphicsQueueCount;
				mGraphicsFamily.FamilyIndex = i;
			}
			else if (properties.queueFlags & VK_QUEUE_COMPUTE_BIT && mComputeFamily.FamilyIndex == 255)
			{
				mComputeFamily.Capacity = properties.queueCount;
				mComputeFamily.RequestedCount = pDesc->ComputeQueueCount > properties.queueCount ? properties.queueCount : pDesc->ComputeQueueCount;
				mComputeFamily.FamilyIndex = i;
			}
			else if (properties.queueFlags & VK_QUEUE_TRANSFER_BIT && mTransferFamily.FamilyIndex == 255)
			{
				mTransferFamily.Capacity = properties.queueCount;
				mTransferFamily.RequestedCount = pDesc->TransferQueueCount > properties.queueCount ? properties.queueCount : pDesc->TransferQueueCount;
				mTransferFamily.FamilyIndex = i;
			}
		}

		//Get queue create informations
		constexpr float queuePriorities[] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
		std::vector<VkDeviceQueueCreateInfo> queueCreateInformations;
		{
			VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
			graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			graphicsQueueCreateInfo.queueFamilyIndex = mGraphicsFamily.FamilyIndex;
			graphicsQueueCreateInfo.queueCount = mGraphicsFamily.RequestedCount;
			graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;
			graphicsQueueCreateInfo.pNext = nullptr;

			VkDeviceQueueCreateInfo computeQueueCreateInfo = {};
			computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			computeQueueCreateInfo.queueFamilyIndex = mComputeFamily.FamilyIndex;
			computeQueueCreateInfo.queueCount = mComputeFamily.RequestedCount;
			computeQueueCreateInfo.pQueuePriorities = queuePriorities;
			computeQueueCreateInfo.pNext = nullptr;

			VkDeviceQueueCreateInfo transferQueueCreateInfo = {};
			transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			transferQueueCreateInfo.queueFamilyIndex = mTransferFamily.FamilyIndex;
			transferQueueCreateInfo.queueCount = mTransferFamily.RequestedCount;
			transferQueueCreateInfo.pQueuePriorities = queuePriorities;
			transferQueueCreateInfo.pNext = nullptr;

			if(mGraphicsFamily.RequestedCount > 0)
				queueCreateInformations.push_back(graphicsQueueCreateInfo);
			if(mComputeFamily.RequestedCount > 0)
				queueCreateInformations.push_back(computeQueueCreateInfo);
			if(mTransferFamily.RequestedCount > 0)
				queueCreateInformations.push_back(transferQueueCreateInfo);
		}

		//Make queue validations
		DEV_ASSERT(queueCreateInformations.size() > 0, "VulkanDevice", "No queue requested!");

		//Get logical device extensions
		std::vector<const char*> extensions;
		for (const std::string& extension : pDesc->Extensions)
			extensions.push_back(extension.c_str());

		//Get logical device layers
		std::vector<const char*> layers;
		for (const std::string& layer : pDesc->Layers)
			layers.push_back(layer.c_str());

		//Create logical device
		VkPhysicalDeviceFeatures* pFeatures = nullptr;
		if (pDesc->pRequestedFeatures != nullptr)
			pFeatures = &(((VulkanDeviceFeatures*)pDesc->pRequestedFeatures)->Features);

		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.pNext = nullptr;
		deviceInfo.queueCreateInfoCount = queueCreateInformations.size();
		deviceInfo.pQueueCreateInfos = queueCreateInformations.data();
		deviceInfo.enabledExtensionCount = extensions.size();
		deviceInfo.ppEnabledExtensionNames = extensions.data();
		deviceInfo.enabledLayerCount = layers.size();
		deviceInfo.ppEnabledLayerNames = layers.data();
		deviceInfo.pEnabledFeatures = pFeatures;
		deviceInfo.flags = VkDeviceCreateFlags();

		DEV_ASSERT(vkCreateDevice(pVkAdapter->GetVkPhysicalDevice(), &deviceInfo, nullptr, &mLogicalDevice) == VK_SUCCESS, "VulkanDevice", "Failed to create logical device");

		//Get queues
		mGraphicsFamily.FreeQueues.reserve(mGraphicsFamily.RequestedCount);
		for (unsigned int i = 0; i < mGraphicsFamily.RequestedCount; i++)
		{
			VkQueue queue = VK_NULL_HANDLE;
			vkGetDeviceQueue(mLogicalDevice, mGraphicsFamily.FamilyIndex, i, &queue);
			mGraphicsFamily.FreeQueues.push_back(queue);
		}

		mComputeFamily.FreeQueues.reserve(mComputeFamily.RequestedCount);
		for (unsigned int i = 0; i < mComputeFamily.RequestedCount; i++)
		{
			VkQueue queue = VK_NULL_HANDLE;
			vkGetDeviceQueue(mLogicalDevice, mComputeFamily.FamilyIndex, i, &queue);
			mComputeFamily.FreeQueues.push_back(queue);
		}

		mTransferFamily.FreeQueues.reserve(mTransferFamily.RequestedCount);
		for (unsigned int i = 0; i < mTransferFamily.RequestedCount; i++)
		{
			VkQueue queue = VK_NULL_HANDLE;
			vkGetDeviceQueue(mLogicalDevice, mTransferFamily.FamilyIndex, i, &queue);
			mTransferFamily.FreeQueues.push_back(queue);
		}
	}
	VulkanDevice::~VulkanDevice()
	{
		
	}
	VkQueue VulkanDevice::vkOwnQueue(const GraphicsQueueType type)
	{
		switch (type)
		{
			default:
			case Dream::GraphicsQueueType::Graphics:
				return mGraphicsFamily.OwnQueue();
			case Dream::GraphicsQueueType::Compute:
				return mComputeFamily.OwnQueue();
			case Dream::GraphicsQueueType::Transfer:
				return mTransferFamily.OwnQueue();
		}
	}
	void VulkanDevice::vkReturnQueue(const GraphicsQueueType type, const VkQueue queue)
	{
		switch (type)
		{
			default:
			case Dream::GraphicsQueueType::Graphics:
			{
				mGraphicsFamily.ReturnQueue(queue);
				break;
			}
			case Dream::GraphicsQueueType::Compute:
			{
				mComputeFamily.ReturnQueue(queue);
				break;
			}
			case Dream::GraphicsQueueType::Transfer:
			{
				mTransferFamily.ReturnQueue(queue);
				break;
			}
		}
	}
	bool VulkanDevice::HasQueue(const GraphicsQueueType type) const noexcept
	{
		switch (type)
		{
			case Dream::GraphicsQueueType::Graphics:
				return mGraphicsFamily.FreeQueues.size() > 0;
			case Dream::GraphicsQueueType::Compute:
				return mComputeFamily.FreeQueues.size() > 0;
			case Dream::GraphicsQueueType::Transfer:
				return mTransferFamily.FreeQueues.size() > 0;
			default:
				return false;
		}
	}
	GraphicsQueue* VulkanDevice::CreateQueueCore(const GraphicsQueueDesc& desc)
	{
		return new VulkanQueue(desc,this);
	}
}

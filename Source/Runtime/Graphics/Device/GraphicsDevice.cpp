#include "GraphicsDevice.h"
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>

namespace Dream
{
	int GraphicsDevice::GetPresentQueueFamily(const VkSurfaceKHR surface) const noexcept
	{
		//Check graphics
		VkBool32 bGraphicsCanPresent = false;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(mOwnerAdapter->GetPhysicalDevice(), mGraphicsQueueFamily.FamilyIndex, surface, &bGraphicsCanPresent) == VK_SUCCESS, "VulkanDevice", "Failed to check surface support");
		if (bGraphicsCanPresent)
			return mGraphicsQueueFamily.FamilyIndex;

		//Check compute
		VkBool32 bComputeCanPresent = false;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(mOwnerAdapter->GetPhysicalDevice(), mComputeQueueFamily.FamilyIndex, surface, &bComputeCanPresent) == VK_SUCCESS, "VulkanDevice", "Failed to check surface support");
		if (bComputeCanPresent)
			return mComputeQueueFamily.FamilyIndex;

		//Check transfer
		VkBool32 bTransferCanPresent = false;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(mOwnerAdapter->GetPhysicalDevice(), mTransferQueueFamily.FamilyIndex, surface, &bTransferCanPresent) == VK_SUCCESS, "VulkanDevice", "Failed to check surface support");
		if (bTransferCanPresent)
			return mTransferQueueFamily.FamilyIndex;

		return -1;
	}
	VkQueue GraphicsDevice::GetPresentQueue(const VkSurfaceKHR surface) const noexcept
	{
		//Check graphics
		VkBool32 bGraphicsCanPresent = false;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(mOwnerAdapter->GetPhysicalDevice(), mGraphicsQueueFamily.FamilyIndex, surface, &bGraphicsCanPresent) == VK_SUCCESS, "VulkanDevice", "Failed to check surface support");
		if (bGraphicsCanPresent)
			return mGraphicsQueueFamily.DefaultQueue;

		//Check compute
		VkBool32 bComputeCanPresent = false;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(mOwnerAdapter->GetPhysicalDevice(), mComputeQueueFamily.FamilyIndex, surface, &bComputeCanPresent) == VK_SUCCESS, "VulkanDevice", "Failed to check surface support");
		if (bComputeCanPresent)
			return mComputeQueueFamily.DefaultQueue;

		//Check transfer
		VkBool32 bTransferCanPresent = false;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(mOwnerAdapter->GetPhysicalDevice(), mTransferQueueFamily.FamilyIndex, surface, &bTransferCanPresent) == VK_SUCCESS, "VulkanDevice", "Failed to check surface support");
		if (bTransferCanPresent)
			return mTransferQueueFamily.DefaultQueue;

		return VK_NULL_HANDLE;
	}
	GraphicsMemoryHeap* GraphicsDevice::AllocateMemory(const GraphicsMemoryHeapDesc& desc)
	{
		GraphicsMemoryHeap* pHeap = new GraphicsMemoryHeap(desc,this);
		RegisterChildObject(pHeap);
		return pHeap;
	}
	GraphicsBuffer* GraphicsDevice::CreateBuffer(const GraphicsBufferDesc& desc)
	{
		GraphicsBuffer* pBuffer = new GraphicsBuffer(desc, this);
		RegisterChildObject(pBuffer);
		return pBuffer;
	}
	GraphicsDevice::~GraphicsDevice()
	{
		mOwnerAdapter->_OnDeviceDeleted(this);
		vkDestroyDevice(mLogicalDevice, nullptr);
	}
	GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc,GraphicsAdapter* pAdapter) : mOwnerAdapter(pAdapter)
	{
		//Get queue family count
		unsigned int queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(pAdapter->GetPhysicalDevice(), &queueFamilyCount, nullptr);
		DEV_ASSERT(queueFamilyCount > 0, "VulkanDevice", "No queue families found!");

		//Get queue families
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(pAdapter->GetPhysicalDevice(), &queueFamilyCount, queueFamilies.data());

		//Collect queue families
		{
			unsigned char familyIndex = 0;
			for (const VkQueueFamilyProperties& properties : queueFamilies)
			{
				if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && mGraphicsQueueFamily.FamilyIndex == 255)
				{
					mGraphicsQueueFamily.QueueCapacity = properties.queueCount;
					mGraphicsQueueFamily.FamilyIndex = familyIndex;
				}
				else if (properties.queueFlags & VK_QUEUE_COMPUTE_BIT && mComputeQueueFamily.FamilyIndex == 255)
				{
					mComputeQueueFamily.QueueCapacity = properties.queueCount;
					mComputeQueueFamily.FamilyIndex = familyIndex;
				}
				else if (properties.queueFlags & VK_QUEUE_TRANSFER_BIT && mTransferQueueFamily.FamilyIndex == 255)
				{
					mTransferQueueFamily.QueueCapacity = properties.queueCount;
					mTransferQueueFamily.FamilyIndex = familyIndex;
				}
				familyIndex++;
			}
		}

		//Get queue create informations
		constexpr float queuePriorities[] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
		std::vector<VkDeviceQueueCreateInfo> queueCreateInformations;
		{
			VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
			graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			graphicsQueueCreateInfo.queueFamilyIndex = mGraphicsQueueFamily.FamilyIndex;
			graphicsQueueCreateInfo.queueCount = mGraphicsQueueFamily.QueueCapacity;
			graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;
			graphicsQueueCreateInfo.pNext = nullptr;

			VkDeviceQueueCreateInfo computeQueueCreateInfo = {};
			computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			computeQueueCreateInfo.queueFamilyIndex = mComputeQueueFamily.FamilyIndex;
			computeQueueCreateInfo.queueCount = mComputeQueueFamily.QueueCapacity;
			computeQueueCreateInfo.pQueuePriorities = queuePriorities;
			computeQueueCreateInfo.pNext = nullptr;

			VkDeviceQueueCreateInfo transferQueueCreateInfo = {};
			transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			transferQueueCreateInfo.queueFamilyIndex = mTransferQueueFamily.FamilyIndex;
			transferQueueCreateInfo.queueCount = mTransferQueueFamily.QueueCapacity;
			transferQueueCreateInfo.pQueuePriorities = queuePriorities;
			transferQueueCreateInfo.pNext = nullptr;

			if(mGraphicsQueueFamily.FamilyIndex != 255)
				queueCreateInformations.push_back(graphicsQueueCreateInfo);
			if(mComputeQueueFamily.FamilyIndex != 255)
				queueCreateInformations.push_back(computeQueueCreateInfo);
			if(mTransferQueueFamily.FamilyIndex != 255)
				queueCreateInformations.push_back(transferQueueCreateInfo);
		}

		//Get logical device extensions
		std::vector<const char*> logicalDeviceExtensions;
		for (const std::string& extension : desc.Extensions)
			logicalDeviceExtensions.push_back(extension.c_str());

		//Create logical device
		VkDeviceCreateInfo logicalDeviceInfo = {};
		logicalDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		logicalDeviceInfo.pNext = nullptr;
		logicalDeviceInfo.queueCreateInfoCount = queueCreateInformations.size();
		logicalDeviceInfo.pQueueCreateInfos = queueCreateInformations.data();
		logicalDeviceInfo.enabledExtensionCount = logicalDeviceExtensions.size();
		logicalDeviceInfo.ppEnabledExtensionNames = logicalDeviceExtensions.data();
		logicalDeviceInfo.enabledLayerCount = 0;
		logicalDeviceInfo.ppEnabledLayerNames = nullptr;

		DEV_ASSERT(vkCreateDevice(pAdapter->GetPhysicalDevice(), &logicalDeviceInfo, nullptr, &mLogicalDevice) == VK_SUCCESS, "VulkanDevice", "Failed to create logical device");

		//Get default queues
		if (mGraphicsQueueFamily.FamilyIndex != 255)
			vkGetDeviceQueue(mLogicalDevice, mGraphicsQueueFamily.FamilyIndex, 0, &mGraphicsQueueFamily.DefaultQueue);
		if (mComputeQueueFamily.FamilyIndex != 255)
			vkGetDeviceQueue(mLogicalDevice, mComputeQueueFamily.FamilyIndex, 0, &mComputeQueueFamily.DefaultQueue);
		if (mTransferQueueFamily.FamilyIndex != 255)
			vkGetDeviceQueue(mLogicalDevice, mTransferQueueFamily.FamilyIndex, 0, &mTransferQueueFamily.DefaultQueue);
	}
	void GraphicsDevice::RegisterChildObject(GraphicsDeviceObject* pObject)
	{
		mChildObjects.push_back(pObject);
	}
	void GraphicsDevice::RemoveChildObject(GraphicsDeviceObject* pObject)
	{
		const auto findIterator = std::find(mChildObjects.begin(), mChildObjects.end(), pObject);
		if (findIterator == mChildObjects.end())
			return;

		mChildObjects.erase(findIterator);
	}
	void GraphicsDevice::_OnChildObjectDeleted(GraphicsDeviceObject* pObject)
	{
		RemoveChildObject(pObject);
	}
}

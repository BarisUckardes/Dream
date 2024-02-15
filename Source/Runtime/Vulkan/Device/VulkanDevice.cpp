#include "VulkanDevice.h"
#include <Runtime/Vulkan/Adapter/VulkanAdapter.h>
#include <Runtime/Vulkan/Device/VulkanDeviceFeatures.h>
#include <Runtime/Vulkan/Queue/VulkanQueue.h>
#include <Runtime/Vulkan/Buffer/VulkanBuffer.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorPool.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorSetLayout.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorSet.h>
#include <Runtime/Vulkan/Memory/VulkanMemory.h>
#include <Runtime/Vulkan/Fence/VulkanFence.h>
#include <Runtime/Vulkan/Sampler/VulkanSampler.h>
#include <Runtime/Vulkan/Shader/VulkanShader.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>
#include <Runtime/Vulkan/Texture/VulkanTextureView.h>
#include <Runtime/Vulkan/Swapchain/VulkanSwapchain.h>
#include <Runtime/Vulkan/Pipeline/VulkanPipeline.h>
#include <Runtime/Vulkan/RenderPass/VulkanRenderPass.h>
#include <Runtime/Vulkan/Command/VulkanCommandPool.h>
#include <Runtime/Vulkan/Command/VulkanCommandList.h>

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
	unsigned char VulkanDevice::vkGetQueueFamilyIndex(const GraphicsQueueType type) const noexcept
	{
		switch (type)
		{
			case Dream::GraphicsQueueType::Graphics:
				return mGraphicsFamily.FamilyIndex;
			case Dream::GraphicsQueueType::Compute:
				return mComputeFamily.FamilyIndex;
			case Dream::GraphicsQueueType::Transfer:
				return mTransferFamily.FamilyIndex;
			default:
				return 255;
		}
	}
	Texture* VulkanDevice::vkCreateSwapchainTexture(const TextureDesc& desc, const VkImage image)
	{
		Texture* pTexture = new VulkanTexture(desc, image, this);
		RegisterObject(pTexture);
		return pTexture;
	}
	TextureView* VulkanDevice::vkCreateSwapchainTextureView(const TextureViewDesc& desc, const VkImageView view)
	{
		TextureView* pView = new VulkanTextureView(desc, view, this);
		RegisterObject(pView);
		return pView;
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
		const VkQueue queue = vkOwnQueue(desc.Type);
		const unsigned char familyIndex = vkGetQueueFamilyIndex(desc.Type);
		DEV_ASSERT(familyIndex != 255, "VulkanDevice", "Requested invalid queue family index!");

		return new VulkanQueue(desc,queue,familyIndex,this);
	}
	GraphicsBuffer* VulkanDevice::CreateBufferCore(const GraphicsBufferDesc& desc)
	{
		return new VulkanBuffer(desc,this);
	}
	DescriptorSet* VulkanDevice::CreateDescriptorSetCore(const DescriptorSetDesc& desc)
	{
		return new VulkanDescriptorSet(desc,this);
	}
	DescriptorPool* VulkanDevice::CreateDescriptorPoolCore(const DescriptorPoolDesc& desc)
	{
		return new VulkanDescriptorPool(desc,this);
	}
	DescriptorSetLayout* VulkanDevice::CreateDescriptorSetLayoutCore(const DescriptorSetLayoutDesc& desc)
	{
		return new VulkanDescriptorSetLayout(desc,this);
	}
	Fence* VulkanDevice::CreateFenceCore(const FenceDesc& desc)
	{
		return new VulkanFence(desc,this);
	}
	GraphicsMemory* VulkanDevice::AllocateMemoryCore(const GraphicsMemoryDesc& desc)
	{
		return new VulkanMemory(desc,this);
	}
	Sampler* VulkanDevice::CreateSamplerCore(const SamplerDesc& desc)
	{
		return new VulkanSampler(desc,this);
	}
	Shader* VulkanDevice::CreateShaderCore(const ShaderDesc& desc)
	{
		return new VulkanShader(desc,this);
	}
	Texture* VulkanDevice::CreateTextureCore(const TextureDesc& desc)
	{
		return new VulkanTexture(desc,this);
	}
	TextureView* VulkanDevice::CreateTextureViewCore(const TextureViewDesc& desc)
	{
		return new VulkanTextureView(desc,this);
	}
	Swapchain* VulkanDevice::CreateSwapchainCore(const SwapchainDesc& desc)
	{
		return new VulkanSwapchain(desc,this);
	}
	void VulkanDevice::SubmitCommandsCore(const CommandList** ppCmdLists, const unsigned char count, const GraphicsQueue* pTargetQueue, const Fence* pFence)
	{
		const VulkanFence* pVkFence = (const VulkanFence*)pFence;
		const VulkanQueue* pVkQueue = (const VulkanQueue*)pTargetQueue;

		VkCommandBuffer vkCmdBuffers[255];
		for (unsigned int cmdListIndex = 0; cmdListIndex < count; cmdListIndex++)
		{
			const VulkanCommandList* pCmdList = (const VulkanCommandList*)ppCmdLists[cmdListIndex];
			vkCmdBuffers[cmdListIndex] = pCmdList->GetVkCmdBuffer();
		}

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = nullptr;
		submitInfo.waitSemaphoreCount = 0;
		submitInfo.pWaitSemaphores = nullptr;
		submitInfo.pCommandBuffers = vkCmdBuffers;
		submitInfo.commandBufferCount = count;

		DEV_ASSERT(vkQueueSubmit(pVkQueue->GetVkQueue(), count, &submitInfo, pVkFence != nullptr ? pVkFence->GetVkFence() :VK_NULL_HANDLE) == VK_SUCCESS, "VulkanDevice", "Failed to submit the command lists");
	}
	void VulkanDevice::ResetFencesCore(Fence** ppFences, const unsigned int count)
	{
		VkFence fences[512];
		for (unsigned int i = 0; i < count; i++)
		{
			const VulkanFence* pFence = (const VulkanFence*)ppFences[i];
			fences[i] = pFence->GetVkFence();
		}

		DEV_ASSERT(vkResetFences(mLogicalDevice, count, fences) == VK_SUCCESS, "VulkanDevice", "Failed to reset the fences");
	}
	void VulkanDevice::WaitFencesCore(Fence** ppFences, const unsigned int count)
	{
		VkFence fences[512];
		for (unsigned int i = 0; i < count; i++)
		{
			const VulkanFence* pFence = (const VulkanFence*)ppFences[i];
			fences[i] = pFence->GetVkFence();
		}

		DEV_ASSERT(vkWaitForFences(mLogicalDevice, count, fences, VK_TRUE, UINT64_MAX) == VK_SUCCESS, "VulkanDevice", "Failed to wait for fences");
	}
	void VulkanDevice::WaitDeviceIdleCore()
	{
		DEV_ASSERT(vkDeviceWaitIdle(mLogicalDevice) == VK_SUCCESS, "VulkanDevice", "Failed to wait device idle");
	}
	void VulkanDevice::WaitQueueIdleCore(GraphicsQueue* pQueue)
	{
		const VulkanQueue* pVkQueue = (const VulkanQueue*)pQueue;

		DEV_ASSERT(vkQueueWaitIdle(pVkQueue->GetVkQueue()) == VK_SUCCESS, "VulkanDevice", "Failed to wait for queue");
	}
	CommandPool* VulkanDevice::CreateCommandPoolCore(const CommandPoolDesc& desc)
	{
		return new VulkanCommandPool(desc,this);
	}
	CommandList* VulkanDevice::CreateCommandListCore(const CommandListDesc& desc)
	{
		return new VulkanCommandList(desc,this);
	}
	Pipeline* VulkanDevice::CreateGraphicsPipelineCore(const GraphicsPipelineDesc& desc)
	{
		return new VulkanPipeline(desc,this);
	}
	Pipeline* VulkanDevice::CreateComputePipelineCore(const ComputePipelineDesc& desc)
	{
		return new VulkanPipeline(desc,this);
	}
	RenderPass* VulkanDevice::CreateRenderPassCore(const RenderPassDesc& desc)
	{
		return new VulkanRenderPass(desc,this);
	}
}

#include "VulkanSwapchain.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/Adapter/VulkanAdapter.h>

#ifdef DREAM_PLATFORM_WINDOWS
#include <Runtime/Windows/Win32Window.h>
#include <Windows.h>
#include <vulkan_win32.h>
#endif
#include <Runtime/Vulkan/Instance/VulkanInstance.h>
#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include "VulkanSwapchainUtils.h"
#include <algorithm>
#include <Runtime/Vulkan/Queue/VulkanQueue.h>
#include <Runtime/Vulkan/Fence/VulkanFence.h>
#include <Runtime/Vulkan/Semaphore/VulkanSemaphore.h>

namespace Dream
{
	VulkanSwapchain::VulkanSwapchain(const SwapchainDesc& desc, VulkanDevice* pDevice) : Swapchain(desc,pDevice),mLogicalDevice(pDevice->GetVkLogicalDevice()),mPhysicalDevice(VK_NULL_HANDLE),mSwapchain(VK_NULL_HANDLE),mSurface(VK_NULL_HANDLE)
	{
		//Get and set some pre data
		const VulkanInstance* pInstance = (const VulkanInstance*)pDevice->GetAdapter()->GetOwnerInstance();
		const VulkanAdapter* pAdapter = (const VulkanAdapter*)pDevice->GetAdapter();
		mPhysicalDevice = pAdapter->GetVkPhysicalDevice();
		mInstance = pInstance->GetVkInstance();

#ifdef DREAM_PLATFORM_WINDOWS
		const Win32Window* pWindow = (const Win32Window*)desc.pWindow;
		
		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.hinstance = GetModuleHandle(NULL);
		surfaceInfo.hwnd = pWindow->GetWin32WindowHandle();
		surfaceInfo.pNext = nullptr;
		surfaceInfo.flags = VkWin32SurfaceCreateFlagsKHR();

		DEV_ASSERT(vkCreateWin32SurfaceKHR(pInstance->GetVkInstance(), &surfaceInfo, nullptr, &mSurface) == VK_SUCCESS, "VulkanSwapchain", "Failed to create surface");
#endif

		//Get surface capabilities
		VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pAdapter->GetVkPhysicalDevice(), mSurface, &surfaceCapabilities) == VK_SUCCESS, "VulkanSwapchain", "Failed to get surface capabilities");
		DEV_ASSERT(surfaceCapabilities.maxImageCount >= desc.BufferCount, "VulkanSwapchain", "This surface only supports %d buffers whereas requested amount is %d",surfaceCapabilities.maxImageCount,desc.BufferCount);

		//Get supported formats
		unsigned int supportedFormatCount = 0;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(pAdapter->GetVkPhysicalDevice(), mSurface, &supportedFormatCount, nullptr) == VK_SUCCESS, "VulkanSwapchain", "Failed to get supported format count");
		DEV_ASSERT(supportedFormatCount > 0, "VulkanSwapchain", "No swapchain format is supported!");

		std::vector<VkSurfaceFormatKHR> supportedFormats(supportedFormatCount);
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(pAdapter->GetVkPhysicalDevice(), mSurface, &supportedFormatCount, supportedFormats.data()) == VK_SUCCESS, "VulkanSwapchain", "Failed to get supported formats!");

		//Get color format
		const VkFormat requestedFormat = VulkanTextureUtils::GetTextureFormat(desc.ColorFormat);

		bool bRequiredFormatSupported = false;
		for (const VkSurfaceFormatKHR& format : supportedFormats)
		{
			if (format.format == requestedFormat && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				bRequiredFormatSupported = true;
				break;
			}
		}
		DEV_ASSERT(bRequiredFormatSupported, "VulkanSwapchain", "Requested format is not supported!");

		//Get present modes
		unsigned int supportedPresentModeCount = 0;
		DEV_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(pAdapter->GetVkPhysicalDevice(), mSurface, &supportedPresentModeCount, nullptr) == VK_SUCCESS, "VulkanSwapchain", "Failed to get supported present mode count");
		DEV_ASSERT(supportedPresentModeCount > 0, "VulkanSwapchain", "No present mode is supported");

		std::vector<VkPresentModeKHR> supportedPresentModes(supportedPresentModeCount);
		DEV_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(pAdapter->GetVkPhysicalDevice(), mSurface, &supportedPresentModeCount, supportedPresentModes.data()) == VK_SUCCESS, "VulkanSwapchain", "Failed to get supported present modes");

		//Get present mode
		const VkPresentModeKHR requestedPresentMode = VulkanSwapchainUtils::GetPresentMode(desc.Mode);

		bool bRequiredPresentModeSupported = false;
		for (const VkPresentModeKHR mode : supportedPresentModes)
		{
			if (mode == requestedPresentMode)
			{
				bRequiredFormatSupported = true;
				break;
			}
		}
		DEV_ASSERT(bRequiredFormatSupported, "VulkanSwapchain", "Requsted present mode is not supported!");

		//Select extent
		const VkExtent2D selectedExtent =
		{
			std::clamp(pWindow->GetWidth(),surfaceCapabilities.minImageExtent.width,surfaceCapabilities.maxImageExtent.width),
			std::clamp(pWindow->GetHeight(),surfaceCapabilities.minImageExtent.height,surfaceCapabilities.maxImageExtent.height)
		};

		//Get present family index
		const VulkanQueue* pQueue = (const VulkanQueue*)desc.pQueue;
		const unsigned int presentFamilyIndex = pQueue->GetVkFamilyIndex();

		VkBool32 bFamilyIndexSupportsPresentation = false;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(pAdapter->GetVkPhysicalDevice(), presentFamilyIndex, mSurface, &bFamilyIndexSupportsPresentation) == VK_SUCCESS, "VulkanSwapchain", "Failed to query for queue family presentation support");
		DEV_ASSERT(bFamilyIndexSupportsPresentation, "VulkanSwapchain", "Given queue does not support presentation");

		//Create swapchain
		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = mSurface;
		swapchainInfo.imageFormat = requestedFormat;
		swapchainInfo.minImageCount = desc.BufferCount;
		swapchainInfo.imageExtent = selectedExtent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
		swapchainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainInfo.presentMode = requestedPresentMode;
		swapchainInfo.clipped = VK_FALSE;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

		swapchainInfo.pQueueFamilyIndices = &presentFamilyIndex;
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.flags = VkSwapchainCreateFlagsKHR();
		swapchainInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateSwapchainKHR(mLogicalDevice, &swapchainInfo, nullptr, &mSwapchain) == VK_SUCCESS, "VulkanSwapchain", "Failed to create swapchain");

		//Set custom size for the underlying swapchain
		SetCustomSize(selectedExtent.width, selectedExtent.height);

		//Get swapchain images
		unsigned int swapchainImageCount = 0;
		DEV_ASSERT(vkGetSwapchainImagesKHR(mLogicalDevice, mSwapchain, &swapchainImageCount, nullptr) == VK_SUCCESS, "VulkanSwapchain", "Failed to get swapchain image count");
		DEV_ASSERT(swapchainImageCount == desc.BufferCount, "VulkanSwapchain", "Swapchain created with %d color buffers, but the requested amount was %s", swapchainImageCount, desc.BufferCount);

		std::vector<VkImage> swapchainImages(swapchainImageCount);
		DEV_ASSERT(vkGetSwapchainImagesKHR(mLogicalDevice, mSwapchain, &swapchainImageCount, swapchainImages.data()) == VK_SUCCESS, "VulkanSwapchain", "Failed to get swapchain images");

		//Create texture and texture views
		std::vector<Texture*> textures;
		for (const VkImage image : swapchainImages)
		{
			//Create texture
			TextureDesc textureDesc = {};
			textureDesc.Type = TextureType::Texture2D;
			textureDesc.Format = desc.ColorFormat;
			textureDesc.Usages = TextureUsage::ColorAttachment;
			textureDesc.Width = selectedExtent.width;
			textureDesc.Height = selectedExtent.height;
			textureDesc.Depth = 1;
			textureDesc.MipLevels = 1;
			textureDesc.ArrayLevels = 1;
			textureDesc.SampleCount = TextureSampleCount::SAMPLE_COUNT_1;
			textureDesc.pMemory = nullptr;

			Texture* pTexture = pDevice->vkCreateSwapchainTexture(textureDesc, image);

			textures.push_back(pTexture);
		}

		//Create depth stencil texture

		SetCustomSwapchainTextures(textures);
	}
	VulkanSwapchain::~VulkanSwapchain()
	{
		Delete();
	}
	void VulkanSwapchain::ResizeCore(const unsigned int width, const unsigned int height)
	{
		//First delete
		Delete();

		//Get and set some pre data
		const VulkanInstance* pInstance = (const VulkanInstance*)GetDevice()->GetAdapter()->GetOwnerInstance();
		const VulkanAdapter* pAdapter = (const VulkanAdapter*)GetDevice()->GetAdapter();
		VulkanDevice* pDevice = (VulkanDevice*)GetDevice();
		mPhysicalDevice = pAdapter->GetVkPhysicalDevice();
		mInstance = pInstance->GetVkInstance();

#ifdef DREAM_PLATFORM_WINDOWS
		const Win32Window* pWindow = (const Win32Window*)GetWindow();

		VkSurfaceFullScreenExclusiveInfoEXT fullscreenExclusiveInfo = {};
		fullscreenExclusiveInfo.fullScreenExclusive = VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT;
		fullscreenExclusiveInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;
		fullscreenExclusiveInfo.pNext = nullptr;

		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.hinstance = GetModuleHandle(NULL);
		surfaceInfo.hwnd = pWindow->GetWin32WindowHandle();
		surfaceInfo.flags = VkWin32SurfaceCreateFlagsKHR();
		surfaceInfo.pNext = &fullscreenExclusiveInfo;

		DEV_ASSERT(vkCreateWin32SurfaceKHR(pInstance->GetVkInstance(), &surfaceInfo, nullptr, &mSurface) == VK_SUCCESS, "VulkanSwapchain", "Failed to create surface");
#endif

		//Get surface capabilities
		VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pAdapter->GetVkPhysicalDevice(), mSurface, &surfaceCapabilities) == VK_SUCCESS, "VulkanSwapchain", "Failed to get surface capabilities");
		DEV_ASSERT(surfaceCapabilities.maxImageCount >= GetBufferCount(), "VulkanSwapchain", "This surface only supports %d buffers whereas requested amount is %d", surfaceCapabilities.maxImageCount, GetBufferCount());

		//Get supported formats
		unsigned int supportedFormatCount = 0;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(pAdapter->GetVkPhysicalDevice(), mSurface, &supportedFormatCount, nullptr) == VK_SUCCESS, "VulkanSwapchain", "Failed to get supported format count");
		DEV_ASSERT(supportedFormatCount > 0, "VulkanSwapchain", "No swapchain format is supported!");

		std::vector<VkSurfaceFormatKHR> supportedFormats(supportedFormatCount);
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(pAdapter->GetVkPhysicalDevice(), mSurface, &supportedFormatCount, supportedFormats.data()) == VK_SUCCESS, "VulkanSwapchain", "Failed to get supported formats!");

		//Get color format
		const VkFormat requestedFormat = VulkanTextureUtils::GetTextureFormat(GetColorBufferFormat());

		bool bRequiredFormatSupported = false;
		for (const VkSurfaceFormatKHR& format : supportedFormats)
		{
			if (format.format == requestedFormat && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				bRequiredFormatSupported = true;
				break;
			}
		}
		DEV_ASSERT(bRequiredFormatSupported, "VulkanSwapchain", "Requested format is not supported!");

		//Get present modes
		unsigned int supportedPresentModeCount = 0;
		DEV_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(pAdapter->GetVkPhysicalDevice(), mSurface, &supportedPresentModeCount, nullptr) == VK_SUCCESS, "VulkanSwapchain", "Failed to get supported present mode count");
		DEV_ASSERT(supportedPresentModeCount > 0, "VulkanSwapchain", "No present mode is supported");

		std::vector<VkPresentModeKHR> supportedPresentModes(supportedPresentModeCount);
		DEV_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(pAdapter->GetVkPhysicalDevice(), mSurface, &supportedPresentModeCount, supportedPresentModes.data()) == VK_SUCCESS, "VulkanSwapchain", "Failed to get supported present modes");

		//Get present mode
		const VkPresentModeKHR requestedPresentMode = VulkanSwapchainUtils::GetPresentMode(GetMode());

		bool bRequiredPresentModeSupported = false;
		for (const VkPresentModeKHR mode : supportedPresentModes)
		{
			if (mode == requestedPresentMode)
			{
				bRequiredFormatSupported = true;
				break;
			}
		}
		DEV_ASSERT(bRequiredFormatSupported, "VulkanSwapchain", "Requsted present mode is not supported!");

		//Select extent
		const VkExtent2D selectedExtent =
		{
			std::clamp(width,surfaceCapabilities.minImageExtent.width,surfaceCapabilities.maxImageExtent.width),
			std::clamp(height,surfaceCapabilities.minImageExtent.height,surfaceCapabilities.maxImageExtent.height)
		};

		//Get present family index
		const VulkanQueue* pQueue = (const VulkanQueue*)GetQueue();
		const unsigned int presentFamilyIndex = pQueue->GetVkFamilyIndex();

		VkBool32 bFamilyIndexSupportsPresentation = false;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(pAdapter->GetVkPhysicalDevice(), presentFamilyIndex, mSurface, &bFamilyIndexSupportsPresentation) == VK_SUCCESS, "VulkanSwapchain", "Failed to query for queue family presentation support");
		DEV_ASSERT(bFamilyIndexSupportsPresentation, "VulkanSwapchain", "Given queue does not support presentation");

		//Create swapchain
		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = mSurface;
		swapchainInfo.imageFormat = requestedFormat;
		swapchainInfo.minImageCount = GetBufferCount();
		swapchainInfo.imageExtent = selectedExtent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
		swapchainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainInfo.presentMode = requestedPresentMode;
		swapchainInfo.clipped = VK_FALSE;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

		swapchainInfo.pQueueFamilyIndices = &presentFamilyIndex;
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.flags = VkSwapchainCreateFlagsKHR();
		swapchainInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateSwapchainKHR(mLogicalDevice, &swapchainInfo, nullptr, &mSwapchain) == VK_SUCCESS, "VulkanSwapchain", "Failed to create swapchain");

		//Set custom size for the underlying swapchain
		SetCustomSize(selectedExtent.width, selectedExtent.height);

		//Get swapchain images
		unsigned int swapchainImageCount = 0;
		DEV_ASSERT(vkGetSwapchainImagesKHR(mLogicalDevice, mSwapchain, &swapchainImageCount, nullptr) == VK_SUCCESS, "VulkanSwapchain", "Failed to get swapchain image count");
		DEV_ASSERT(swapchainImageCount == GetBufferCount(), "VulkanSwapchain", "Swapchain created with %d color buffers, but the requested amount was %s", swapchainImageCount, GetBufferCount());

		std::vector<VkImage> swapchainImages(swapchainImageCount);
		DEV_ASSERT(vkGetSwapchainImagesKHR(mLogicalDevice, mSwapchain, &swapchainImageCount, swapchainImages.data()) == VK_SUCCESS, "VulkanSwapchain", "Failed to get swapchain images");

		//Create texture and texture views
		std::vector<Texture*> textures;
		for (const VkImage image : swapchainImages)
		{
			//Create texture
			TextureDesc textureDesc = {};
			textureDesc.Type = TextureType::Texture2D;
			textureDesc.Format = GetColorBufferFormat();
			textureDesc.Usages = TextureUsage::ColorAttachment;
			textureDesc.Width = selectedExtent.width;
			textureDesc.Height = selectedExtent.height;
			textureDesc.Depth = 1;
			textureDesc.MipLevels = 1;
			textureDesc.ArrayLevels = 1;
			textureDesc.SampleCount = TextureSampleCount::SAMPLE_COUNT_1;
			textureDesc.pMemory = nullptr;

			Texture* pTexture = pDevice->vkCreateSwapchainTexture(textureDesc, image);

			textures.push_back(pTexture);
		}

		//Create depth stencil texture

		//Set textures
		SetCustomSwapchainTextures(textures);
	}
	void VulkanSwapchain::PresentCore(Semaphore** ppWaitSemahpores, const unsigned int waitSemaphoreCount)
	{
		VkFence fence = ((VulkanFence*)GetPresentFence(GetCurrentImageIndex()))->GetVkFence();

		unsigned int imageIndex = 0;
		if (vkAcquireNextImageKHR(mLogicalDevice, mSwapchain, uint64_max, VK_NULL_HANDLE, fence, &imageIndex) != VK_SUCCESS)
		{
			DEV_LOG("VulkanSwapchain", "Failed to acquire image!");
			return;
		}

		//Get semaphores
		VkSemaphore vkWaitSemaphores[32];
		for (unsigned int i = 0; i < waitSemaphoreCount; i++)
		{
			const VulkanSemaphore* pVkSemaphore = (const VulkanSemaphore*)ppWaitSemahpores[i];
			vkWaitSemaphores[i] = pVkSemaphore->GetVkSemaphore();
		}
		
		//Present
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = waitSemaphoreCount;
		presentInfo.pWaitSemaphores = vkWaitSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &mSwapchain;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		//Get queue
		const VulkanQueue* pQueue = (const VulkanQueue*)GetQueue();
		if (vkQueuePresentKHR(pQueue->GetVkQueue(), &presentInfo) != VK_SUCCESS)
		{
			DEV_LOG("VulkanSwapchain", "Failed to present!");
			return;
		}
	}
	void VulkanSwapchain::Delete()
	{
		vkDestroySwapchainKHR(mLogicalDevice, mSwapchain, nullptr);
		vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
	}
}

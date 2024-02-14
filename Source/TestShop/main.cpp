#include <Runtime/Window/Window.h>
#include <Runtime/Monitor/Monitor.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Vulkan/Instance/VulkanInstanceDesc.h>
#include <Runtime/Vulkan/Device/VulkanDeviceDesc.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>
#include <vulkan.h>
#include <Windows.h>
#include <vulkan_win32.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Vulkan/Device/VulkanDeviceFeatures.h>

namespace Dream
{
	void Run()
	{
		//Create window
		WindowDesc windowDesc = {};
		windowDesc.Title = "DreamTestShop";
		windowDesc.X = 100;
		windowDesc.Y = 100;
		windowDesc.Width = 1024;
		windowDesc.Height = 1024;

		Window* pWindow = Window::Create(windowDesc);
		pWindow->Show();

		//Create instance
		VulkanInstanceDesc instanceDesc = {};
		instanceDesc.Extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
		instanceDesc.Extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		instanceDesc.Extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		instanceDesc.Extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef DREAM_DEBUG
		instanceDesc.Extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		instanceDesc.Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
		instanceDesc.APIVersion = VK_API_VERSION_1_0;
		instanceDesc.EngineName = "Runtime";
		instanceDesc.ApplicationVersion = VK_MAKE_VERSION(1, 0, 0);
		instanceDesc.ApplicationName = "Runtime";
		instanceDesc.Backend = GraphicsBackend::Vulkan;
		GraphicsInstance* pInstance = GraphicsInstance::Create(&instanceDesc);

		//Get adapter
		GraphicsAdapter* pAdapter = pInstance->GetAdapter(0);
		
		//Create device
		VulkanDeviceDesc deviceDesc = {};
		deviceDesc.Extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		deviceDesc.Extensions.push_back(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME);
		deviceDesc.Extensions.push_back(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME);
		deviceDesc.Extensions.push_back(VK_KHR_MULTIVIEW_EXTENSION_NAME);
		deviceDesc.Extensions.push_back(VK_KHR_MAINTENANCE2_EXTENSION_NAME);
		deviceDesc.pOwnerAdapter = pAdapter;
		deviceDesc.pRequestedFeatures = nullptr;
		deviceDesc.GraphicsQueueCount = 1;
		deviceDesc.ComputeQueueCount = 1;
		deviceDesc.TransferQueueCount = 1;
		GraphicsDevice* pDevice = pAdapter->CreateDevice(&deviceDesc);

		//Create queue
		GraphicsQueueDesc queueDesc = {};
		queueDesc.Type = GraphicsQueueType::Graphics;
		GraphicsQueue* pQueue = pDevice->CreateQueue(queueDesc);

		//Create device
		while (pWindow->IsActive())
		{
			pWindow->PollEvents();
		}
	}
}
int main(const unsigned int count, const char** ppArguments)
{
	Dream::Run();
	return 0;
}

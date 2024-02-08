#include <Runtime/Window/Window.h>
#include <Runtime/Monitor/Monitor.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>

int main(const unsigned int count, const char** ppArguments)
{
	//Create graphics instance
	Dream::GraphicsInstanceDesc instanceDesc = {};
	instanceDesc.Extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	instanceDesc.Extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
	instanceDesc.Extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
	Dream::GraphicsInstance* pInstance = new Dream::GraphicsInstance(instanceDesc);

	//Get primary adapter
	Dream::GraphicsAdapter* pAdapter = pInstance->GetPrimaryAdapter();

	//Create device
	Dream::GraphicsDeviceDesc deviceDesc = {};
	deviceDesc.Extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	deviceDesc.Extensions.push_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
	deviceDesc.Extensions.push_back(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME);
	deviceDesc.Extensions.push_back(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME);
	deviceDesc.Extensions.push_back(VK_KHR_MULTIVIEW_EXTENSION_NAME);
	deviceDesc.Extensions.push_back(VK_KHR_MAINTENANCE2_EXTENSION_NAME);
	Dream::GraphicsDevice* pDevice = pAdapter->CreateDevice(deviceDesc);

	//Allocate memory
	Dream::GraphicsMemoryHeapDesc memoryDesc = {};
	memoryDesc.Type = Dream::GraphicsMemoryType::Device;
	memoryDesc.SizeInBytes = MB_TO_BYTE(1024);
	Dream::GraphicsMemoryHeap* pMemoryHeap = pDevice->AllocateMemory(memoryDesc);

	//Create window
	Dream::WindowDesc windowDesc = {};
	windowDesc.Title = "Dream Vulkan Renderer Layer";
	windowDesc.Size[0] = 1024;
	windowDesc.Size[1] = 1024;
	windowDesc.Position[0] = 100;
	windowDesc.Position[1] = 100;

	Dream::Window* pWindow = new Dream::Window(windowDesc);
	pWindow->Show();

	while (pWindow->IsAlive())
	{
		pWindow->PollMessages();
	}
}

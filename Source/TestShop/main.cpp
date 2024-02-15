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
	void InvalidateRenderPasses(GraphicsDevice* pDevice,Swapchain* pSwapchain,CommandList* pCmdList,Fence* pFence,GraphicsQueue* pQueue, std::vector<RenderPass*>& passes)
	{
		//First clear the existing passes
		for (RenderPass* pPass : passes)
			delete pPass;
		passes.clear();

		//Transition the swapchain textures to color attachment optimal
		const std::vector<Texture*>& swapchainTextures = pSwapchain->GetColorTextures();
		pCmdList->BeginRecording();
		for (const Texture* pTexture : swapchainTextures)
		{
			TextureMemoryBarrierDesc barrier = {};
			barrier.MipIndex = 0;
			barrier.ArrayIndex = 0;
			barrier.AspectFlags = TextureAspectFlags::Color;
			barrier.SourceAccessFlags = GraphicsMemoryAccessFlags::Unknown;
			barrier.SourceLayout = TextureMemoryLayout::Unknown;
			barrier.SourceQueue = GraphicsQueueType::Graphics;
			barrier.SourceStageFlags = PipelineStageFlags::TopOfPipe;

			barrier.DestinationAccessFlags = GraphicsMemoryAccessFlags::ColorAttachmentRead;
			barrier.DestinationLayout = TextureMemoryLayout::Present;
			barrier.DestinationQueue = GraphicsQueueType::Graphics;
			barrier.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;

			pCmdList->SetTextureMemoryBarrier(pTexture, barrier);
		}
		pCmdList->EndRecording();
		pDevice->SubmitCommands((const CommandList**)&pCmdList, 1, pQueue, pFence);
		pDevice->WaitFences(&pFence, 1);
		pDevice->ResetFences(&pFence, 1);

		const std::vector<TextureView*>& swapchainTextureViews = pSwapchain->GetColorTextureViews();
		for (unsigned char i = 0; i < pSwapchain->GetBufferCount(); i++)
		{
			Texture* pTexture = swapchainTextures[i];
			TextureView* pView = swapchainTextureViews[i];

			RenderPassDesc renderPassDesc = {};
			renderPassDesc.TargetRenderWidth = pSwapchain->GetWidth();
			renderPassDesc.TargetRenderHeight = pSwapchain->GetHeight();
			renderPassDesc.AttachmentViews = { pView };
			RenderPassAttachmentDesc colorAttachmentDesc = {};
			colorAttachmentDesc.ArrayLevel = 0;
			colorAttachmentDesc.MipLevel = 0;
			colorAttachmentDesc.ColorLoadOperation = RenderPassLoadOperation::Clear;
			colorAttachmentDesc.ColorStoreOperation = RenderPassStoreOperation::Store;
			colorAttachmentDesc.InputLayout = TextureMemoryLayout::Present;
			colorAttachmentDesc.OutputLayout = TextureMemoryLayout::Present;
			colorAttachmentDesc.StencilLoadOperation = RenderPassLoadOperation::Clear;
			colorAttachmentDesc.StencilStoreOperation = RenderPassStoreOperation::Ignore;
			colorAttachmentDesc.Format = pTexture->GetFormat();
			colorAttachmentDesc.SampleCount = pTexture->GetSampleCount();
			renderPassDesc.ColorAttachments.push_back(colorAttachmentDesc);

			RenderPassSubpassDesc subpassDesc = {};
			subpassDesc.Attachments = { 0 };
			subpassDesc.BindPoint = PipelineBindPoint::Graphics;
			subpassDesc.DepthStencilInput = 0;
			renderPassDesc.Subpasses.push_back(subpassDesc);

			passes.push_back(pDevice->CreateRenderPass(renderPassDesc));
		}
	}
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
		
		//Create swapchain
		SwapchainDesc swapchainDesc = {};
		swapchainDesc.BufferCount = 3;
		swapchainDesc.ColorFormat = TextureFormat::R8_G8_B8_A8_UNorm;
		swapchainDesc.DepthStencilFormat = TextureFormat::None;
		swapchainDesc.Mode = PresentMode::VsyncImmediate;
		swapchainDesc.pQueue = pQueue;
		swapchainDesc.pWindow = pWindow;
		Swapchain* pSwapchain = pDevice->CreateSwapchain(swapchainDesc);

		//Create command pool
		CommandPoolDesc cmdPoolDesc = {};
		cmdPoolDesc.Type = CommandPoolType::Graphics;
		CommandPool* pCmdPool = pDevice->CreateCommandPool(cmdPoolDesc);

		//Create command list
		CommandListDesc cmdListDesc = {};
		cmdListDesc.pCmdPool = pCmdPool;
		CommandList* pCmdList = pDevice->CreateCommandList(cmdListDesc);

		//Create fence
		FenceDesc fenceDesc = {};
		fenceDesc.bSignalled = false;
		Fence* pFence = pDevice->CreateFence(fenceDesc);

		//Create render pass
		std::vector<RenderPass*> renderPasses;
		InvalidateRenderPasses(pDevice, pSwapchain, pCmdList, pFence, pQueue, renderPasses);

		//Create device
		unsigned char presentIndex = 0;
		while (pWindow->IsActive())
		{
			//Poll events
			pWindow->PollEvents();

			//Resize the render pass and swapchain
			for (const WindowEventData& eventData : pWindow->GetBufferedEvents())
			{
				if (eventData.Type == WindowEventType::WindowResized)
				{
					pSwapchain->Resize(pWindow->GetWidth(), pWindow->GetHeight());
					InvalidateRenderPasses(pDevice, pSwapchain, pCmdList, pFence, pQueue, renderPasses);
					break;
				}
			}

			//Begin recording
			pCmdList->BeginRecording();

			//Start render pass
			constexpr float clearColor[] = {100/255.0f,149/255.0f,237/255.0f,1 };
			pCmdList->BeginRenderPass(renderPasses[presentIndex], clearColor);

			//End render pass
			pCmdList->EndRenderPass();

			//End recording
			pCmdList->EndRecording();

			//Submit and wait for the commands to finish
			pDevice->SubmitCommands((const CommandList**)& pCmdList, 1, pQueue, pFence);
			pDevice->WaitFences(&pFence, 1);
			pDevice->ResetFences(&pFence, 1);

			//Present and wait for the present to finish
			pSwapchain->Present();
			pSwapchain->WaitForPresent(presentIndex);
			presentIndex = (presentIndex + 1) % 3;
		}
	}
}
int main(const unsigned int count, const char** ppArguments)
{
	Dream::Run();
	return 0;
}

#include <Runtime/Window/Window.h>
#include <Runtime/Monitor/Monitor.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/ShaderCompiler/ShaderCompiler.h>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#define PARTICLE_COUNT 500

#define USE_VULKAN

#ifdef USE_VULKAN
#include <vulkan.h>

#ifdef DREAM_PLATFORM_WINDOWS
#include <Windows.h>
#include <vulkan_win32.h>
#endif

#include <Runtime/Vulkan/Instance/VulkanInstanceDesc.h>
#include <Runtime/Vulkan/Device/VulkanDeviceDesc.h>
#include <Runtime/Vulkan/Device/VulkanDeviceFeatures.h>
#endif

namespace Dream
{
	static const char computeShaderSource[] =

	"\
	#pragma kernel main\n\
\
	layout(set = 0, binding = 0) RWTexture2D<float4> TargetTexture;\
	[numthreads(256, 1, 1)]\
	void main(uint3 id : SV_DispatchThreadID)\
	{\
		TargetTexture[id.xy] = float4(id.x / 3840.0f,id.y / 2160.f,0.0f,1.0f);\
	}";

	struct ParticleData
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 Color;
	};

	void InvalidateScene(GraphicsDevice* pDevice, GraphicsMemory* pDeviceMemory, Swapchain* pSwapchain, CommandList* pCmdList, Fence* pFence, GraphicsQueue* pQueue, Shader* pComputeShader, DescriptorSetLayout* pSetLayout, std::vector<RenderPass*>& passes)
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
		pDevice->SubmitCommands(&pCmdList, 1, pQueue,nullptr,0,nullptr,nullptr,0, pFence);
		pDevice->WaitFences(&pFence, 1);
		pDevice->ResetFences(&pFence, 1);

		//Create render passes and pipeline per swapchain textures
		const std::vector<TextureView*>& swapchainTextureViews = pSwapchain->GetColorTextureViews();
		for (unsigned char i = 0; i < pSwapchain->GetBufferCount(); i++)
		{
			Texture* pTexture = swapchainTextures[i];
			TextureView* pView = swapchainTextureViews[i];

			//Create render pass
			RenderPassDesc renderPassDesc = {};
			renderPassDesc.TargetRenderWidth = pSwapchain->GetWidth();
			renderPassDesc.TargetRenderHeight = pSwapchain->GetHeight();
			renderPassDesc.pDepthStencilAttachment = nullptr;

			RenderPassAttachmentDesc colorAttachmentDesc = {};
			colorAttachmentDesc.ArrayLevel = 0;
			colorAttachmentDesc.MipLevel = 0;
			colorAttachmentDesc.ColorLoadOperation = RenderPassLoadOperation::Clear;
			colorAttachmentDesc.ColorStoreOperation = RenderPassStoreOperation::Store;
			colorAttachmentDesc.InputLayout = TextureMemoryLayout::ColorAttachment;
			colorAttachmentDesc.OutputLayout = TextureMemoryLayout::Present;
			colorAttachmentDesc.StencilLoadOperation = RenderPassLoadOperation::Ignore;
			colorAttachmentDesc.StencilStoreOperation = RenderPassStoreOperation::Ignore;
			colorAttachmentDesc.Format = pTexture->GetFormat();
			colorAttachmentDesc.SampleCount = pTexture->GetSampleCount();
			colorAttachmentDesc.pView = pView;
			renderPassDesc.ColorAttachments.push_back(colorAttachmentDesc);

			RenderPass* pRenderPass = pDevice->CreateRenderPass(renderPassDesc);
			passes.push_back(pRenderPass);
		}
	}
	void Run()
	{
		//Setup particle scene
		std::vector<ParticleData> particleData(PARTICLE_COUNT);
		for (unsigned int i = 0; i < PARTICLE_COUNT; i++)
		{
			ParticleData particle = {};
			particle.Position =
			{
				(rand() % 5) / 10.0f,
				(rand() % 5) / 10.0f
			};
			particle.Velocity =
			{
				(rand() % 5) / 10.0f,
				(rand() % 5) / 10.0f
			};
			particle.Color =
			{
				(rand() % 5) / 10.0f,
				(rand() % 5) / 10.0f,
				(rand() % 5) / 10.0f,
				1.0f
			};

			particleData[i] = particle;
		}

		//Create window
		WindowDesc windowDesc = {};
		windowDesc.Title = "Compute texture";
		windowDesc.X = 100;
		windowDesc.Y = 100;
		windowDesc.Width = 1024;
		windowDesc.Height = 1024;

		Window* pWindow = Window::Create(windowDesc);

		pWindow->Show();

		pWindow->SetMode(WindowMode::Borderless);

		Monitor* pMonitor = Monitor::GetPrimaryMonitor();
		pWindow->SetOffset(pMonitor->GetPositionX(), pMonitor->GetPositionY());
		pWindow->SetSize(pMonitor->GetWidth(), pMonitor->GetHeight());

		//Create instance
#ifdef USE_VULKAN
		VulkanInstanceDesc instanceDesc = {};
		instanceDesc.Extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
		instanceDesc.Extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#ifdef DREAM_PLATFORM_WINDOWS
		instanceDesc.Extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
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
#endif

		//Get adapter
		GraphicsAdapter* pAdapter = pInstance->GetAdapter(0);

		//Create device
#ifdef USE_VULKAN
		VulkanDeviceDesc deviceDesc = {};
		deviceDesc.Extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		deviceDesc.Extensions.push_back(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME);
		deviceDesc.Extensions.push_back(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME);
		deviceDesc.Extensions.push_back(VK_KHR_MULTIVIEW_EXTENSION_NAME);
		deviceDesc.Extensions.push_back(VK_KHR_MAINTENANCE2_EXTENSION_NAME);
		VulkanDeviceFeatures deviceFeatures = {};
		deviceDesc.pRequestedFeatures = &deviceFeatures;
		deviceDesc.pOwnerAdapter = pAdapter;
		deviceDesc.GraphicsQueueCount = 1;
		deviceDesc.ComputeQueueCount = 1;
		deviceDesc.TransferQueueCount = 1;
		GraphicsDevice* pDevice = pAdapter->CreateDevice(&deviceDesc);
#endif

		//Create queue
		GraphicsQueueDesc queueDesc = {};
		queueDesc.Type = GraphicsQueueType::Graphics;
		GraphicsQueue* pQueue = pDevice->CreateQueue(queueDesc);

		GraphicsQueueDesc computeQueueDesc = {};
		computeQueueDesc.Type = GraphicsQueueType::Compute;
		GraphicsQueue* pComputeQueue = pDevice->CreateQueue(computeQueueDesc);

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

		CommandPoolDesc cmdComputePoolDesc = {};
		cmdComputePoolDesc.Type = CommandPoolType::Compute;
		CommandPool* pCmdComputePool = pDevice->CreateCommandPool(cmdComputePoolDesc);

		//Create command list
		CommandListDesc cmdListDesc = {};
		cmdListDesc.pCmdPool = pCmdPool;
		CommandList* pCmdList = pDevice->CreateCommandList(cmdListDesc);

		CommandListDesc cmdComputeListDesc = {};
		cmdComputeListDesc.pCmdPool = pCmdComputePool;
		CommandList* pComputeCmdList = pDevice->CreateCommandList(cmdComputeListDesc);

		//Create fence
		FenceDesc fenceDesc = {};
		fenceDesc.bSignalled = false;
		Fence* pFence = pDevice->CreateFence(fenceDesc);

		//Allocate memory
		GraphicsMemoryDesc hostMemoryDesc = {};
		hostMemoryDesc.SizeInBytes = MB_TO_BYTE(1024);
		hostMemoryDesc.Type = GraphicsMemoryType::Host;
		GraphicsMemory* pHostMemory = pDevice->AllocateMemory(hostMemoryDesc);

		GraphicsMemoryDesc deviceMemoryDesc = {};
		deviceMemoryDesc.SizeInBytes = MB_TO_BYTE(1024);
		deviceMemoryDesc.Type = GraphicsMemoryType::Device;
		GraphicsMemory* pDeviceMemory = pDevice->AllocateMemory(deviceMemoryDesc);

		//Create particle buffer
		GraphicsBufferDesc particleBufferDesc = {};
		particleBufferDesc.Usage = GraphicsBufferUsage::Storage | GraphicsBufferUsage::TransferDestination;
		particleBufferDesc.SubItemCount = 1;
		particleBufferDesc.SubItemSizeInBytes = sizeof(ParticleData) * PARTICLE_COUNT;
		particleBufferDesc.pMemory = pDeviceMemory;
		GraphicsBuffer* pParticleBuffer = pDevice->CreateBuffer(particleBufferDesc);

		//Create staging buffers
		GraphicsBufferDesc particleStageBufferDesc = {};
		particleStageBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
		particleStageBufferDesc.SubItemCount = 1;
		particleStageBufferDesc.SubItemSizeInBytes = sizeof(ParticleData) * PARTICLE_COUNT;
		particleStageBufferDesc.pMemory = pHostMemory;

		GraphicsBuffer* pParticleStageBuffer = pDevice->CreateBuffer(particleStageBufferDesc);

		//Compile compute shader
		unsigned char* pComputeSPIRVBytes = nullptr;
		unsigned int computeSPIRVByteCount = 0;
		std::string computeSPIRVErrorMessage;
		ShaderCompiler::CompileToSPIRV(computeShaderSource, "main", ShaderStage::Compute, ShaderLanguage::HLSL, &pComputeSPIRVBytes, computeSPIRVByteCount, computeSPIRVErrorMessage);

		unsigned char* pComputeBytes = nullptr;
		unsigned int computeByteCount = 0;
		std::string computeErrorMessage;
		ShaderCompiler::CompileFromSPIRV(pComputeSPIRVBytes, computeSPIRVByteCount, GraphicsBackend::Vulkan, &pComputeBytes, computeByteCount, computeErrorMessage);

		//Create compute shader
		ShaderDesc computeShaderDesc = {};
		computeShaderDesc.pByteCode = pComputeBytes;
		computeShaderDesc.ByteCodeLength = computeByteCount;
		computeShaderDesc.EntryMethod = "main";
		computeShaderDesc.Stage = ShaderStage::Compute;

		Shader* pComputeShader = pDevice->CreateShader(computeShaderDesc);

		//Create descriptor set pools
		DescriptorPoolDesc descriptorHostPoolDesc = {};
		descriptorHostPoolDesc.Sizes =
		{
			{DescriptorResourceType::StorageBuffer,5},
			{DescriptorResourceType::StorageTexture,5}
		};
		descriptorHostPoolDesc.SetCount = 3;
		descriptorHostPoolDesc.Type = DescriptorMemoryType::Host;
		DescriptorPool* pDescriptorHostPool = pDevice->CreateDescriptorPool(descriptorHostPoolDesc);

		DescriptorPoolDesc descriptorDevicePoolDesc = {};
		descriptorDevicePoolDesc.Sizes =
		{
			{DescriptorResourceType::StorageBuffer,5},
			{DescriptorResourceType::StorageTexture,5}
		};
		descriptorDevicePoolDesc.SetCount = 3;
		descriptorDevicePoolDesc.Type = DescriptorMemoryType::Device;
		DescriptorPool* pDescriptorDevicePool = pDevice->CreateDescriptorPool(descriptorDevicePoolDesc);

		//Create compute layout
		DescriptorSetLayoutDesc computeDescriptorLayoutDesc = {};
		{
			DescriptorSetLayoutEntry renderTarget = {};
			renderTarget.Binding = 0;
			renderTarget.Stages = ShaderStage::Compute;
			renderTarget.Type = DescriptorResourceType::StorageTexture;
			computeDescriptorLayoutDesc.Entries.push_back(renderTarget);

			DescriptorSetLayoutEntry buffer = {};
			buffer.Binding = 1;
			buffer.Stages = ShaderStage::Compute;
			buffer.Type = DescriptorResourceType::StorageBuffer;
			computeDescriptorLayoutDesc.Entries.push_back(buffer);
		}
		DescriptorSetLayout* pComputeSetLayout = pDevice->CreateDescriptorSetLayout(computeDescriptorLayoutDesc);

		ComputePipelineDesc computePipelineDesc = {};
		computePipelineDesc.DescriptorSetLayouts.push_back(pComputeSetLayout);
		computePipelineDesc.pComputeShader = pComputeShader;
		Pipeline* pComputePipeline = pDevice->CreateComputePipeline(computePipelineDesc);

		//Invalidate scene for render pass and pipelines
		std::vector<RenderPass*> renderPasses;
		InvalidateScene(pDevice, pDeviceMemory, pSwapchain, pCmdList, pFence, pQueue, pComputeShader, nullptr, renderPasses);

		//Create descriptor sets
		DescriptorSetDesc hostDescriptorSetDesc = {};
		hostDescriptorSetDesc.pLayout = pComputeSetLayout;
		hostDescriptorSetDesc.pPool = pDescriptorHostPool;
		DescriptorSet* pHostDescriptorSet = pDevice->CreateDescriptorSet(hostDescriptorSetDesc);

		std::vector<DescriptorSet*> descriptorSets;

		DescriptorSetDesc deviceDescriptorSetDesc0 = {};
		deviceDescriptorSetDesc0.pLayout = pComputeSetLayout;
		deviceDescriptorSetDesc0.pPool = pDescriptorDevicePool;
		descriptorSets.push_back(pDevice->CreateDescriptorSet(deviceDescriptorSetDesc0));

		DescriptorSetDesc deviceDescriptorSetDesc1 = {};
		deviceDescriptorSetDesc1.pLayout = pComputeSetLayout;
		deviceDescriptorSetDesc1.pPool = pDescriptorDevicePool;
		descriptorSets.push_back(pDevice->CreateDescriptorSet(deviceDescriptorSetDesc1));

		DescriptorSetDesc deviceDescriptorSetDesc2 = {};
		deviceDescriptorSetDesc2.pLayout = pComputeSetLayout;
		deviceDescriptorSetDesc2.pPool = pDescriptorDevicePool;
		descriptorSets.push_back(pDevice->CreateDescriptorSet(deviceDescriptorSetDesc2));

		{
			DescriptorSetUpdateDesc descriptorSetUpdate = {};
			descriptorSetUpdate.Entries =
			{
				{pSwapchain->GetColorTextureViews()[0],DescriptorResourceType::StorageTexture,1,0,0,0},
				{pParticleBuffer,DescriptorResourceType::StorageBuffer,1,0,0,1}
			};
			pDevice->UpdateDescriptorSet(pHostDescriptorSet, descriptorSetUpdate);

			DescriptorSetCopyDesc descriptorSetCopyDesc = {};
			descriptorSetCopyDesc.Entries =
			{
				{0,0,0,0,1},
				{1,0,1,0,1}
			};
			pDevice->CopyDescriptorSet(pHostDescriptorSet, descriptorSets[0], descriptorSetCopyDesc);
		}

		{
			DescriptorSetUpdateDesc descriptorSetUpdate = {};
			descriptorSetUpdate.Entries =
			{
				{pSwapchain->GetColorTextureViews()[1],DescriptorResourceType::StorageTexture,1,0,0,0},
				{pParticleBuffer,DescriptorResourceType::StorageBuffer,1,0,0,1}
			};
			pDevice->UpdateDescriptorSet(pHostDescriptorSet, descriptorSetUpdate);

			DescriptorSetCopyDesc descriptorSetCopyDesc = {};
			descriptorSetCopyDesc.Entries =
			{
				{0,0,0,0,1},
				{1,0,1,0,1}
			};
			pDevice->CopyDescriptorSet(pHostDescriptorSet, descriptorSets[1], descriptorSetCopyDesc);
		}

		{
			DescriptorSetUpdateDesc descriptorSetUpdate = {};
			descriptorSetUpdate.Entries =
			{
				{pSwapchain->GetColorTextureViews()[2],DescriptorResourceType::StorageTexture,1,0,0,0},
				{pParticleBuffer,DescriptorResourceType::StorageBuffer,1,0,0,1}
			};
			pDevice->UpdateDescriptorSet(pHostDescriptorSet, descriptorSetUpdate);

			DescriptorSetCopyDesc descriptorSetCopyDesc = {};
			descriptorSetCopyDesc.Entries =
			{
				{0,0,0,0,1},
				{1,0,1,0,1}
			};
			pDevice->CopyDescriptorSet(pHostDescriptorSet, descriptorSets[2], descriptorSetCopyDesc);
		}

		//Create semaphores
		SemaphoreDesc semaphoreDesc = {};
		semaphoreDesc.bSignalled = false;
		Semaphore* pSemaphore = pDevice->CreateSyncObject(semaphoreDesc);
		Semaphore* pSemaphore2 = pDevice->CreateSyncObject(semaphoreDesc);

		//Create scene variables
		unsigned char presentIndex = 0;
		const glm::vec3 cameraPosition = { 0,0,-5 };
		const glm::vec3 relativeUp = { 0,1,0 };

		while (pWindow->IsActive())
		{
			Texture* pSwapchainTexture = pSwapchain->GetColorTextures()[presentIndex];
			DescriptorSet* pSwapchainDescriptorSet = descriptorSets[presentIndex];

			//Poll events
			pWindow->PollEvents();

			//Get screen size
			const unsigned int screenWidth = pWindow->GetWidth();
			const unsigned int screenHeight = pWindow->GetHeight();

			//Resize the render pass and swapchain
			unsigned int currentScreenWidth = screenWidth;
			unsigned int currentScreenHeight = screenHeight;
			for (const WindowEventData& eventData : pWindow->GetBufferedEvents())
			{
				if (eventData.Type == WindowEventType::WindowResized)
				{
					currentScreenWidth = eventData.WindowSize[0];
					currentScreenHeight = eventData.WindowSize[1];
				}
			}
			if (currentScreenWidth != screenWidth || currentScreenHeight != screenHeight)
			{
				pSwapchain->Resize(currentScreenWidth, currentScreenHeight);
				InvalidateScene(pDevice, pDeviceMemory, pSwapchain, pCmdList, pFence, pQueue, pComputeShader, nullptr, renderPasses);
			}

			//Record and submit commands
			pCmdList->BeginRecording();

			TextureMemoryBarrierDesc computeOwnershipBarrierDesc = {};
			computeOwnershipBarrierDesc.AspectFlags = TextureAspectFlags::Color;
			computeOwnershipBarrierDesc.ArrayIndex = 0;
			computeOwnershipBarrierDesc.MipIndex = 0;
			computeOwnershipBarrierDesc.SourceAccessFlags = GraphicsMemoryAccessFlags::Unknown;
			computeOwnershipBarrierDesc.SourceLayout = TextureMemoryLayout::Unknown;
			computeOwnershipBarrierDesc.SourceQueue = GraphicsQueueType::Graphics;
			computeOwnershipBarrierDesc.SourceStageFlags = PipelineStageFlags::TopOfPipe;
			computeOwnershipBarrierDesc.DestinationAccessFlags = GraphicsMemoryAccessFlags::ShaderWrite;
			computeOwnershipBarrierDesc.DestinationLayout = TextureMemoryLayout::General;
			computeOwnershipBarrierDesc.DestinationQueue = GraphicsQueueType::Compute;
			computeOwnershipBarrierDesc.DestinationStageFlags = PipelineStageFlags::ComputeShader;
			pCmdList->SetTextureMemoryBarrier(pSwapchainTexture, computeOwnershipBarrierDesc);

			pCmdList->EndRecording();

			pDevice->SubmitCommands(&pCmdList, 1, pQueue, &pSemaphore, 1, nullptr, nullptr, 0,nullptr);

			pComputeCmdList->BeginRecording();

			TextureMemoryBarrierDesc computeBarrierDesc = {};
			computeBarrierDesc.AspectFlags = TextureAspectFlags::Color;
			computeBarrierDesc.ArrayIndex = 0;
			computeBarrierDesc.MipIndex = 0;
			computeBarrierDesc.SourceAccessFlags = GraphicsMemoryAccessFlags::Unknown;
			computeBarrierDesc.SourceLayout = TextureMemoryLayout::Unknown;
			computeBarrierDesc.SourceQueue = GraphicsQueueType::Compute;
			computeBarrierDesc.SourceStageFlags = PipelineStageFlags::TopOfPipe;
			computeBarrierDesc.DestinationAccessFlags = GraphicsMemoryAccessFlags::ShaderWrite;
			computeBarrierDesc.DestinationLayout = TextureMemoryLayout::General;
			computeBarrierDesc.DestinationQueue = GraphicsQueueType::Compute;
			computeBarrierDesc.DestinationStageFlags = PipelineStageFlags::ComputeShader;
			pComputeCmdList->SetTextureMemoryBarrier(pSwapchainTexture, computeBarrierDesc);

			pComputeCmdList->SetPipeline(pComputePipeline);
			pComputeCmdList->CommitResourceSets(&pSwapchainDescriptorSet, 1);
			pComputeCmdList->DispatchCompute(pSwapchain->GetWidth() / 256, pSwapchain->GetHeight(), 1);

			TextureMemoryBarrierDesc presentOwnershipBarrierDesc = {};
			presentOwnershipBarrierDesc.AspectFlags = TextureAspectFlags::Color;
			presentOwnershipBarrierDesc.ArrayIndex = 0;
			presentOwnershipBarrierDesc.MipIndex = 0;
			presentOwnershipBarrierDesc.SourceAccessFlags = GraphicsMemoryAccessFlags::ShaderWrite;
			presentOwnershipBarrierDesc.SourceLayout = TextureMemoryLayout::General;
			presentOwnershipBarrierDesc.SourceQueue = GraphicsQueueType::Compute;
			presentOwnershipBarrierDesc.SourceStageFlags = PipelineStageFlags::ComputeShader;
			presentOwnershipBarrierDesc.DestinationAccessFlags = GraphicsMemoryAccessFlags::ShaderWrite;
			presentOwnershipBarrierDesc.DestinationLayout = TextureMemoryLayout::General;
			presentOwnershipBarrierDesc.DestinationQueue = GraphicsQueueType::Graphics;
			presentOwnershipBarrierDesc.DestinationStageFlags = PipelineStageFlags::ComputeShader;
			pComputeCmdList->SetTextureMemoryBarrier(pSwapchainTexture, presentOwnershipBarrierDesc);

			const PipelineStageFlags computeWaitFlags = PipelineStageFlags::ComputeShader;
			pComputeCmdList->EndRecording();
			pDevice->SubmitCommands(&pComputeCmdList, 1, pComputeQueue, &pSemaphore2, 1, &pSemaphore, &computeWaitFlags, 1, pFence);
			pDevice->WaitFences(&pFence, 1);
			pDevice->ResetFences(&pFence, 1);

			pCmdList->BeginRecording();
			TextureMemoryBarrierDesc presentBarrierDesc = {};
			presentBarrierDesc.AspectFlags = TextureAspectFlags::Color;
			presentBarrierDesc.ArrayIndex = 0;
			presentBarrierDesc.MipIndex = 0;
			presentBarrierDesc.SourceAccessFlags = GraphicsMemoryAccessFlags::Unknown;
			presentBarrierDesc.SourceLayout = TextureMemoryLayout::Unknown;
			presentBarrierDesc.SourceQueue = GraphicsQueueType::Graphics;
			presentBarrierDesc.SourceStageFlags = PipelineStageFlags::TopOfPipe;
			presentBarrierDesc.DestinationAccessFlags = GraphicsMemoryAccessFlags::ColorAttachmentRead;
			presentBarrierDesc.DestinationLayout = TextureMemoryLayout::Present;
			presentBarrierDesc.DestinationAccessFlags = GraphicsMemoryAccessFlags::ColorAttachmentRead;
			presentBarrierDesc.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;
			pCmdList->SetTextureMemoryBarrier(pSwapchainTexture, presentBarrierDesc);
			pCmdList->EndRecording();

			const PipelineStageFlags presentWaitFlags = PipelineStageFlags::ComputeShader;
			pDevice->SubmitCommands(&pCmdList, 1, pQueue, &pSemaphore, 1, &pSemaphore2, &presentWaitFlags, 1, pFence);

			pDevice->WaitFences(&pFence, 1);
			pDevice->ResetFences(&pFence, 1);

			//Present and wait for the present to finish
			pSwapchain->Present(&pSemaphore,1);
			pSwapchain->WaitForPresent(presentIndex);
			presentIndex = (presentIndex + 1) % swapchainDesc.BufferCount;
		}
	}
}
int main(const unsigned int count, const char** ppArguments)
{
	Dream::Run();
	return 0;
}

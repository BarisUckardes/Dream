#include <Runtime/Window/Window.h>
#include <Runtime/Monitor/Monitor.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/ShaderCompiler/ShaderCompiler.h>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		const std::vector<Texture*>& swapchainTextures = pSwapchain->buffers();
		pCmdList->begin();
		for (const Texture* pTexture : swapchainTextures)
		{
			TextureMemoryBarrierDesc barrier = {};
			barrier.MipIndex = 0;
			barrier.ArrayIndex = 0;
			barrier.AspectFlags = TextureAspectFlags::Color;
			barrier.SourceAccessFlags = GraphicsMemoryAccessFlags::Unknown;
			barrier.SourceLayout = TextureMemoryLayout::Unknown;
			barrier.SourceQueue = GraphicsQueueFamilyType::Graphics;
			barrier.SourceStageFlags = PipelineStageFlags::TopOfPipe;

			barrier.DestinationAccessFlags = GraphicsMemoryAccessFlags::ColorAttachmentRead;
			barrier.DestinationLayout = TextureMemoryLayout::Present;
			barrier.DestinationQueue = GraphicsQueueFamilyType::Graphics;
			barrier.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;

			pCmdList->set_texture_barrier(pTexture, barrier);
		}

		pCmdList->end();
		pDevice->submit_commands(&pCmdList, 1, pQueue,nullptr,0,nullptr,nullptr,0, pFence);
		pDevice->wait_fences(&pFence, 1);
		pDevice->reset_fences(&pFence, 1);

		//Create render passes and pipeline per swapchain textures
		const std::vector<TextureView*>& swapchainTextureViews = pSwapchain->buffer_views();
		for (unsigned char i = 0; i < pSwapchain->buffer_count(); i++)
		{
			Texture* pTexture = swapchainTextures[i];
			TextureView* pView = swapchainTextureViews[i];

			//Create render pass
			RenderPassDesc renderPassDesc = {};
			renderPassDesc.TargetRenderWidth = pSwapchain->width();
			renderPassDesc.TargetRenderHeight = pSwapchain->height();
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
			colorAttachmentDesc.Format = pTexture->format();
			colorAttachmentDesc.SampleCount = pTexture->sample_count();
			colorAttachmentDesc.pView = pView;
			renderPassDesc.ColorAttachments.push_back(colorAttachmentDesc);

			RenderPass* pRenderPass = pDevice->create_render_pass(renderPassDesc);
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

		Window* pWindow = Window::create(windowDesc);

		pWindow->show();

		pWindow->set_mode(WindowMode::Borderless);

		Monitor* pMonitor = Monitor::primary_monitor();
		pWindow->set_offset(pMonitor->x(), pMonitor->y());
		pWindow->set_size(pMonitor->width(), pMonitor->height());

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
		GraphicsAdapter* pAdapter = pInstance->adapter(0);

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
		GraphicsDevice* pDevice = pAdapter->create_device(&deviceDesc);
#endif

		//Create queue
		GraphicsQueueDesc queueDesc = {};
		queueDesc.Type = GraphicsQueueFamilyType::Graphics;
		GraphicsQueue* pQueue = pDevice->own_queue(queueDesc);

		GraphicsQueueDesc computeQueueDesc = {};
		computeQueueDesc.Type = GraphicsQueueFamilyType::Compute;
		GraphicsQueue* pComputeQueue = pDevice->own_queue(computeQueueDesc);

		//Create swapchain
		SwapchainDesc swapchainDesc = {};
		swapchainDesc.BufferCount = 3;
		swapchainDesc.ColorFormat = TextureFormat::R8_G8_B8_A8_UNorm;
		swapchainDesc.Mode = PresentMode::VsyncImmediate;
		swapchainDesc.pQueue = pQueue;
		swapchainDesc.pWindow = pWindow;
		Swapchain* pSwapchain = pDevice->create_swapchain(swapchainDesc);

		//Create command pool
		CommandPoolDesc cmdPoolDesc = {};
		cmdPoolDesc.Type = CommandPoolType::Graphics;
		CommandPool* pCmdPool = pDevice->create_cmd_pool(cmdPoolDesc);

		CommandPoolDesc cmdComputePoolDesc = {};
		cmdComputePoolDesc.Type = CommandPoolType::Compute;
		CommandPool* pCmdComputePool = pDevice->create_cmd_pool(cmdComputePoolDesc);

		//Create command list
		CommandListDesc cmdListDesc = {};
		cmdListDesc.pCmdPool = pCmdPool;
		CommandList* pCmdList = pDevice->create_cmd_list(cmdListDesc);

		CommandListDesc cmdComputeListDesc = {};
		cmdComputeListDesc.pCmdPool = pCmdComputePool;
		CommandList* pComputeCmdList = pDevice->create_cmd_list(cmdComputeListDesc);

		//Create fence
		FenceDesc fenceDesc = {};
		fenceDesc.bSignalled = false;
		Fence* pFence = pDevice->create_fence(fenceDesc);

		//Allocate memory
		GraphicsMemoryDesc hostMemoryDesc = {};
		hostMemoryDesc.SizeInBytes = MB_TO_BYTE(1024);
		hostMemoryDesc.Type = GraphicsMemoryType::Host;
		GraphicsMemory* pHostMemory = pDevice->allocate_memory(hostMemoryDesc);

		GraphicsMemoryDesc deviceMemoryDesc = {};
		deviceMemoryDesc.SizeInBytes = MB_TO_BYTE(1024);
		deviceMemoryDesc.Type = GraphicsMemoryType::Device;
		GraphicsMemory* pDeviceMemory = pDevice->allocate_memory(deviceMemoryDesc);

		//Create particle buffer
		GraphicsBufferDesc particleBufferDesc = {};
		particleBufferDesc.Usage = GraphicsBufferUsage::Storage | GraphicsBufferUsage::TransferDestination;
		particleBufferDesc.SubItemCount = 1;
		particleBufferDesc.SubItemSizeInBytes = sizeof(ParticleData) * PARTICLE_COUNT;
		particleBufferDesc.pMemory = pDeviceMemory;
		GraphicsBuffer* pParticleBuffer = pDevice->create_buffer(particleBufferDesc);

		//Create staging buffers
		GraphicsBufferDesc particleStageBufferDesc = {};
		particleStageBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
		particleStageBufferDesc.SubItemCount = 1;
		particleStageBufferDesc.SubItemSizeInBytes = sizeof(ParticleData) * PARTICLE_COUNT;
		particleStageBufferDesc.pMemory = pHostMemory;

		GraphicsBuffer* pParticleStageBuffer = pDevice->create_buffer(particleStageBufferDesc);

		//Compile compute shader
		unsigned char* pComputeSPIRVBytes = nullptr;
		unsigned int computeSPIRVByteCount = 0;
		std::string computeSPIRVErrorMessage;
		ShaderCompiler::compile_to_spirv(computeShaderSource, "main", ShaderStage::Compute, ShaderLanguage::HLSL, &pComputeSPIRVBytes, computeSPIRVByteCount, computeSPIRVErrorMessage);

		unsigned char* pComputeBytes = nullptr;
		unsigned int computeByteCount = 0;
		std::string computeErrorMessage;
		ShaderCompiler::compile_spirv_to_backend(pComputeSPIRVBytes, computeSPIRVByteCount, GraphicsBackend::Vulkan, &pComputeBytes, computeByteCount, computeErrorMessage);

		//Create compute shader
		ShaderDesc computeShaderDesc = {};
		computeShaderDesc.pByteCode = pComputeBytes;
		computeShaderDesc.ByteCodeLength = computeByteCount;
		computeShaderDesc.EntryMethod = "main";
		computeShaderDesc.Stage = ShaderStage::Compute;

		Shader* pComputeShader = pDevice->create_shader(computeShaderDesc);

		//Create descriptor set pools
		DescriptorPoolDesc descriptorHostPoolDesc = {};
		descriptorHostPoolDesc.Sizes =
		{
			{DescriptorResourceType::StorageBuffer,5},
			{DescriptorResourceType::StorageTexture,5}
		};
		descriptorHostPoolDesc.Type = DescriptorMemoryType::Host;
		DescriptorPool* pDescriptorHostPool = pDevice->create_descriptor_pool(descriptorHostPoolDesc);

		DescriptorPoolDesc descriptorDevicePoolDesc = {};
		descriptorDevicePoolDesc.Sizes =
		{
			{DescriptorResourceType::StorageBuffer,5},
			{DescriptorResourceType::StorageTexture,5}
		};
		descriptorDevicePoolDesc.Type = DescriptorMemoryType::Device;
		DescriptorPool* pDescriptorDevicePool = pDevice->create_descriptor_pool(descriptorDevicePoolDesc);

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
		DescriptorSetLayout* pComputeSetLayout = pDevice->create_descriptor_set_layout(computeDescriptorLayoutDesc);

		ComputePipelineDesc computePipelineDesc = {};
		computePipelineDesc.DescriptorSetLayouts.push_back(pComputeSetLayout);
		computePipelineDesc.pComputeShader = pComputeShader;
		Pipeline* pComputePipeline = pDevice->create_compute_pipeline(computePipelineDesc);

		//Invalidate scene for render pass and pipelines
		std::vector<RenderPass*> renderPasses;
		InvalidateScene(pDevice, pDeviceMemory, pSwapchain, pCmdList, pFence, pQueue, pComputeShader, nullptr, renderPasses);

		//Create descriptor sets
		DescriptorSetDesc hostDescriptorSetDesc = {};
		hostDescriptorSetDesc.pLayout = pComputeSetLayout;
		hostDescriptorSetDesc.pPool = pDescriptorHostPool;
		DescriptorSet* pHostDescriptorSet = pDevice->create_descriptor_set(hostDescriptorSetDesc);

		std::vector<DescriptorSet*> descriptorSets;

		DescriptorSetDesc deviceDescriptorSetDesc0 = {};
		deviceDescriptorSetDesc0.pLayout = pComputeSetLayout;
		deviceDescriptorSetDesc0.pPool = pDescriptorDevicePool;
		descriptorSets.push_back(pDevice->create_descriptor_set(deviceDescriptorSetDesc0));

		DescriptorSetDesc deviceDescriptorSetDesc1 = {};
		deviceDescriptorSetDesc1.pLayout = pComputeSetLayout;
		deviceDescriptorSetDesc1.pPool = pDescriptorDevicePool;
		descriptorSets.push_back(pDevice->create_descriptor_set(deviceDescriptorSetDesc1));

		DescriptorSetDesc deviceDescriptorSetDesc2 = {};
		deviceDescriptorSetDesc2.pLayout = pComputeSetLayout;
		deviceDescriptorSetDesc2.pPool = pDescriptorDevicePool;
		descriptorSets.push_back(pDevice->create_descriptor_set(deviceDescriptorSetDesc2));

		{
			DescriptorSetUpdateDesc descriptorSetUpdate = {};
			descriptorSetUpdate.Entries =
			{
				{pSwapchain->buffers()[0],DescriptorResourceType::StorageTexture,1,0,0,0},
				{pParticleBuffer,DescriptorResourceType::StorageBuffer,1,0,0,1}
			};
			pDevice->update_host_descriptor_set(pHostDescriptorSet, descriptorSetUpdate);

			DescriptorSetCopyDesc descriptorSetCopyDesc = {};
			descriptorSetCopyDesc.Entries =
			{
				{0,0,0,0,1},
				{1,0,1,0,1}
			};
			pDevice->copy_descriptor_set(pHostDescriptorSet, descriptorSets[0], descriptorSetCopyDesc);
		}

		{
			DescriptorSetUpdateDesc descriptorSetUpdate = {};
			descriptorSetUpdate.Entries =
			{
				{pSwapchain->buffers()[1],DescriptorResourceType::StorageTexture,1,0,0,0},
				{pParticleBuffer,DescriptorResourceType::StorageBuffer,1,0,0,1}
			};
			pDevice->update_host_descriptor_set(pHostDescriptorSet, descriptorSetUpdate);

			DescriptorSetCopyDesc descriptorSetCopyDesc = {};
			descriptorSetCopyDesc.Entries =
			{
				{0,0,0,0,1},
				{1,0,1,0,1}
			};
			pDevice->copy_descriptor_set(pHostDescriptorSet, descriptorSets[1], descriptorSetCopyDesc);
		}

		{
			DescriptorSetUpdateDesc descriptorSetUpdate = {};
			descriptorSetUpdate.Entries =
			{
				{pSwapchain->buffers()[2],DescriptorResourceType::StorageTexture,1,0,0,0},
				{pParticleBuffer,DescriptorResourceType::StorageBuffer,1,0,0,1}
			};
			pDevice->update_host_descriptor_set(pHostDescriptorSet, descriptorSetUpdate);

			DescriptorSetCopyDesc descriptorSetCopyDesc = {};
			descriptorSetCopyDesc.Entries =
			{
				{0,0,0,0,1},
				{1,0,1,0,1}
			};
			pDevice->copy_descriptor_set(pHostDescriptorSet, descriptorSets[2], descriptorSetCopyDesc);
		}

		//Create semaphores
		SemaphoreDesc semaphoreDesc = {};
		semaphoreDesc.bSignalled = false;
		Semaphore* pSemaphore = pDevice->create_sync_object(semaphoreDesc);
		Semaphore* pSemaphore2 = pDevice->create_sync_object(semaphoreDesc);

		//Create scene variables
		unsigned char PresentIndex = 0;
		const glm::vec3 cameraPosition = { 0,0,-5 };
		const glm::vec3 relativeUp = { 0,1,0 };

		while (pWindow->active())
		{
			Texture* pSwapchainTexture = pSwapchain->buffers()[PresentIndex];
			DescriptorSet* pSwapchainDescriptorSet = descriptorSets[PresentIndex];

			//Poll events
			pWindow->poll_events();

			//Get screen size
			const unsigned int screenWidth = pWindow->width();
			const unsigned int screenHeight = pWindow->height();

			//resize the render pass and swapchain
			unsigned int currentScreenWidth = screenWidth;
			unsigned int currentScreenHeight = screenHeight;
			for (const WindowEventData& eventData : pWindow->buffered_events())
			{
				if (eventData.Type == WindowEventType::Windowresized)
				{
					currentScreenWidth = eventData.WindowSize[0];
					currentScreenHeight = eventData.WindowSize[1];
				}
			}
			if (currentScreenWidth != screenWidth || currentScreenHeight != screenHeight)
			{
				pSwapchain->resize(currentScreenWidth, currentScreenHeight);
				InvalidateScene(pDevice, pDeviceMemory, pSwapchain, pCmdList, pFence, pQueue, pComputeShader, nullptr, renderPasses);
			}

			//Record and submit commands
			pCmdList->begin();

			TextureMemoryBarrierDesc computeOwnershipBarrierDesc = {};
			computeOwnershipBarrierDesc.AspectFlags = TextureAspectFlags::Color;
			computeOwnershipBarrierDesc.ArrayIndex = 0;
			computeOwnershipBarrierDesc.MipIndex = 0;
			computeOwnershipBarrierDesc.SourceAccessFlags = GraphicsMemoryAccessFlags::Unknown;
			computeOwnershipBarrierDesc.SourceLayout = TextureMemoryLayout::Unknown;
			computeOwnershipBarrierDesc.SourceQueue = GraphicsQueueFamilyType::Graphics;
			computeOwnershipBarrierDesc.SourceStageFlags = PipelineStageFlags::TopOfPipe;
			computeOwnershipBarrierDesc.DestinationAccessFlags = GraphicsMemoryAccessFlags::ShaderWrite;
			computeOwnershipBarrierDesc.DestinationLayout = TextureMemoryLayout::General;
			computeOwnershipBarrierDesc.DestinationQueue = GraphicsQueueFamilyType::Compute;
			computeOwnershipBarrierDesc.DestinationStageFlags = PipelineStageFlags::ComputeShader;
			pCmdList->set_texture_barrier(pSwapchainTexture, computeOwnershipBarrierDesc);

			pCmdList->end();

			pDevice->submit_commands(&pCmdList, 1, pQueue, &pSemaphore, 1, nullptr, nullptr, 0,nullptr);

			pComputeCmdList->begin();

			TextureMemoryBarrierDesc computeBarrierDesc = {};
			computeBarrierDesc.AspectFlags = TextureAspectFlags::Color;
			computeBarrierDesc.ArrayIndex = 0;
			computeBarrierDesc.MipIndex = 0;
			computeBarrierDesc.SourceAccessFlags = GraphicsMemoryAccessFlags::Unknown;
			computeBarrierDesc.SourceLayout = TextureMemoryLayout::Unknown;
			computeBarrierDesc.SourceQueue = GraphicsQueueFamilyType::Compute;
			computeBarrierDesc.SourceStageFlags = PipelineStageFlags::TopOfPipe;
			computeBarrierDesc.DestinationAccessFlags = GraphicsMemoryAccessFlags::ShaderWrite;
			computeBarrierDesc.DestinationLayout = TextureMemoryLayout::General;
			computeBarrierDesc.DestinationQueue = GraphicsQueueFamilyType::Compute;
			computeBarrierDesc.DestinationStageFlags = PipelineStageFlags::ComputeShader;
			pComputeCmdList->set_texture_barrier(pSwapchainTexture, computeBarrierDesc);

			pComputeCmdList->set_pipeline(pComputePipeline);
			pComputeCmdList->commit_resource_sets(&pSwapchainDescriptorSet, 1);
			pComputeCmdList->dispatch(pSwapchain->width() / 256, pSwapchain->height(), 1);

			TextureMemoryBarrierDesc PresentOwnershipBarrierDesc = {};
			PresentOwnershipBarrierDesc.AspectFlags = TextureAspectFlags::Color;
			PresentOwnershipBarrierDesc.ArrayIndex = 0;
			PresentOwnershipBarrierDesc.MipIndex = 0;
			PresentOwnershipBarrierDesc.SourceAccessFlags = GraphicsMemoryAccessFlags::ShaderWrite;
			PresentOwnershipBarrierDesc.SourceLayout = TextureMemoryLayout::General;
			PresentOwnershipBarrierDesc.SourceQueue = GraphicsQueueFamilyType::Compute;
			PresentOwnershipBarrierDesc.SourceStageFlags = PipelineStageFlags::ComputeShader;
			PresentOwnershipBarrierDesc.DestinationAccessFlags = GraphicsMemoryAccessFlags::ShaderWrite;
			PresentOwnershipBarrierDesc.DestinationLayout = TextureMemoryLayout::General;
			PresentOwnershipBarrierDesc.DestinationQueue = GraphicsQueueFamilyType::Graphics;
			PresentOwnershipBarrierDesc.DestinationStageFlags = PipelineStageFlags::ComputeShader;
			pComputeCmdList->set_texture_barrier(pSwapchainTexture, PresentOwnershipBarrierDesc);

			const PipelineStageFlags computeWaitFlags = PipelineStageFlags::ComputeShader;
			pComputeCmdList->end();
			pDevice->submit_commands(&pComputeCmdList, 1, pComputeQueue, &pSemaphore2, 1, &pSemaphore, &computeWaitFlags, 1, pFence);
			pDevice->wait_fences(&pFence, 1);
			pDevice->reset_fences(&pFence, 1);

			pCmdList->begin();
			TextureMemoryBarrierDesc PresentBarrierDesc = {};
			PresentBarrierDesc.AspectFlags = TextureAspectFlags::Color;
			PresentBarrierDesc.ArrayIndex = 0;
			PresentBarrierDesc.MipIndex = 0;
			PresentBarrierDesc.SourceAccessFlags = GraphicsMemoryAccessFlags::Unknown;
			PresentBarrierDesc.SourceLayout = TextureMemoryLayout::Unknown;
			PresentBarrierDesc.SourceQueue = GraphicsQueueFamilyType::Graphics;
			PresentBarrierDesc.SourceStageFlags = PipelineStageFlags::TopOfPipe;
			PresentBarrierDesc.DestinationAccessFlags = GraphicsMemoryAccessFlags::ColorAttachmentRead;
			PresentBarrierDesc.DestinationLayout = TextureMemoryLayout::Present;
			PresentBarrierDesc.DestinationAccessFlags = GraphicsMemoryAccessFlags::ColorAttachmentRead;
			PresentBarrierDesc.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;
			pCmdList->set_texture_barrier(pSwapchainTexture, PresentBarrierDesc);
			pCmdList->end();

			const PipelineStageFlags PresentWaitFlags = PipelineStageFlags::ComputeShader;
			pDevice->submit_commands(&pCmdList, 1, pQueue, &pSemaphore, 1, &pSemaphore2, &PresentWaitFlags, 1, pFence);

			pDevice->wait_fences(&pFence, 1);
			pDevice->reset_fences(&pFence, 1);

			//Present and wait for the Present to finish
			pSwapchain->Present(&pSemaphore,1);
			pSwapchain->wait_present(PresentIndex);
			PresentIndex = (PresentIndex + 1) % swapchainDesc.BufferCount;
		}
	}
}
int main(const unsigned int count, const char** ppArguments)
{
	Dream::Run();
	return 0;
}

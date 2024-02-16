#include <Runtime/Window/Window.h>
#include <Runtime/Monitor/Monitor.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/ShaderCompiler/ShaderCompiler.h>
#include <stb_image.h>

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
	static const char vertexShaderSource[] =
		"struct VS_INPUT\
            {\
              float2 pos : POSITION;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
              PS_INPUT output;\
              output.pos = float4(input.pos.xy, 0.f, 1.f);\
              output.uv  = input.uv;\
              return output;\
            }";

	static const char pixelShaderSource[] =
		"struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float2 uv  : TEXCOORD0;\
            };\
            layout(set=0,binding=0) sampler sampler0;\
            layout(set=0,binding=1) Texture2D texture0;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
            float4 out_col = texture0.Sample(sampler0,input.uv) + float4(input.uv.x,input.uv.y,0,1.0f); \
            return out_col; \
            }";

	void InvalidateScene(GraphicsDevice* pDevice, Swapchain* pSwapchain, CommandList* pCmdList, Fence* pFence, GraphicsQueue* pQueue, Shader* pVertexShader, Shader* pFragmentShader, DescriptorSetLayout* pSetLayout, std::vector<RenderPass*>& passes, std::vector<Pipeline*>& pipelines)
	{
		//Clear the exsiting pipelines
		for (Pipeline* pPipeline : pipelines)
			delete pPipeline;
		pipelines.clear();

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
		pDevice->SubmitCommands(&pCmdList, 1, pQueue, pFence);
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

			RenderPass* pRenderPass = pDevice->CreateRenderPass(renderPassDesc);
			passes.push_back(pRenderPass);

			//Create pipeline
			GraphicsPipelineDesc pipelineDesc = {};
			pipelineDesc.SubpassIndex = 0;

			pipelineDesc.Viewport.X = 0;
			pipelineDesc.Viewport.Y = 0;
			pipelineDesc.Viewport.Width = pTexture->GetWidth();
			pipelineDesc.Viewport.Height = pTexture->GetHeight();

			BlendStateDesc blendStateDesc = {};
			blendStateDesc.bLogicOperationEnabled = false;
			blendStateDesc.LogicOperation = LogicOperation::Clear;
			BlendStateAttachment blendAttachment = {};
			blendAttachment.bEnabled = false;
			blendAttachment.SourceColorFactor = BlendFactor::One;
			blendAttachment.DestinationColorFactor = BlendFactor::One;
			blendAttachment.ColorOperation = BlendOperation::Add;
			blendAttachment.SourceAlphaFactor = BlendFactor::One;
			blendAttachment.DestinationAlphaFactor = BlendFactor::One;
			blendAttachment.AlphaOperation = BlendOperation::Add;
			blendAttachment.WriteMask = BlendColorWriteMask::All;
			blendStateDesc.Attachments.push_back(blendAttachment);
			pipelineDesc.BlendState = blendStateDesc;

			DepthStencilStateDesc depthStencilStateDesc = {};
			depthStencilStateDesc.bDepthTestEnabled = false;
			depthStencilStateDesc.bDepthWriteEnabled = false;
			depthStencilStateDesc.bStencilTestEnabled = false;
			depthStencilStateDesc.DepthTestOperation = CompareOperation::Never;
			depthStencilStateDesc.StencilBackFace = {};
			depthStencilStateDesc.StencilFrontFace = {};
			pipelineDesc.DepthStencilState = depthStencilStateDesc;

			pipelineDesc.GraphicsShaders = { pVertexShader,pFragmentShader };

			InputLayoutDesc inputLayoutDesc = {};
			inputLayoutDesc.Topology = MeshTopology::TriangleList;
			InputBinding inputBinding = {};
			inputBinding.StepRate = InputBindingStepRate::Vertex;
			inputBinding.Elements.push_back({ TextureFormat::R32_G32_Float,InputElementSemantic::Position });
			inputBinding.Elements.push_back({ TextureFormat::R32_G32_Float,InputElementSemantic::TexCoord });
			inputLayoutDesc.Bindings.push_back(inputBinding);
			pipelineDesc.InputLayout = inputLayoutDesc;

			pipelineDesc.Multisample.bSampleShadingEnabled = false;
			pipelineDesc.Multisample.Samples = TextureSampleCount::SAMPLE_COUNT_1;

			pipelineDesc.pRenderPass = pRenderPass;

			RasterizerStateDesc rasterizerStateDesc = {};
			rasterizerStateDesc.bDepthBiasEnabled = false;
			rasterizerStateDesc.bFrontCounterClockwise = true;
			rasterizerStateDesc.CullFlags = FaceCullMode::None;
			rasterizerStateDesc.DepthBiasClamp = 0;
			rasterizerStateDesc.DepthBiasFactor = 0;
			rasterizerStateDesc.DepthBiasSlope = 0;
			rasterizerStateDesc.FillMode = PolygonMode::Fill;
			pipelineDesc.RasterizerState = rasterizerStateDesc;

			pipelineDesc.ResourceLayout.ResourceLayouts.push_back(pSetLayout);

			Pipeline* pPipeline = pDevice->CreateGraphicsPipeline(pipelineDesc);
			pipelines.push_back(pPipeline);

		}
	}
	void Run()
	{
		//Create window
		WindowDesc windowDesc = {};
		windowDesc.Title = "HelloTriangle Sample";
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

		//Allocate memory
		GraphicsMemoryDesc hostMemoryDesc = {};
		hostMemoryDesc.SizeInBytes = MB_TO_BYTE(1024);
		hostMemoryDesc.Type = GraphicsMemoryType::Host;
		GraphicsMemory* pHostMemory = pDevice->AllocateMemory(hostMemoryDesc);

		GraphicsMemoryDesc deviceMemoryDesc = {};
		deviceMemoryDesc.SizeInBytes = MB_TO_BYTE(1024);
		deviceMemoryDesc.Type = GraphicsMemoryType::Device;
		GraphicsMemory* pDeviceMemory = pDevice->AllocateMemory(deviceMemoryDesc);

		//Create vertex buffer
		struct Vertex
		{
			float X;
			float Y;

			float U;
			float V;
		};

		constexpr Vertex vertexes[] =
		{
			{-0.5f,-0.5f,0,0},
			{-0.5f,0.5f,0.0f,1.0f},
			{0.5f,0.5f,1.0f,1.0f},
			{0.5f,-0.5f,1.0f,0.0f},
		};

		GraphicsBufferDesc vertexBufferDesc = {};
		vertexBufferDesc.Usage = GraphicsBufferUsage::VertexBuffer | GraphicsBufferUsage::TransferDestination;
		vertexBufferDesc.SubItemCount = sizeof(vertexes) / sizeof(Vertex);
		vertexBufferDesc.SubItemSizeInBytes = sizeof(Vertex);
		vertexBufferDesc.pMemory = pDeviceMemory;
		GraphicsBuffer* pVertexBuffer = pDevice->CreateBuffer(vertexBufferDesc);

		//Create index buffer
		constexpr unsigned short indexes[] =
		{
			0,1,2,
			0,2,3
		};

		GraphicsBufferDesc indexBufferDesc = {};
		indexBufferDesc.Usage = GraphicsBufferUsage::IndexBuffer | GraphicsBufferUsage::TransferDestination;
		indexBufferDesc.SubItemCount = sizeof(indexes) / sizeof(unsigned short);
		indexBufferDesc.SubItemSizeInBytes = sizeof(unsigned short);
		indexBufferDesc.pMemory = pDeviceMemory;
		GraphicsBuffer* pIndexBuffer = pDevice->CreateBuffer(indexBufferDesc);

		//Create constant buffer
		struct Color
		{
			float R;
			float G;
			float B;
			float A;
		};
		constexpr Color color = { 1.0f,0,0,1.0f };

		GraphicsBufferDesc constantBufferDesc = {};
		constantBufferDesc.Usage = GraphicsBufferUsage::ConstantBuffer | GraphicsBufferUsage::TransferDestination;
		constantBufferDesc.SubItemCount = 1;
		constantBufferDesc.SubItemSizeInBytes = sizeof(color);
		constantBufferDesc.pMemory = pDeviceMemory;

		GraphicsBuffer* pConstantBuffer = pDevice->CreateBuffer(constantBufferDesc);

		//Create staging buffers
		GraphicsBufferDesc vertexStageBufferDesc = {};
		vertexStageBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
		vertexStageBufferDesc.SubItemCount = sizeof(vertexes) / sizeof(Vertex);
		vertexStageBufferDesc.SubItemSizeInBytes = sizeof(Vertex);
		vertexStageBufferDesc.pMemory = pHostMemory;

		GraphicsBufferDesc indexStageBufferDesc = {};
		indexStageBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
		indexStageBufferDesc.SubItemCount = sizeof(indexes) / sizeof(unsigned short);
		indexStageBufferDesc.SubItemSizeInBytes = sizeof(unsigned short);
		indexStageBufferDesc.pMemory = pHostMemory;

		GraphicsBufferDesc constantStageBufferDesc = {};
		constantStageBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
		constantStageBufferDesc.SubItemCount = 1;
		constantStageBufferDesc.SubItemSizeInBytes = sizeof(color);
		constantStageBufferDesc.pMemory = pHostMemory;

		GraphicsBuffer* pVertexStageBuffer = pDevice->CreateBuffer(vertexStageBufferDesc);
		GraphicsBuffer* pIndexStageBuffer = pDevice->CreateBuffer(indexStageBufferDesc);
		GraphicsBuffer* pConstantStageBuffer = pDevice->CreateBuffer(constantStageBufferDesc);

		//Update stage buffers
		HostBufferUpdateDesc vertexStageBufferUpdateDesc = {};
		vertexStageBufferUpdateDesc.OffsetInBytes = 0;
		vertexStageBufferUpdateDesc.SizeInBytes = sizeof(vertexes);
		vertexStageBufferUpdateDesc.pBuffer = (unsigned char*)vertexes;
		pDevice->UpdateHostBuffer(pVertexStageBuffer, vertexStageBufferUpdateDesc);

		HostBufferUpdateDesc indexStageBufferUpdateDesc = {};
		indexStageBufferUpdateDesc.OffsetInBytes = 0;
		indexStageBufferUpdateDesc.SizeInBytes = sizeof(indexes);
		indexStageBufferUpdateDesc.pBuffer = (unsigned char*)indexes;
		pDevice->UpdateHostBuffer(pIndexStageBuffer, indexStageBufferUpdateDesc);

		HostBufferUpdateDesc constantBufferUpdateDesc = {};
		constantBufferUpdateDesc.OffsetInBytes = 0;
		constantBufferUpdateDesc.SizeInBytes = sizeof(color);
		constantBufferUpdateDesc.pBuffer = (unsigned char*)&color;
		pDevice->UpdateHostBuffer(pConstantStageBuffer, constantBufferUpdateDesc);

		//Create sampler
		SamplerDesc samplerDesc = {};
		samplerDesc.AddressModeU = SamplerAddressMode::Repeat;
		samplerDesc.AddressModeV = SamplerAddressMode::Repeat;
		samplerDesc.AddressModeW = SamplerAddressMode::Repeat;
		samplerDesc.bComparisonEnabled = false;
		samplerDesc.BorderColor = SamplerBorderColor::TransparentBlack;
		samplerDesc.CompareOperation = CompareOperation::Never;
		samplerDesc.MagFilter = SamplerFiltering::Linear;
		samplerDesc.MinFilter = SamplerFiltering::Linear;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.MaxLod = 0;
		samplerDesc.MinLod = 0;
		samplerDesc.MipLodBias = 0;
		samplerDesc.MipmapMode = SamplerMapMode::Linear;
		Sampler* pSampler = pDevice->CreateSampler(samplerDesc);

		//Load texture
		int textureWidth = 0;
		int textureHeight = 0;
		int textureBitsPerPixel = 0;
		unsigned char* pTextureData = stbi_load("texture.png", &textureWidth, &textureHeight, &textureBitsPerPixel, 4);

		//Create texture
		TextureDesc textureDesc = {};
		textureDesc.Type = TextureType::Texture2D;
		textureDesc.Format = TextureFormat::R8_G8_B8_A8_UNorm;
		textureDesc.Usages = TextureUsage::Sampled | TextureUsage::TransferDestination;
		textureDesc.Width = textureWidth;
		textureDesc.Height = textureHeight;
		textureDesc.Depth = 1;
		textureDesc.ArrayLevels = 1;
		textureDesc.MipLevels = 1;
		textureDesc.SampleCount = TextureSampleCount::SAMPLE_COUNT_1;
		textureDesc.pMemory = pDeviceMemory;
		Texture* pTexture = pDevice->CreateTexture(textureDesc);

		//Create texture view
		TextureViewDesc textureViewDesc = {};
		textureViewDesc.ArrayLevel = 0;
		textureViewDesc.MipLevel = 0;
		textureViewDesc.pTexture = pTexture;
		TextureView* pTextureView = pDevice->CreateTextureView(textureViewDesc);

		//Create texture stage buffer
		GraphicsBufferDesc textureStageBufferDesc = {};
		textureStageBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
		textureStageBufferDesc.SubItemCount = 1;
		textureStageBufferDesc.SubItemSizeInBytes = textureWidth * textureHeight * 4;
		textureStageBufferDesc.pMemory = pHostMemory;
		GraphicsBuffer* pTextureStageBuffer = pDevice->CreateBuffer(textureStageBufferDesc);

		//Update texture stage buffer
		HostBufferUpdateDesc textureStageBufferUpdateDesc = {};
		textureStageBufferUpdateDesc.OffsetInBytes = 0;
		textureStageBufferUpdateDesc.pBuffer = pTextureData;
		textureStageBufferUpdateDesc.SizeInBytes = textureStageBufferDesc.SubItemSizeInBytes;
		pDevice->UpdateHostBuffer(pTextureStageBuffer,textureStageBufferUpdateDesc);
		
		//Update buffers and texture layouts
		pCmdList->BeginRecording();

		BufferBufferCopyDesc vertexBufferCopyDesc = {};
		vertexBufferCopyDesc.DestinationOffsetInBytes = 0;
		vertexBufferCopyDesc.SizeInBytes = sizeof(vertexes);
		vertexBufferCopyDesc.SourceOffsetInBytes = 0;
		pCmdList->CopyBufferToBuffer(pVertexStageBuffer, pVertexBuffer, vertexBufferCopyDesc);

		BufferBufferCopyDesc indexBufferCopyDesc = {};
		indexBufferCopyDesc.DestinationOffsetInBytes = 0;
		indexBufferCopyDesc.SizeInBytes = sizeof(indexes);
		indexBufferCopyDesc.SourceOffsetInBytes = 0;
		pCmdList->CopyBufferToBuffer(pIndexStageBuffer, pIndexBuffer, indexBufferCopyDesc);

		BufferBufferCopyDesc constantBufferCopyDesc = {};
		constantBufferCopyDesc.DestinationOffsetInBytes = 0;
		constantBufferCopyDesc.SizeInBytes = sizeof(color);
		constantBufferCopyDesc.SourceOffsetInBytes = 0;
		pCmdList->CopyBufferToBuffer(pConstantStageBuffer, pConstantBuffer, constantBufferCopyDesc);

		TextureMemoryBarrierDesc preTextureBarrier = {};
		preTextureBarrier.MipIndex = 0;
		preTextureBarrier.ArrayIndex = 0;
		preTextureBarrier.SourceAccessFlags = GraphicsMemoryAccessFlags::Unknown;
		preTextureBarrier.SourceLayout = TextureMemoryLayout::Unknown;
		preTextureBarrier.SourceQueue = GraphicsQueueType::Graphics;
		preTextureBarrier.SourceStageFlags = PipelineStageFlags::TopOfPipe;
		preTextureBarrier.DestinationAccessFlags = GraphicsMemoryAccessFlags::TransferWrite;
		preTextureBarrier.DestinationLayout = TextureMemoryLayout::TransferDestination;
		preTextureBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		preTextureBarrier.DestinationStageFlags = PipelineStageFlags::Transfer;
		preTextureBarrier.AspectFlags = TextureAspectFlags::Color;
		pCmdList->SetTextureMemoryBarrier(pTexture, preTextureBarrier);

		BufferTextureCopyDesc textureCopyDesc = {};
		textureCopyDesc.BufferOffsetInBytes = 0;
		textureCopyDesc.Width = pTexture->GetWidth();
		textureCopyDesc.Height = pTexture->GetHeight();
		textureCopyDesc.Depth = pTexture->GetDepth();
		textureCopyDesc.TextureOffsetX = 0;
		textureCopyDesc.TextureOffsetY = 0;
		textureCopyDesc.TextureOffsetZ = 0;
		textureCopyDesc.TargetArrayIndex = 0;
		textureCopyDesc.TargetMipIndex = 0;
		pCmdList->CopyBufferToTexture(pTextureStageBuffer, pTexture, textureCopyDesc);

		TextureMemoryBarrierDesc postTextureBarrier = {};
		postTextureBarrier.MipIndex = 0;
		postTextureBarrier.ArrayIndex = 0;
		postTextureBarrier.SourceAccessFlags = GraphicsMemoryAccessFlags::TransferWrite;
		postTextureBarrier.SourceLayout = TextureMemoryLayout::TransferDestination;
		postTextureBarrier.SourceQueue = GraphicsQueueType::Graphics;
		postTextureBarrier.SourceStageFlags = PipelineStageFlags::Transfer;
		postTextureBarrier.DestinationAccessFlags = GraphicsMemoryAccessFlags::ShaderRead;
		postTextureBarrier.DestinationLayout = TextureMemoryLayout::ShaderReadOnly;
		postTextureBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		postTextureBarrier.DestinationStageFlags = PipelineStageFlags::FragmentShader;
		postTextureBarrier.AspectFlags = TextureAspectFlags::Color;
		pCmdList->SetTextureMemoryBarrier(pTexture, postTextureBarrier);

		pCmdList->EndRecording();
		pDevice->SubmitCommands(&pCmdList, 1, pQueue, pFence);
		pDevice->WaitFences(&pFence, 1);
		pDevice->ResetFences(&pFence, 1);

		//Compile shaders to spirv
		unsigned char* pVertexSPIRVBytes = nullptr;
		unsigned int vertexSPIRVByteCount = 0;
		std::string vertexSPIRVCompilationError;
		ShaderCompiler::CompileToSPIRV(vertexShaderSource, "main", ShaderStage::Vertex, ShaderLanguage::HLSL, &pVertexSPIRVBytes, vertexSPIRVByteCount, vertexSPIRVCompilationError);

		unsigned char* pFragmentSPIRVBytes = nullptr;
		unsigned int fragmentSPIRVByteCount = 0;
		std::string fragmentSPIRVCompilationError;
		ShaderCompiler::CompileToSPIRV(pixelShaderSource, "main", ShaderStage::Fragment, ShaderLanguage::HLSL, &pFragmentSPIRVBytes, fragmentSPIRVByteCount, fragmentSPIRVCompilationError);

		//Compile spirv shaders to backend
		unsigned char* pVertexShaderBytes = nullptr;
		unsigned int vertexShaderByteCount = 0;
		std::string vertexShaderCompilationError;
		ShaderCompiler::CompileFromSPIRV(pVertexSPIRVBytes, vertexSPIRVByteCount, pDevice->GetBackend(), &pVertexShaderBytes, vertexShaderByteCount, vertexShaderCompilationError);

		unsigned char* pFragmentShaderBytes = nullptr;
		unsigned int fragmentShaderByteCount = 0;
		std::string fragmentShaderCompilationError;
		ShaderCompiler::CompileFromSPIRV(pFragmentSPIRVBytes, fragmentSPIRVByteCount, pDevice->GetBackend(), &pFragmentShaderBytes, fragmentShaderByteCount, fragmentShaderCompilationError);

		//Create shaders
		ShaderDesc vertexShaderDesc = {};
		vertexShaderDesc.EntryMethod = "main";
		vertexShaderDesc.Stage = ShaderStage::Vertex;
		vertexShaderDesc.pByteCode = pVertexShaderBytes;
		vertexShaderDesc.ByteCodeLength = vertexShaderByteCount;
		Shader* pVertexShader = pDevice->CreateShader(vertexShaderDesc);

		ShaderDesc fragmentShaderDesc = {};
		fragmentShaderDesc.EntryMethod = "main";
		fragmentShaderDesc.Stage = ShaderStage::Fragment;
		fragmentShaderDesc.pByteCode = pFragmentShaderBytes;
		fragmentShaderDesc.ByteCodeLength = fragmentShaderByteCount;
		Shader* pFragmentShader = pDevice->CreateShader(fragmentShaderDesc);

		//Create descriptor pool
		DescriptorPoolDesc hostDescriptorPoolDesc = {};
		hostDescriptorPoolDesc.Type = DescriptorMemoryType::Host;
		hostDescriptorPoolDesc.SetCount = 3;
		hostDescriptorPoolDesc.Sizes.push_back({ DescriptorResourceType::SampledTexture,1 });
		hostDescriptorPoolDesc.Sizes.push_back({ DescriptorResourceType::Sampler,1 });
		DescriptorPool* pDescriptorHostPool = pDevice->CreateDescriptorPool(hostDescriptorPoolDesc);

		DescriptorPoolDesc deviceDescriptorPoolDesc = {};
		deviceDescriptorPoolDesc.Type = DescriptorMemoryType::Device;
		deviceDescriptorPoolDesc.SetCount = 3;
		deviceDescriptorPoolDesc.Sizes.push_back({ DescriptorResourceType::SampledTexture,1 });
		deviceDescriptorPoolDesc.Sizes.push_back({ DescriptorResourceType::Sampler,1 });
		DescriptorPool* pDescriptorDevicePool = pDevice->CreateDescriptorPool(deviceDescriptorPoolDesc);

		//Create descriptor set layout
		DescriptorSetLayoutDesc setLayoutDesc = {};
		{
			DescriptorSetLayoutEntry textureEntry = {};
			textureEntry.Binding = 0;
			textureEntry.Stages = ShaderStage::Fragment;
			textureEntry.Type = DescriptorResourceType::Sampler;
			setLayoutDesc.Entries.push_back(textureEntry);

			DescriptorSetLayoutEntry samplerEntry = {};
			samplerEntry.Binding = 1;
			samplerEntry.Stages = ShaderStage::Fragment;
			samplerEntry.Type = DescriptorResourceType::SampledTexture;
			setLayoutDesc.Entries.push_back(samplerEntry);
		}
		DescriptorSetLayout* pDescriptorSetLayout = pDevice->CreateDescriptorSetLayout(setLayoutDesc);

		//Create descriptor set
		DescriptorSetDesc descriptorHostSetDesc = {};
		descriptorHostSetDesc.pLayout = pDescriptorSetLayout;
		descriptorHostSetDesc.pPool = pDescriptorHostPool;
		DescriptorSet* pDescriptorSetHost = pDevice->CreateDescriptorSet(descriptorHostSetDesc);

		DescriptorSetDesc descriptorSetDeviceDesc = {};
		descriptorSetDeviceDesc.pLayout = pDescriptorSetLayout;
		descriptorSetDeviceDesc.pPool = pDescriptorDevicePool;
		DescriptorSet* pDescriptorSetDevice = pDevice->CreateDescriptorSet(descriptorSetDeviceDesc);

		//Update and copy descriptor sets
		DescriptorSetUpdateDesc descriptorSetHostUpdateDesc = {};
		descriptorSetHostUpdateDesc.Entries =
		{
			{pSampler,DescriptorResourceType::Sampler,1,0,0,0},
			{pTextureView,DescriptorResourceType::SampledTexture,1,0,0,1}
		};
		pDevice->UpdateDescriptorSet(pDescriptorSetHost, descriptorSetHostUpdateDesc);

		DescriptorSetCopyDesc descriptorSetCopyDesc = {};
		descriptorSetCopyDesc.Entries =
		{
			{0,0,0,0,1},
			{1,0,1,0,1}
		};
		pDevice->CopyDescriptorSet(pDescriptorSetHost, pDescriptorSetDevice, descriptorSetCopyDesc);

		//Invalidate scene for render pass and pipelines

		std::vector<RenderPass*> renderPasses;
		std::vector<Pipeline*> pipelines;
		InvalidateScene(pDevice, pSwapchain, pCmdList, pFence, pQueue, pVertexShader, pFragmentShader, pDescriptorSetLayout, renderPasses, pipelines);

		//Create device
		unsigned char presentIndex = 0;
		while (pWindow->IsActive())
		{
			//Get screen size
			const unsigned int screenWidth = pWindow->GetWidth();
			const unsigned int screenHeight = pWindow->GetHeight();

			//Poll events
			pWindow->PollEvents();

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
				InvalidateScene(pDevice, pSwapchain, pCmdList, pFence, pQueue, pVertexShader, pFragmentShader, pDescriptorSetLayout, renderPasses, pipelines);
			}

			//Begin recording
			pCmdList->BeginRecording();

			//Start render pass
			constexpr float clearColor[] = { 100 / 255.0f,149 / 255.0f,237 / 255.0f,1 };

			pCmdList->BeginRenderPass(renderPasses[presentIndex], clearColor);

			pCmdList->SetVertexBuffers(&pVertexBuffer, 1);
			pCmdList->SetIndexBuffer(pIndexBuffer, IndexBufferType::UInt16);

			pCmdList->SetPipeline(pipelines[presentIndex]);
			ScissorDesc sc = {};
			sc.X = 0;
			sc.Y = 0;
			sc.Width = currentScreenWidth;
			sc.Height = currentScreenHeight;
			pCmdList->SetScissors(&sc, 1);
			pCmdList->CommitResourceSets(&pDescriptorSetDevice, 1);
			pCmdList->DrawIndexed(sizeof(indexes)/sizeof(unsigned short), 0, 0, 0, 1);

			//End render pass
			pCmdList->EndRenderPass();

			//End recording
			pCmdList->EndRecording();

			//Submit and wait for the commands to finish
			pDevice->SubmitCommands(&pCmdList, 1, pQueue, pFence);
			pDevice->WaitFences(&pFence, 1);
			pDevice->ResetFences(&pFence, 1);

			//Present and wait for the present to finish
			pSwapchain->Present();
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

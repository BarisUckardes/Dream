#include "GraphicsDevice.h"

namespace Dream
{
    GraphicsDevice::~GraphicsDevice()
    {

    }
    GraphicsQueue* GraphicsDevice::CreateQueue(const GraphicsQueueDesc& desc)
    {
        //First validate if device can give queue
        if (!HasQueue(desc.Type))
            return nullptr;

        GraphicsQueue* pQueue = CreateQueueCore(desc);

        RegisterObject(pQueue);

        return pQueue;
    }
    GraphicsBuffer* GraphicsDevice::CreateBuffer(const GraphicsBufferDesc& desc)
    {
        //Validations
        if (desc.pMemory == nullptr)
            return nullptr;

        GraphicsBuffer* pBuffer = CreateBufferCore(desc);

        RegisterObject(pBuffer);

        return pBuffer;
    }
    DescriptorSet* GraphicsDevice::CreateDescriptorSet(const DescriptorSetDesc& desc)
    {
        //Validations
        if (desc.pLayout == nullptr || desc.pPool == nullptr)
            return nullptr;

        DescriptorSet* pSet = CreateDescriptorSetCore(desc);

        RegisterObject(pSet);

        return pSet;
    }
    DescriptorPool* GraphicsDevice::CreateDescriptorPool(const DescriptorPoolDesc& desc)
    {
        DescriptorPool* pPool = CreateDescriptorPoolCore(desc);

        RegisterObject(pPool);

        return pPool;
    }
    DescriptorSetLayout* GraphicsDevice::CreateDescriptorSetLayout(const DescriptorSetLayoutDesc& desc)
    {
        DescriptorSetLayout* pLayout = CreateDescriptorSetLayoutCore(desc);

        RegisterObject(pLayout);

        return pLayout;
    }
    Fence* GraphicsDevice::CreateFence(const FenceDesc& desc)
    {
        Fence* pFence = CreateFenceCore(desc);

        RegisterObject(pFence);

        return pFence;
    }
    GraphicsMemory* GraphicsDevice::AllocateMemory(const GraphicsMemoryDesc& desc)
    {
        GraphicsMemory* pMemory = AllocateMemoryCore(desc);

        RegisterObject(pMemory);

        return pMemory;
    }
    Sampler* GraphicsDevice::CreateSampler(const SamplerDesc& desc)
    {
        Sampler* pSampler = CreateSamplerCore(desc);

        RegisterObject(pSampler);

        return pSampler;
    }
    Shader* GraphicsDevice::CreateShader(const ShaderDesc& desc)
    {
        Shader* pShader = CreateShaderCore(desc);

        RegisterObject(pShader);

        return pShader;
    }
    Texture* GraphicsDevice::CreateTexture(const TextureDesc& desc)
    {
        Texture* pTexture = CreateTextureCore(desc);

        RegisterObject(pTexture);

        return pTexture;
    }
    TextureView* GraphicsDevice::CreateTextureView(const TextureViewDesc& desc)
    {
        TextureView* pView = CreateTextureViewCore(desc);

        RegisterObject(pView);

        return pView;
    }
    Swapchain* GraphicsDevice::CreateSwapchain(const SwapchainDesc& desc)
    {
        Swapchain* pSwapchain = CreateSwapchainCore(desc);

        RegisterObject(pSwapchain);

        return pSwapchain;
    }
    Pipeline* GraphicsDevice::CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
    {
        Pipeline* pPipeline = CreateGraphicsPipelineCore(desc);

        RegisterObject(pPipeline);

        return pPipeline;
    }
    Pipeline* GraphicsDevice::CreateComputePipeline(const ComputePipelineDesc& desc)
    {
        Pipeline* pPipeline = CreateComputePipelineCore(desc);

        RegisterObject(pPipeline);

        return pPipeline;
    }
    RenderPass* GraphicsDevice::CreateRenderPass(const RenderPassDesc& desc)
    {
        RenderPass* pRenderPass = CreateRenderPassCore(desc);

        RegisterObject(pRenderPass);

        return pRenderPass;
    }
    CommandPool* GraphicsDevice::CreateCommandPool(const CommandPoolDesc& desc)
    {
        CommandPool* pCmdPool = CreateCommandPoolCore(desc);

        RegisterObject(pCmdPool);

        return pCmdPool;
    }
    CommandList* GraphicsDevice::CreateCommandList(const CommandListDesc& desc)
    {
        CommandList* pCmdList = CreateCommandListCore(desc);

        RegisterObject(pCmdList);

        return pCmdList;
    }
    void GraphicsDevice::UpdateDescriptorSet(DescriptorSet* pSet, const DescriptorSetUpdateDesc& desc)
    {
        UpdateDescriptorSetCore(pSet, desc);
    }
    void GraphicsDevice::CopyDescriptorSet(DescriptorSet* pSourceSet, DescriptorSet* pDestinationSet, const DescriptorSetCopyDesc& desc)
    {
        CopyDescriptorSetCore(pSourceSet, pDestinationSet,desc);
    }
    void GraphicsDevice::UpdateHostBuffer(GraphicsBuffer* pTargetBuffer, const HostBufferUpdateDesc& desc)
    {
        UpdateHostBufferCore(pTargetBuffer, desc);
    }
    void GraphicsDevice::SubmitCommands(CommandList** ppCmdLists, const unsigned char count,const GraphicsQueue* pTargetQueue,const Fence* pFence)
    {
        SubmitCommandsCore(ppCmdLists, count, pTargetQueue,pFence);
    }
    void GraphicsDevice::ResetFences(Fence** ppFences, const unsigned int count)
    {
        ResetFencesCore(ppFences, count);
    }
    void GraphicsDevice::WaitFences(Fence** ppFences, const unsigned int count)
    {
        WaitFencesCore(ppFences, count);
    }
    void GraphicsDevice::WaitDeviceIdle()
    {
        WaitDeviceIdleCore();
    }
    void GraphicsDevice::WaitQueueIdle(GraphicsQueue* pQueue)
    {
        WaitQueueIdleCore(pQueue);
    }
    GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc* pDesc)
    {
        mOwnerAdapter = pDesc->pOwnerAdapter;
    }
    void GraphicsDevice::RegisterObject(GraphicsDeviceObject* pObject)
    {
        mObjects.push_back(pObject);
    }
    void GraphicsDevice::RemoveObject(GraphicsDeviceObject* pObject)
    {
        auto findIterator = std::find(mObjects.begin(), mObjects.end(), pObject);
        if (findIterator == mObjects.end())
            return;

        mObjects.erase(findIterator);
    }
}

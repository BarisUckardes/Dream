#include "GraphicsDevice.h"

namespace Dream
{
    GraphicsDevice::~GraphicsDevice()
    {

    }
    GraphicsQueue* GraphicsDevice::own_queue(const GraphicsQueueDesc& desc)
    {
        //First validate if device can give queue
        if (!has_queue(desc.Type))
            return nullptr;

        GraphicsQueue* pQueue = own_queue_impl(desc);

        register_object(pQueue);

        return pQueue;
    }
    GraphicsBuffer* GraphicsDevice::create_buffer(const GraphicsBufferDesc& desc)
    {
        //Validations
        if (desc.pMemory == nullptr)
            return nullptr;

        GraphicsBuffer* pBuffer = create_buffer_impl(desc);

        register_object(pBuffer);

        return pBuffer;
    }
    DescriptorSet* GraphicsDevice::create_descriptor_set(const DescriptorSetDesc& desc)
    {
        //Validations
        if (desc.pLayout == nullptr || desc.pPool == nullptr)
            return nullptr;

        DescriptorSet* pSet = create_descriptor_set_impl(desc);

        register_object(pSet);

        return pSet;
    }
    DescriptorPool* GraphicsDevice::create_descriptor_pool(const DescriptorPoolDesc& desc)
    {
        DescriptorPool* pPool = create_descriptor_pool_impl(desc);

        register_object(pPool);

        return pPool;
    }
    DescriptorSetLayout* GraphicsDevice::create_descriptor_set_layout(const DescriptorSetLayoutDesc& desc)
    {
        DescriptorSetLayout* pLayout = create_descriptor_set_layout_impl(desc);

        register_object(pLayout);

        return pLayout;
    }
    Fence* GraphicsDevice::create_fence(const FenceDesc& desc)
    {
        Fence* pFence = create_fence_impl(desc);

        register_object(pFence);

        return pFence;
    }
    Semaphore* GraphicsDevice::create_sync_object(const SemaphoreDesc& desc)
    {
        Semaphore* pSemaphore = create_sync_object_impl(desc);

        register_object(pSemaphore);

        return pSemaphore;
    }
    GraphicsMemory* GraphicsDevice::allocate_memory(const GraphicsMemoryDesc& desc)
    {
        GraphicsMemory* pMemory = allocate_memory_impl(desc);

        register_object(pMemory);

        return pMemory;
    }
    Sampler* GraphicsDevice::create_sampler(const SamplerDesc& desc)
    {
        Sampler* pSampler = create_sampler_impl(desc);

        register_object(pSampler);

        return pSampler;
    }
    Shader* GraphicsDevice::create_shader(const ShaderDesc& desc)
    {
        Shader* pShader = create_shader_impl(desc);

        register_object(pShader);

        return pShader;
    }
    Texture* GraphicsDevice::create_texture(const TextureDesc& desc)
    {
        Texture* pTexture = create_texture_impl(desc);

        register_object(pTexture);

        return pTexture;
    }
    TextureView* GraphicsDevice::create_texture_view(const TextureViewDesc& desc)
    {
        TextureView* pView = create_texture_view_impl(desc);

        register_object(pView);

        return pView;
    }
    Swapchain* GraphicsDevice::create_swapchain(const SwapchainDesc& desc)
    {
        Swapchain* pSwapchain = create_swapchain_impl(desc);

        register_object(pSwapchain);

        return pSwapchain;
    }
    Pipeline* GraphicsDevice::create_graphics_pipeline(const GraphicsPipelineDesc& desc)
    {
        Pipeline* pPipeline = create_graphics_pipeline_impl(desc);

        register_object(pPipeline);

        return pPipeline;
    }
    Pipeline* GraphicsDevice::create_compute_pipeline(const ComputePipelineDesc& desc)
    {
        Pipeline* pPipeline = create_compute_pipeline_impl(desc);

        register_object(pPipeline);

        return pPipeline;
    }
    RenderPass* GraphicsDevice::create_render_pass(const RenderPassDesc& desc)
    {
        RenderPass* pRenderPass = create_render_pass_impl(desc);

        register_object(pRenderPass);

        return pRenderPass;
    }
    CommandPool* GraphicsDevice::create_cmd_pool(const CommandPoolDesc& desc)
    {
        CommandPool* pCmdPool = create_cmd_pool_impl(desc);

        register_object(pCmdPool);

        return pCmdPool;
    }
    CommandList* GraphicsDevice::create_cmd_list(const CommandListDesc& desc)
    {
        CommandList* pCmdList = create_cmd_list_impl(desc);

        register_object(pCmdList);

        return pCmdList;
    }
    void GraphicsDevice::update_host_descriptor_set(DescriptorSet* pSet, const DescriptorSetUpdateDesc& desc)
    {
        update_host_descriptor_set_impl(pSet, desc);
    }
    void GraphicsDevice::copy_descriptor_set(DescriptorSet* pSourceSet, DescriptorSet* pDestinationSet, const DescriptorSetCopyDesc& desc)
    {
        copy_descriptor_set_impl(pSourceSet, pDestinationSet,desc);
    }
    void GraphicsDevice::update_host_buffer(GraphicsBuffer* pTargetBuffer, const HostBufferUpdateDesc& desc)
    {
        update_host_buffer_impl(pTargetBuffer, desc);
    }
    void GraphicsDevice::submit_commands(CommandList** ppCmdLists, const unsigned char cmdListCount, const GraphicsQueue* pTargetQueue, Semaphore** ppSignalSemaphores, const unsigned int signalSemaphoreCount, Semaphore** ppWaitSemaphores,const PipelineStageFlags* pWaitStageFlags, const unsigned int waitSemaphoreCount, const Fence* pSignalFence)
    {
        submit_commands_impl(ppCmdLists, cmdListCount, pTargetQueue,ppSignalSemaphores,signalSemaphoreCount,ppWaitSemaphores,pWaitStageFlags,waitSemaphoreCount,pSignalFence);
    }
    void GraphicsDevice::reset_fences(Fence** ppFences, const unsigned int count)
    {
        reset_fences_impl(ppFences, count);
    }
    void GraphicsDevice::wait_fences(Fence** ppFences, const unsigned int count)
    {
        wait_fences_impl(ppFences, count);
    }
    void GraphicsDevice::wait_device_idle()
    {
        wait_device_idle_impl();
    }
    void GraphicsDevice::wait_queue_idle(GraphicsQueue* pQueue)
    {
        wait_queue_idle_impl(pQueue);
    }
    GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc* pDesc)
    {
        mOwnerAdapter = pDesc->pOwnerAdapter;
    }
    void GraphicsDevice::register_object(GraphicsDeviceObject* pObject)
    {
        mObjects.push_back(pObject);
    }
    void GraphicsDevice::remove_object(GraphicsDeviceObject* pObject)
    {
        auto findIterator = std::find(mObjects.begin(), mObjects.end(), pObject);
        if (findIterator == mObjects.end())
            return;

        mObjects.erase(findIterator);
    }
}

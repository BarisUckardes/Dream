#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/GraphicsBackend.h>
#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetLayout.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Semaphore/Semaphore.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Graphics/Sampler/Sampler.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureView.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Command/CommandList.h>
#include <Runtime/Graphics/Common/HostBufferUpdateDesc.h>
#include <vector>
#include <Runtime/Graphics/Common/DescriptorSetUpdateDesc.h>
#include <Runtime/Graphics/Common/DescriptorSetCopyDesc.h>

namespace Dream
{
	class RUNTIME_API GraphicsDevice
	{
	public:
		~GraphicsDevice();

		FORCEINLINE GraphicsAdapter* adapter() const noexcept
		{
			return mOwnerAdapter;
		}

		virtual GraphicsBackend backend() const noexcept = 0;
		virtual bool has_queue(const GraphicsQueueFamilyType type) const noexcept = 0;

		GraphicsQueue* own_queue(const GraphicsQueueDesc& desc);
		GraphicsBuffer* create_buffer(const GraphicsBufferDesc& desc);
		DescriptorSet* create_descriptor_set(const DescriptorSetDesc& desc);
		DescriptorPool* create_descriptor_pool(const DescriptorPoolDesc& desc);
		DescriptorSetLayout* create_descriptor_set_layout(const DescriptorSetLayoutDesc& desc);
		Fence* create_fence(const FenceDesc& desc);
		Semaphore* create_sync_object(const SemaphoreDesc& desc);
		GraphicsMemory* allocate_memory(const GraphicsMemoryDesc& desc);
		Sampler* create_sampler(const SamplerDesc& desc);
		Shader* create_shader(const ShaderDesc& desc);
		Texture* create_texture(const TextureDesc& desc);
		TextureView* create_texture_view(const TextureViewDesc& desc);
		Swapchain* create_swapchain(const SwapchainDesc& desc);
		Pipeline* create_graphics_pipeline(const GraphicsPipelineDesc& desc);
		Pipeline* create_compute_pipeline(const ComputePipelineDesc& desc);
		RenderPass* create_render_pass(const RenderPassDesc& desc);
		CommandPool* create_cmd_pool(const CommandPoolDesc& desc);
		CommandList* create_cmd_list(const CommandListDesc& desc);

		void update_host_descriptor_set(DescriptorSet* pSet,const DescriptorSetUpdateDesc& desc);
		void copy_descriptor_set(DescriptorSet* pSourceSet, DescriptorSet* pDestinationSet,const DescriptorSetCopyDesc& desc);
		void update_host_buffer(GraphicsBuffer* pTargetBuffer,const HostBufferUpdateDesc& desc);
		void submit_commands(CommandList** ppCmdLists, const unsigned char cmdListCount,const GraphicsQueue* pTargetQueue, Semaphore** ppSignalSemaphores,const unsigned int signalSemaphoreCount,Semaphore** ppWaitSemaphores, const PipelineStageFlags* pWaitStageFlags,const unsigned int waitSemaphoreCount,const Fence* pSignalFence);
		void reset_fences(Fence** ppFences, const unsigned int count);
		void wait_fences(Fence** ppFences, const unsigned int count);
		void wait_device_idle();
		void wait_queue_idle(GraphicsQueue* pQueue);
	protected:
		GraphicsDevice(const GraphicsDeviceDesc* pDesc);

		void register_object(GraphicsDeviceObject* pObject);
		void remove_object(GraphicsDeviceObject* pObject);

		virtual GraphicsQueue* own_queue_impl(const GraphicsQueueDesc& desc) = 0;
		virtual GraphicsBuffer* create_buffer_impl(const GraphicsBufferDesc& desc) = 0;
		virtual DescriptorSet* create_descriptor_set_impl(const DescriptorSetDesc& desc) = 0;
		virtual DescriptorPool* create_descriptor_pool_impl(const DescriptorPoolDesc& desc) = 0;
		virtual DescriptorSetLayout* create_descriptor_set_layout_impl(const DescriptorSetLayoutDesc& desc) = 0;
		virtual Fence* create_fence_impl(const FenceDesc& desc) = 0;
		virtual Semaphore* create_sync_object_impl(const SemaphoreDesc& desc) = 0;
		virtual GraphicsMemory* allocate_memory_impl(const GraphicsMemoryDesc& desc) = 0;
		virtual Sampler* create_sampler_impl(const SamplerDesc& desc) = 0;
		virtual Shader* create_shader_impl(const ShaderDesc& desc) = 0;
		virtual Texture* create_texture_impl(const TextureDesc& desc) = 0;
		virtual TextureView* create_texture_view_impl(const TextureViewDesc& desc) = 0;
		virtual Swapchain* create_swapchain_impl(const SwapchainDesc& desc) = 0;
		virtual Pipeline* create_graphics_pipeline_impl(const GraphicsPipelineDesc& desc) = 0;
		virtual Pipeline* create_compute_pipeline_impl(const ComputePipelineDesc& desc) = 0;
		virtual RenderPass* create_render_pass_impl(const RenderPassDesc& desc) = 0;
		virtual CommandPool* create_cmd_pool_impl(const CommandPoolDesc& desc) = 0;
		virtual CommandList* create_cmd_list_impl(const CommandListDesc& desc) = 0;

		virtual void update_host_descriptor_set_impl(DescriptorSet* pSet, const DescriptorSetUpdateDesc& desc) = 0;
		virtual void copy_descriptor_set_impl(DescriptorSet* pSourceSet, DescriptorSet* pDestinationSet, const DescriptorSetCopyDesc& desc) = 0;
		virtual void update_host_buffer_impl(GraphicsBuffer* pTargetBuffer, const HostBufferUpdateDesc& desc) = 0;
		virtual void submit_commands_impl(CommandList** ppCmdLists, const unsigned char cmdListCount, const GraphicsQueue* pTargetQueue, Semaphore** ppSignalSemaphores, const unsigned int signalSemaphoreCount, Semaphore** ppWaitSemaphores,const PipelineStageFlags* pWaitStageFlags, const unsigned int waitSemaphoreCount, const Fence* pSignalFence) = 0;
		virtual void reset_fences_impl(Fence** ppFences, const unsigned int count) =  0;
		virtual void wait_fences_impl(Fence** ppFences, const unsigned int count) =  0;
		virtual void wait_device_idle_impl() =  0;
		virtual void wait_queue_idle_impl(GraphicsQueue* pQueue) =  0;
	private:
		std::vector<GraphicsDeviceObject*> mObjects;
		GraphicsAdapter* mOwnerAdapter;
	};
}

#pragma once
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Vulkan/Device/VulkanDeviceDesc.h>
#include <vulkan.h>

namespace Dream
{
	class RUNTIME_API VulkanDevice : public GraphicsDevice
	{
        struct DeviceQueueFamily
        {
            DeviceQueueFamily() : FamilyIndex(255), Capacity(0),RequestedCount(0)
            {

            }

			VkQueue OwnQueue()
			{
				if (FreeQueues.size() == 0)
					return VK_NULL_HANDLE;

				VkQueue queue = FreeQueues[0];
				FreeQueues.erase(FreeQueues.begin());
				return queue;
			}

			void ReturnQueue(VkQueue queue)
			{
				FreeQueues.push_back(queue);
			}

            unsigned char FamilyIndex;
            unsigned char Capacity;
			unsigned char RequestedCount;
            std::vector<VkQueue> FreeQueues;
        };
	public:
		VulkanDevice(const VulkanDeviceDesc* pDesc);
		~VulkanDevice();

		FORCEINLINE VkDevice GetVkLogicalDevice() const noexcept
		{
			return mLogicalDevice;
		}
		VkQueue vkOwnQueue(const GraphicsQueueFamilyType type);
		void vkReturnQueue(const GraphicsQueueFamilyType type,const VkQueue queue);
		unsigned char vkGetQueueFamilyIndex(const GraphicsQueueFamilyType type) const noexcept;
		Texture* vkcreate_swapchainTexture(const TextureDesc& desc,const VkImage image);
		TextureView* vkcreate_swapchainTextureView(const TextureViewDesc& desc, const VkImageView view);
	private:
		virtual GraphicsBackend backend() const noexcept
		{
			return GraphicsBackend::Vulkan;
		}

		virtual bool has_queue(const GraphicsQueueFamilyType type) const noexcept override;
		virtual GraphicsQueue* own_queue_impl(const GraphicsQueueDesc& desc) override;
		virtual GraphicsBuffer* create_buffer_impl(const GraphicsBufferDesc& desc) override;
		virtual DescriptorSet* create_descriptor_set_impl(const DescriptorSetDesc& desc) override;
		virtual DescriptorPool* create_descriptor_pool_impl(const DescriptorPoolDesc& desc) override;
		virtual DescriptorSetLayout* create_descriptor_set_layout_impl(const DescriptorSetLayoutDesc& desc) override;
		virtual Fence* create_fence_impl(const FenceDesc& desc) override;
		virtual Semaphore* create_sync_object_impl(const SemaphoreDesc& desc) override;
		virtual GraphicsMemory* allocate_memory_impl(const GraphicsMemoryDesc& desc) override;
		virtual Sampler* create_sampler_impl(const SamplerDesc& desc) override;
		virtual Shader* create_shader_impl(const ShaderDesc& desc) override;
		virtual Texture* create_texture_impl(const TextureDesc& desc) override;
		virtual TextureView* create_texture_view_impl(const TextureViewDesc& desc) override;
		virtual Swapchain* create_swapchain_impl(const SwapchainDesc& desc) override;
		virtual Pipeline* create_graphics_pipeline_impl(const GraphicsPipelineDesc& desc) override;
		virtual Pipeline* create_compute_pipeline_impl(const ComputePipelineDesc& desc) override;
		virtual RenderPass* create_render_pass_impl(const RenderPassDesc& desc) override;
		virtual CommandPool* create_cmd_pool_impl(const CommandPoolDesc& desc) override;
		virtual CommandList* create_cmd_list_impl(const CommandListDesc& desc) override;

		virtual void update_host_descriptor_set_impl(DescriptorSet* pSet, const DescriptorSetUpdateDesc& desc) override;
		virtual void copy_descriptor_set_impl(DescriptorSet* pSourceSet, DescriptorSet* pDestinationSet, const DescriptorSetCopyDesc& desc) override;
		virtual void update_host_buffer_impl(GraphicsBuffer* pTargetBuffer, const HostBufferUpdateDesc& desc) override;
		virtual void submit_commands_impl(CommandList** ppCmdLists, const unsigned char cmdListCount, const GraphicsQueue* pTargetQueue, Semaphore** ppSignalSemaphores, const unsigned int signalSemaphoreCount, Semaphore** ppWaitSemaphores,const  PipelineStageFlags* pWaitStageFlags, const unsigned int waitSemaphoreCount, const Fence* pSignalFence) override;
		virtual void reset_fences_impl(Fence** ppFences, const unsigned int count) override;
		virtual void wait_fences_impl(Fence** ppFences, const unsigned int count) override;
		virtual void wait_device_idle_impl() override;
		virtual void wait_queue_idle_impl(GraphicsQueue* pQueue) override;
	private:
		VkDevice mLogicalDevice;
        DeviceQueueFamily mGraphicsFamily;
        DeviceQueueFamily mComputeFamily;
        DeviceQueueFamily mTransferFamily;
	};
}

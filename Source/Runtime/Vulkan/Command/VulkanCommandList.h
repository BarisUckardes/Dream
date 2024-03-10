#pragma once
#include <Runtime/Graphics/Command/CommandList.h>
#include <vulkan.h>

namespace Dream
{
	class VulkanDevice;
	class RUNTIME_API VulkanCommandList : public CommandList
	{
	public:
		VulkanCommandList(const CommandListDesc& desc, VulkanDevice* pDevice);
		~VulkanCommandList();

		FORCEINLINE VkCommandBuffer vk_cmd_buffer() const noexcept
		{
			return mCommandBuffer;
		}

	private:
		virtual void begin_impl() override;
		virtual void end_impl() override;
		virtual void set_vertex_buffers_impl(GraphicsBuffer** ppBuffers,const unsigned char count) override;
		virtual void set_index_buffer_impl(GraphicsBuffer* pBuffer, const IndexBufferType type) override;
		virtual void draw_indexed_impl(const unsigned int indexCount, const unsigned int indexOffset, const unsigned int vertexOffset, const unsigned int instanceOffset, const unsigned int instanceCount) override;
		virtual void dispatch_impl(const unsigned int x, const unsigned int y, const unsigned int z) override;
		virtual void set_pipeline_impl(Pipeline* pPipeline) override;
		virtual void begin_render_impl(RenderPass* pPass,const ClearValue* pClearColorValues, const unsigned char clearColorValueCount, const double clearDepth, const double clearStencil) override;
		virtual void end_render_impl() override;
		virtual void set_viewports_impl(ViewportDesc* pViewports, const unsigned char count) override;
		virtual void set_scissors_impl(ScissorDesc* pScissors, const unsigned char count) override;
		virtual void copy_buffer_buffer_impl(const GraphicsBuffer* pSourceBuffer, const GraphicsBuffer* pDestinationBuffer, const BufferBufferCopyDesc& desc) override;
		virtual void copy_buffer_texture_impl(const GraphicsBuffer* pSourceBuffer, const Texture* pDestinationTexture, const BufferTextureCopyDesc& desc) override;
		virtual void copy_texture_texture_impl(const Texture* pSourceTexture, const Texture* pDestinationTexture, const TextureCopyDesc& desc) override;
		virtual void set_texture_barrier_impl(const Texture* pTexture, const TextureMemoryBarrierDesc& desc) override;
		virtual void set_buffer_barrier_impl(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc) override;
		virtual void commit_resource_sets_impl(DescriptorSet** ppSets, const unsigned char count) override;
	private:
		VkCommandBuffer mCommandBuffer;
		VkCommandPool mCommandPool;
		VkDevice mLogicalDevice;

	};
}

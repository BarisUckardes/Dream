#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Command/CommandListDesc.h>
#include <Runtime/Graphics/Common/IndexBufferType.h>
#include <Runtime/Graphics/Common/ViewportDesc.h>
#include <Runtime/Graphics/Common/ScissorDesc.h>
#include <Runtime/Graphics/Common/BufferBufferCopyDesc.h>
#include <Runtime/Graphics/Common/BufferTextureCopyDesc.h>
#include <Runtime/Graphics/Common/TextureCopyDesc.h>
#include <Runtime/Graphics/Common/TextureMemoryBarrierDesc.h>
#include <Runtime/Graphics/Common/BufferMemoryBarrierDesc.h>
#include <Runtime/Graphics/Common/ClearValue.h>

namespace Dream
{
	class GraphicsBuffer;
	class Pipeline;
	class RenderPass;
	class Texture;
	class DescriptorSet;
	class RUNTIME_API CommandList : public GraphicsDeviceObject
	{
	public:
		~CommandList();

		FORCEINLINE const CommandPool* cmd_pool() const noexcept
		{
			return mCmdPool;
		}
		FORCEINLINE bool recording() const noexcept
		{
			return mRecording;
		}
		virtual GraphicsDeviceObjectType object_type() const noexcept override final
		{
			return GraphicsDeviceObjectType::CommandList;
		}

		void begin();
		void end();
		void set_vertex_buffers(GraphicsBuffer** ppBuffers, const unsigned char count);
		void set_index_buffer(GraphicsBuffer* pBuffer, const IndexBufferType type);
		void draw_indexed(const unsigned int indexCount, const unsigned int indexOffset, const unsigned int vertexOffset, const unsigned int instanceOffset, const unsigned int instanceCount);
		void dispatch(const unsigned int x, const unsigned int y, const unsigned int z);
		void set_pipeline(Pipeline* pPipeline);
		void begin_render(RenderPass* pPass,const ClearValue* pClearColorValues,const unsigned char clearColorValueCount,const double clearDepth,const double clearStencil);
		void end_render();
		void set_viewports(ViewportDesc* pViewports,const unsigned char count);
		void set_scissors(ScissorDesc* pScissors, const unsigned char count);
		void copy_buffer_buffer(const GraphicsBuffer* pSourceBuffer,const GraphicsBuffer* pDestinationBuffer,const BufferBufferCopyDesc& desc);
		void copy_buffer_texture(const GraphicsBuffer* pSourceBuffer,const Texture* pDestinationTexture,const BufferTextureCopyDesc& desc);
		void copy_texture_texture(const Texture* pSourceTexture,const Texture* pDestinationTexture,const TextureCopyDesc& desc);
		void set_texture_barrier(const Texture* pTexture,const TextureMemoryBarrierDesc& desc);
		void set_buffer_barrier(const GraphicsBuffer* pBuffer,const BufferMemoryBarrierDesc& desc);
		void commit_resource_sets(DescriptorSet** ppSets, const unsigned char count);
	protected:
		CommandList(const CommandListDesc& desc, GraphicsDevice* pDevice);

		virtual void begin_impl() = 0;
		virtual void end_impl() = 0;
		virtual void set_vertex_buffers_impl(GraphicsBuffer** ppBuffers, const unsigned char count) = 0;
		virtual void set_index_buffer_impl(GraphicsBuffer* pBuffer, const IndexBufferType type) = 0;
		virtual void draw_indexed_impl(const unsigned int indexCount, const unsigned int indexOffset, const unsigned int vertexOffset, const unsigned int instanceOffset, const unsigned int instanceCount) = 0;
		virtual void dispatch_impl(const unsigned int x, const unsigned int y, const unsigned int z) = 0;
		virtual void set_pipeline_impl(Pipeline* pPipeline) = 0;
		virtual void begin_render_impl(RenderPass* pPass,const ClearValue* pClearColorValues,const unsigned char clearColorValueCount, const double clearDepth, const double clearStencil) = 0;
		virtual void end_render_impl() = 0;
		virtual void set_viewports_impl(ViewportDesc* pViewports, const unsigned char count) = 0;
		virtual void set_scissors_impl(ScissorDesc* pScissors, const unsigned char count) = 0;
		virtual void copy_buffer_buffer_impl(const GraphicsBuffer* pSourceBuffer, const GraphicsBuffer* pDestinationBuffer, const BufferBufferCopyDesc& desc) = 0;
		virtual void copy_buffer_texture_impl(const GraphicsBuffer* pSourceBuffer, const Texture* pDestinationTexture, const BufferTextureCopyDesc& desc) = 0;
		virtual void copy_texture_texture_impl(const Texture* pSourceTexture, const Texture* pDestinationTexture, const TextureCopyDesc& desc) = 0;
		virtual void set_texture_barrier_impl(const Texture* pTexture, const TextureMemoryBarrierDesc& desc) = 0;
		virtual void set_buffer_barrier_impl(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc) = 0;
		virtual void commit_resource_sets_impl(DescriptorSet** ppSets, const unsigned char count) = 0;

		FORCEINLINE GraphicsBuffer* GetBoundIndexBuffer() const noexcept
		{
			return mBoundIndexBuffer;
		}
		FORCEINLINE Pipeline* GetBoundPipeline() const noexcept
		{
			return mBoundPipeline;
		}
		FORCEINLINE RenderPass* GetBoundRenderPass() const noexcept
		{
			return mBoundRenderPass;
		}

		void ClearCachedState();
	private:
		const CommandPool* mCmdPool;
		GraphicsBuffer* mBoundIndexBuffer;
		Pipeline* mBoundPipeline;
		RenderPass* mBoundRenderPass;
		bool mRecording;
	};
}


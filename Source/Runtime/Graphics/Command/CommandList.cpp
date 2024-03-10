#include "CommandList.h"

namespace Dream
{
	CommandList::~CommandList()
	{
	}
	CommandList::CommandList(const CommandListDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),mCmdPool(desc.pCmdPool),mRecording(false),
		mBoundIndexBuffer(nullptr),mBoundPipeline(nullptr),mBoundRenderPass(nullptr)
	{
	}
	void CommandList::begin()
	{
		begin_impl();
		mRecording = true;
	}
	void CommandList::end()
	{
		end_impl();
		ClearCachedState();
	}
	void CommandList::set_vertex_buffers(GraphicsBuffer** ppBuffers, const unsigned char count)
	{
		set_vertex_buffers_impl(ppBuffers,count);
	}
	void CommandList::set_index_buffer(GraphicsBuffer* pBuffer, const IndexBufferType type)
	{
		set_index_buffer_impl(pBuffer, type);
		mBoundIndexBuffer = pBuffer;
	}
	void CommandList::draw_indexed(const unsigned int indexCount, const unsigned int indexOffset, const unsigned int vertexOffset, const unsigned int instanceOffset, const unsigned int instanceCount)
	{
		draw_indexed_impl(indexCount, indexOffset, vertexOffset, instanceOffset, instanceCount);
	}
	void CommandList::dispatch(const unsigned int x, const unsigned int y, const unsigned int z)
	{
		dispatch_impl(x, y, z);
	}
	void CommandList::set_pipeline(Pipeline* pPipeline)
	{
		set_pipeline_impl(pPipeline);
		mBoundPipeline = pPipeline;
	}
	void CommandList::begin_render(RenderPass* pPass, const ClearValue* pClearColorValues, const unsigned char clearColorValueCount, const double clearDepth, const double clearStencil)
	{
		begin_render_impl(pPass, pClearColorValues,clearColorValueCount,clearDepth,clearStencil);
		mBoundRenderPass = pPass;
	}
	void CommandList::end_render()
	{
		end_render_impl();
		mBoundRenderPass = nullptr;
	}
	void CommandList::set_viewports(ViewportDesc* pViewports, const unsigned char count)
	{
		set_viewports_impl(pViewports, count);
	}
	void CommandList::set_scissors(ScissorDesc* pScissors, const unsigned char count)
	{
		set_scissors_impl(pScissors, count);
	}
	void CommandList::copy_buffer_buffer(const GraphicsBuffer* pSourceBuffer, const GraphicsBuffer* pDestinationBuffer, const BufferBufferCopyDesc& desc)
	{
		copy_buffer_buffer_impl(pSourceBuffer, pDestinationBuffer, desc);
	}
	void CommandList::copy_buffer_texture(const GraphicsBuffer* pSourceBuffer, const Texture* pDestinationTexture, const BufferTextureCopyDesc& desc)
	{
		copy_buffer_texture_impl(pSourceBuffer, pDestinationTexture, desc);
	}
	void CommandList::copy_texture_texture(const Texture* pSourceTexture, const Texture* pDestinationTexture, const TextureCopyDesc& desc)
	{
		copy_texture_texture_impl(pSourceTexture, pDestinationTexture, desc);
	}
	void CommandList::set_texture_barrier(const Texture* pTexture, const TextureMemoryBarrierDesc& desc)
	{
		set_texture_barrier_impl(pTexture, desc);
	}
	void CommandList::set_buffer_barrier(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc)
	{
		set_buffer_barrier_impl(pBuffer, desc);
	}
	void CommandList::commit_resource_sets(DescriptorSet** ppSets, const unsigned char count)
	{
		commit_resource_sets_impl(ppSets, count);
	}
	void CommandList::ClearCachedState()
	{
		mBoundIndexBuffer = nullptr;
		mBoundPipeline = nullptr;
		mBoundRenderPass = nullptr;
	}
}

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
	void CommandList::BeginRecording()
	{
		BeginRecordingCore();
		mRecording = true;
	}
	void CommandList::EndRecording()
	{
		EndRecordingCore();
		ClearCachedState();
	}
	void CommandList::SetVertexBuffer(GraphicsBuffer** ppBuffers, const unsigned char count)
	{
		SetVertexBufferCore(ppBuffers,count);
	}
	void CommandList::SetIndexBuffer(GraphicsBuffer* pBuffer, const IndexBufferType type)
	{
		SetIndexBufferCore(pBuffer, type);
		mBoundIndexBuffer = pBuffer;
	}
	void CommandList::DrawIndexed(const unsigned int indexCount, const unsigned int indexOffset, const unsigned int vertexOffset, const unsigned int instanceOffset, const unsigned int instanceCount)
	{
		DrawIndexedCore(indexCount, indexOffset, vertexOffset, instanceOffset, instanceCount);
	}
	void CommandList::DispatchCompute(const unsigned int x, const unsigned int y, const unsigned int z)
	{
		DispatchComputeCore(x, y, z);
	}
	void CommandList::SetPipeline(Pipeline* pPipeline)
	{
		SetPipelineCore(pPipeline);
		mBoundPipeline = pPipeline;
	}
	void CommandList::BeginRenderPass(RenderPass* pPass, const float clearColor[4])
	{
		BeginRenderPassCore(pPass, clearColor);
		mBoundRenderPass = pPass;
	}
	void CommandList::EndRenderPass()
	{
		EndRenderPassCore();
		mBoundRenderPass = nullptr;
	}
	void CommandList::SetViewports(const ViewportDesc** ppViewports, const unsigned char count)
	{
		SetViewportsCore(ppViewports, count);
	}
	void CommandList::SetScissors(const ScissorDesc** ppScissors, const unsigned char count)
	{
		SetScissorsCore(ppScissors, count);
	}
	void CommandList::CopyBufferToBuffer(const GraphicsBuffer* pSourceBuffer, const GraphicsBuffer* pDestinationBuffer, const BufferBufferCopyDesc& desc)
	{
		CopyBufferToBufferCore(pSourceBuffer, pDestinationBuffer, desc);
	}
	void CommandList::CopyBufferToTexture(const GraphicsBuffer* pSourceBuffer, const Texture* pDestinationTexture, const BufferTextureCopyDesc& desc)
	{
		CopyBufferToTextureCore(pSourceBuffer, pDestinationTexture, desc);
	}
	void CommandList::CopyTextureToTexture(const Texture* pSourceTexture, const Texture* pDestinationTexture, const TextureCopyDesc& desc)
	{
		CopyTextureToTextureCore(pSourceTexture, pDestinationTexture, desc);
	}
	void CommandList::SetTextureMemoryBarrier(const Texture* pTexture, const TextureMemoryBarrierDesc& desc)
	{
		SetTextureMemoryBarrierCore(pTexture, desc);
	}
	void CommandList::SetBufferMemoryBarrier(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc)
	{
		SetBufferMemoryBarrierCore(pBuffer, desc);
	}
	void CommandList::CommitResourceSets(DescriptorSet** ppSets, const unsigned char count)
	{
		CommitResourceSetsCore(ppSets, count);
	}
	void CommandList::ClearCachedState()
	{
		mBoundIndexBuffer = nullptr;
		mBoundPipeline = nullptr;
		mBoundRenderPass = nullptr;
	}
}

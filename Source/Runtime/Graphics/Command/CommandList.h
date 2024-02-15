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

		FORCEINLINE const CommandPool* GetCmdPool() const noexcept
		{
			return mCmdPool;
		}
		FORCEINLINE bool IsRecording() const noexcept
		{
			return mRecording;
		}
		virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
		{
			return GraphicsDeviceObjectType::CommandList;
		}

		void BeginRecording();
		void EndRecording();
		void SetVertexBuffer(GraphicsBuffer** ppBuffers, const unsigned char count);
		void SetIndexBuffer(GraphicsBuffer* pBuffer, const IndexBufferType type);
		void DrawIndexed(const unsigned int indexCount, const unsigned int indexOffset, const unsigned int vertexOffset, const unsigned int instanceOffset, const unsigned int instanceCount);
		void DispatchCompute(const unsigned int x, const unsigned int y, const unsigned int z);
		void SetPipeline(Pipeline* pPipeline);
		void BeginRenderPass(RenderPass* pPass, const float clearColor[4]);
		void EndRenderPass();
		void SetViewports(const ViewportDesc** ppViewports,const unsigned char count);
		void SetScissors(const ScissorDesc** ppScissors, const unsigned char count);
		void CopyBufferToBuffer(const GraphicsBuffer* pSourceBuffer,const GraphicsBuffer* pDestinationBuffer,const BufferBufferCopyDesc& desc);
		void CopyBufferToTexture(const GraphicsBuffer* pSourceBuffer,const Texture* pDestinationTexture,const BufferTextureCopyDesc& desc);
		void CopyTextureToTexture(const Texture* pSourceTexture,const Texture* pDestinationTexture,const TextureCopyDesc& desc);
		void SetTextureMemoryBarrier(const Texture* pTexture,const TextureMemoryBarrierDesc& desc);
		void SetBufferMemoryBarrier(const GraphicsBuffer* pBuffer,const BufferMemoryBarrierDesc& desc);
		void CommitResourceSets(DescriptorSet** ppSets, const unsigned char count);
	protected:
		CommandList(const CommandListDesc& desc, GraphicsDevice* pDevice);

		virtual void BeginRecordingCore() = 0;
		virtual void EndRecordingCore() = 0;
		virtual void SetVertexBufferCore(GraphicsBuffer** ppBuffers, const unsigned char count) = 0;
		virtual void SetIndexBufferCore(GraphicsBuffer* pBuffer, const IndexBufferType type) = 0;
		virtual void DrawIndexedCore(const unsigned int indexCount, const unsigned int indexOffset, const unsigned int vertexOffset, const unsigned int instanceOffset, const unsigned int instanceCount) = 0;
		virtual void DispatchComputeCore(const unsigned int x, const unsigned int y, const unsigned int z) = 0;
		virtual void SetPipelineCore(Pipeline* pPipeline) = 0;
		virtual void BeginRenderPassCore(RenderPass* pPass, const float clearColor[4]) = 0;
		virtual void EndRenderPassCore() = 0;
		virtual void SetViewportsCore(const ViewportDesc** ppViewports, const unsigned char count) = 0;
		virtual void SetScissorsCore(const ScissorDesc** ppScissors, const unsigned char count) = 0;
		virtual void CopyBufferToBufferCore(const GraphicsBuffer* pSourceBuffer, const GraphicsBuffer* pDestinationBuffer, const BufferBufferCopyDesc& desc) = 0;
		virtual void CopyBufferToTextureCore(const GraphicsBuffer* pSourceBuffer, const Texture* pDestinationTexture, const BufferTextureCopyDesc& desc) = 0;
		virtual void CopyTextureToTextureCore(const Texture* pSourceTexture, const Texture* pDestinationTexture, const TextureCopyDesc& desc) = 0;
		virtual void SetTextureMemoryBarrierCore(const Texture* pTexture, const TextureMemoryBarrierDesc& desc) = 0;
		virtual void SetBufferMemoryBarrierCore(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc) = 0;
		virtual void CommitResourceSetsCore(DescriptorSet** ppSets, const unsigned char count) = 0;

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


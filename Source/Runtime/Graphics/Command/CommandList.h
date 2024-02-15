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
		void SetVertexBuffer(GraphicsBuffer* pBuffer);
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

		FORCEINLINE GraphicsBuffer* GetBoundVertexBuffer() const noexcept
		{
			return mBoundVertexBuffer;
		}
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
		GraphicsBuffer* mBoundVertexBuffer;
		GraphicsBuffer* mBoundIndexBuffer;
		Pipeline* mBoundPipeline;
		RenderPass* mBoundRenderPass;
		bool mRecording;
	};
}


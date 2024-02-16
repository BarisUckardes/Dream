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

		FORCEINLINE VkCommandBuffer GetVkCmdBuffer() const noexcept
		{
			return mCommandBuffer;
		}

	private:
		virtual void BeginRecordingCore() override;
		virtual void EndRecordingCore() override;
		virtual void SetVertexBuffersCore(GraphicsBuffer** ppBuffers,const unsigned char count) override;
		virtual void SetIndexBufferCore(GraphicsBuffer* pBuffer, const IndexBufferType type) override;
		virtual void DrawIndexedCore(const unsigned int indexCount, const unsigned int indexOffset, const unsigned int vertexOffset, const unsigned int instanceOffset, const unsigned int instanceCount) override;
		virtual void DispatchComputeCore(const unsigned int x, const unsigned int y, const unsigned int z) override;
		virtual void SetPipelineCore(Pipeline* pPipeline) override;
		virtual void BeginRenderPassCore(RenderPass* pPass, const float clearColor[4]) override;
		virtual void EndRenderPassCore() override;
		virtual void SetViewportsCore(ViewportDesc* pViewports, const unsigned char count) override;
		virtual void SetScissorsCore(ScissorDesc* pScissors, const unsigned char count) override;
		virtual void CopyBufferToBufferCore(const GraphicsBuffer* pSourceBuffer, const GraphicsBuffer* pDestinationBuffer, const BufferBufferCopyDesc& desc) override;
		virtual void CopyBufferToTextureCore(const GraphicsBuffer* pSourceBuffer, const Texture* pDestinationTexture, const BufferTextureCopyDesc& desc) override;
		virtual void CopyTextureToTextureCore(const Texture* pSourceTexture, const Texture* pDestinationTexture, const TextureCopyDesc& desc) override;
		virtual void SetTextureMemoryBarrierCore(const Texture* pTexture, const TextureMemoryBarrierDesc& desc) override;
		virtual void SetBufferMemoryBarrierCore(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc) override;
		virtual void CommitResourceSetsCore(DescriptorSet** ppSets, const unsigned char count) override;
	private:
		VkCommandBuffer mCommandBuffer;
		VkCommandPool mCommandPool;
		VkDevice mLogicalDevice;

	};
}

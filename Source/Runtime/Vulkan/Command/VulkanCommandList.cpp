#include "VulkanCommandList.h"
#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/Command/VulkanCommandPool.h>
#include <Runtime/Vulkan/Buffer/VulkanBuffer.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>
#include <Runtime/Vulkan/Sampler/VulkanSamplerUtils.h>
#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/Memory/VulkanMemoryUtils.h>
#include <Runtime/Vulkan/Pipeline/VulkanPipelineUtils.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorSet.h>
#include <Runtime/Vulkan/RenderPass/VulkanRenderPass.h>
#include <Runtime/Vulkan/Pipeline/VulkanPipeline.h>


namespace Dream
{
	VulkanCommandList::VulkanCommandList(const CommandListDesc& desc, VulkanDevice* pDevice) : CommandList(desc,pDevice),mCommandPool(VK_NULL_HANDLE),mCommandBuffer(VK_NULL_HANDLE),mLogicalDevice(pDevice->GetVkLogicalDevice())
	{
		const VulkanCommandPool* pCmdPool = (const VulkanCommandPool*)desc.pCmdPool;
		mCommandPool = pCmdPool->GetVkPool();

		/**
		* Create command buffer
		*/
		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandPool = pCmdPool->GetVkPool();
		allocateInfo.commandBufferCount = 1;
		allocateInfo.pNext = nullptr;

		DEV_ASSERT(vkAllocateCommandBuffers(pDevice->GetVkLogicalDevice(), &allocateInfo, &mCommandBuffer) == VK_SUCCESS, "VulkanCommandList", "Failed to allocate command buffer");
	}
	VulkanCommandList::~VulkanCommandList()
	{
		vkFreeCommandBuffers(mLogicalDevice, mCommandPool, 1, &mCommandBuffer);
	}
	void VulkanCommandList::BeginRecordingCore()
	{
		DEV_ASSERT(vkResetCommandBuffer(mCommandBuffer, VkCommandPoolResetFlags()) == VK_SUCCESS, "VulkanCommandList", "Failed to reset the cmd list");

		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags = 0;
		info.pInheritanceInfo = nullptr;
		info.pNext = nullptr;

		DEV_ASSERT(vkBeginCommandBuffer(mCommandBuffer, &info) == VK_SUCCESS, "VulkanCommandList", "Failed to begin command list");
	}
	void VulkanCommandList::EndRecordingCore()
	{
		DEV_ASSERT(vkEndCommandBuffer(mCommandBuffer) == VK_SUCCESS, "VulkanCommandList", "Failed to end command list");
	}
	void VulkanCommandList::SetVertexBuffersCore(GraphicsBuffer** ppBuffers, const unsigned char count)
	{
		VkDeviceSize offsets[32];
		VkBuffer buffers[32];
		unsigned long long offset = 0;
		for (unsigned char i = 0; i < count; i++)
		{
			const VulkanBuffer* pBuffer = (const VulkanBuffer*)ppBuffers[i];

			buffers[i] = pBuffer->GetVkBuffer();
			offsets[i] = offset;
			offset += pBuffer->GetTotalSize();
		}

		vkCmdBindVertexBuffers(mCommandBuffer,0,count,buffers,offsets);
	}
	void VulkanCommandList::SetIndexBufferCore(GraphicsBuffer* pBuffer, const IndexBufferType type)
	{
		const VulkanBuffer* pVkBuffer = (const VulkanBuffer*)pBuffer;
		vkCmdBindIndexBuffer(mCommandBuffer, pVkBuffer->GetVkBuffer(), 0, type == IndexBufferType::UInt32 ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16);
	}
	void VulkanCommandList::DrawIndexedCore(const unsigned int indexCount, const unsigned int indexOffset, const unsigned int vertexOffset, const unsigned int instanceOffset, const unsigned int instanceCount)
	{
		vkCmdDrawIndexed(mCommandBuffer, indexCount, instanceCount, indexOffset, vertexOffset, instanceOffset);
	}
	void VulkanCommandList::DispatchComputeCore(const unsigned int x, const unsigned int y, const unsigned int z)
	{
		vkCmdDispatch(mCommandBuffer, x, y, z);
	}
	void VulkanCommandList::SetPipelineCore(Pipeline* pPipeline)
	{
		const VulkanPipeline* pVkPipeline = (const VulkanPipeline*)pPipeline;

		vkCmdBindPipeline(mCommandBuffer, pVkPipeline->GetVkPipelinBindPoint(), pVkPipeline->GetVkPipeline());
	}
	void VulkanCommandList::BeginRenderPassCore(RenderPass* pPass,const ClearValue* pClearColorValues, const unsigned char clearColorValueCount, const double clearDepth, const double clearStencil)
	{
		//Get framebuffer
		const VulkanRenderPass* pVkPass = (const VulkanRenderPass*)pPass;
		const VkFramebuffer framebuffer = pVkPass->GetVkFramebuffer();

		//Create render pass begin info
		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = pVkPass->GetVkRenderPass();
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = { pPass->GetRenderWidth(),pPass->GetRenderHeight()};

		VkClearValue clearValues[32];
		unsigned int clearValueCount = clearColorValueCount;
		for (unsigned char i = 0; i < clearColorValueCount; i++)
		{
			VkClearValue clearColor = {};
			clearColor.color.float32[0] = pClearColorValues[i].R;
			clearColor.color.float32[1] = pClearColorValues[i].G;
			clearColor.color.float32[2] = pClearColorValues[i].B;
			clearColor.color.float32[3] = pClearColorValues[i].A;
			clearValues[i] = clearColor;
		}

		if (clearDepth != -1 || clearStencil != -1)
		{
			VkClearValue clearDepthStencilValue = {};
			clearDepthStencilValue.depthStencil.depth = clearDepth;
			clearDepthStencilValue.depthStencil.stencil = clearStencil;
			clearValues[clearColorValueCount] = clearDepthStencilValue;
			clearValueCount++;
		}
		
		const unsigned char totalClearValues = clearValueCount;
		renderPassInfo.clearValueCount = totalClearValues;
		renderPassInfo.pClearValues = clearValues;


		//Start render pass
		vkCmdBeginRenderPass(mCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}
	void VulkanCommandList::EndRenderPassCore()
	{
		vkCmdEndRenderPass(mCommandBuffer);
	}
	void VulkanCommandList::SetViewportsCore(ViewportDesc* pViewports, const unsigned char count)
	{
		VkViewport vkViewports[32];
		for (unsigned char viewportIndex = 0; viewportIndex < count; viewportIndex++)
		{
			const ViewportDesc& viewport = pViewports[viewportIndex];

			VkViewport vkViewport = {};
			vkViewport.x = viewport.X;
			vkViewport.y = viewport.Y;
			vkViewport.width = viewport.Width;
			vkViewport.height = viewport.Height;
			vkViewport.minDepth = viewport.DepthMin;
			vkViewport.maxDepth = viewport.DepthMax;

			vkViewports[viewportIndex] = vkViewport;
		}
		vkCmdSetViewport(mCommandBuffer, 0, count, vkViewports);
	}
	void VulkanCommandList::SetScissorsCore(ScissorDesc* pScissors, const unsigned char count)
	{
		VkRect2D vkScissors[32];
		for (unsigned char scissorIndex = 0; scissorIndex < count; scissorIndex++)
		{
			const ScissorDesc& scissor = pScissors[scissorIndex];

			VkRect2D rect = {};
			rect.offset = { (int)scissor.X,(int)scissor.Y };
			rect.extent = { scissor.Width,scissor.Height };

			vkScissors[scissorIndex] = rect;
		}

		vkCmdSetScissor(mCommandBuffer, 0, count, vkScissors);
	}
	void VulkanCommandList::CopyBufferToBufferCore(const GraphicsBuffer* pSourceBuffer, const GraphicsBuffer* pDestinationBuffer, const BufferBufferCopyDesc& desc)
	{
		const VulkanBuffer* pVkSourceBuffer = (const VulkanBuffer*)pSourceBuffer;
		const VulkanBuffer* pVkDestinationBuffer = (const VulkanBuffer*)pDestinationBuffer;

		VkBufferCopy copyDesc = {};
		copyDesc.srcOffset = desc.SourceOffsetInBytes;
		copyDesc.dstOffset = desc.DestinationOffsetInBytes;
		copyDesc.size = desc.SizeInBytes;

		vkCmdCopyBuffer(mCommandBuffer, pVkSourceBuffer->GetVkBuffer(), pVkDestinationBuffer->GetVkBuffer(), 1, &copyDesc);
	}
	void VulkanCommandList::CopyBufferToTextureCore(const GraphicsBuffer* pSourceBuffer, const Texture* pDestinationTexture, const BufferTextureCopyDesc& desc)
	{
		const VulkanBuffer* pVkBuffer = (const VulkanBuffer*)pSourceBuffer;
		const VulkanTexture* pVkTexture = (const VulkanTexture*)pDestinationTexture;

		VkBufferImageCopy copyDesc = {};
		copyDesc.bufferOffset = desc.BufferOffsetInBytes;
		copyDesc.bufferRowLength = 0;
		copyDesc.bufferImageHeight = 0;

		copyDesc.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copyDesc.imageSubresource.baseArrayLayer = desc.TargetArrayIndex;
		copyDesc.imageSubresource.mipLevel = desc.TargetMipIndex;
		copyDesc.imageSubresource.layerCount = pDestinationTexture->GetArrayLevels();

		copyDesc.imageOffset = { (int)desc.TextureOffsetX,(int)desc.TextureOffsetY,(int)desc.TextureOffsetZ };
		copyDesc.imageExtent = { desc.Width,desc.Height,desc.Depth};

		vkCmdCopyBufferToImage(mCommandBuffer, pVkBuffer->GetVkBuffer(), pVkTexture->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyDesc);
	}
	void VulkanCommandList::CopyTextureToTextureCore(const Texture* pSourceTexture, const Texture* pDestinationTexture, const TextureCopyDesc& desc)
	{
		const VulkanTexture* pVkSourceTexture = (const VulkanTexture*)pSourceTexture;
		const VulkanTexture* pVkDestinationTexture = (const VulkanTexture*)pDestinationTexture;

		VkImageBlit blitInfo = {};

		blitInfo.srcOffsets[0] = { (int)desc.SourceOffsetX,(int)desc.SourceOffsetY,(int)desc.SourceOffsetZ };
		blitInfo.srcSubresource.aspectMask = VulkanTextureUtils::GetImageAspects(desc.SourceAspect);
		blitInfo.srcSubresource.baseArrayLayer = desc.SourceArrayIndex;
		blitInfo.srcSubresource.layerCount = pSourceTexture->GetMipLevels();
		blitInfo.srcSubresource.mipLevel = desc.SourceMipIndex;

		blitInfo.dstOffsets[0] = { (int)desc.DestinationOffsetX,(int)desc.DestinationOffsetY,(int)desc.DestinationOffsetZ };
		blitInfo.dstSubresource.aspectMask = VulkanTextureUtils::GetImageAspects(desc.DestinationAspect);
		blitInfo.dstSubresource.baseArrayLayer = desc.DestinationArrayIndex;
		blitInfo.dstSubresource.layerCount = pDestinationTexture->GetMipLevels();
		blitInfo.dstSubresource.mipLevel = desc.DestinationMipIndex;

		vkCmdBlitImage(mCommandBuffer, pVkSourceTexture->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, pVkDestinationTexture->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blitInfo, VulkanSamplerUtils::GetFilter(desc.Filtering));
	}
	void VulkanCommandList::SetTextureMemoryBarrierCore(const Texture* pTexture, const TextureMemoryBarrierDesc& desc)
	{
		const VulkanDevice* pDevice = (const VulkanDevice*)GetDevice();
		const VulkanTexture* pVkTexture = (const VulkanTexture*)pTexture;
		const VkImage image = pVkTexture->GetVkImage();

		VkImageMemoryBarrier memoryBarrier = {};
		memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		memoryBarrier.pNext = nullptr;
		memoryBarrier.image = image;
		memoryBarrier.oldLayout = VulkanTextureUtils::GetImageLayout(desc.SourceLayout);
		memoryBarrier.newLayout = VulkanTextureUtils::GetImageLayout(desc.DestinationLayout);
		memoryBarrier.srcQueueFamilyIndex = pDevice->vkGetQueueFamilyIndex(desc.SourceQueue);
		memoryBarrier.dstQueueFamilyIndex = pDevice->vkGetQueueFamilyIndex(desc.DestinationQueue);
		memoryBarrier.subresourceRange.aspectMask = VulkanTextureUtils::GetImageAspects(desc.AspectFlags);
		memoryBarrier.subresourceRange.baseMipLevel = desc.MipIndex;
		memoryBarrier.subresourceRange.levelCount = pTexture->GetMipLevels();
		memoryBarrier.subresourceRange.baseArrayLayer = desc.ArrayIndex;
		memoryBarrier.subresourceRange.layerCount = pTexture->GetArrayLevels();
		memoryBarrier.srcAccessMask = VulkanMemoryUtils::GetMemoryAccessFlags(desc.SourceAccessFlags);
		memoryBarrier.dstAccessMask = VulkanMemoryUtils::GetMemoryAccessFlags(desc.DestinationAccessFlags);

		vkCmdPipelineBarrier(mCommandBuffer,
			VulkanPipelineUtils::GetStageFlags(desc.SourceStageFlags), VulkanPipelineUtils::GetStageFlags(desc.DestinationStageFlags),
			0,
			0, nullptr,
			0, nullptr,
			1, &memoryBarrier);
	}
	void VulkanCommandList::SetBufferMemoryBarrierCore(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc)
	{
		const VulkanDevice* pDevice = (const VulkanDevice*)GetDevice();
		const VulkanBuffer* pVkBuffer = (const VulkanBuffer*)pBuffer;

		VkBufferMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		barrier.buffer = pVkBuffer->GetVkBuffer();
		barrier.srcAccessMask = VulkanMemoryUtils::GetMemoryAccessFlags(desc.SourceAccessFlags);
		barrier.srcQueueFamilyIndex = pDevice->vkGetQueueFamilyIndex(desc.SourceQueue);
		barrier.dstAccessMask = VulkanMemoryUtils::GetMemoryAccessFlags(desc.DestinationAccessFlags);
		barrier.dstAccessMask = pDevice->vkGetQueueFamilyIndex(desc.DestinationQueue);
		barrier.offset = desc.OffsetInBytes;
		barrier.size = desc.SizeInBytes;

		vkCmdPipelineBarrier(mCommandBuffer,
			VulkanPipelineUtils::GetStageFlags(desc.SourceStageFlags), VulkanPipelineUtils::GetStageFlags(desc.DestinationStageFlags),
			0,
			0, nullptr,
			1, &barrier,
			0, nullptr);
	}
	void VulkanCommandList::CommitResourceSetsCore(DescriptorSet** ppSets, const unsigned char count)
	{
		const VulkanPipeline* pPipeline = (const VulkanPipeline*)GetBoundPipeline();

		VkDescriptorSet descriptorSets[32];

		for (unsigned char resourceIndex = 0; resourceIndex < count; resourceIndex++)
		{
			const VulkanDescriptorSet* pSet = (const VulkanDescriptorSet*)ppSets[resourceIndex];

			descriptorSets[resourceIndex] = pSet->GetVkSet();
		}

		vkCmdBindDescriptorSets(mCommandBuffer, pPipeline->GetVkPipelinBindPoint(), pPipeline->GetVkPipelineLayout(), 0, count, descriptorSets, 0, nullptr);
	}
}

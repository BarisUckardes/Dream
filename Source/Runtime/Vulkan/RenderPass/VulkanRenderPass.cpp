#include "VulkanRenderPass.h"
#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/RenderPass/VulkanRenderPassUtils.h>
#include <Runtime/Vulkan/Memory/VulkanMemoryUtils.h>
#include <Runtime/Vulkan/Pipeline/VulkanPipelineUtils.h>
#include <Runtime/Vulkan/Texture/VulkanTextureView.h>

namespace Dream
{
	VulkanRenderPass::VulkanRenderPass(const RenderPassDesc& desc, VulkanDevice* pDevice) : RenderPass(desc,pDevice),mLogicalDevice(pDevice->GetVkLogicalDevice()),mRenderPass(VK_NULL_HANDLE),mFramebuffer(VK_NULL_HANDLE)
	{
		//Create color attachment descs
		std::vector<VkAttachmentDescription> attachments(desc.ColorAttachments.size());
		for (unsigned char i = 0; i < desc.ColorAttachments.size(); i++)
		{
			const RenderPassAttachmentDesc& attachmentDesc = desc.ColorAttachments[i];
			VkAttachmentDescription attachment = {};
			attachment.format = VulkanTextureUtils::GetTextureFormat(attachmentDesc.Format);
			attachment.samples = (VkSampleCountFlagBits)VulkanTextureUtils::GetSampleCount(attachmentDesc.SampleCount);
			attachment.loadOp = VulkanRenderPassUtils::GetLoadOperation(attachmentDesc.ColorLoadOperation);
			attachment.storeOp = VulkanRenderPassUtils::GetStoreOperation(attachmentDesc.ColorStoreOperation);
			attachment.initialLayout = VulkanTextureUtils::GetImageLayout(attachmentDesc.InputLayout);
			attachment.finalLayout = VulkanTextureUtils::GetImageLayout(attachmentDesc.OutputLayout);
			attachment.stencilLoadOp = VulkanRenderPassUtils::GetLoadOperation(attachmentDesc.StencilLoadOperation);
			attachment.stencilStoreOp = VulkanRenderPassUtils::GetStoreOperation(attachmentDesc.StencilStoreOperation);

			attachments[i] = attachment;
		}

		//Create depth stencil attachment
		if (desc.pDepthStencilAttachment != nullptr)
		{
			const RenderPassAttachmentDesc& attachmentDesc = *desc.pDepthStencilAttachment;
			VkAttachmentDescription attachment = {};
			attachment.format = VulkanTextureUtils::GetTextureFormat(attachmentDesc.Format);
			attachment.samples = (VkSampleCountFlagBits)VulkanTextureUtils::GetSampleCount(attachmentDesc.SampleCount);
			attachment.loadOp = VulkanRenderPassUtils::GetLoadOperation(attachmentDesc.ColorLoadOperation);
			attachment.storeOp = VulkanRenderPassUtils::GetStoreOperation(attachmentDesc.ColorStoreOperation);
			attachment.initialLayout = VulkanTextureUtils::GetImageLayout(attachmentDesc.InputLayout);
			attachment.finalLayout = VulkanTextureUtils::GetImageLayout(attachmentDesc.OutputLayout);
			attachment.stencilLoadOp = VulkanRenderPassUtils::GetLoadOperation(attachmentDesc.StencilLoadOperation);
			attachment.stencilStoreOp = VulkanRenderPassUtils::GetStoreOperation(attachmentDesc.StencilStoreOperation);

			attachments.push_back(attachment);
		}

		//Create attachment references
		VkAttachmentReference attachmentReferences[16];
		for (unsigned char i = 0; i < attachments.size(); i++)
		{
			const VkAttachmentDescription& attachment = attachments[i];

			VkAttachmentReference reference = {};
			reference.attachment = i;
			reference.layout = attachment.initialLayout;
			attachmentReferences[i] = reference;
		}
		
		//Create sub pass
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = nullptr;

		subpass.colorAttachmentCount = desc.ColorAttachments.size();
		subpass.pColorAttachments = attachmentReferences;

		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = nullptr;

		subpass.pDepthStencilAttachment = desc.pDepthStencilAttachment != nullptr ? &attachmentReferences[desc.ColorAttachments.size()] : nullptr;

		subpass.pResolveAttachments = nullptr;

		//Create render pass
		VkRenderPassCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.flags = VkRenderPassCreateFlags();
		info.attachmentCount = attachments.size();
		info.pAttachments = attachments.data();
		info.subpassCount = 1;
		info.pSubpasses = &subpass;
		info.dependencyCount = 0;
		info.pDependencies = nullptr;
		info.pNext = nullptr;

		DEV_ASSERT(vkCreateRenderPass(mLogicalDevice, &info, nullptr, &mRenderPass) == VK_SUCCESS, "VulkanRenderPass", "Failed to create render pass!");

		//Create framebuffer
		VkImageView imageViews[8] = {};
		for (unsigned char i = 0; i < desc.ColorAttachments.size(); i++)
		{
			const VulkanTextureView* pView = (const VulkanTextureView*)desc.ColorAttachments[i].pView;
			imageViews[i] = pView->GetVkView();
		}

		if (desc.pDepthStencilAttachment != nullptr)
		{
			const VulkanTextureView* pView = (const VulkanTextureView*)desc.pDepthStencilAttachment->pView;
			imageViews[desc.ColorAttachments.size()] = pView->GetVkView();
		}

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.width = desc.TargetRenderWidth;
		framebufferInfo.height = desc.TargetRenderHeight;
		framebufferInfo.renderPass = mRenderPass;
		framebufferInfo.layers = 1;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = imageViews;
		framebufferInfo.flags = VkFramebufferCreateFlags();
		framebufferInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateFramebuffer(mLogicalDevice, &framebufferInfo, nullptr, &mFramebuffer) == VK_SUCCESS, "VulkanRenderPass", "Failed to create normal framebuffer!");
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		//Clear frame buffers first
		vkDestroyFramebuffer(mLogicalDevice, mFramebuffer, nullptr);

		vkDestroyRenderPass(mLogicalDevice, mRenderPass, nullptr);
	}
	
}

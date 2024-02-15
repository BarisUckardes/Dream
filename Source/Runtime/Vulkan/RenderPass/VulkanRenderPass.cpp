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
		const unsigned char colorAttachmentCount = desc.ColorAttachments.size();
		const bool bHasDepthStencilAttachment = desc.DepthStencilAttachment.pTexture != nullptr;
		const unsigned totalAttachmentCount = colorAttachmentCount + bHasDepthStencilAttachment;
		std::vector<VkAttachmentDescription> attachments(totalAttachmentCount);
		for (unsigned char i = 0; i < totalAttachmentCount; i++)
		{
			const RenderPassAttachmentDesc& attachmentDesc = desc.ColorAttachments[i];
			VkAttachmentDescription attachment = {};
			attachment.format = VulkanTextureUtils::GetTextureFormat(attachmentDesc.pTexture->GetFormat());
			attachment.samples = (VkSampleCountFlagBits)VulkanTextureUtils::GetSampleCount(attachmentDesc.pTexture->GetSampleCount());
			attachment.loadOp = VulkanRenderPassUtils::GetLoadOperation(attachmentDesc.ColorLoadOperation);
			attachment.storeOp = VulkanRenderPassUtils::GetStoreOperation(attachmentDesc.ColorStoreOperation);
			attachment.initialLayout = VulkanTextureUtils::GetImageLayout(attachmentDesc.InputLayout);
			attachment.finalLayout = VulkanTextureUtils::GetImageLayout(attachmentDesc.OutputLayout);
			attachment.stencilLoadOp = VulkanRenderPassUtils::GetLoadOperation(attachmentDesc.StencilLoadOperation);
			attachment.stencilStoreOp = VulkanRenderPassUtils::GetStoreOperation(attachmentDesc.StencilStoreOperation);

			attachments[i] = attachment;
		}

		//Create depth stencil attachment desc
		{
			if (bHasDepthStencilAttachment)
			{
				VkAttachmentDescription attachment = {};
				attachment.format = VulkanTextureUtils::GetTextureFormat(desc.DepthStencilAttachment.pTexture->GetFormat());
				attachment.samples = (VkSampleCountFlagBits)VulkanTextureUtils::GetSampleCount(desc.DepthStencilAttachment.pTexture->GetSampleCount());
				attachment.loadOp = VulkanRenderPassUtils::GetLoadOperation(desc.DepthStencilAttachment.ColorLoadOperation);
				attachment.storeOp = VulkanRenderPassUtils::GetStoreOperation(desc.DepthStencilAttachment.ColorStoreOperation);
				attachment.initialLayout = VulkanTextureUtils::GetImageLayout(desc.DepthStencilAttachment.InputLayout);
				attachment.finalLayout = VulkanTextureUtils::GetImageLayout(desc.DepthStencilAttachment.OutputLayout);
				attachment.stencilLoadOp = VulkanRenderPassUtils::GetLoadOperation(desc.DepthStencilAttachment.StencilLoadOperation);
				attachment.stencilStoreOp = VulkanRenderPassUtils::GetStoreOperation(desc.DepthStencilAttachment.StencilStoreOperation);
				attachments[attachments.size()-1] = attachment;
			}
		}

		//Create attachment references
		std::vector<VkAttachmentReference> references(totalAttachmentCount);
		for (unsigned char i = 0; i < totalAttachmentCount; i++)
		{
			VkAttachmentReference reference = {};
			reference.attachment = i;
			reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			references[i] = reference;
		}

		//Create sub passes
		std::vector<VkSubpassDescription> subpasses;
		for (const RenderPassSubpassDesc& subpassDesc : desc.Subpasses)
		{
			VkAttachmentReference colorAttachmentsReferences[16];
			for (unsigned char i = 0; i < subpassDesc.Attachments.size();i++)
			{
				const unsigned char attachmentIndex = subpassDesc.Attachments[i];
				colorAttachmentsReferences[i] = references[attachmentIndex];
			}
			VkAttachmentReference inputAttachmentReferences[16];
			for (unsigned char i = 0; i < subpassDesc.Inputs.size(); i++)
			{
				const unsigned char attachmentIndex = subpassDesc.Inputs[i];
				inputAttachmentReferences[i] = references[attachmentIndex];
			}
			VkAttachmentReference multiSampleAttachmentReferences[16];
			for (unsigned char i = 0; i < subpassDesc.MultisampleInputs.size(); i++)
			{
				const unsigned char attachmentIndex = subpassDesc.MultisampleInputs[i];
				multiSampleAttachmentReferences[i] = references[attachmentIndex];
			}
			uint32_t preserveAttachmentReferences[16];
			for (unsigned char i = 0; i < subpassDesc.PreserveAttachments.size(); i++)
			{
				const unsigned char attachmentIndex = subpassDesc.PreserveAttachments[i];
				preserveAttachmentReferences[i] = attachmentIndex;
			}

			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = subpassDesc.BindPoint == PipelineBindPoint::Graphics ? VK_PIPELINE_BIND_POINT_GRAPHICS : VK_PIPELINE_BIND_POINT_COMPUTE;
			subpass.preserveAttachmentCount = subpassDesc.PreserveAttachments.size();;
			subpass.pPreserveAttachments = preserveAttachmentReferences;

			subpass.colorAttachmentCount = subpassDesc.Attachments.size();
			subpass.pColorAttachments = colorAttachmentsReferences;

			subpass.inputAttachmentCount = subpassDesc.Inputs.size();
			subpass.pInputAttachments = inputAttachmentReferences;

			subpass.pResolveAttachments = nullptr;

			subpasses.push_back(subpass);
		}

		//Create dependencies
		std::vector<VkSubpassDependency> subpassDependencies;
		for (const RenderPassSubpassDependencyDesc& dependecyDesc : desc.Dependencies)
		{
			VkSubpassDependency subpassDependency = {};
			subpassDependency.srcSubpass = dependecyDesc.InputSubpass;
			subpassDependency.srcAccessMask = VulkanMemoryUtils::GetMemoryAccessFlags(dependecyDesc.InputAccess);
			subpassDependency.srcStageMask = VulkanPipelineUtils::GetStageFlags(dependecyDesc.InputStageFlags);
			subpassDependency.dstSubpass = dependecyDesc.OutputSubpass;
			subpassDependency.dstAccessMask = VulkanMemoryUtils::GetMemoryAccessFlags(dependecyDesc.OutputAccess);
			subpassDependency.dstStageMask = VulkanPipelineUtils::GetStageFlags(dependecyDesc.OutputStageFlags);
			subpassDependency.dependencyFlags = VkDependencyFlags();
			subpassDependencies.push_back(subpassDependency);
		}

		//Create render pass
		VkRenderPassCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.attachmentCount = attachments.size();
		info.subpassCount = subpasses.size();
		info.dependencyCount = subpassDependencies.size();
		info.flags = VkRenderPassCreateFlags();
		info.pAttachments = attachments.data();
		info.pSubpasses = subpasses.data();
		info.pDependencies = subpassDependencies.data();
		info.pNext = nullptr;

		DEV_ASSERT(vkCreateRenderPass(mLogicalDevice, &info, nullptr, &mRenderPass) == VK_SUCCESS, "VulkanRenderPass", "Failed to create render pass!");

		//Create framebuffer
		VkImageView imageViews[8] = {};
		for (unsigned char i = 0; i < desc.AttachmentViews.size(); i++)
		{
			const VulkanTextureView* pView = (const VulkanTextureView*)desc.AttachmentViews[i];
			imageViews[i] = pView->GetVkView();
		}

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.width = desc.TargetRenderWidth;
		framebufferInfo.height = desc.TargetRenderHeight;
		framebufferInfo.renderPass = mRenderPass;
		framebufferInfo.layers = 1;
		framebufferInfo.attachmentCount = desc.ColorAttachments.size();
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

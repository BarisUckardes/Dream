#pragma once
#include <Runtime/Graphics/RenderPass/RenderPassAttachmentDesc.h>
#include <Runtime/Graphics/RenderPass/RenderPassSubpassDesc.h>
#include <Runtime/Graphics/RenderPass/RenderPassSubpassDependencyDesc.h>
#include <Runtime/Graphics/Texture/TextureView.h>
#include <vector>

namespace Dream
{
	struct RUNTIME_API RenderPassDesc
	{
		unsigned int TargetRenderWidth;
		unsigned int TargetRenderHeight;
		std::vector<RenderPassAttachmentDesc> ColorAttachments;
		RenderPassAttachmentDesc DepthStencilAttachment;
		std::vector<RenderPassSubpassDesc> Subpasses;
		std::vector<RenderPassSubpassDependencyDesc> Dependencies;
		std::vector<TextureView*> AttachmentViews;
	};
}

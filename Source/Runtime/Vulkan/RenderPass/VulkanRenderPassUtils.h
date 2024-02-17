#pragma once
#include <Runtime/Graphics/RenderPass/RenderPassDesc.h>
#include <vulkan.h>

namespace Dream
{
	class RUNTIME_API VulkanRenderPassUtils
	{
	public:
		static VkAttachmentLoadOp GetLoadOperation( RenderPassLoadOperation op)
		{
			switch (op)
			{
			default:
			case Dream::RenderPassLoadOperation::Ignore:
				return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			case Dream::RenderPassLoadOperation::Load:
				return VK_ATTACHMENT_LOAD_OP_LOAD;
			case Dream::RenderPassLoadOperation::Clear:
				return VK_ATTACHMENT_LOAD_OP_CLEAR;
			}
		}
		static VkAttachmentStoreOp GetStoreOperation( RenderPassStoreOperation op)
		{
			switch (op)
			{
			case Dream::RenderPassStoreOperation::Store:
				return VK_ATTACHMENT_STORE_OP_STORE;
			case Dream::RenderPassStoreOperation::Ignore:
			default:
				return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			}
		}
	public:
		VulkanRenderPassUtils() = delete;
		~VulkanRenderPassUtils() = delete;
	};
}

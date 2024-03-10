#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/RenderPass/RenderPassDesc.h>
#include <Runtime/Graphics/Texture/Texture.h>

namespace Dream
{
	class RUNTIME_API RenderPass : public GraphicsDeviceObject
	{
	public:
		~RenderPass() = default;

		FORCEINLINE const std::vector<RenderPassAttachmentDesc>& color_attachments() const noexcept
		{
			return mAttachments;
		}
		FORCEINLINE unsigned int render_width() const noexcept
		{
			return mTargetRenderWidth;
		}
		FORCEINLINE unsigned int render_height() const noexcept
		{
			return mTargetRenderHeight;
		}
		FORCEINLINE virtual GraphicsDeviceObjectType object_type() const noexcept override final
		{
			return GraphicsDeviceObjectType::RenderPass;
		}
	protected:
		RenderPass(const RenderPassDesc& desc,GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
			mAttachments(desc.ColorAttachments),mTargetRenderWidth(desc.TargetRenderWidth),mTargetRenderHeight(desc.TargetRenderHeight)
		{

		}
	private:
		const std::vector<RenderPassAttachmentDesc> mAttachments;
		const unsigned int mTargetRenderWidth;
		const unsigned int mTargetRenderHeight;;
	};
}

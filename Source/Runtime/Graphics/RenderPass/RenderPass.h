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

		FORCEINLINE const std::vector<RenderPassAttachmentDesc>& GetColorAttachments() const noexcept
		{
			return mColorAttachments;
		}
		FORCEINLINE const RenderPassAttachmentDesc& GetDepthStencilAttachment() const noexcept
		{
			return mDepthStencilAttachment;
		}
		FORCEINLINE const std::vector<RenderPassSubpassDesc>& GetSubpasses() const noexcept
		{
			return mSubpasses;
		}
		FORCEINLINE const std::vector<RenderPassSubpassDependencyDesc> GetSubpassDependencies() const noexcept
		{
			return mDependencies;
		}
		FORCEINLINE unsigned int GetRenderWidth() const noexcept
		{
			return mTargetRenderWidth;
		}
		FORCEINLINE unsigned int GetRenderHeight() const noexcept
		{
			return mTargetRenderHeight;
		}
		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
		{
			return GraphicsDeviceObjectType::RenderPass;
		}
	protected:
		RenderPass(const RenderPassDesc& desc,GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
			mColorAttachments(desc.ColorAttachments), mDepthStencilAttachment(desc.DepthStencilAttachment), mSubpasses(desc.Subpasses),
			mDependencies(desc.Dependencies),mTargetRenderWidth(desc.TargetRenderWidth),mTargetRenderHeight(desc.TargetRenderHeight)
		{

		}
	private:
		const std::vector<RenderPassAttachmentDesc> mColorAttachments;
		const RenderPassAttachmentDesc mDepthStencilAttachment;
		const std::vector<RenderPassSubpassDesc> mSubpasses;
		const std::vector<RenderPassSubpassDependencyDesc> mDependencies;
		const unsigned int mTargetRenderWidth;
		const unsigned int mTargetRenderHeight;;
	};
}

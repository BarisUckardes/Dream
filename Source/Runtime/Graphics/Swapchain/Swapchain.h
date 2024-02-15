#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Swapchain/SwapchainDesc.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureView.h>

namespace Dream
{
	class RUNTIME_API Swapchain : public GraphicsDeviceObject
	{
	public:
		~Swapchain();

		FORCEINLINE PresentMode GetMode() const noexcept
		{
			return mMode;
		}
		FORCEINLINE unsigned char GetBufferCount() const noexcept
		{
			return mBufferCount;
		}
		FORCEINLINE TextureFormat GetColorBufferFormat() const noexcept
		{
			return mColorBufferFormat;
		}
		FORCEINLINE TextureFormat GetDepthStencilBufferFormat() const noexcept
		{
			return mDepthStencilBufferFormat;
		}
		FORCEINLINE const Window* GetWindow() const noexcept
		{
			return mWindow;
		}
		FORCEINLINE unsigned int GetWidth() const noexcept
		{
			return mWidth;
		}
		FORCEINLINE unsigned int GetHeight() const noexcept
		{
			return mHeight;
		}
		FORCEINLINE unsigned char GetCurrentIndex() const noexcept
		{
			return mIndex;
		}
		FORCEINLINE const GraphicsQueue* GetQueue() const noexcept
		{
			return mQueue;
		}
		virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
		{
			return GraphicsDeviceObjectType::Swapchain;
		}

		void Resize(const unsigned int width, const unsigned int height);
		void Present();
		void WaitForPresent(const unsigned char index);
	protected:
		Swapchain(const SwapchainDesc& desc, GraphicsDevice* pDevice);

		void SetCustomSize(const unsigned int width, const unsigned int height);
		void SetCustomSwapchainTextures(const std::vector<Texture*>& textures,Texture* pDepthStencilTexture, const std::vector<TextureView*>& views,TextureView* pDepthStencilView);

		FORCEINLINE Fence* GetPresentFence(const unsigned char index)
		{
			return mPresentFences[index];
		}

		virtual void ResizeCore(const unsigned int width, const unsigned int height) = 0;
		virtual void PresentCore() = 0;
	private:
		const PresentMode mMode;
		const unsigned char mBufferCount;
		const TextureFormat mColorBufferFormat;
		const TextureFormat mDepthStencilBufferFormat;
		const Window* mWindow;
		const GraphicsQueue* mQueue;
		std::vector<Fence*> mPresentFences;
		std::vector<Texture*> mColorTextures;
		std::vector<TextureView*> mColorTextureViews;
		Texture* mDepthStencilTexture;
		TextureView* mDepthStencilTextureView;
		unsigned int mWidth;
		unsigned int mHeight;
		unsigned char mIndex;
	};
}

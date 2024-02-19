#include "Swapchain.h"
#include <Runtime/Graphics/Device/GraphicsDevice.h>

namespace Dream
{
	Swapchain::~Swapchain()
	{
		ClearTextures();
	}
	void Swapchain::Resize(const unsigned int width, const unsigned int height)
	{
		//Wait device idle
		GetDevice()->WaitDeviceIdle();

		//Clear the textures
		ClearTextures();

		//Resize
		ResizeCore(width, height);

		mWidth = width;
		mHeight = height;
	}
	void Swapchain::Present(Semaphore** ppWaitSemahpores, const unsigned int waitSemaphoreCount)
	{
		//First wait for target fence
		GetDevice()->WaitFences(&mPresentFences[mImageIndex], 1);
		GetDevice()->ResetFences(&mPresentFences[mImageIndex],1);

		//Present
		PresentCore(ppWaitSemahpores,waitSemaphoreCount);

		//Increment index
		mImageIndex = (mImageIndex + 1) % mBufferCount;
	}
	void Swapchain::WaitForPresent(const unsigned char index)
	{
		GetDevice()->WaitFences(&mPresentFences[index], 1);
	}
	
	Swapchain::Swapchain(const SwapchainDesc& desc, GraphicsDevice* pDevice) :
		GraphicsDeviceObject(pDevice),mMode(desc.Mode),mBufferCount(desc.BufferCount),mColorBufferFormat(desc.ColorFormat),mDepthStencilBufferFormat(desc.DepthStencilFormat),mWindow(desc.pWindow),
		mImageIndex(0),mQueue(desc.pQueue)
	{
		mWidth = mWindow->GetWidth();
		mHeight = mWindow->GetHeight();

		for (unsigned char i = 0; i < desc.BufferCount; i++)
			mPresentFences.push_back(pDevice->CreateFence({ true }));
	}
	void Swapchain::SetCustomSize(const unsigned int width, const unsigned int height)
	{
		mWidth = width;
		mHeight = height;
	}
	void Swapchain::ClearTextures()
	{
		for (unsigned char i = 0; i < mColorTextures.size(); i++)
		{
			delete mColorTextureViews[i];
			delete mColorTextures[i];
		}
		mColorTextureViews.clear();
		mColorTextures.clear();
	}
	void Swapchain::SetCustomSwapchainTextures(const std::vector<Texture*>& textures)
	{
		mColorTextures = textures;
		for (Texture* pTexture : textures)
		{
			TextureViewDesc desc = {};
			desc.AspectFlags = TextureAspectFlags::Color;
			desc.pTexture = pTexture;
			desc.ArrayLevel = 0;
			desc.MipLevel = 0;

			mColorTextureViews.push_back(GetDevice()->CreateTextureView(desc));
		}
	}
}

#include "Swapchain.h"
#include <Runtime/Graphics/Device/GraphicsDevice.h>

namespace Dream
{
	Swapchain::~Swapchain()
	{

	}
	void Swapchain::Resize(const unsigned int width, const unsigned int height)
	{
		//Wait device idle
		GetDevice()->WaitDeviceIdle();

		//Resize
		ResizeCore(width, height);

		mWidth = width;
		mHeight = height;
	}
	void Swapchain::Present()
	{
		//First wait for target fence
		GetDevice()->WaitFences(&mPresentFences[mIndex], 1);
		GetDevice()->ResetFences(&mPresentFences[mIndex],1);

		//Present
		PresentCore();

		//Increment index
		mIndex = (mIndex + 1) % mBufferCount;
	}
	void Swapchain::WaitForPresent(const unsigned char index)
	{
		GetDevice()->WaitFences(&mPresentFences[index], 1);
	}
	Swapchain::Swapchain(const SwapchainDesc& desc, GraphicsDevice* pDevice) :
		GraphicsDeviceObject(pDevice),mMode(desc.Mode),mBufferCount(desc.BufferCount),mColorBufferFormat(desc.ColorFormat),mDepthStencilBufferFormat(desc.DepthStencilFormat),mWindow(desc.pWindow),
		mIndex(0),mQueue(desc.pQueue)
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
	void Swapchain::SetCustomSwapchainTextures(const std::vector<Texture*>& textures,const std::vector<TextureView*>& views)
	{
		mColorTextures = textures;
		mColorTextureViews = views;
	}
}

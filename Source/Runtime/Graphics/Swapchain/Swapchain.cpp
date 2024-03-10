#include "Swapchain.h"
#include <Runtime/Graphics/Device/GraphicsDevice.h>

namespace Dream
{
	Swapchain::~Swapchain()
	{
		clear_textures();
	}
	void Swapchain::resize(const unsigned int width, const unsigned int height)
	{
		//Wait device idle
		device()->wait_device_idle();

		//Clear the textures
		clear_textures();

		//resize
		resize_impl(width, height);

		mWidth = width;
		mHeight = height;
	}
	void Swapchain::Present(Semaphore** ppWaitSemahpores, const unsigned int waitSemaphoreCount)
	{
		//First wait for target fence
		device()->wait_fences(&mPresentFences[mImageIndex], 1);
		device()->reset_fences(&mPresentFences[mImageIndex],1);

		//Present
		Present_impl(ppWaitSemahpores,waitSemaphoreCount);

		//Increment index
		mImageIndex = (mImageIndex + 1) % mBufferCount;
	}
	void Swapchain::wait_present(const unsigned char index)
	{
		device()->wait_fences(&mPresentFences[index], 1);
	}
	
	Swapchain::Swapchain(const SwapchainDesc& desc, GraphicsDevice* pDevice) :
		GraphicsDeviceObject(pDevice),mMode(desc.Mode),mBufferCount(desc.BufferCount),mColorBufferFormat(desc.ColorFormat),mWindow(desc.pWindow),
		mImageIndex(0),mQueue(desc.pQueue)
	{
		mWidth = mWindow->width();
		mHeight = mWindow->height();

		for (unsigned char i = 0; i < desc.BufferCount; i++)
			mPresentFences.push_back(pDevice->create_fence({ true }));
	}
	void Swapchain::set_custom_size(const unsigned int width, const unsigned int height)
	{
		mWidth = width;
		mHeight = height;
	}
	void Swapchain::clear_textures()
	{
		for (unsigned char i = 0; i < mColorTextures.size(); i++)
		{
			delete mColorTextureViews[i];
			delete mColorTextures[i];
		}
		mColorTextureViews.clear();
		mColorTextures.clear();
	}
	void Swapchain::set_custom_textures(const std::vector<Texture*>& textures)
	{
		mColorTextures = textures;
		for (Texture* pTexture : textures)
		{
			TextureViewDesc desc = {};
			desc.AspectFlags = TextureAspectFlags::Color;
			desc.pTexture = pTexture;
			desc.ArrayLevel = 0;
			desc.MipLevel = 0;

			mColorTextureViews.push_back(device()->create_texture_view(desc));
		}
	}
}

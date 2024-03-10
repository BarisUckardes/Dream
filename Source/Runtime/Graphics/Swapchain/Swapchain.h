#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Swapchain/SwapchainDesc.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureView.h>
#include <Runtime/Graphics/Semaphore/Semaphore.h>

namespace Dream
{
	class RUNTIME_API Swapchain : public GraphicsDeviceObject
	{
	public:
		~Swapchain();

		FORCEINLINE PresentMode mode() const noexcept
		{
			return mMode;
		}
		FORCEINLINE unsigned char buffer_count() const noexcept
		{
			return mBufferCount;
		}
		FORCEINLINE TextureFormat buffer_format() const noexcept
		{
			return mColorBufferFormat;
		}
		FORCEINLINE const std::vector<Texture*>& buffers() const noexcept
		{
			return mColorTextures;
		}
		FORCEINLINE const std::vector<TextureView*>& buffer_views() const noexcept
		{
			return mColorTextureViews;
		}
		FORCEINLINE const Window* window() const noexcept
		{
			return mWindow;
		}
		FORCEINLINE unsigned int width() const noexcept
		{
			return mWidth;
		}
		FORCEINLINE unsigned int height() const noexcept
		{
			return mHeight;
		}
		FORCEINLINE unsigned char image_index() const noexcept
		{
			return mImageIndex;
		}
		FORCEINLINE const GraphicsQueue* queue() const noexcept
		{
			return mQueue;
		}
		virtual GraphicsDeviceObjectType object_type() const noexcept override final
		{
			return GraphicsDeviceObjectType::Swapchain;
		}

		void resize(const unsigned int width, const unsigned int height);
		void Present(Semaphore** ppWaitSemahpores,const unsigned int waitSemaphoreCount);
		void wait_present(const unsigned char index);
	protected:
		Swapchain(const SwapchainDesc& desc, GraphicsDevice* pDevice);

		void set_custom_size(const unsigned int width, const unsigned int height);
		void set_custom_textures(const std::vector<Texture*>& textures);

		FORCEINLINE Fence* get_Present_fence(const unsigned char index)
		{
			return mPresentFences[index];
		}

		virtual void resize_impl(const unsigned int width, const unsigned int height) = 0;
		virtual void Present_impl(Semaphore** ppWaitSemahpores, const unsigned int waitSemaphoreCount) = 0;
	private:
		void clear_textures();
	private:
		const PresentMode mMode;
		const unsigned char mBufferCount;
		const TextureFormat mColorBufferFormat;
		const Window* mWindow;
		const GraphicsQueue* mQueue;
		std::vector<Fence*> mPresentFences;
		std::vector<Texture*> mColorTextures;
		std::vector<TextureView*> mColorTextureViews;
		unsigned int mWidth;
		unsigned int mHeight;
		unsigned char mImageIndex;
	};
}

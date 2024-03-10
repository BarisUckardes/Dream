#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapterDesc.h>
#include <vector>
#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>

namespace Dream
{
	class GraphicsDevice;
	class RUNTIME_API GraphicsAdapter
	{
	public:
		~GraphicsAdapter();


		FORCEINLINE std::string product_name() const noexcept { return mProductName; }
		FORCEINLINE std::string vendor_name() const noexcept { return mVendorName; }
		FORCEINLINE bool support_geometryShader() const noexcept { return mSupportGeometryShader; }
		FORCEINLINE bool support_compute_shader() const noexcept { return mSupportComputeShader; }
		FORCEINLINE unsigned int color_attachment_count() const noexcept { return mColorAttachmentCount; }
		FORCEINLINE unsigned int shader_texture_count() const noexcept { return mShaderTextureCount; }
		FORCEINLINE unsigned int shader_sampler_count() const noexcept { return mShaderSamplerCount; }
		FORCEINLINE unsigned int shader_buffer_count() const noexcept { return mShaderBufferCount; }
		FORCEINLINE unsigned long long memory() const noexcept { return mMemory; }
		FORCEINLINE unsigned int image_width() const noexcept
		{
			return mImageWidth;
		}
		FORCEINLINE unsigned int image_height() const noexcept
		{
			return mImageHeight;
		}
		FORCEINLINE unsigned int image_depth() const noexcept
		{
			return mImageDepth;
		}
		FORCEINLINE GraphicsInstance* owner_instance() const noexcept
		{
			return mOwnerInstance;
		}

		GraphicsDevice* create_device(const GraphicsDeviceDesc* pDesc);
	protected:
		GraphicsAdapter(const GraphicsAdapterDesc& desc);
	protected:
		virtual GraphicsDevice* create_device_impl(const GraphicsDeviceDesc* pDesc) = 0;
	private:
		const std::string mProductName;
		const std::string mVendorName;
		const bool mSupportGeometryShader;
		const bool mSupportComputeShader;
		const unsigned char mColorAttachmentCount;
		const unsigned int mShaderTextureCount;
		const unsigned int mShaderSamplerCount;
		const unsigned int mShaderBufferCount;
		const unsigned long long mMemory;
		const unsigned int mImageWidth;
		const unsigned int mImageHeight;
		const unsigned int mImageDepth;
		GraphicsInstance* mOwnerInstance;
		std::vector<GraphicsDevice*> mDevices;
	};
}

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


		FORCEINLINE std::string GetProductName() const noexcept { return mProductName; }
		FORCEINLINE std::string GetVendorName() const noexcept { return mVendorName; }
		FORCEINLINE bool DoesSupportGeometryShader() const noexcept { return mSupportGeometryShader; }
		FORCEINLINE bool DoesSupportComputeShader() const noexcept { return mSupportComputeShader; }
		FORCEINLINE unsigned int GetColorAttachmentCount() const noexcept { return mColorAttachmentCount; }
		FORCEINLINE unsigned int GetShaderTextureCount() const noexcept { return mShaderTextureCount; }
		FORCEINLINE unsigned int GetShaderSamplerCount() const noexcept { return mShaderSamplerCount; }
		FORCEINLINE unsigned int GetShaderBufferCount() const noexcept { return mShaderBufferCount; }
		FORCEINLINE unsigned long long GetMemory() const noexcept { return mMemory; }
		FORCEINLINE unsigned int GetImageWidth() const noexcept
		{
			return mImageWidth;
		}
		FORCEINLINE unsigned int GetImageHeight() const noexcept
		{
			return mImageHeight;
		}
		FORCEINLINE unsigned int GetImageDepth() const noexcept
		{
			return mImageDepth;
		}
		FORCEINLINE GraphicsInstance* GetOwnerInstance() const noexcept
		{
			return mOwnerInstance;
		}

		GraphicsDevice* CreateDevice(const GraphicsDeviceDesc* pDesc);
	protected:
		GraphicsAdapter(const GraphicsAdapterDesc& desc);
	protected:
		virtual GraphicsDevice* CreateDeviceCore(const GraphicsDeviceDesc* pDesc) = 0;
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

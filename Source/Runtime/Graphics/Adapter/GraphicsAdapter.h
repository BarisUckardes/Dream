#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapterDesc.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <vulkan.h>

namespace Dream
{
	class RUNTIME_API GraphicsAdapter final
	{
		friend class GraphicsDevice;
		friend class GraphicsInstance;
	public:
		~GraphicsAdapter();

		FORCEINLINE VkPhysicalDevice GetPhysicalDevice() const noexcept
		{
			return mPhysicalDevice;
		}
		FORCEINLINE std::string GetProductName() const noexcept
		{
			return mProductName;
		}
		FORCEINLINE std::string GetVendorName() const noexcept
		{
			return mVendorName;
		}
		FORCEINLINE bool DoesSupportGeometryShader() const noexcept
		{
			return mSupportGeometryShader;
		}
		FORCEINLINE bool DoesSupportComputeShader() const noexcept
		{
			return mSupportComputeShader;
		}
		FORCEINLINE unsigned char GetColorAttachmentCount() const noexcept
		{
			return mColorAttachmentCount;
		}
		FORCEINLINE unsigned int GetShaderTextureCount() const noexcept
		{
			return mShaderTextureCount;
		}
		FORCEINLINE unsigned int GetShaderSamplerCount() const noexcept
		{
			return mShaderSamplerCount;
		}
		FORCEINLINE unsigned int GetShaderBufferCount() const noexcept
		{
			return mShaderBufferCount;
		}
		FORCEINLINE unsigned long long GetDeviceMemory() const noexcept
		{
			return mDeviceMemory;
		}
		FORCEINLINE unsigned int GetMaxImageDimensionWidth() const noexcept
		{
			return mImageDimensions[0];
		}
		FORCEINLINE unsigned int GetMaxImageDimensionHeight() const noexcept
		{
			return mImageDimensions[1];
		}
		FORCEINLINE unsigned int GetMaxImageDimensionDepth() const noexcept
		{
			return mImageDimensions[2];
		}
		FORCEINLINE GraphicsInstance* GetOwnerInstance() const noexcept
		{
			return mOwnerInstance;
		}

		GraphicsDevice* CreateDevice(const GraphicsDeviceDesc& desc);
	private:
		GraphicsAdapter(const GraphicsAdapterDesc desc);

		void _OnDeviceDeleted(GraphicsDevice* pDevice);
	private:
		const VkPhysicalDevice mPhysicalDevice;
		const std::string mProductName;
		const std::string mVendorName;
		const bool mSupportGeometryShader;
		const bool mSupportComputeShader;
		const unsigned char mColorAttachmentCount;
		const unsigned int mShaderTextureCount;
		const unsigned int mShaderSamplerCount;
		const unsigned int mShaderBufferCount;
		const unsigned long long mDeviceMemory;
		unsigned int mImageDimensions[3];
		GraphicsInstance* mOwnerInstance;
		std::vector<GraphicsDevice*> mDevices;
	};
}

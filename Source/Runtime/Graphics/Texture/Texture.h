#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Texture/TextureDesc.h>

namespace Dream
{
	class RUNTIME_API Texture : public GraphicsDeviceObject
	{
	public:
		~Texture();

		FORCEINLINE TextureType GetTextureType() const noexcept
		{
			return mType;
		}
		FORCEINLINE TextureUsage GetUsages() const noexcept
		{
			return mUsages;
		}
		FORCEINLINE TextureFormat GetFormat() const noexcept
		{
			return mFormat;
		}
		FORCEINLINE unsigned int GetWidth() const noexcept
		{
			return mWidth;
		}
		FORCEINLINE unsigned int GetHeight() const noexcept
		{
			return mHeight;
		}
		FORCEINLINE unsigned int GetDepth() const noexcept
		{
			return mDepth;
		}
		FORCEINLINE unsigned char GetMipLevels() const noexcept
		{
			return mMipLevels;
		}
		FORCEINLINE unsigned char GetArrayLevels() const noexcept
		{
			return mArrayLevels;
		}
		FORCEINLINE TextureSampleCount GetSampleCount() const noexcept
		{
			return mSampleCount;
		}
		virtual GraphicsDeviceObjectType GetObjectType() const noexcept
		{
			return GraphicsDeviceObjectType::Texture;
		}
	protected:
		Texture(const TextureDesc& desc, GraphicsDevice* pDevice);

	private:
		const TextureType mType;
		const TextureUsage mUsages;
		const TextureFormat mFormat;
		const unsigned int mWidth;
		const unsigned int mHeight;
		const unsigned int mDepth;
		const unsigned char mMipLevels;
		const unsigned char mArrayLevels;
		const TextureSampleCount mSampleCount;
	};
}

#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Texture/TextureDesc.h>

namespace Dream
{
	class RUNTIME_API Texture : public GraphicsDeviceObject
	{
	public:
		~Texture();

		FORCEINLINE TextureType texture_type() const noexcept
		{
			return mType;
		}
		FORCEINLINE TextureUsage usages() const noexcept
		{
			return mUsages;
		}
		FORCEINLINE TextureFormat format() const noexcept
		{
			return mFormat;
		}
		FORCEINLINE unsigned int width() const noexcept
		{
			return mWidth;
		}
		FORCEINLINE unsigned int height() const noexcept
		{
			return mHeight;
		}
		FORCEINLINE unsigned int depth() const noexcept
		{
			return mDepth;
		}
		FORCEINLINE unsigned char mip_levels() const noexcept
		{
			return mMipLevels;
		}
		FORCEINLINE unsigned char array_levels() const noexcept
		{
			return mArrayLevels;
		}
		FORCEINLINE TextureSampleCount sample_count() const noexcept
		{
			return mSampleCount;
		}
		virtual GraphicsDeviceObjectType object_type() const noexcept
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

#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <RUntime/Graphics/Texture/TextureViewDesc.h>

namespace Dream
{
	class RUNTIME_API TextureView : public GraphicsDeviceObject
	{
	public:
		~TextureView();

		FORCEINLINE unsigned char array_level() const noexcept
		{
			return mArrayLevel;
		}
		FORCEINLINE unsigned char mip_level() const noexcept
		{
			return mMipLevel;
		}
		FORCEINLINE const Texture* target_texture() const noexcept
		{
			return mTexture;
		}

		virtual GraphicsDeviceObjectType object_type() const noexcept override final
		{
			return GraphicsDeviceObjectType::TextureView;
		}
	protected:
		TextureView(const TextureViewDesc& desc, GraphicsDevice* pDevice);

	private:
		const unsigned char mArrayLevel;
		const unsigned char mMipLevel;
		const Texture* mTexture;
	};
}

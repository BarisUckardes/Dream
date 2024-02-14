#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <RUntime/Graphics/Texture/TextureViewDesc.h>

namespace Dream
{
	class RUNTIME_API TextureView : public GraphicsDeviceObject
	{
	public:
		~TextureView();

		FORCEINLINE unsigned char GetArrayLevel() const noexcept
		{
			return mArrayLevel;
		}
		FORCEINLINE unsigned char GetMipLevel() const noexcept
		{
			return mMipLevel;
		}
		FORCEINLINE const Texture* GetTargetTexture() const noexcept
		{
			return mTexture;
		}

		virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
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

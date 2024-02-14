#include "Texture.h"

namespace Dream
{
	Texture::~Texture()
	{
	}
	Texture::Texture(const TextureDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
		mType(desc.Type), mUsages(desc.Usages), mFormat(desc.Format),
		mWidth(desc.Width),mHeight(desc.Height),mDepth(desc.Depth), mMipLevels(desc.MipLevels), mArrayLevels(desc.ArrayLevels),
		mSampleCount(desc.SampleCount)
	{

	}
}

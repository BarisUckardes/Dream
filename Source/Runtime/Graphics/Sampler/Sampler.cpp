#include "Sampler.h"

namespace Dream
{
    Sampler::~Sampler()
    {
    }
    Sampler::Sampler(const SamplerDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
        mMinFilter(desc.MinFilter),
        mMagFilter(desc.MagFilter),
        mMipmapMode(desc.MipmapMode),
        mAddressModeU(desc.AddressModeU),
        mAddressModeV(desc.AddressModeV),
        mAddressModeW(desc.AddressModeW),
        mMipLodBias(desc.MipLodBias),
        mMaxAnisotropy(desc.MaxAnisotropy),
        mComparisonEnabled(desc.bComparisonEnabled),
        mCompareOperation(desc.CompareOperation),
        mMinLod(desc.MinLod),
        mMaxLod(desc.MaxLod),
        mBorderColor(desc.BorderColor)
    {

    }
}

#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Sampler/SamplerDesc.h>

namespace Dream
{
	class RUNTIME_API Sampler : public GraphicsDeviceObject
	{
	public:
		~Sampler();

        FORCEINLINE const SamplerFiltering GetMinFilter() const
        {
            return mMinFilter;
        }
        FORCEINLINE const SamplerFiltering GetMagFilter() const
        {
            return mMagFilter;
        }
        FORCEINLINE const SamplerMapMode GetMipmapMode() const
        {
            return mMipmapMode;
        }
        FORCEINLINE const SamplerAddressMode GetAddressModeU() const
        {
            return mAddressModeU;
        }
        FORCEINLINE const SamplerAddressMode GetAddressModeV() const
        {
            return mAddressModeV;
        }
        FORCEINLINE const SamplerAddressMode GetAddressModeW() const
        {
            return mAddressModeW;
        }
        FORCEINLINE const float GetMipLodBias() const
        {
            return mMipLodBias;
        }
        FORCEINLINE const float GetMaxAnisotropy() const
        {
            return mMaxAnisotropy;
        }
        FORCEINLINE const bool IsComparisonEnabled() const
        {
            return mComparisonEnabled;
        }
        FORCEINLINE const CompareOperation GetCompareOperation() const
        {
            return mCompareOperation;
        }
        FORCEINLINE const float GetMinLod() const
        {
            return mMinLod;
        }
        FORCEINLINE const float GetMaxLod() const
        {
            return mMaxLod;
        }
        FORCEINLINE const SamplerBorderColor GetBorderColor() const
        {
            return mBorderColor;
        }

        FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept override
        {
            return GraphicsDeviceObjectType::Sampler;
        }
	protected:
		Sampler(const SamplerDesc& desc, GraphicsDevice* pDevice);
	private:
        const SamplerFiltering mMinFilter;
        const SamplerFiltering mMagFilter;
        const SamplerMapMode mMipmapMode;
        const SamplerAddressMode mAddressModeU;
        const SamplerAddressMode mAddressModeV;
        const SamplerAddressMode mAddressModeW;
        const float mMipLodBias;
        const float mMaxAnisotropy;
        const bool mComparisonEnabled;
        const CompareOperation mCompareOperation;
        const float mMinLod;
        const float mMaxLod;
        const SamplerBorderColor mBorderColor;
	};
}

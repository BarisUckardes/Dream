#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Sampler/SamplerDesc.h>

namespace Dream
{
	class RUNTIME_API Sampler : public GraphicsDeviceObject
	{
	public:
		~Sampler();

        FORCEINLINE const SamplerFiltering min_filter() const
        {
            return mMinFilter;
        }
        FORCEINLINE const SamplerFiltering mag_filter() const
        {
            return mMagFilter;
        }
        FORCEINLINE const SamplerMapMode mipmap_mode() const
        {
            return mMipmapMode;
        }
        FORCEINLINE const SamplerAddressMode address_u() const
        {
            return mAddressModeU;
        }
        FORCEINLINE const SamplerAddressMode address_v() const
        {
            return mAddressModeV;
        }
        FORCEINLINE const SamplerAddressMode address_w() const
        {
            return mAddressModeW;
        }
        FORCEINLINE const float mip_lod_bias() const
        {
            return mMipLodBias;
        }
        FORCEINLINE const float mas_anisotropy() const
        {
            return mMaxAnisotropy;
        }
        FORCEINLINE const bool comparision_enabled() const
        {
            return mComparisonEnabled;
        }
        FORCEINLINE const CompareOperation compare_operation() const
        {
            return mCompareOperation;
        }
        FORCEINLINE const float min_lod() const
        {
            return mMinLod;
        }
        FORCEINLINE const float max_lod() const
        {
            return mMaxLod;
        }
        FORCEINLINE const SamplerBorderColor border_color() const
        {
            return mBorderColor;
        }

        FORCEINLINE virtual GraphicsDeviceObjectType object_type() const noexcept override
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

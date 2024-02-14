#pragma once
#include <Runtime/Graphics/Sampler/SamplerFiltering.h>
#include <Runtime/Graphics/Sampler/SamplerMapMode.h>
#include <Runtime/Graphics/Sampler/SamplerAddressMode.h>
#include <Runtime/Graphics/Sampler/SamplerBorderColor.h>
#include <Runtime/Graphics/Common/CompareOperation.h>

namespace Dream
{
    struct RUNTIME_API SamplerDesc
    {
        SamplerFiltering MinFilter;
        SamplerFiltering MagFilter;
        SamplerMapMode MipmapMode;
        SamplerAddressMode AddressModeU;
        SamplerAddressMode AddressModeV;
        SamplerAddressMode AddressModeW;
        float MipLodBias;
        float MaxAnisotropy;
        bool bComparisonEnabled;
        CompareOperation CompareOperation;
        float MinLod;
        float MaxLod;
        SamplerBorderColor BorderColor;
    };
}

#pragma once
#include <Runtime/Graphics/Pipeline/RasterizerState/PolygonMode.h>
#include <Runtime/Graphics/Pipeline/RasterizerState/FaceCullMode.h>

namespace Dream
{
    struct RUNTIME_API RasterizerStateDesc
    {
        bool bDepthBiasEnabled;
        float DepthBiasClamp;
        float DepthBiasFactor;
        float DepthBiasSlope;
        PolygonMode FillMode;
        FaceCullMode CullFlags;
        bool bFrontCounterClockwise;
    };
}

#pragma once
#include <Runtime/Graphics/Texture/TextureSampleCount.h>

namespace Dream
{
    struct RUNTIME_API MultisampleDesc
    {
        bool bSampleShadingEnabled;
        TextureSampleCount Samples;
    };
}

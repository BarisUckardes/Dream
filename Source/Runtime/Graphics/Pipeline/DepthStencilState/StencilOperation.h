#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
    enum class RUNTIME_API StencilOperation : unsigned char
    {
        Keep,
        Zero,
        Replace,
        IncrementAndClamp,
        DecrementAndClamp,
        Invert,
        IncrementAndWrap,
        DecrementAndWrap
    };
}

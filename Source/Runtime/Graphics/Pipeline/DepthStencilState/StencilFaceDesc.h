#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/DepthStencilState/StencilOperation.h>
#include <Runtime/Graphics/Common/CompareOperation.h>

namespace Dream
{
    struct RUNTIME_API StencilFaceDesc
    {
        StencilOperation FailOperation;
        StencilOperation PassOperation;
        StencilOperation DepthFailOperation;
        CompareOperation CompareOperation;
        unsigned int CompareMask;
        unsigned int WriteMask;
        unsigned int Reference;
    };
}

#pragma once
#include <Runtime/Graphics/Pipeline/InputLayout/MeshTopology.h>
#include <Runtime/Graphics/Pipeline/InputLayout/InputBinding.h>

namespace Dream
{
    struct RUNTIME_API InputLayoutDesc
    {
        MeshTopology Topology;
        std::vector<InputBinding> Bindings;
    };
}

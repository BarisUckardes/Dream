#pragma once
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetLayout.h>
#include <vector>

namespace Dream
{
    struct RUNTIME_API ComputePipelineDesc
    {
        std::vector<DescriptorSetLayout*> DescriptorSetLayouts;
        Shader* pComputeShader;
    };
}

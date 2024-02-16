#pragma once
#include <vector>

namespace Dream
{
    class DescriptorSetLayout;
    struct RUNTIME_API ResourceLayoutDesc
    {
        std::vector<DescriptorSetLayout*> ResourceLayouts;
    };
}

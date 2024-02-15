#pragma once
#include <vector>

namespace Dream
{
    class ResourceTableLayout;
    struct RUNTIME_API ResourceLayoutDesc
    {
        std::vector<ResourceTableLayout*> ResourceLayouts;
    };
}

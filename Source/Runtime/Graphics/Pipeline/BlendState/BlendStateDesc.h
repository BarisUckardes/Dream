#pragma once
#include <Runtime/Graphics/Common/LogicOperation.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendStateAttachment.h>
#include <vector>

namespace Dream
{
    struct RUNTIME_API BlendStateDesc
    {
        bool bLogicOperationEnabled;
        LogicOperation LogicOperation;
        std::vector<BlendStateAttachment> Attachments;
    };
}

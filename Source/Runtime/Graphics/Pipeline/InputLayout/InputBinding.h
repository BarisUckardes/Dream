#pragma once
#include <Runtime/Graphics/Pipeline/InputLayout/InputBindingStepRate.h>
#include <Runtime/Graphics/Pipeline/InputLayout/InputElement.h>
#include <vector>

namespace Dream
{
    struct RUNTIME_API InputBinding
    {
        InputBindingStepRate StepRate;
        std::vector<InputElement> Elements;
    };
}

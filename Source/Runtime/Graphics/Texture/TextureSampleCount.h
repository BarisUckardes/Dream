#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
    enum class RUNTIME_API TextureSampleCount : unsigned char
    {
        SAMPLE_COUNT_1 = 1,
        SAMPLE_COUNT_2 = 2,
        SAMPLE_COUNT_4 = 4,
        SAMPLE_COUNT_8 = 8,
        SAMPLE_COUNT_16 = 16,
        SAMPLE_COUNT_32 = 32,
        SAMPLE_COUNT_64 = 64
    };
}

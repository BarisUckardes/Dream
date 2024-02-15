#pragma
#include <Runtime/Core/Core.h>

namespace Dream
{
    enum class RUNTIME_API InputElementSemantic : unsigned char
    {
        Position,
        Normal,
        Tangent,
        Binormal,
        Color,
        TexCoord
    };
}

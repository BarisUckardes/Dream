#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <string>

namespace Dream
{
    class RUNTIME_API TextureUtils
    {
    public:
        TextureUtils() = delete;
        ~TextureUtils() = delete;

        static unsigned char format_size(const TextureFormat format);

    };
}

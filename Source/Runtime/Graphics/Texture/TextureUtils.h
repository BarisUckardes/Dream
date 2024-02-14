#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>

namespace Dream
{
    class RUNTIME_API TextureUtils
    {
    public:
        TextureUtils() = delete;
        ~TextureUtils() = delete;

        static unsigned char GetFormatSize(const TextureFormat format);
    };
}

#pragma once
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureAspectFlags.h>

namespace Dream
{
	struct RUNTIME_API TextureViewDesc
	{
		TextureAspectFlags AspectFlags;
		unsigned char MipLevel;
		unsigned char ArrayLevel;
		Texture* pTexture;
	};
}

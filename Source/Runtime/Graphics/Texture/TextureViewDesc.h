#pragma once

namespace Dream
{
	class Texture;
	struct RUNTIME_API TextureViewDesc
	{
		unsigned char MipLevel;
		unsigned char ArrayLevel;
		Texture* pTexture;
	};
}

#pragma once

namespace Dream
{
	struct RUNTIME_API BufferTextureCopyDesc
	{
		unsigned long long BufferOffsetInBytes;
		unsigned int TextureOffsetX;
		unsigned int TextureOffsetY;
		unsigned int TextureOffsetZ;
		unsigned char TargetArrayIndex;
		unsigned char TargetMipIndex;
	};
}

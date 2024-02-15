#pragma once

namespace Dream
{
	struct RUNTIME_API BufferTextureCopyDesc
	{
		unsigned long long BufferOffsetInBytes;
		unsigned int TextureOffsetX;
		unsigned int TextureOffsetY;
		unsigned int TextureOffsetZ;
		unsigned int Width;
		unsigned int Height;
		unsigned int Depth;
		unsigned char TargetArrayIndex;
		unsigned char TargetMipIndex;
	};
}

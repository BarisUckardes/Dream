#pragma once
#include <Runtime/Graphics/Texture/TextureAspectFlags.h>
#include <Runtime/Graphics/Sampler/SamplerFiltering.h>

namespace Dream
{
	struct RUNTIME_API TextureCopyDesc
	{
		TextureAspectFlags SourceAspect;
		unsigned int SourceOffsetX;
		unsigned int SourceOffsetY;
		unsigned int SourceOffsetZ;
		unsigned char SourceArrayIndex;
		unsigned char SourceMipIndex;

		TextureAspectFlags DestinationAspect;
		unsigned int DestinationOffsetX;
		unsigned int DestinationOffsetY;
		unsigned int DestinationOffsetZ;
		unsigned char DestinationArrayIndex;
		unsigned char DestinationMipIndex;

		SamplerFiltering Filtering;
	};
}

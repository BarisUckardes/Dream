#pragma once
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureSampleCount.h>
#include <Runtime/Graphics/Texture/TextureType.h>
#include <Runtime/Graphics/Texture/TextureUsage.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>

namespace Dream
{

	struct RUNTIME_API TextureDesc
	{
		TextureType Type;
		TextureUsage Usages;
		TextureFormat Format;
		unsigned int Width;
		unsigned int Height;
		unsigned int Depth;
		unsigned char ArrayLevels;
		unsigned char MipLevels;
		TextureSampleCount SampleCount;
		GraphicsMemory* pMemory;
	};
}

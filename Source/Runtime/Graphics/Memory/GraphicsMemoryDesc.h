#pragma once
#include <Runtime/Graphics/Memory/GraphicsMemoryType.h>

namespace Dream
{
	struct RUNTIME_API GraphicsMemoryDesc
	{
		GraphicsMemoryType Type;
		unsigned long long SizeInBytes;
	};
}

#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	enum class RUNTIME_API TextureAspectFlags : unsigned char
	{
		Color = 1,
		Depth = 2,
		Stencil = 4
	};
	GENERATE_FLAGS(TextureAspectFlags, unsigned char);
}

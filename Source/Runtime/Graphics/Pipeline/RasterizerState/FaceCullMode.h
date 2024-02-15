#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	enum class RUNTIME_API FaceCullMode : unsigned char
	{
		None = 1,
		Front = 2,
		Back = 4
	};

	GENERATE_FLAGS(FaceCullMode, unsigned char)
}
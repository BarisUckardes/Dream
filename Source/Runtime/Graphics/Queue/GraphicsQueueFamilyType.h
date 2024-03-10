#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	enum class RUNTIME_API GraphicsQueueFamilyType : unsigned char
	{
		Graphics,
		Compute,
		Transfer,
	};
}

#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	enum class RUNTIME_API GraphicsQueueType : unsigned char
	{
		Graphics,
		Compute,
		Transfer
	};
}

#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	enum class RUNTIME_API RenderPassLoadOperation : unsigned char
	{
		Ignore,
		Load,
		Clear
	};
}

#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	enum class RUNTIME_API ShaderStage : unsigned char
	{
		Vertex = 1,
		Fragment = 2,
		Geometry = 4,
		TesellationEval = 8,
		TesellationControl = 16,
		Compute = 32
	};
	GENERATE_FLAGS(ShaderStage,unsigned char)
}

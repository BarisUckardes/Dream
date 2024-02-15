#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	enum class RUNTIME_API MeshTopology : unsigned char
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		PatchList,
		TriangleStrip,
		TriangleFan
	};
}
#pragma once

namespace Dream
{
	struct RUNTIME_API ViewportDesc
	{
		unsigned int Width;
		unsigned int Height;
		unsigned int X;
		unsigned int Y;
		float DepthMin;
		float DepthMax;
	};
}

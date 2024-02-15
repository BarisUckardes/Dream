#pragma once

namespace Dream
{
	enum class RUNTIME_API PresentMode : unsigned char
	{
		Immediate,
		VsyncImmediate,
		VsyncQueued
	};
}

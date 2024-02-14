#pragma once
#include <Runtime/Window/WindowMode.h>
#include <string>

namespace Dream
{
	struct RUNTIME_API WindowDesc
	{
		std::string Title;
		int X;
		int Y;
		unsigned int Width;
		unsigned int Height;
	};
}

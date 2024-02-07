#pragma once
#include <string>
#include <Runtime/Window/WindowMode.h>

namespace Dream
{
	struct RUNTIME_API WindowCreateDesc
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		int X;
		int Y;
		WindowMode Mode;
	};
}

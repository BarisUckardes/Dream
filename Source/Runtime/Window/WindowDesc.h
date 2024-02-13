#pragma once
#include <Runtime/Window/WindowMode.h>
#include <string>

namespace Dream
{
	struct RUNTIME_API WindowDesc
	{
		std::string Title;
		int Offset[2];
		unsigned int Size[2];
	};
}

#pragma once
#include <string>
#include <Runtime/Window/WindowMode.h>

namespace Dream
{
	struct RUNTIME_API WindowCreateDesc
	{
		std::string Title;
		unsigned int Size[2];
		int Position[2];
	};
}

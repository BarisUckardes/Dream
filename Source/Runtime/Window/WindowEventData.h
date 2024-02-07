#pragma once
#include <Runtime/Window/WindowEventType.h>
#include <Runtime/Input/KeyboardKeys.h>
#include <Runtime/Input/MouseButtons.h>
#include <string>
#include <vector>

namespace Dream
{
	struct RUNTIME_API WindowEventData
	{
		WindowEventType Type;

		unsigned int WindowSize[2];
		unsigned int WindowPosition[2];

		MouseButtons MouseButton;
		int MousePosition[2];
		float MouseWheelDelta;

		KeyboardKeys KeyboardKey;
		char KeyboardChar;

		std::vector<std::string> DropItems;
	};
}

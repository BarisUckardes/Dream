#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Window/WindowEventType.h>
#include <Runtime/Input/KeyboardKeys.h>
#include <Runtime/Input/MouseButtons.h>
#include <string>
#include <vector>

namespace Dream
{
	/// <summary>
	/// Event data that PlatformWindow broadcasts
	/// </summary>
	struct RUNTIME_API WindowEventData
	{
		WindowEventType Type;

		unsigned int WindowSize[2];
		int WindowPosition[2];

		MouseButtons MouseButton;
		int MousePosition[2];
		float MouseWheelDelta;

		KeyboardKeys KeyboardKey;
		char KeyboardChar;

		unsigned char WindowIndex;

		std::vector<std::string> DropItems;
	};
}
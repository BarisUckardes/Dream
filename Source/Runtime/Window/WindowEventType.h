#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	/// <summary>
	/// RePresents the available types of PlatformWindow events
	/// </summary>
	enum class RUNTIME_API WindowEventType : unsigned char
	{
		None,

		WindowClosed,
		WindowMoved,
		Windowresized,

		DragDrop,

		KeyboardDown,
		KeyboardUp,
		Char,

		MouseButtonDown,
		MouseButtonUp,
		MouseMoved,
		MouseScrolled,

		GamepadButtonDown,
		GamepadButtonUp,
		GamepadTriggerMove,
		GamepadThumbMove
	};
}
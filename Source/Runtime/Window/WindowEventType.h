#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	/// <summary>
	/// Represents the available types of PlatformWindow events
	/// </summary>
	enum class RUNTIME_API WindowEventType : unsigned char
	{
		None,

		WindowClosed,
		WindowMoved,
		WindowResized,

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
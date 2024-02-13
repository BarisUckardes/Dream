#include "Window.h"

#ifdef DREAM_PLATFORM_WINDOWS
#include <Runtime/Windows/Win32Window.h>
typedef Dream::Win32Window PlatformAbstraction;
#endif

namespace Dream
{
	Window* Window::Create(const WindowDesc& desc)
	{
		return new PlatformAbstraction(desc);
	}
	Window::~Window()
	{

	}
	void Window::SetTitle(const std::string& title)
	{
		SetTitleCore(title);

		mTitle = title;
	}
	void Window::SetOffset(const int x, const int y)
	{
		SetOffsetCore(x, y);

		mOffset[0] = x;
		mOffset[1] = y;
	}
	void Window::SetSize(const unsigned int width, const unsigned int height)
	{
		SetSizeCore(width, height);

		mSize[0] = width;
		mSize[1] = height;
	}
	void Window::SetMode(const WindowMode mode, Monitor* pTargetMonitor)
	{
		SetModeCore(mode, pTargetMonitor);

		mMode = mode;
		mMonitor = pTargetMonitor;
	}
	void Window::PollEvents()
	{
		mBufferedEvents.clear();

		PollEventsCore();
	}
	void Window::Show()
	{
		ShowCore();

		mVisible = true;
	}
	void Window::Hide()
	{
		HideCore();

		mVisible = false;
	}
	Window::Window(const WindowDesc& desc) : mMode(WindowMode::Windowed),mMonitor(nullptr),mTitle(desc.Title),mOffset(),mSize(),mVisible(false),mActive(true)
	{
		mOffset[0] = desc.Offset[0];
		mOffset[1] = desc.Offset[1];
		mSize[0] = desc.Size[0];
		mSize[1] = desc.Size[1];
	}
	void Window::DispatchWindowEvent(const WindowEventData& event)
	{
		switch (event.Type)
		{
		case Dream::WindowEventType::None:
			break;
		case Dream::WindowEventType::WindowClosed:
		{
			mVisible = false;
			mActive = false;
			break;
		}
		case Dream::WindowEventType::WindowMoved:
			break;
		case Dream::WindowEventType::WindowResized:
			break;
		case Dream::WindowEventType::DragDrop:
			break;
		case Dream::WindowEventType::KeyboardDown:
			break;
		case Dream::WindowEventType::KeyboardUp:
			break;
		case Dream::WindowEventType::Char:
			break;
		case Dream::WindowEventType::MouseButtonDown:
			break;
		case Dream::WindowEventType::MouseButtonUp:
			break;
		case Dream::WindowEventType::MouseMoved:
			break;
		case Dream::WindowEventType::MouseScrolled:
			break;
		case Dream::WindowEventType::GamepadButtonDown:
			break;
		case Dream::WindowEventType::GamepadButtonUp:
			break;
		case Dream::WindowEventType::GamepadTriggerMove:
			break;
		case Dream::WindowEventType::GamepadThumbMove:
			break;
		default:
			break;
		}

		mBufferedEvents.push_back(event);
	}
}

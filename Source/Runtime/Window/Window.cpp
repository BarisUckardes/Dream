#include "Window.h"

#ifdef DREAM_PLATFORM_WINDOWS
#include <Runtime/Windows/Win32Window.h>
typedef Dream::Win32Window PlatformAbstraction;
#endif

namespace Dream
{
    Window* Window::create(const WindowDesc& desc)
    {
        return new PlatformAbstraction(desc);
    }
    void Window::set_title(const std::string& title)
    {
        set_title_impl(title);

        mTitle = title;
    }
    void Window::set_mode(const WindowMode mode)
    {
        set_mode_impl(mode);

        set_size(mWidth, mHeight);

        mMode = mode;
    }
    void Window::set_offset(const int x, const int y)
    {
        set_offset_impl(x, y);

        mX = x;
        mY = y;
    }
    void Window::set_size(const unsigned int width, const unsigned int height)
    {
        set_size_impl(width, height);
        
        mWidth = width;
        mHeight = height;
    }
    void Window::poll_events()
    {
        mBufferedEvents.clear();

        poll_events_impl();
    }
    void Window::show()
    {
        show_impl();

        mVisible = true;
    }
    void Window::hide()
    {
        hide_impl();

        mVisible = false;
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
		{
			mX = event.WindowPosition[0];
			mY = event.WindowPosition[1];
			break;
		}
		case Dream::WindowEventType::Windowresized:
		{
			mWidth = event.WindowSize[0];
			mHeight = event.WindowSize[1];
			break;
		}
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

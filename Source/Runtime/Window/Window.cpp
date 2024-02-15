#include "Window.h"
#include <Runtime/Monitor/Monitor.h>

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

		mX = x;
		mY = y;
	}
	void Window::SetSize(const unsigned int width, const unsigned int height)
	{
		SetSizeCore(width, height);

		mWidth = width;
		mHeight = height;
	}
	void Window::SetMode(const WindowMode mode, Monitor* pTargetMonitor)
	{
		//Validations
		if (mode == WindowMode::Fullscreen && pTargetMonitor == nullptr)
		{
			DEV_LOG("Window", "You cannot set for Fullscreen while the pTargetMonitor is nullptr!");
			return;
		}

		//Call implementation
		SetModeCore(mode, pTargetMonitor);

		//Make position and size arrangements
		if (mode == WindowMode::Fullscreen)
		{
			SetOffset(pTargetMonitor->GetPositionX(), pTargetMonitor->GetPositionY());
			SetSize(pTargetMonitor->GetWidth(), pTargetMonitor->GetHeight());
		}
		else
		{
			Monitor* pPrimaryMonitor = Monitor::GetPrimaryMonitor();
			DEV_ASSERT(pPrimaryMonitor != nullptr, "Monitor", "Failed to get the primary monitor!");

			SetOffset(pPrimaryMonitor->GetPositionX(), pPrimaryMonitor->GetPositionY());
			SetSize(pPrimaryMonitor->GetWidth(), pPrimaryMonitor->GetHeight());

			delete pPrimaryMonitor;
		}

		//Set properties
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
	Window::Window(const WindowDesc& desc) : mMode(WindowMode::Windowed),mMonitor(nullptr),mTitle(desc.Title),mX(desc.X),mY(desc.Y),mWidth(desc.Width),mHeight(desc.Height), mVisible(false), mActive(true)
	{

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
		case Dream::WindowEventType::WindowResized:
		{
			mWidth = event.WindowSize[0];
			mHeight = event.WindowSize[1];
			DEV_LOG("Window", "Window new size: %d:%d", mWidth, mHeight);
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

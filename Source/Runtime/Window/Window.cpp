#include "Window.h"
#include <Runtime/Core/Core.h>
#include <Runtime/Input/Win32Keys.h>

namespace Dream
{
	Window* GetUserWindowData(const HWND hwnd)
	{
		return (Window*)GetWindowLongPtr(hwnd, -21); // -21 is a specific number that holds the user provided information
	}
	LRESULT Window::Win32WindowMessageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_CREATE:
			{
				//Set user data ptr
				Window* pWindow = (Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;
				SetWindowLongPtr(hwnd, -21, (LONG_PTR)pWindow);

				//Accept files
				DragAcceptFiles(hwnd, TRUE);
				break;
			}
			case WM_DESTROY:
			{
				DragAcceptFiles(hwnd, FALSE);
				break;
			}
			case WM_CLOSE:
			{
				Window* pWindow = GetUserWindowData(hwnd);
				WindowEventData event = {};
				event.Type = WindowEventType::WindowClosed;

				pWindow->DispatchEvent(event);
				break;
			}
			case WM_MOVE:
			{
				Window* pWindow = GetUserWindowData(hwnd);

				WindowEventData event = {};
				event.Type = WindowEventType::WindowMoved;
				event.WindowPosition[0] = LOWORD(lParam);
				event.WindowPosition[1] = HIWORD(lParam);

				pWindow->DispatchEvent(event);

				break;
			}
			case WM_SIZE:
			{
				Window* pWindow = GetUserWindowData(hwnd);

				WindowEventData event = {};
				event.Type = WindowEventType::WindowResized;
				event.WindowSize[0] = LOWORD(lParam);
				event.WindowSize[1] = HIWORD(lParam);

				pWindow->DispatchEvent(event);
				break;
			}

			case WM_MOUSEMOVE:
			{
				Window* pWindow = GetUserWindowData(hwnd);

				WindowEventData event = {};
				event.Type = WindowEventType::MouseMoved;
				event.MousePosition[0] = LOWORD(lParam);
				event.MousePosition[1] = HIWORD(lParam);

				pWindow->DispatchEvent(event);
				break;
			}
			case WM_MOUSEWHEEL:
			{
				Window* pWindow = GetUserWindowData(hwnd);

				WindowEventData event = {};
				event.Type = WindowEventType::MouseScrolled;
				event.MouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);

				pWindow->DispatchEvent(event);
				break;
			}
			case WM_LBUTTONDOWN:
			{
				Window* pWindow = GetUserWindowData(hwnd);

				WindowEventData event = {};
				event.Type = WindowEventType::MouseButtonDown;
				event.MouseButton = MouseButtons::Left;

				pWindow->DispatchEvent(event);
				break;
			}
			case WM_LBUTTONUP:
			{
				Window* pWindow = GetUserWindowData(hwnd);

				WindowEventData event = {};
				event.Type = WindowEventType::MouseButtonUp;
				event.MouseButton = MouseButtons::Left;

				pWindow->DispatchEvent(event);
				break;
			}
			case WM_RBUTTONDOWN:
			{
				Window* pWindow = GetUserWindowData(hwnd);

				WindowEventData event = {};
				event.Type = WindowEventType::MouseButtonDown;
				event.MouseButton = MouseButtons::Right;

				pWindow->DispatchEvent(event);
				break;
			}
			case WM_RBUTTONUP:
			{
				Window* pWindow = GetUserWindowData(hwnd);

				WindowEventData event = {};
				event.Type = WindowEventType::MouseButtonUp;
				event.MouseButton = MouseButtons::Right;

				pWindow->DispatchEvent(event);
				break;
			}
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				Window* pWindow = GetUserWindowData(hwnd);
				WindowEventData event = {};
				event.Type = WindowEventType::KeyboardDown;
				event.KeyboardKey = Win32Keys::GetKey(wParam);

				pWindow->DispatchEvent(event);
				break;
			}
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				Window* pWindow = GetUserWindowData(hwnd);

				WindowEventData event = {};
				event.Type = WindowEventType::KeyboardUp;
				event.KeyboardKey = Win32Keys::GetKey(wParam);

				pWindow->DispatchEvent(event);
				break;
			}
			case WM_CHAR:
			{
				Window* pWindow = GetUserWindowData(hwnd);

				WindowEventData event = {};
				event.Type = WindowEventType::Char;
				event.KeyboardChar = wParam;

				pWindow->DispatchEvent(event);
				break;
			}
			case WM_DEVICECHANGE:
			{
				/*
				* Check if any input device has been removed!
				*/
				break;
			}
			case WM_DROPFILES:
			{
				//Get drop handle
				HDROP dropHandle = (HDROP)wParam;

				//Get item count
				const unsigned int itemCount = DragQueryFile(dropHandle, 0xFFFFFFFF, NULL, 0);

				//Create window event header
				WindowEventData event = {};
				event.Type = WindowEventType::DragDrop;

				//Collect items
				for (unsigned int i = 0; i < itemCount; i++)
				{
					char buffer[MAX_PATH];
					DragQueryFile(dropHandle, i, buffer, MAX_PATH);
					event.DropItems.push_back(buffer);
				}

				Window* pWindow = GetUserWindowData(hwnd);
				pWindow->DispatchEvent(event);
				break;
			}
			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
		}

	}
	Window::Window(const WindowCreateDesc& desc)
	{
		constexpr const char WINDOW_CLASS_NAME[] = "DreamWin32WindowName";

		//Get module handle
		const HINSTANCE processHandle = GetModuleHandle(NULL);

		//Register window class
		WNDCLASSEX windowClass = {};
		windowClass.cbSize = sizeof(windowClass);
		windowClass.hInstance = processHandle;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = sizeof(Window*);
		windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = WINDOW_CLASS_NAME;
		windowClass.lpfnWndProc = Win32WindowMessageProc;

		DEV_ASSERT(RegisterClassEx(&windowClass) != NULL, "Win32Window", "Failed to register the class");

		//Create window
		const HWND windowHandle = CreateWindowEx(
			0,
			WINDOW_CLASS_NAME,
			desc.Title.c_str(),
			WS_OVERLAPPEDWINDOW | WS_EX_ACCEPTFILES,
			desc.X,desc.Y,desc.Width,desc.Height,
			NULL,NULL,processHandle,
			this
		);
		DEV_ASSERT(windowHandle != NULL, "Win32Window", "Failed to create window");

		//Add message filters
		ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
		ChangeWindowMessageFilter(WM_COPYDATA, MSGFLT_ADD);
		ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);


		//Set properties
		mTitle = desc.Title;
		mWidth = desc.Width;
		mHeight = desc.Height;
		mX = desc.X;
		mY = desc.Y;
		mMode = desc.Mode;
		mVisible = false;
		mAlive = true;
		mWindowHandle = windowHandle;
		mContextHandle = GetDC(windowHandle);
		mBufferedEvents.reserve(1000);

	}
	Window::~Window()
	{
		ReleaseDC(mWindowHandle, mContextHandle);
		DestroyWindow(mWindowHandle);
	}
	void Window::SetTitle(const std::string& title)
	{
		SetWindowText(mWindowHandle, title.c_str());
		mTitle = title;
	}
	void Window::SetSize(const unsigned int width, const unsigned int height)
	{
		SetWindowPos(mWindowHandle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_SHOWWINDOW);
		mWidth = width;
		mHeight = height;
	}
	void Window::SetPosition(const int x, const int y)
	{
		SetWindowPos(mWindowHandle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		mX = x;
		mY = y;
	}
	void Window::SetMode(const WindowMode mode)
	{
		//TODO
		mMode = mode;
	}
	void Window::Show()
	{
		ShowWindow(mWindowHandle, SW_SHOW);
		mVisible = true;
	}
	void Window::Hide()
	{
		ShowWindow(mWindowHandle, SW_HIDE);
		mVisible = false;
	}
	void Window::PollMessages()
	{
		MSG msg = {};
		while (PeekMessage(&msg, mWindowHandle, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		mBufferedEvents.clear();
	}
	void Window::DispatchEvent(const WindowEventData& event)
	{
		mBufferedEvents.push_back(event);

		switch (event.Type)
		{
			case Dream::WindowEventType::None:
				break;
			case Dream::WindowEventType::WindowClosed:
			{
				OnClose();
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
	}
	void Window::OnClose()
	{
		mAlive = false;
		mVisible = false;
	}
}

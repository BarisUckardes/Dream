#pragma once
#include <Runtime/Window/WindowCreateDesc.h>
#include <Runtime/Window/WindowEventData.h>
#include <Windows.h>
#include <vector>

namespace Dream
{
	class RUNTIME_API Window final
	{
	private:
		static LRESULT CALLBACK Win32WindowMessageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	public:
		Window(const WindowCreateDesc& desc);
		~Window();

		FORCEINLINE const std::vector<WindowEventData>& GetBufferedEvents() const noexcept
		{
			return mBufferedEvents;
		}
		FORCEINLINE std::string GetTitle() const noexcept
		{
			return mTitle;
		}
		FORCEINLINE unsigned int GetWidth() const noexcept
		{
			return mWidth;
		}
		FORCEINLINE unsigned int GetHeight() const noexcept
		{
			return mHeight;
		}
		FORCEINLINE int GetPositionX() const noexcept
		{
			return mX;
		}
		FORCEINLINE int GetPositionY() const noexcept
		{
			return mY;
		}
		FORCEINLINE WindowMode GetMode() const noexcept
		{
			return mMode;
		}
		FORCEINLINE HWND GetWindowHandle() const noexcept
		{
			return mWindowHandle;
		}
		FORCEINLINE HDC GetContextHandle() const noexcept
		{
			return mContextHandle;
		}
		FORCEINLINE bool IsVisible() const noexcept
		{
			return mVisible;
		}
		FORCEINLINE bool IsAlive() const noexcept
		{
			return mAlive;
		}

		void SetTitle(const std::string& title);
		void SetSize(const unsigned int width, const unsigned int height);
		void SetPosition(const int x, const int y);
		void SetMode(const WindowMode mode);
		void Show();
		void Hide();
		void PollMessages();
	
	private:
		void DispatchEvent(const WindowEventData& event);
		void OnClose();

	private:
		std::vector<WindowEventData> mBufferedEvents;
		std::string mTitle;
		unsigned int mWidth;
		unsigned int mHeight;
		int mX;
		int mY;
		WindowMode mMode;
		HWND mWindowHandle;
		HDC mContextHandle;
		bool mVisible;
		bool mAlive;
	};
}

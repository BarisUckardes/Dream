#pragma once
#include <Runtime/Window/WindowDesc.h>
#include <Runtime/Window/WindowEventData.h>
#include <Windows.h>
#include <vector>

namespace Dream
{
	class Monitor;
	class RUNTIME_API Window final
	{
	private:
		static LRESULT CALLBACK Win32WindowMessageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	public:
		Window(const WindowDesc& desc);
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
			return mSize[0];
		}
		FORCEINLINE unsigned int GetHeight() const noexcept
		{
			return mSize[1];
		}
		FORCEINLINE int GetPositionX() const noexcept
		{
			return mPosition[0];
		}
		FORCEINLINE int GetPositionY() const noexcept
		{
			return mPosition[1];
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
		void Show();
		void Hide();
		void PollMessages();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="mode"></param>
		/// <param name="pTargetFullscreenMonitor">Must be not nullptr when requesting a transition to fullscreen</param>
		void SetMode(const WindowMode mode,Monitor* pTargetFullscreenMonitor);
	private:
		void DispatchEvent(const WindowEventData& event);
		void OnClose();

	private:
		std::vector<WindowEventData> mBufferedEvents;
		Monitor* mMonitor;
		std::string mTitle;
		unsigned int mSize[2];
		int mPosition[2];
		WindowMode mMode;
		HWND mWindowHandle;
		HDC mContextHandle;
		bool mVisible;
		bool mAlive;
	};
}

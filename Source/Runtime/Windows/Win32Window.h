#pragma once
#include <Runtime/Window/Window.h>
#include <Windows.h>

namespace Dream
{
	class RUNTIME_API Win32Window : public Window
	{
	private:
		static LRESULT CALLBACK WindowsProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	public:
		Win32Window(const WindowDesc& desc);
		~Win32Window();

		FORCEINLINE HWND GetWin32WindowHandle() const noexcept
		{
			return mHandle;
		}
		FORCEINLINE HDC GetWin32Context() const noexcept
		{
			return mContext;
		}
	private:
		// Inherited via Window
		void SetTitleCore(const std::string& title) override;
		void SetOffsetCore(const int x, const int y) override;
		void SetSizeCore(const unsigned int width, const unsigned int height) override;
		void SetModeCore(const WindowMode mode) override;
		void PollEventsCore() override;
		void ShowCore() override;
		void HideCore() override;
	private:
		HWND mHandle;
		HDC mContext;

	};
}

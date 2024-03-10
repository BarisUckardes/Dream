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

		FORCEINLINE HWND win32_handle() const noexcept
		{
			return mHandle;
		}
		FORCEINLINE HDC win32_context() const noexcept
		{
			return mContext;
		}
	private:
		// Inherited via Window
		void set_title_impl(const std::string& title) override;
		void set_offset_impl(const int x, const int y) override;
		void set_size_impl(const unsigned int width, const unsigned int height) override;
		void set_mode_impl(const WindowMode mode) override;
		void poll_events_impl() override;
		void show_impl() override;
		void hide_impl() override;
	private:
		HWND mHandle;
		HDC mContext;

	};
}

#pragma once
#include <Runtime/Window/WindowDesc.h>
#include <Runtime/Window/WindowEventData.h>
#include <vector>

namespace Dream
{
	class Monitor;
	class RUNTIME_API Window
	{
	public:
		static Window* Create(const WindowDesc& desc);
	public:
		~Window();

		FORCEINLINE const std::vector<WindowEventData>& GetBufferedEvents() const noexcept
		{
			return mBufferedEvents;
		}
		FORCEINLINE std::string GetTitle() const noexcept
		{
			return mTitle;
		}
		FORCEINLINE int GetPositionX() const noexcept
		{
			return mX;
		}
		FORCEINLINE int GetPositionY() const noexcept
		{
			return mY;
		}
		FORCEINLINE unsigned int GetWidth() const noexcept
		{
			return mWidth;
		}
		FORCEINLINE unsigned int GetHeight() const noexcept
		{
			return mHeight;
		}
		FORCEINLINE WindowMode GetMode() const noexcept
		{
			return mMode;
		}
		FORCEINLINE bool IsVisible() const noexcept
		{
			return mVisible;
		}
		FORCEINLINE bool IsActive() const noexcept
		{
			return mActive;
		}

		void SetTitle(const std::string& title);
		void SetOffset(const int x, const int y);
		void SetSize(const unsigned int width, const unsigned int height);
		void SetMode(const WindowMode mode, Monitor* pTargetMonitor);
		void PollEvents();
		void Show();
		void Hide();
	protected:
		Window(const WindowDesc& desc);

		virtual void SetTitleCore(const std::string& title) = 0;
		virtual void SetOffsetCore(const int x, const int y) = 0;
		virtual void SetSizeCore(const unsigned int width, const unsigned int height) = 0;
		virtual void SetModeCore(const WindowMode mode, Monitor* pTargetMonitor) = 0;
		virtual void PollEventsCore() = 0;
		virtual void ShowCore() = 0;
		virtual void HideCore() = 0;

		void DispatchWindowEvent(const WindowEventData& event);
	private:
		std::vector<WindowEventData> mBufferedEvents;
		std::string mTitle;
		int mX;
		int mY;
		unsigned int mWidth;
		unsigned int mHeight;
		WindowMode mMode;
		Monitor* mMonitor;
		bool mVisible;
		bool mActive;
	};
}

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
			return mOffset[0];
		}
		FORCEINLINE int GetPositionY() const noexcept
		{
			return mOffset[1];
		}
		FORCEINLINE unsigned int GetWidth() const noexcept
		{
			return mSize[0];
		}
		FORCEINLINE unsigned int GetHeight() const noexcept
		{
			return mSize[1];
		}
		FORCEINLINE WindowMode GetMode() const noexcept
		{
			return mMode;
		}

		void SetTitle(const std::string& title);
		void SetOffset(const int x, const int y);
		void SetSize(const unsigned int width, const unsigned int height);
		void SetMode(const WindowMode mode, Monitor* pTargetMonitor);
		void PollEvents();
	protected:
		Window(const WindowDesc& desc);

		virtual void SetTitleCore(const std::string& title) = 0;
		virtual void SetOffsetCore(const int x, const int y) = 0;
		virtual void SetSizeCore(const unsigned int width, const unsigned int height) = 0;
		virtual void SetModeCore(const WindowMode mode, Monitor* pTargetMonitor) = 0;
		virtual void PollEventsCore() = 0;
	private:
		std::vector<WindowEventData> mBufferedEvents;
		std::string mTitle;
		int mOffset[2];
		int mSize[2];
		WindowMode mMode;
		Monitor* mMonitor;
	};
}

#pragma once
#include <Runtime/Window/WindowDesc.h>
#include <Runtime/Window/WindowEventData.h>
#include <vector>

namespace Dream
{
	class RUNTIME_API Window
	{
	public:
		static Window* create(const WindowDesc& desc);
	public:
		~Window()
		{

		}

		FORCEINLINE const std::vector<WindowEventData>& buffered_events() const noexcept
		{
			return mBufferedEvents;
		}
		FORCEINLINE std::string title() const noexcept
		{
			return mTitle;
		}
		FORCEINLINE WindowMode mode() const noexcept
		{
			return mMode;
		}
		FORCEINLINE int x() const noexcept
		{
			return mX;
		}
		FORCEINLINE int y() const noexcept
		{
			return mY;
		}
		FORCEINLINE unsigned int width() const noexcept
		{
			return mWidth;
		}
		FORCEINLINE unsigned int height() const noexcept
		{
			return mHeight;
		}
		FORCEINLINE bool active() const noexcept
		{
			return mActive;
		}
		FORCEINLINE bool visible() const noexcept
		{
			return mVisible;
		}

		void set_title(const std::string& title);
		void set_mode(const WindowMode mode);
		void set_offset(const int x,const int y);
		void set_size(const unsigned int width,const unsigned int height);
		void poll_events();
		void show();
		void hide();
	protected:
		Window(const WindowDesc& desc) : mMode(WindowMode::Fixed), mTitle(desc.Title), mX(desc.X), mY(desc.Y), mWidth(desc.Width), mHeight(desc.Height), mVisible(false), mActive(true)
		{

		}


		void DispatchWindowEvent(const WindowEventData& event);

		virtual void set_title_impl(const std::string& title) = 0;
		virtual void set_mode_impl(const WindowMode mode) = 0;
		virtual void set_offset_impl(const int x,const int y) = 0;
		virtual void set_size_impl(const unsigned int width,const unsigned int height) = 0;
		virtual void poll_events_impl() = 0;
		virtual void show_impl() = 0;
		virtual void hide_impl() = 0;
	private:
		std::vector<WindowEventData> mBufferedEvents;
		std::string mTitle;
		WindowMode mMode;
		int mX;
		int mY;
		unsigned int mWidth;
		unsigned int mHeight;
		bool mActive;
		bool mVisible;
	};
}

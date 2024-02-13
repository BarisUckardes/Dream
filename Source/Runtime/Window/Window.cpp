#include "Window.h"

namespace Dream
{
	Window* Window::Create(const WindowDesc& desc)
	{
		return nullptr;
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

		mOffset[0] = x;
		mOffset[1] = y;
	}
	void Window::SetSize(const unsigned int width, const unsigned int height)
	{
		SetSizeCore(width, height);

		mSize[0] = width;
		mSize[1] = height;
	}
	void Window::SetMode(const WindowMode mode, Monitor* pTargetMonitor)
	{

	}
	void Window::PollEvents()
	{
		mBufferedEvents.clear();

		PollEventsCore();
	}
	Window::Window(const WindowDesc& desc) : mMode(WindowMode::Windowed),mMonitor(nullptr),mTitle(desc.Title),mOffset(),mSize()
	{
		mOffset[0] = desc.Offset[0];
		mOffset[1] = desc.Offset[1];
		mSize[0] = desc.Size[0];
		mSize[1] = desc.Size[1];
	}
}

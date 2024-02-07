#include <Runtime/Window/Window.h>

int main(const unsigned int count, const char** ppArguments)
{
	Dream::WindowCreateDesc windowDesc = {};
	windowDesc.Title = "Dream Vulkan Renderer Layer";
	windowDesc.Width = 1024;
	windowDesc.Height = 1024;
	windowDesc.X = 100;
	windowDesc.Y = 100;
	windowDesc.Mode = Dream::WindowMode::Windowed;

	Dream::Window* pWindow = new Dream::Window(windowDesc);

	pWindow->Show();

	while (pWindow->IsAlive())
	{
		pWindow->PollMessages();
	}
}

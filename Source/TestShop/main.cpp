#include <Runtime/Window/Window.h>
#include <Runtime/Monitor/Monitor.h>

int main(const unsigned int count, const char** ppArguments)
{
	Dream::WindowCreateDesc windowDesc = {};
	windowDesc.Title = "Dream Vulkan Renderer Layer";
	windowDesc.Size[0] = 1024;
	windowDesc.Size[1] = 1024;
	windowDesc.Position[0] = 100;
	windowDesc.Position[1] = 100;

	Dream::Window* pWindow = new Dream::Window(windowDesc);

	pWindow->Show();

	while (pWindow->IsAlive())
	{
		pWindow->PollMessages();
	}
}

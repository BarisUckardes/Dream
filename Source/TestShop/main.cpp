#include <Runtime/Window/Window.h>
#include <Runtime/Monitor/Monitor.h>

namespace Dream
{
	void Run()
	{
		//Create window
		WindowDesc windowDesc = {};
		windowDesc.Title = "DreamTestShop";
		windowDesc.X = 100;
		windowDesc.Y = 100;
		windowDesc.Width = 1024;
		windowDesc.Height = 1024;

		Window* pWindow = Window::Create(windowDesc);
		pWindow->Show();

		while (pWindow->IsActive())
		{
			pWindow->PollEvents();
		}
	}
}
int main(const unsigned int count, const char** ppArguments)
{
	Dream::Run();
	return 0;
}

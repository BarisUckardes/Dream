#include <Runtime/Window/Window.h>

namespace Dream
{
	void Run()
	{
		//Create window
		WindowDesc windowDesc = {};
		windowDesc.Title = "DreamTestShop";
		windowDesc.Offset[0] = 100;
		windowDesc.Offset[1] = 100;
		windowDesc.Size[0] = 512;
		windowDesc.Size[1] = 512;

		Window* pWindow = Window::Create(windowDesc);
		pWindow->Show();

		pWindow->SetMode(WindowMode::Fullscreen, nullptr);
		pWindow->SetOffset(0, 0);
		pWindow->SetSize(3840, 2160);

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

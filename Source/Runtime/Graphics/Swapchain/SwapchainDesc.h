#pragma once
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Window/Window.h>
#include <Runtime/Graphics/Swapchain/PresentMode.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>

namespace Dream
{
	struct RUNTIME_API SwapchainDesc
	{
		PresentMode Mode;
		unsigned char BufferCount;
		TextureFormat ColorFormat;
		TextureFormat DepthStencilFormat;
		Window* pWindow;
		GraphicsQueue* pQueue;
	};
}

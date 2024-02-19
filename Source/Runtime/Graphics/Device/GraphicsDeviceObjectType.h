#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	enum class RUNTIME_API GraphicsDeviceObjectType : unsigned char
	{
		RenderPass,
		Buffer,
		Texture,
		TextureView,
		Sampler,
		Shader,
		CommandList,
		CommandPool,
		Pipeline,
		Swapchain,
		Fence,
		MemoryHeap,
		DescriptorPool,
		DescriptorLayout,
		DescriptorSet,
		Queue,
		Semaphore
	};
}

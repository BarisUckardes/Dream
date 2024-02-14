#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferUsage.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>

namespace Dream
{
	struct RUNTIME_API GraphicsBufferDesc
	{
		GraphicsBufferUsage Usage;
		unsigned int SubItemCount;
		unsigned int SubItemSizeInBytes;
		GraphicsMemory* pMemory;
	};
}

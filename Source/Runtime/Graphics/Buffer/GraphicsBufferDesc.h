#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferUsage.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryHeap.h>

namespace Dream
{
	/**
	 * @struct GraphicsBufferDesc
	 * 
	 * @brief All the necessary information to create a graphics related buffer.
	 */
	struct RUNTIME_API GraphicsBufferDesc
	{
		GraphicsBufferUsage Usage;
		unsigned int SubItemCount;
		unsigned int SubItemSizeInBytes;
		GraphicsMemoryHeap* pHeap;
	};
}

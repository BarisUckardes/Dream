#pragma once
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryAccessFlags.h>
#include <Runtime/Graphics/Queue/GraphicsQueueFamilyType.h>

namespace Dream
{
	struct RUNTIME_API BufferMemoryBarrierDesc
	{
		unsigned long long OffsetInBytes;
		unsigned long long SizeInBytes;

		PipelineStageFlags SourceStageFlags;
		GraphicsMemoryAccessFlags SourceAccessFlags;
		GraphicsQueueFamilyType SourceQueue;

		PipelineStageFlags DestinationStageFlags;
		GraphicsMemoryAccessFlags DestinationAccessFlags;
		GraphicsQueueFamilyType DestinationQueue;

	};
}

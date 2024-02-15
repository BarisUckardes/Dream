#pragma once
#include <Runtime/Graphics/Texture/TextureMemoryLayout.h>
#include <Runtime/Graphics/Texture/TextureAspectFlags.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryAccessFlags.h>
#include <Runtime/Graphics/Queue/GraphicsQueueType.h>
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>

namespace Dream
{
	struct RUNTIME_API TextureMemoryBarrierDesc
	{
		TextureAspectFlags AspectFlags;
		unsigned char ArrayIndex;
		unsigned char MipIndex;

		TextureMemoryLayout SourceLayout;
		GraphicsMemoryAccessFlags SourceAccessFlags;
		PipelineStageFlags SourceStageFlags;
		GraphicsQueueType SourceQueue;

		TextureMemoryLayout DestinationLayout;
		GraphicsMemoryAccessFlags DestinationAccessFlags;
		PipelineStageFlags DestinationStageFlags;
		GraphicsQueueType DestinationQueue;
	};
}

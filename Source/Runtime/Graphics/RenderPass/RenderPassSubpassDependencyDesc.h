#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryAccessFlags.h>
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>

namespace Dream
{
	struct RUNTIME_API RenderPassSubpassDependencyDesc
	{
		unsigned char InputSubpass;
		GraphicsMemoryAccessFlags InputAccess;
		PipelineStageFlags InputStageFlags;

		unsigned char OutputSubpass;
		GraphicsMemoryAccessFlags OutputAccess;
		PipelineStageFlags OutputStageFlags;
	};
}

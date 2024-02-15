#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPassLoadOperation.h>
#include <Runtime/Graphics/RenderPass/RenderPassStoreOperation.h>
#include <Runtime/Graphics/Texture/TextureMemoryLayout.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureSampleCount.h>

namespace Dream
{
	class Texture;
	struct RUNTIME_API RenderPassAttachmentDesc
	{
		TextureFormat Format;
		TextureSampleCount SampleCount;
		RenderPassLoadOperation ColorLoadOperation;
		RenderPassStoreOperation ColorStoreOperation;
		RenderPassLoadOperation StencilLoadOperation;
		RenderPassStoreOperation StencilStoreOperation;
		TextureMemoryLayout InputLayout;
		TextureMemoryLayout OutputLayout;
		unsigned char ArrayLevel;
		unsigned char MipLevel;
	};
}

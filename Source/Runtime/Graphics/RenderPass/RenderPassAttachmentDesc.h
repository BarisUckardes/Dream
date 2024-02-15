#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPassLoadOperation.h>
#include <Runtime/Graphics/RenderPass/RenderPassStoreOperation.h>
#include <Runtime/Graphics/Texture/TextureMemoryLayout.h>

namespace Dream
{
	class Texture;
	struct RUNTIME_API RenderPassAttachmentDesc
	{
		Texture* pTexture;
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

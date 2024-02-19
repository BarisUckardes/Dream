#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	enum class RUNTIME_API TextureMemoryLayout : unsigned char
	{
		Unknown,
		General,
		ColorAttachment,
		DepthStencilAttachment,
		ShaderReadOnly,
		TransferSource,
		TransferDestination,
		DepthAttachment,
		Present
	};
}
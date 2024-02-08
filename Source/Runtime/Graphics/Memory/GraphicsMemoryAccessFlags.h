#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	enum class RUNTIME_API GraphicsMemoryAccessFlags : unsigned short
	{
		Unknown = 0,
		IndexRead = 1,
		VertexAttributeRead = 2,
		UniformRead = 4,
		InputAttachmentRead = 8,
		ShaderRead = 16,
		ShaderWrite = 32,
		ColorAttachmentRead = 64,
		ColorAttachmentWrite = 128,
		DepthStencilAttachmentRead = 256,
		DepthStencilAttachmentWrite = 512,
		TransferRead = 1024,
		TransferWrite = 2048,
		HostRead = 4096,
		HostWrite = 8192,
		MemoryRead = 16384,
		MemoryWrite = 32768
	};
	GENERATE_FLAGS(GraphicsMemoryAccessFlags,unsigned short)
}

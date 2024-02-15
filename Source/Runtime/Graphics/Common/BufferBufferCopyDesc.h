#pragma once

namespace Dream
{
	struct RUNTIME_API BufferBufferCopyDesc
	{
		unsigned long long SourceOffsetInBytes;
		unsigned long long DestinationOffsetInBytes;
		unsigned long long SizeInBytes;
	};
}

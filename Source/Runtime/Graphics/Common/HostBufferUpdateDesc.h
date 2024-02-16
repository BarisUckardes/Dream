#pragma once

namespace Dream
{
	struct RUNTIME_API HostBufferUpdateDesc
	{
		unsigned char* pBuffer;
		unsigned long long SizeInBytes;
		unsigned long long OffsetInBytes;
	};
}

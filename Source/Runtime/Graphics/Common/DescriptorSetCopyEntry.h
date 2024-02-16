#pragma once

namespace Dream
{
	struct DescriptorSetCopyEntry
	{
		unsigned int SourceBinding;
		unsigned int SourceArrayElement;

		unsigned int DestinationBinding;
		unsigned int DestinationArrayElement;

		unsigned int DescriptorCount;
	};
}

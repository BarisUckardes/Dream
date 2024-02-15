#pragma once
#include <Runtime/Graphics/Descriptor/DescriptorResourceType.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>

namespace Dream
{
	struct RUNTIME_API DescriptorSetLayoutEntry
	{
		DescriptorResourceType Type;
		ShaderStage Stages;
		unsigned int Binding;
	};
}

#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/PipelineBindPoint.h>
#include <vector>

namespace Dream
{
	struct RUNTIME_API RenderPassSubpassDesc
	{
		PipelineBindPoint BindPoint;
		std::vector<unsigned char> Attachments;
		std::vector<unsigned char> Inputs;
		std::vector<unsigned char> MultisampleInputs;
		std::vector<unsigned char> PreserveAttachments;
		std::vector<unsigned char> DepthStencilInput;
	};
}

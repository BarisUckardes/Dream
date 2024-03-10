#pragma once
#include <Runtime/Graphics/Instance/GraphicsInstance.h>

namespace Dream
{
	class RUNTIME_API D3D12Instance : public GraphicsInstance
	{
	public:
		D3D12Instance(const GraphicsInstanceDesc& desc);
		~D3D12Instance();
	private:
	};
}

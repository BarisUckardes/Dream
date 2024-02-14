#include "GraphicsInstance.h"
#include <Runtime/Vulkan/Instance/VulkanInstance.h>

namespace Dream
{
	GraphicsInstance* GraphicsInstance::Create(const GraphicsInstanceDesc* pDesc)
	{
		GraphicsInstance* pInstance = nullptr;
		switch (pDesc->Backend)
		{
			case Dream::GraphicsBackend::Directx12:
				break;
			case Dream::GraphicsBackend::Vulkan:
			{
				pInstance = new VulkanInstance((VulkanInstanceDesc*)pDesc);
				break;
			}
			default:
				break;
		}

		return pInstance;
	}
	GraphicsInstance::~GraphicsInstance()
	{

	}
	GraphicsInstance::GraphicsInstance(const GraphicsInstanceDesc* pDesc) : mBackend(pDesc->Backend)
	{

	}
	void GraphicsInstance::SetAdapters(const std::vector<GraphicsAdapter*>& adapters)
	{
		mAdapters = adapters;
	}
}

#pragma once
#include <Runtime/Graphics/Instance/GraphicsInstanceDesc.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>
#include <Windows.h>
#include <vulkan.h>
#include <vulkan_win32.h>

namespace Dream
{
	class RUNTIME_API GraphicsInstance final
	{
	public:
		GraphicsInstance(const GraphicsInstanceDesc& desc);
		~GraphicsInstance();

		FORCEINLINE VkInstance GetInstance() const noexcept
		{
			return mInstance;
		}
		FORCEINLINE GraphicsAdapter* GetPrimaryAdapter() const noexcept
		{
			if (mAdapters.size() == 0)
				return nullptr;

			return mAdapters[0];
		}
		FORCEINLINE const std::vector<GraphicsAdapter*>& GetAdapters() const noexcept
		{
			return mAdapters;
		}
	private:
		VkInstance mInstance;
#ifdef DREAM_DEBUG
		VkDebugUtilsMessengerEXT mDebugMessenger;
#endif
		std::vector<GraphicsAdapter*> mAdapters;
	};
}


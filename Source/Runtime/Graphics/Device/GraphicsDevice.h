#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryHeap.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <vulkan.h>
#include <vector>

namespace Dream
{
	class GraphicsAdapter;
	class RUNTIME_API GraphicsDevice final
	{
		friend class GraphicsAdapter;
		friend class GraphicsDeviceObject;
	private:
		struct DeviceQueueFamily
		{
			DeviceQueueFamily() : FamilyIndex(255), QueueCapacity(0), DefaultQueue(VK_NULL_HANDLE), bCanPresent(false)
			{

			}

			unsigned char FamilyIndex;
			unsigned char QueueCapacity;
			VkQueue DefaultQueue;
			VkBool32 bCanPresent;
		};
	public:
		FORCEINLINE VkDevice GetLogicalDevice() const noexcept
		{
			return mLogicalDevice;
		}
		FORCEINLINE VkQueue GetGraphicsQueue() const noexcept
		{
			return mGraphicsQueueFamily.DefaultQueue;
		}
		FORCEINLINE VkQueue GetComputeQueue() const noexcept
		{
			return mComputeQueueFamily.DefaultQueue;
		}
		FORCEINLINE VkQueue GetTransferQueue() const noexcept
		{
			return mTransferQueueFamily.DefaultQueue;
		}

		int GetPresentQueueFamily(const VkSurfaceKHR surface) const noexcept;
		VkQueue GetPresentQueue(const VkSurfaceKHR surface) const noexcept;

		GraphicsMemoryHeap* AllocateMemory(const GraphicsMemoryHeapDesc& desc);
		GraphicsBuffer* CreateBuffer(const GraphicsBufferDesc& desc);
		~GraphicsDevice();
	private:
		GraphicsDevice(const GraphicsDeviceDesc& desc,GraphicsAdapter* pAdapter);

		void RegisterChildObject(GraphicsDeviceObject* pObject);
		void RemoveChildObject(GraphicsDeviceObject* pObject);
		void _OnChildObjectDeleted(GraphicsDeviceObject* pObject);
	private:
		std::vector<GraphicsDeviceObject*> mChildObjects;
		GraphicsAdapter* mOwnerAdapter;
		DeviceQueueFamily mGraphicsQueueFamily;
		DeviceQueueFamily mComputeQueueFamily;
		DeviceQueueFamily mTransferQueueFamily;
		VkDevice mLogicalDevice;
	};
}

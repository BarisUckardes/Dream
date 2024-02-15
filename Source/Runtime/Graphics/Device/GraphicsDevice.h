#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/GraphicsBackend.h>
#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetLayout.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Graphics/Sampler/Sampler.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureView.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <vector>

namespace Dream
{
	class RUNTIME_API GraphicsDevice
	{
	public:
		~GraphicsDevice();

		FORCEINLINE GraphicsAdapter* GetAdapter() const noexcept
		{
			return mOwnerAdapter;
		}

		virtual GraphicsBackend GetBackend() const noexcept = 0;
		virtual bool HasQueue(const GraphicsQueueType type) const noexcept = 0;

		GraphicsQueue* CreateQueue(const GraphicsQueueDesc& desc);
		GraphicsBuffer* CreateBuffer(const GraphicsBufferDesc& desc);
		DescriptorSet* CreateDescriptorSet(const DescriptorSetDesc& desc);
		DescriptorPool* CreateDescriptorPool(const DescriptorPoolDesc& desc);
		DescriptorSetLayout* CreateDescriptorSetLayout(const DescriptorSetLayoutDesc& desc);
		Fence* CreateFence(const FenceDesc& desc);
		GraphicsMemory* AllocateMemory(const GraphicsMemoryDesc& desc);
		Sampler* CreateSampler(const SamplerDesc& desc);
		Shader* CreateShader(const ShaderDesc& desc);
		Texture* CreateTexture(const TextureDesc& desc);
		TextureView* CreateTextureView(const TextureViewDesc& desc);
		Swapchain* CreateSwapchain(const SwapchainDesc& desc);

		void ResetFences(Fence** ppFences, const unsigned int count);
		void WaitFences(Fence** ppFences, const unsigned int count);
		void WaitDeviceIdle();
		void WaitQueueIdle(GraphicsQueue* pQueue);
	protected:
		GraphicsDevice(const GraphicsDeviceDesc* pDesc);

		void RegisterObject(GraphicsDeviceObject* pObject);
		void RemoveObject(GraphicsDeviceObject* pObject);

		virtual GraphicsQueue* CreateQueueCore(const GraphicsQueueDesc& desc) = 0;
		virtual GraphicsBuffer* CreateBufferCore(const GraphicsBufferDesc& desc) = 0;
		virtual DescriptorSet* CreateDescriptorSetCore(const DescriptorSetDesc& desc) = 0;
		virtual DescriptorPool* CreateDescriptorPoolCore(const DescriptorPoolDesc& desc) = 0;
		virtual DescriptorSetLayout* CreateDescriptorSetLayoutCore(const DescriptorSetLayoutDesc& desc) = 0;
		virtual Fence* CreateFenceCore(const FenceDesc& desc) = 0;
		virtual GraphicsMemory* AllocateMemoryCore(const GraphicsMemoryDesc& desc) = 0;
		virtual Sampler* CreateSamplerCore(const SamplerDesc& desc) = 0;
		virtual Shader* CreateShaderCore(const ShaderDesc& desc) = 0;
		virtual Texture* CreateTextureCore(const TextureDesc& desc) = 0;
		virtual TextureView* CreateTextureViewCore(const TextureViewDesc& desc) = 0;
		virtual Swapchain* CreateSwapchainCore(const SwapchainDesc& desc) = 0;

		virtual void ResetFencesCore(Fence** ppFences, const unsigned int count) =  0;
		virtual void WaitFencesCore(Fence** ppFences, const unsigned int count) =  0;
		virtual void WaitDeviceIdleCore() =  0;
		virtual void WaitQueueIdleCore(GraphicsQueue* pQueue) =  0;
	private:
		std::vector<GraphicsDeviceObject*> mObjects;
		GraphicsAdapter* mOwnerAdapter;
	};
}

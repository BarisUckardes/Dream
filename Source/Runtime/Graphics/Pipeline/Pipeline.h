#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Pipeline/GraphicsPipelineDesc.h>
#include <Runtime/Graphics/Pipeline/ComputePipelineDesc.h>

namespace Dream
{
    class RUNTIME_API Pipeline : public GraphicsDeviceObject
    {
    public:
        Pipeline(const GraphicsPipelineDesc& desc,GraphicsDevice* pDevice) :GraphicsDeviceObject(pDevice), mBindPoint(PipelineBindPoint::Graphics),
            mBlendState(desc.BlendState), mDepthStencilState(desc.DepthStencilState),
            mInputLayout(desc.InputLayout), mMultisample(desc.Multisample),
            mRasterizerState(desc.RasterizerState),
            mResourceLayout(desc.ResourceLayout), mGraphicsShaders(desc.GraphicsShaders),mSubpassIndex(desc.SubpassIndex)
        {
        }

        Pipeline(const ComputePipelineDesc& desc,GraphicsDevice* pDevice) :GraphicsDeviceObject(pDevice), mBindPoint(PipelineBindPoint::Compute),
            mBlendState(), mDepthStencilState(),
            mInputLayout(), mMultisample(),
            mRasterizerState(),
            mResourceLayout(), mComputeShader(desc.pComputeShader),mSubpassIndex(255)
        {
        }

        ~Pipeline() = default;

        FORCEINLINE PipelineBindPoint GetBindPoint() const
        {
            return mBindPoint;
        }
        FORCEINLINE const BlendStateDesc& GetBlendState() const
        {
            return mBlendState;
        }
        FORCEINLINE const DepthStencilStateDesc& GetDepthStencilState() const
        {
            return mDepthStencilState;
        }
        FORCEINLINE const InputLayoutDesc& GetInputLayout() const
        {
            return mInputLayout;
        }
        FORCEINLINE const MultisampleDesc& GetMultisample() const
        {
            return mMultisample;
        }
        FORCEINLINE const RasterizerStateDesc& GetRasterizerState() const
        {
            return mRasterizerState;
        }
        FORCEINLINE const ResourceLayoutDesc& GetResourceLayout() const
        {
            return mResourceLayout;
        }
        FORCEINLINE const std::vector<Shader*>& GetGraphicsShaders() const
        {
            return mGraphicsShaders;
        }
        FORCEINLINE const Shader* GetComputeShader() const noexcept
        {
            return mComputeShader;
        }
        FORCEINLINE unsigned char GetSubpassIndex() const noexcept
        {
            return mSubpassIndex;
        }

        FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final
        {
            return GraphicsDeviceObjectType::Pipeline;
        }

    private:
        const PipelineBindPoint mBindPoint;
        const BlendStateDesc mBlendState;
        const DepthStencilStateDesc mDepthStencilState;
        const InputLayoutDesc mInputLayout;
        const MultisampleDesc mMultisample;
        const RasterizerStateDesc mRasterizerState;
        const ResourceLayoutDesc mResourceLayout;
        const std::vector<Shader*> mGraphicsShaders;
        const Shader* mComputeShader;
        const unsigned char mSubpassIndex;
    };

}

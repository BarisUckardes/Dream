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
            mResourceLayout(desc.ResourceLayout), mGraphicsShaders(desc.GraphicsShaders),mSubpassIndex(desc.SubpassIndex),mComputeShader(nullptr)
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

        FORCEINLINE PipelineBindPoint bind_point() const
        {
            return mBindPoint;
        }
        FORCEINLINE const BlendStateDesc& blend_state() const
        {
            return mBlendState;
        }
        FORCEINLINE const DepthStencilStateDesc& depth_stencil_state() const
        {
            return mDepthStencilState;
        }
        FORCEINLINE const InputLayoutDesc& input_layout() const
        {
            return mInputLayout;
        }
        FORCEINLINE const MultisampleDesc& multisample() const
        {
            return mMultisample;
        }
        FORCEINLINE const RasterizerStateDesc& rasterizer_state() const
        {
            return mRasterizerState;
        }
        FORCEINLINE const ResourceLayoutDesc& resource_layout() const
        {
            return mResourceLayout;
        }
        FORCEINLINE const std::vector<Shader*>& graphics_shaders() const
        {
            return mGraphicsShaders;
        }
        FORCEINLINE const Shader* compute_shader() const noexcept
        {
            return mComputeShader;
        }
        FORCEINLINE unsigned char subpass_index() const noexcept
        {
            return mSubpassIndex;
        }

        FORCEINLINE virtual GraphicsDeviceObjectType object_type() const noexcept override final
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

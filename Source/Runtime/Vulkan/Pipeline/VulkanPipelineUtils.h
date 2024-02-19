#pragma once
#include <Runtime/Graphics/Pipeline/BlendState/BlendStateAttachment.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendColorWriteMask.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendFactor.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendOperation.h>
#include <Runtime/Graphics/Pipeline/RasterizerState/PolygonMode.h>
#include <Runtime/Graphics/Pipeline/InputLayout/MeshTopology.h>
#include <Runtime/Graphics/Pipeline/RasterizerState/FaceCullMode.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <Runtime/Graphics/Texture/TextureSampleCount.h>
#include <Runtime/Graphics/Pipeline/DepthStencilState/StencilFaceDesc.h>
#include <Runtime/Graphics/Pipeline/DepthStencilState/StencilOperation.h>
#include <Runtime/Graphics/Pipeline/InputLayout/InputBindingStepRate.h>
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>
#include <Runtime/Vulkan/VulkanUtils.h>
#include <vulkan.h>

namespace Dream
{

    class RUNTIME_API VulkanPipelineUtils
    {
    public:
        FORCEINLINE static VkShaderStageFlags GetShaderStage(const ShaderStage stage)
        {
            switch (stage)
            {
            case ShaderStage::Vertex:
                return VK_SHADER_STAGE_VERTEX_BIT;
            case ShaderStage::Fragment:
                return VK_SHADER_STAGE_FRAGMENT_BIT;
            case ShaderStage::Compute:
                return VK_SHADER_STAGE_COMPUTE_BIT;
            default:
                return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
            }
        }
        FORCEINLINE static VkPrimitiveTopology GetTopology(const MeshTopology topology)
        {
            switch (topology)
            {
            case MeshTopology::PointList:
                return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
            case MeshTopology::LineList:
                return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
            case MeshTopology::TriangleList:
                return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            case MeshTopology::PatchList:
                return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
            default:
                return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
            }
        }
        FORCEINLINE static VkPolygonMode GetPolygonMode(const PolygonMode mode)
        {
            switch (mode)
            {
            case PolygonMode::Fill:
                return VK_POLYGON_MODE_FILL;
            case PolygonMode::Line:
                return VK_POLYGON_MODE_LINE;
            case PolygonMode::Point:
                return VK_POLYGON_MODE_POINT;
            default:
                return VK_POLYGON_MODE_MAX_ENUM;
            }
        }
        FORCEINLINE static VkCullModeFlags GetCullModes(const FaceCullMode modes)
        {
            switch (modes)
            {
            case FaceCullMode::None:
                return VK_CULL_MODE_NONE;
            case FaceCullMode::Front:
                return VK_CULL_MODE_FRONT_BIT;
            case FaceCullMode::Back:
                return VK_CULL_MODE_BACK_BIT;
            default:
                return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
            }
        }
        FORCEINLINE static VkSampleCountFlags GetSampleFlags(const TextureSampleCount count)
        {
            switch (count)
            {
            case TextureSampleCount::SAMPLE_COUNT_1:
                return VK_SAMPLE_COUNT_1_BIT;
            case TextureSampleCount::SAMPLE_COUNT_2:
                return VK_SAMPLE_COUNT_2_BIT;
            case TextureSampleCount::SAMPLE_COUNT_4:
                return VK_SAMPLE_COUNT_4_BIT;
            case TextureSampleCount::SAMPLE_COUNT_8:
                return VK_SAMPLE_COUNT_8_BIT;
            case TextureSampleCount::SAMPLE_COUNT_16:
                return VK_SAMPLE_COUNT_16_BIT;
            case TextureSampleCount::SAMPLE_COUNT_32:
                return VK_SAMPLE_COUNT_32_BIT;
            case TextureSampleCount::SAMPLE_COUNT_64:
                return VK_SAMPLE_COUNT_64_BIT;
            default:
                return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
            }
        }
        FORCEINLINE static VkPipelineStageFlags GetStageFlags(const PipelineStageFlags stages) noexcept
        {
            VkPipelineStageFlags flags = 0;
            if (stages & PipelineStageFlags::BottomOfPipe)
                flags |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            if (stages & PipelineStageFlags::ColorAttachmentOutput)
                flags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            if (stages & PipelineStageFlags::ComputeShader)
                flags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            if (stages & PipelineStageFlags::EarlyFragmentTests)
                flags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            if (stages & PipelineStageFlags::FragmentShader)
                flags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            if (stages & PipelineStageFlags::GeometryShader)
                flags |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
            if (stages & PipelineStageFlags::LateFragmentTests)
                flags |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            if (stages & PipelineStageFlags::TessellationControlShader)
                flags |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
            if (stages & PipelineStageFlags::TessellationEvaluationShader)
                flags |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
            if (stages & PipelineStageFlags::TopOfPipe)
                flags |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            if (stages & PipelineStageFlags::Transfer)
                flags |= VK_PIPELINE_STAGE_TRANSFER_BIT;
            if (stages & PipelineStageFlags::VertexInput)
                flags |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
            if (stages & PipelineStageFlags::VertexShader)
                flags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;

            return flags;
        }
        FORCEINLINE static VkStencilOp GetStencilOp(const StencilOperation operation) noexcept
        {
            switch (operation)
            {
            case StencilOperation::Keep:
                return VK_STENCIL_OP_KEEP;
            case StencilOperation::Zero:
                return VK_STENCIL_OP_ZERO;
            case StencilOperation::Replace:
                return VK_STENCIL_OP_REPLACE;
            case StencilOperation::IncrementAndClamp:
                return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
            case StencilOperation::DecrementAndClamp:
                return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
            case StencilOperation::Invert:
                return VK_STENCIL_OP_INVERT;
            case StencilOperation::IncrementAndWrap:
                return VK_STENCIL_OP_INCREMENT_AND_WRAP;
            case StencilOperation::DecrementAndWrap:
                return VK_STENCIL_OP_DECREMENT_AND_WRAP;
            default:
                return VK_STENCIL_OP_MAX_ENUM;
            }
        }
        FORCEINLINE static VkStencilOpState GetStencilFaceState(const StencilFaceDesc& desc) noexcept
        {
            return {
                    .failOp = GetStencilOp(desc.FailOperation),
                    .passOp = GetStencilOp(desc.PassOperation),
                    .depthFailOp = GetStencilOp(desc.DepthFailOperation),
                    .compareOp = VulkanUtils::GetCompareOperation(desc.CompareOperation),
                    .compareMask = desc.CompareMask,
                    .writeMask = desc.WriteMask,
                    .reference = desc.Reference
            };
        }
        FORCEINLINE static VkBlendFactor GetBlendFactor(const BlendFactor factor) noexcept
        {
            switch (factor)
            {
            case BlendFactor::Zero:
                return VK_BLEND_FACTOR_ZERO;
            case BlendFactor::One:
                return VK_BLEND_FACTOR_ONE;
            case BlendFactor::SrcColor:
                return VK_BLEND_FACTOR_SRC_COLOR;
            case BlendFactor::OneMinusSrcColor:
                return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
            case BlendFactor::DstColor:
                return VK_BLEND_FACTOR_DST_COLOR;
            case BlendFactor::OneMinusDstColor:
                return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
            case BlendFactor::SrcAlpha:
                return VK_BLEND_FACTOR_SRC_ALPHA;
            case BlendFactor::OneMinusSrcAlpha:
                return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            case BlendFactor::DstAlpha:
                return VK_BLEND_FACTOR_DST_ALPHA;
            case BlendFactor::OneMinusDstAlpha:
                return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
            case BlendFactor::ConstantColor:
                return VK_BLEND_FACTOR_CONSTANT_COLOR;
            case BlendFactor::OneMinusConstantColor:
                return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
            case BlendFactor::ConstantAlpha:
                return VK_BLEND_FACTOR_CONSTANT_ALPHA;
            case BlendFactor::OneMinusConstantAlpha:
                return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
            case BlendFactor::SrcAlphaSat:
                return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
            case BlendFactor::Src1Color:
                return VK_BLEND_FACTOR_SRC1_COLOR;
            case BlendFactor::OneMinusSrc1Color:
                return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
            case BlendFactor::Src1Alpha:
                return VK_BLEND_FACTOR_SRC1_ALPHA;
            case BlendFactor::OneMinusSrc1Alpha:
                return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
            default:
                return VK_BLEND_FACTOR_MAX_ENUM;
            }
        }
        FORCEINLINE static VkColorComponentFlags GetBlendMask(const BlendColorWriteMask mask) noexcept
        {
            switch (mask)
            {
                case BlendColorWriteMask::R:
                    return VK_COLOR_COMPONENT_R_BIT;
                case BlendColorWriteMask::G:
                    return VK_COLOR_COMPONENT_G_BIT;
                case BlendColorWriteMask::B:
                    return VK_COLOR_COMPONENT_B_BIT;
                case BlendColorWriteMask::A:
                    return VK_COLOR_COMPONENT_A_BIT;
                case BlendColorWriteMask::All:
                    return VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
                default:
                    return VK_COLOR_COMPONENT_FLAG_BITS_MAX_ENUM;
            }
        }
        FORCEINLINE static VkBlendOp GetBlendOperation(const BlendOperation operation) noexcept
        {
            switch (operation)
            {
            case BlendOperation::Add:
                return VK_BLEND_OP_ADD;
            case BlendOperation::Subtract:
                return VK_BLEND_OP_SUBTRACT;
            case BlendOperation::ReverseSubtract:
                return VK_BLEND_OP_REVERSE_SUBTRACT;
            case BlendOperation::Min:
                return VK_BLEND_OP_MIN;
            case BlendOperation::Max:
                return VK_BLEND_OP_MAX;
            default:
                return VK_BLEND_OP_MAX_ENUM;
            }
        }
        FORCEINLINE static VkPipelineColorBlendAttachmentState GetBlendAttachmentState(const BlendStateAttachment& attachment)
        {
            return {
                    .blendEnable = attachment.bEnabled,
                    .srcColorBlendFactor = GetBlendFactor(attachment.SourceColorFactor),
                    .dstColorBlendFactor = GetBlendFactor(attachment.DestinationColorFactor),
                    .colorBlendOp = GetBlendOperation(attachment.ColorOperation),
                    .srcAlphaBlendFactor = GetBlendFactor(attachment.SourceAlphaFactor),
                    .dstAlphaBlendFactor = GetBlendFactor(attachment.DestinationAlphaFactor),
                    .alphaBlendOp = GetBlendOperation(attachment.AlphaOperation),
                    .colorWriteMask = GetBlendMask(attachment.WriteMask)
            };
        }
        FORCEINLINE static VkVertexInputRate GetInputRate(const InputBindingStepRate stepRate) noexcept
        {
            switch (stepRate)
            {
            case InputBindingStepRate::Vertex:
                return VK_VERTEX_INPUT_RATE_VERTEX;
            case InputBindingStepRate::Instance:
                return VK_VERTEX_INPUT_RATE_INSTANCE;
            default:
                return VK_VERTEX_INPUT_RATE_MAX_ENUM;
            }
        }


    public:
        VulkanPipelineUtils() = delete;
        ~VulkanPipelineUtils() = delete;
    };

}

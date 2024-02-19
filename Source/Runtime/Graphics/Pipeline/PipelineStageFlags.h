#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
    enum class RUNTIME_API PipelineStageFlags : unsigned short
    {
        TopOfPipe = 1,
        VertexInput = 2,
        VertexShader = 4,
        TessellationControlShader = 8,
        TessellationEvaluationShader = 16,
        GeometryShader = 32,
        FragmentShader = 64,
        EarlyFragmentTests = 128,
        LateFragmentTests = 256,
        ColorAttachmentOutput = 512,
        ComputeShader = 1024,
        Transfer = 2048,
        BottomOfPipe = 4096
    };

    GENERATE_FLAGS(PipelineStageFlags, unsigned short)
}

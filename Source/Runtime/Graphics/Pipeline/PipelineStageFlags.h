#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
    enum class RUNTIME_API PipelineStageFlags : unsigned short
    {
        TopOfPipe = 0x0001,
        VertexInput = 0x0002,
        VertexShader = 0x0004,
        TessellationControlShader = 0x0008,
        TessellationEvaluationShader = 0x0010,
        GeometryShader = 0x0032,
        FragmentShader = 0x0064,
        EarlyFragmentTests = 0x0128,
        LateFragmentTests = 0x0256,
        ColorAttachmentOutput = 0x0512,
        ComputeShader = 0x1024,
        Transfer = 0x2048,
        BottomOfPipe = 0x4096
    };

    GENERATE_FLAGS(PipelineStageFlags, unsigned short)
}

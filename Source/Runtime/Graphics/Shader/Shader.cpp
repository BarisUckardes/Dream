#include "Shader.h"

namespace Dream
{
	Shader::~Shader()
	{

	}
	Shader::Shader(const ShaderDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),mStage(desc.Stage),mEntryMethod(desc.EntryMethod)
	{

	}
}

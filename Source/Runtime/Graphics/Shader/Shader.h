#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Shader/ShaderDesc.h>

namespace Dream
{
	class RUNTIME_API Shader : public GraphicsDeviceObject
	{
	public:
		~Shader();

		FORCEINLINE ShaderStage stage() const noexcept
		{
			return mStage;
		}
		FORCEINLINE std::string entry_method() const noexcept
		{
			return mEntryMethod;
		}
		virtual GraphicsDeviceObjectType object_type() const noexcept
		{
			return GraphicsDeviceObjectType::Shader;
		}
	protected:
		Shader(const ShaderDesc& desc, GraphicsDevice* pDevice);
	private:
		const ShaderStage mStage;
		const std::string mEntryMethod;
	};
}

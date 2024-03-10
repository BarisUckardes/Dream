#pragma once
#include <Runtime/Graphics/Instance/GraphicsInstanceDesc.h>
#include <vector>

namespace Dream
{
	class GraphicsAdapter;
	class RUNTIME_API GraphicsInstance
	{
	public:
		static GraphicsInstance* Create(const GraphicsInstanceDesc* pDesc);
	public:
		~GraphicsInstance();

		FORCEINLINE GraphicsBackend backend() const noexcept
		{
			return mBackend;
		}

		FORCEINLINE GraphicsAdapter* adapter(const unsigned char index)
		{
			if (index >= mAdapters.size())
				return nullptr;

			return mAdapters[index];
		}

		FORCEINLINE const std::vector<GraphicsAdapter*>& adapters() const noexcept
		{
			return mAdapters;
		}
	protected:
		GraphicsInstance(const GraphicsInstanceDesc* pDesc);
		void SetAdapters(const std::vector<GraphicsAdapter*>& adapters);
	private:
		const GraphicsBackend mBackend;
		std::vector<GraphicsAdapter*> mAdapters;
	};
}

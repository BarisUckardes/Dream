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

		FORCEINLINE GraphicsBackend GetBackend() const noexcept
		{
			return mBackend;
		}

		FORCEINLINE GraphicsAdapter* GetAdapter(const unsigned char index)
		{
			if (index >= mAdapters.size())
				return nullptr;

			return mAdapters[index];
		}

		FORCEINLINE const std::vector<GraphicsAdapter*>& GetAdapters() const noexcept
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
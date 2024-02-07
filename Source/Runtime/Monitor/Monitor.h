#pragma once
#include <Runtime/Monitor/MonitorDisplayMode.h>
#include <Windows.h>
#include <vector>

namespace Dream
{
	class RUNTIME_API Monitor final
	{
	public:
		static Monitor* GetPrimaryMonitor();
		static std::vector<Monitor*> GetPrimaryMonitors();
	public:
		~Monitor();

		FORCEINLINE const std::vector<MonitorDisplayMode>& GetModes() const noexcept
		{
			return mModes;
		}
		FORCEINLINE const MonitorDisplayMode& GetCurrentMode() const noexcept
		{
			return mCurrentMode;
		}
		FORCEINLINE int GetX() const noexcept
		{
			return mPosition[0];
		}
		FORCEINLINE int GetY() const noexcept
		{
			return mPosition[1];
		}
		FORCEINLINE unsigned int GetWidth() const noexcept
		{
			return mSize[0];
		}
		FORCEINLINE unsigned int GetHeight() const noexcept
		{
			return mSize[1];
		}
		FORCEINLINE HMONITOR GetHandle() const noexcept
		{
			return mHandle;
		}
	private:
		Monitor(const HMONITOR handle,const int x,const int y,const unsigned int width,const unsigned int height,const MonitorDisplayMode& currentMode,const std::vector<MonitorDisplayMode>& modes);

	private:
		std::vector<MonitorDisplayMode> mModes;
		MonitorDisplayMode mCurrentMode;
		int mPosition[2];
		unsigned int mSize[2];
		HMONITOR mHandle;
	};
}

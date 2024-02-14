#pragma once
#include <Runtime/Monitor/MonitorDisplayMode.h>
#include <vector>
#include <string>

namespace Dream
{
	class RUNTIME_API Monitor
	{
	public:
		static Monitor* GetPrimaryMonitor();
		static std::vector<Monitor*> GetMonitors();
	public:
		~Monitor();

		FORCEINLINE std::string GetName() const noexcept
		{
			return mName;
		}
		FORCEINLINE const MonitorDisplayMode& GetCurrentMode() const noexcept
		{
			return mCurrentMode;
		}
		FORCEINLINE const std::vector<MonitorDisplayMode>& GetModes() const noexcept
		{
			return mModes;
		}
		FORCEINLINE unsigned int GetWidth() const noexcept
		{
			return mWidth;
		}
		FORCEINLINE unsigned int GetHeight() const noexcept
		{
			return mHeight;
		}
		FORCEINLINE int GetPositionX() const noexcept
		{
			return mX;
		}
		FORCEINLINE int GetPositionY() const noexcept
		{
			return mY;
		}
	protected:
		Monitor(const std::string& name,const MonitorDisplayMode& currentMode,const std::vector<MonitorDisplayMode>& modes,const unsigned int width,const unsigned int height,const int x,const int y);
	private:
		const std::string mName;
		const MonitorDisplayMode mCurrentMode;
		const std::vector<MonitorDisplayMode> mModes;
		const unsigned int mWidth;
		const unsigned int mHeight;
		const int mX;
		const int mY;
	};
}

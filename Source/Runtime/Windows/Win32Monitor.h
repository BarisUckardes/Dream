#pragma once
#include <Runtime/Monitor/Monitor.h>
#include <Windows.h>

namespace Dream
{
	class RUNTIME_API Win32Monitor : public Monitor
	{
	public:
		static Monitor* primary_monitor();
		static std::vector<Monitor*> monitors();
	public:
		Win32Monitor(const HMONITOR handle,const std::string& name,const MonitorDisplayMode& currentMode,const std::vector<MonitorDisplayMode>& modes,const unsigned int width,const unsigned int height,const int x,const int y);
		~Win32Monitor();

		FORCEINLINE HMONITOR win32_handle() const noexcept
		{
			return mHandle;
		}
	private:
		const HMONITOR mHandle;
	};
}

#include "Monitor.h"

#ifdef DREAM_PLATFORM_WINDOWS
#include <Runtime/Windows/Win32Monitor.h>
typedef Dream::Win32Monitor PlatformAbstraction;
#endif

namespace Dream
{
	Monitor* Monitor::GetPrimaryMonitor()
	{
		return PlatformAbstraction::GetPrimaryMonitor();
	}
	std::vector<Monitor*> Monitor::GetMonitors()
	{
		return PlatformAbstraction::GetMonitors();
	}
	Monitor::~Monitor()
	{

	}
	Monitor::Monitor(const std::string& name, const MonitorDisplayMode& currentMode, const std::vector<MonitorDisplayMode>& modes, const unsigned int width, const unsigned int height, const int x, const int y) :
		mName(name),mCurrentMode(currentMode),mModes(modes),mWidth(width),mHeight(height), mX(x),mY(y)
	{

	}
}

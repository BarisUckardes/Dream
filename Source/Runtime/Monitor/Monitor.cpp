#include "Monitor.h"

#ifdef DREAM_PLATFORM_WINDOWS
#include <Runtime/Windows/Win32Monitor.h>
typedef Dream::Win32Monitor PlatformAbstraction;
#endif

namespace Dream
{
	Monitor* Monitor::primary_monitor()
	{
		return PlatformAbstraction::primary_monitor();
	}
	std::vector<Monitor*> Monitor::monitors()
	{
		return PlatformAbstraction::monitors();
	}
}

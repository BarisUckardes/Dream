#include "Monitor.h"

namespace Dream
{
#define WIN32_MAX_MONITORS 32
	struct MonitorProcData
	{
		HMONITOR Monitors[WIN32_MAX_MONITORS];
		unsigned int ProcIndex;
	};
	BOOL CALLBACK MonitorEnumProc(HMONITOR monitor, HDC monitorContext, LPRECT rect, LPARAM userData)
	{
		MonitorProcData* pData = (MonitorProcData*)userData;
		pData->Monitors[pData->ProcIndex] = monitor;
		pData->ProcIndex++;

		return TRUE;
	}
	Monitor* Monitor::GetPrimaryMonitor()
	{
		//Get process handle
		const HINSTANCE processHandle = GetModuleHandle(NULL);

		//Enum monitors
		MonitorProcData procData = {};
		procData.ProcIndex = 0;
		if (EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&procData) == 0)
			return nullptr;

		//Get monitor
		const HMONITOR monitorHandle = procData.Monitors[0];


		//Get info
		MONITORINFOEX monitorInfo = {};
		monitorInfo.cbSize = sizeof(monitorInfo);
		if (GetMonitorInfo(monitorHandle, &monitorInfo) == 0)
			return nullptr;

		//Get dev mode
		DEVMODE currentDisplayModeWin32 = {};
		currentDisplayModeWin32.dmSize = sizeof(currentDisplayModeWin32);
		if (EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &currentDisplayModeWin32) == 0)
			return nullptr;

		MonitorDisplayMode currentDisplayMode = {};
		currentDisplayMode.BitsPerPixel = currentDisplayModeWin32.dmBitsPerPel;
		currentDisplayMode.RefreshRate = currentDisplayModeWin32.dmDisplayFrequency;
		currentDisplayMode.Width = currentDisplayModeWin32.dmPelsWidth;
		currentDisplayMode.Height = currentDisplayModeWin32.dmPelsHeight;

		std::vector<MonitorDisplayMode> displayModes;
		unsigned int modeIndex = 0;
		if (EnumDisplaySettings(monitorInfo.szDevice, 0, &currentDisplayModeWin32) == 0)
		{
			return nullptr;
		}
		while (true)
		{
			if (EnumDisplaySettings(monitorInfo.szDevice, modeIndex, &currentDisplayModeWin32) == 0)
				break;


			MonitorDisplayMode displayMode = {};
			displayMode.Width = currentDisplayModeWin32.dmPelsWidth;
			displayMode.Height = currentDisplayModeWin32.dmPelsHeight;
			displayMode.BitsPerPixel = currentDisplayModeWin32.dmBitsPerPel;
			displayMode.RefreshRate = currentDisplayModeWin32.dmDisplayFrequency;

			displayModes.push_back(displayMode);
			modeIndex++;
		}

		return new Monitor(monitorHandle, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top, currentDisplayMode, displayModes);
	}
	std::vector<Monitor*> Monitor::GetPrimaryMonitors()
	{
		const HINSTANCE processHandle = GetModuleHandle(NULL);

		MonitorProcData procData = {};
		procData.ProcIndex = 0;
		if (EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&procData) == 0)
		{
			return {};
		}

		std::vector<Monitor*> monitors;
		for (unsigned int monitorIndex = 0; monitorIndex < procData.ProcIndex; monitorIndex++)
		{
			const HMONITOR monitorHandle = procData.Monitors[monitorIndex];

			MONITORINFOEX monitorInformation;
			monitorInformation.cbSize = sizeof(monitorInformation);
			if (GetMonitorInfo(monitorHandle, &monitorInformation) == 0)
			{
				continue;
			}

			DEVMODE  currentDisplayModeWin32 = {};
			currentDisplayModeWin32.dmSize = sizeof(currentDisplayModeWin32);
			if (EnumDisplaySettings(monitorInformation.szDevice, ENUM_CURRENT_SETTINGS, &currentDisplayModeWin32) == 0)
			{

			}
			MonitorDisplayMode currentDisplayMode = {};
			currentDisplayMode.BitsPerPixel = currentDisplayModeWin32.dmBitsPerPel;
			currentDisplayMode.RefreshRate = currentDisplayModeWin32.dmDisplayFrequency;
			currentDisplayMode.Width = currentDisplayModeWin32.dmPelsWidth;
			currentDisplayMode.Height = currentDisplayModeWin32.dmPelsHeight;


			std::vector<MonitorDisplayMode> displayModes;
			unsigned int modeIndex = 0;
			if (EnumDisplaySettings(monitorInformation.szDevice, 0, &currentDisplayModeWin32) == 0)
			{
				continue;
			}
			while (true)
			{
				if (EnumDisplaySettings(monitorInformation.szDevice, modeIndex, &currentDisplayModeWin32) == 0)
					break;


				MonitorDisplayMode displayMode = {};
				displayMode.Width = currentDisplayModeWin32.dmPelsWidth;
				displayMode.Height = currentDisplayModeWin32.dmPelsHeight;
				displayMode.BitsPerPixel = currentDisplayModeWin32.dmBitsPerPel;
				displayMode.RefreshRate = currentDisplayModeWin32.dmDisplayFrequency;

				displayModes.push_back(displayMode);
				modeIndex++;
			}

			Monitor* pMonitor = new Monitor(monitorHandle, monitorInformation.rcMonitor.left, monitorInformation.rcMonitor.top, monitorInformation.rcMonitor.right - monitorInformation.rcMonitor.left, monitorInformation.rcMonitor.bottom - monitorInformation.rcMonitor.top, currentDisplayMode, displayModes);
			monitors.push_back(pMonitor);
		}

		return monitors;
	}
	Monitor::~Monitor()
	{
	}
	Monitor::Monitor(const HMONITOR handle,const int x, const int y, const unsigned int width, const unsigned int height, const MonitorDisplayMode& currentMode, const std::vector<MonitorDisplayMode>& modes)
	{
		mPosition[0] = x;
		mPosition[1] = y;
		mSize[0] = width;
		mSize[1] = height;
		mCurrentMode = currentMode;
		mModes = modes;
		mHandle = handle;
	}
}

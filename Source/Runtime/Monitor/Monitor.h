#pragma once
#include <Runtime/Monitor/MonitorDisplayMode.h>
#include <string>
#include <vector>

namespace Dream
{
	class RUNTIME_API Monitor
	{
	public:
		static Monitor* primary_monitor();
		static std::vector<Monitor*> monitors();
	public:
		~Monitor()
		{

		}

		FORCEINLINE std::string name() const noexcept
		{
			return mName;
		}
		FORCEINLINE MonitorDisplayMode current_mode() const noexcept
		{
			return mCurrentMode;
		}
		FORCEINLINE const std::vector<MonitorDisplayMode>& modes() const noexcept
		{
			return mModes;
		}
		FORCEINLINE unsigned int width() const noexcept
		{
			return mWidth;
		}
		FORCEINLINE unsigned int height() const noexcept
		{
			return mHeight;
		}
		FORCEINLINE int x() const noexcept
		{
			return mX;
		}
		FORCEINLINE int y() const noexcept
		{
			return mY;
		}
	protected:
		Monitor(const std::string& name, const MonitorDisplayMode& currentMode, const std::vector<MonitorDisplayMode>& modes, const unsigned int width, const unsigned int height, const int x, const int y) :
			mName(name), mCurrentMode(currentMode), mModes(modes), mWidth(width), mHeight(height), mX(x), mY(y)
		{

		}
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

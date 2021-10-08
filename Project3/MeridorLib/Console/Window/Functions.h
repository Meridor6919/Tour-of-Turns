#pragma once
#include <array>

#include "DataStructures.h"

namespace MeridorConsoleLib
{
	namespace Window
	{
		void BlockingSleep(int miliseconds) noexcept;
		void SetConsoleEditMode(BOOL enable) noexcept;
		void SetCursor(HANDLE output_handle, BOOL visible) noexcept;
		[[nodiscard]] bool IsMinimizeButtonPressed(HANDLE input_handle) noexcept;
		[[nodiscard]] LONG GetWindowFlags(WindowMode window_mode) noexcept;
	}
}
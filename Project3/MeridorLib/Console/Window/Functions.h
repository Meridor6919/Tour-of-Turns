#pragma once
#include <array>

#include "DataStructures.h"

namespace MeridorConsoleLib
{
	namespace Window
	{
		void BlockingSleep(int miliseconds);
		void SetConsoleEditMode(BOOL enable);
		void SetCursor(HANDLE output_handle, BOOL visible);
		bool IsMinimizeButtonPressed(HANDLE input_handle);
		LONG GetWindowFlags(WindowMode window_mode);
	}
}
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Constants.h"

namespace MeridorConsoleLib
{
	namespace Window
	{
		struct WindowInfo
		{
			HANDLE input_handle = INVALID_HANDLE_VALUE;
			HANDLE output_handle = INVALID_HANDLE_VALUE;
			const char* title = default_title;
			HWND hwnd = nullptr;
			COORD characters_capacity = default_character_capacity;
			COORD window_size = minimum_window_size;
			WindowMode window_mode = WindowMode::windowed;
			bool visible_cursor = false;
		};
	}
}
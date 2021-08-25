#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace MeridorConsoleLib
{
	namespace Window
	{
		constexpr COORD minimum_font_size = { 3, 5 };
		constexpr COORD maximum_font_size = { 45, 72 };
		constexpr COORD minimum_window_size = { 800, 600 };
		constexpr COORD default_character_capacity = { 100, 60 };
		constexpr double font_aspect_ratio = 0.6;
		constexpr int window_immobilizer_refresh_rate = 50;
		constexpr int window_immobilizer_buffer_size = 30;
		constexpr const wchar_t* font_name = L"Lucida Console";
		constexpr const char* default_title = "Change Title attribute inside the WindowInfo structure of your MeridorConsoleLib::Window implementation";

		enum class WindowMode : unsigned int
		{
			fullscreen,
			windowed_fullscreen,
			windowed,
			last
		};
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
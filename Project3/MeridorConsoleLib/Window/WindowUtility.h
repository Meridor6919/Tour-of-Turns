#pragma once
#include <Windows.h>
#include <thread>

namespace MeridorConsoleLib
{
	constexpr COORD minimum_font_size = { 3, 5 };
	constexpr COORD maximum_font_size = { 45, 72 };
	constexpr COORD minimum_window_size = { 800, 600 };
	constexpr COORD default_character_capacity = { 100, 60 };
	constexpr double font_aspect_ratio = 0.6;
	constexpr int window_immobilizer_refresh_rate = 50;
	constexpr int window_immobilizer_buffer_size = 30;

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
		std::string title = "";
		HWND hwnd = NULL;
		COORD characters_capacity = default_character_capacity;
		COORD window_size = minimum_window_size;
		WindowMode window_mode = WindowMode::windowed;
		bool visible_cursor = false;
	};
}
#pragma once
#include <Windows.h>
#include <chrono>
#include <mutex>
#include "Color.h"

namespace MeridorConsoleLib
{
	constexpr COORD minimum_font_size = { 3, 5 };
	constexpr COORD maximum_font_size = { 45, 72 };
	constexpr COORD minimum_window_size = { 800, 600 };
	constexpr double font_aspect_ratio = 0.6;

	enum class TextAlign : unsigned int
	{
		left,
		center,
		right,
		last
	};

	struct MultithreadingData
	{
		std::mutex* mutex = nullptr;
		bool* skip_blocking_functions = nullptr;
		std::chrono::milliseconds delay;
	};
	enum class WindowMode : unsigned int
	{
		fullscreen,
		windowed_fullscreen,
		windowed,
		last
	};
	struct WindowInfo
	{
		HANDLE input_handle;
		HANDLE output_handle;
		Color main_color;
		Color secondary_color;
	};
	struct WindowInfoEx : WindowInfo
	{
		std::string title;
		HWND hwnd;
		COORD characters_capacity = { 100, 50 };
		COORD window_size = minimum_window_size;
		WindowMode window_mode = WindowMode::windowed;
		bool visible_cursor = false;
	};
}
#pragma once
#include <Windows.h>
#include <chrono>
#include <mutex>

namespace MeridorConsoleLib
{
	enum class TextAlign : unsigned int
	{
		left,
		center,
		right,
		last
	};
	enum Color : unsigned short
	{
		blue = 1,
		green,
		cyan,
		red,
		magenta,
		brown,
		light_gray,
		dark_gray,
		light_blue,
		light_green,
		light_cyan,
		light_red,
		light_magenta,
		yellow,
		white,
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
		HANDLE handle;
		Color main_color;
		Color secondary_color;
	};
	struct WindowInfoEx : WindowInfo
	{
		std::string title;
		HWND hwnd;
		short characters_capacity_height;
		COORD window_size;
		WindowMode window_mode;
		bool visible_cursor;
	};
}
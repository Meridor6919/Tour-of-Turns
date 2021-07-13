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
	struct MultithreadingData
	{
		std::mutex* mutex = nullptr;
		bool* skip_blocking_functions = nullptr;
		std::chrono::milliseconds delay;
	};
	enum class WindowMode : unsigned int
	{
		fullscreen,
		windowed,
		last
	};
	struct WindowInfo
	{
		HANDLE handle;
		int main_color;
		int secondary_color;
	};
	struct WindowInfoEx : WindowInfo
	{
		std::string title;
		HWND hwnd;
		COORD characters_capacity;
		COORD window_size;
		WindowMode window_mode;
		bool visible_cursor;
	};
}
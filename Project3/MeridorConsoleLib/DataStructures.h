#pragma once
#include <Windows.h>
#include <chrono>
#include <mutex>

namespace MeridorConsoleLib
{
	enum class TextAlign
	{
		left,
		center,
		right
	};
	struct MultithreadingData
	{
		std::mutex* mutex = nullptr;
		bool* skip_blocking_functions = nullptr;
		std::chrono::milliseconds delay;
	};
	struct WindowInfo
	{
		HANDLE handle;
		int main_color;
		int secondary_color;
	};
}
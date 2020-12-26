#pragma once
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#include "TextLib.h"

namespace MeridorConsoleLib
{
	class VisibleTimer
	{
		COORD position_of_timer;
		const MultithreadingData* multithreading_data;
		const WindowInfo* window_info;
		bool* timer_running = nullptr;

		std::thread thread;
		bool active_thread = false;
		std::chrono::time_point<std::chrono::system_clock> time_goal;

		void ShowRemainingTime();

	public:
		VisibleTimer(COORD position, const MeridorConsoleLib::WindowInfo* window_info, const MeridorConsoleLib::MultithreadingData* multithreading_data);
		void SetTimer(const std::chrono::seconds& time, bool* timer_running);
		void StopTimer();
		void StartShowingTimer();
		~VisibleTimer();
	};
}
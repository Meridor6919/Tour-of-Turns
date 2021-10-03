#pragma once
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

namespace MeridorConsoleLib
{
	class VisibleTimer
	{
		COORD position_of_timer;
		bool* timer_running = nullptr;

		std::thread thread;
		bool active_thread = false;
		std::chrono::time_point<std::chrono::system_clock> time_goal;
		std::chrono::milliseconds delay = std::chrono::milliseconds(50);

		void ShowRemainingTime();

	public:
		void SetTimer(const std::chrono::seconds& time, bool* timer_running);
		void StopTimer();
		void StartShowingTimer();
		~VisibleTimer();
	};
}
#pragma once
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <thread>

namespace MeridorTimer
{
	class Timer
	{
		COORD position_of_timer;
		bool* timer_running = nullptr;

		std::thread thread;
		bool active_thread = false;
		std::chrono::time_point<std::chrono::system_clock> starting_time;
		std::chrono::time_point<std::chrono::system_clock> ending_time;
		std::chrono::milliseconds delay = std::chrono::milliseconds(50);

		void Run();

	public:
		std::string GetText();
		std::chrono::seconds GetRemainingTime();
		void Set(const std::chrono::seconds& time, bool* timer_running);
		void Pause();
		void Unpause();
		void Stop();
		~Timer();
	};
}
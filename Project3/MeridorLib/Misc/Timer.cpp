#include "Timer.h"
namespace MeridorTimer
{
	void Timer::Run()
	{
		while (active_thread)
		{
			if (*timer_running)
			{
				auto time_remaining = ending_time - std::chrono::system_clock::now();
				int time_left = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(time_remaining).count());
				
				if (time_left <= 0)
				{
					*timer_running = false;
				}
			}
			std::this_thread::sleep_for(delay);
		}
	}
	std::string Timer::GetText()
	{
		int time_left = GetRemainingTime().count();

		int seconds_left = time_left % 60;
		int minutes_left = time_left / 60;
		if (time_left > 0)
		{
			return (minutes_left < 10 ? "0" : "") + std::to_string(minutes_left) + ':' + (seconds_left < 10 ? "0" : "") + std::to_string(seconds_left);
		}
		return "00:00";
	}
	std::chrono::seconds Timer::GetRemainingTime()
	{
		std::chrono::seconds remaining_time = std::chrono::duration_cast<std::chrono::seconds>(ending_time - (*timer_running ? std::chrono::system_clock::now() : starting_time));
		return remaining_time > std::chrono::seconds(0) ? remaining_time : std::chrono::seconds(0);
	}
	void Timer::Set(const std::chrono::seconds& time, bool* timer_running)
	{
		if (time.count() > 0)
		{
			Stop();
			*timer_running = true;
			this->timer_running = timer_running;
			starting_time = std::chrono::system_clock::now();
			ending_time = starting_time + time;
			thread = std::thread(&Timer::Run, this);
		}
	}
	void Timer::Pause()
	{
		starting_time = std::chrono::system_clock::now();
		*timer_running = false;
	}
	void Timer::Unpause()
	{
		ending_time = std::chrono::system_clock::now() - starting_time + ending_time;
		*timer_running = true;
	}
	void Timer::Stop()
	{
		if (thread.joinable())
		{
			active_thread = false;
			thread.join();
		}
	}
	Timer::~Timer()
	{
		Stop();
	}
}
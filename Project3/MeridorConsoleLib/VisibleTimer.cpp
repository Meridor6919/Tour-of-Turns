#include "VisibleTimer.h"

MeridorConsoleLib::VisibleTimer::VisibleTimer(COORD position, const WindowInfo *window_info, const MultithreadingData *multithreading_data)
{
	this->multithreading_data = multithreading_data;
	this->window_info = window_info;
	this->position_of_timer = position;
}
void MeridorConsoleLib::VisibleTimer::ShowRemainingTime()
{
	while(active_thread)
	{
		if (*timer_running)
		{
			//Calculate time
			auto time_remaining = time_goal - std::chrono::system_clock::now();
			int time_left = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(time_remaining).count());
			int seconds_left = time_left % 60;
			int minutes_left = time_left / 60;

			//Draw Timer
			multithreading_data->mutex->lock();
			SetConsoleCursorPosition(window_info->output_handle, position_of_timer);
			SetColor(window_info->output_handle, window_info->main_color);
			std::cout << (minutes_left < 10 ? "0" : "") << minutes_left << ':' << (seconds_left < 10 ? "0" : "") << seconds_left;
			multithreading_data->mutex->unlock();

			//if timer shows 00:00
			if (time_left <= 0)
			{
				*timer_running = false;
			}
		}
		std::this_thread::sleep_for(multithreading_data->delay);
	}
}
void MeridorConsoleLib::VisibleTimer::SetTimer(const std::chrono::seconds& time, bool* timer_running)
{
	if (time.count() > 0)
	{
		*timer_running = true;
		this->timer_running = timer_running;
		this->time_goal = std::chrono::system_clock::now() + time;
	}
}
void MeridorConsoleLib::VisibleTimer::StopTimer()
{
	if (thread.joinable())
	{
		active_thread = false;
		thread.join();
	}
	multithreading_data->mutex->lock();
	SetConsoleCursorPosition(window_info->output_handle, position_of_timer);
	std::cout << "     ";
	multithreading_data->mutex->unlock();
}
void MeridorConsoleLib::VisibleTimer::StartShowingTimer()
{
	StopTimer();
	active_thread = true;
	thread = std::thread(&VisibleTimer::ShowRemainingTime, this);
}
MeridorConsoleLib::VisibleTimer::~VisibleTimer()
{
	StopTimer();
}

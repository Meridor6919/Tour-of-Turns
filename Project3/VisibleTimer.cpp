#include "VisibleTimer.h"

VisibleTimer::VisibleTimer(COORD coords, HANDLE handle, bool *timer_running, int color, std::mutex *mutex)
{
	this->position_of_timer = coords;
	this->window_handle = handle;
	this->timer_running = timer_running;
	this->mutex = mutex;
	this->color = color;
	*timer_running = false;
	thread = std::make_unique<std::thread>(&VisibleTimer::ShowTime, this);

}
void VisibleTimer::ShowTime()
{
	while (iterate)
	{
		if (*timer_running)
		{
			auto time_elapsed = std::chrono::system_clock::now() - time;
			int miliseconds_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time_elapsed).count();
			int seconds_required = (seconds - miliseconds_elapsed / 1000) % 60;
			int minutes_required = (seconds - miliseconds_elapsed / 1000) / 60;
			mutex->lock();
			SetConsoleCursorPosition(window_handle, position_of_timer);
			SetConsoleTextAttribute(window_handle, color);
			std::cout << (minutes_required < 10 ? "0" : "") << minutes_required << ':' << (seconds_required < 10 ? "0" : "") << seconds_required;
			mutex->unlock();
			if (miliseconds_elapsed > seconds * 1000)
			{
				*timer_running = false;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}
}
void VisibleTimer::StartTimer(int timer_settings)
{
	*timer_running = true;
	this->seconds = timer_settings * 10;
	time = std::chrono::system_clock::now();
}
void VisibleTimer::StopTimer()
{
	if (thread->joinable())
	{
		iterate = false;
		thread->join();
	}
	mutex->lock();
	SetConsoleCursorPosition(window_handle, position_of_timer);
	std::cout << "     ";
	mutex->unlock();
}
VisibleTimer::~VisibleTimer()
{
	StopTimer();
}

#pragma once
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

class VisibleTimer
{
	COORD position_of_timer;
	HANDLE window_handle;
	bool *timer_running;
	bool iterate = true;
	std::mutex *mutex;
	std::unique_ptr<std::thread> thread;

	int delay = 50;
	int seconds;
	std::chrono::time_point<std::chrono::system_clock> time;
	void ShowTime();


public:
	VisibleTimer(COORD coords, HANDLE handle, bool *timer_running, std::mutex *mutex);
	void StartTimer(int timer_settings);
	void StopTimer();
	~VisibleTimer();
};

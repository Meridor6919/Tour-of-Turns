#pragma once
#include <thread>
#include <Windows.h>

namespace MeridorConsoleLib
{
	class Window;
	constexpr int refresh_rate = 50;
	constexpr int buffer_size = 30;


	class WindowImmobilizer
	{
		bool thread_active = false;
		std::thread main_thread;
		Window* main_window;

		void ImmobilizingWindow();

	public:

		void Init(Window* window);
		void Start();
		void Stop();
		~WindowImmobilizer();
	};
}


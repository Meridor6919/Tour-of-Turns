#pragma once
#include <thread>
#include <Windows.h>

namespace MeridorConsoleLib
{
	class Window;

	class WindowImmobilizer
	{
		std::thread main_thread;
		bool thread_active = false;
		Window* main_window;

		void ImmobilizingWindow();

	public:
		void Init(Window* window);
		void Start();
		void Stop();
		~WindowImmobilizer();
	};
}


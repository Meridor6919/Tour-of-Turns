#pragma once
#include "WindowUtility.h"

namespace MeridorConsoleLib
{
	class Window;

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


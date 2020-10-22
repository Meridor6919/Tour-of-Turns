#pragma once
#include "NetworkConnectorGeneral.h"
namespace NetworkConnector
{
	class ThreadOperatingClass
	{
	protected:
		bool thread_active = false;
		std::unique_ptr<std::thread> main_thread;
		virtual void ThreadFunctionality() = 0;

	public:
		virtual void Start()
		{
			if (!thread_active)
			{
				thread_active = true;
				main_thread = std::make_unique<std::thread>(&ThreadOperatingClass::ThreadFunctionality, this);
			}
		}
		virtual void Stop()
		{
			if (thread_active)
			{
				thread_active = false;
				if (main_thread->joinable())
				{
					main_thread->join();
				}
			}
		}
		virtual ~ThreadOperatingClass()
		{
			Stop();
		}
	};
}
#pragma once
#include "NetworkConnectorGeneral.h"

namespace NetworkConnector
{
	class BroadcastSender
	{
		bool thread_active = false;
		std::thread main_thread;
		bool hamachi = false;
		void Broadcasting();

	public:
		void Stop();
		void Start(bool hamachi);
		~BroadcastSender();
	};
}

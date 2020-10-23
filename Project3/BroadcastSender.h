#pragma once
#include "NetworkConnectorGeneral.h"

namespace NetworkConnector
{
	class BroadcastSender
	{
		bool thread_active = false;
		std::unique_ptr<std::thread> main_thread;
		bool hamachi = false;
		void Broadcasting();
		
	public:
		void SetHamachiFlag(bool flag);
		void Stop();
		void Start();
		~BroadcastSender();
	};
}

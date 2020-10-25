#pragma once
#include "NetworkConnectorGeneral.h"

namespace NetworkConnector
{
	class BroadcastReceiver
	{
		bool thread_active = false;
		std::thread main_thread;
		bool hamachi = false;
		std::mutex broadcast_mutex;
		std::vector<std::string> hosts_broadcasting;

		void Receiving();

	public:
		void Stop();
		void Start(bool hamachi);
		std::vector<std::string> GetHostVector();
		void ResetHostVector();
		~BroadcastReceiver();
	};
}
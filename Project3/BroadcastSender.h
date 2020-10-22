#pragma once
#include "ThreadOperatingClass.h"

namespace NetworkConnector
{
	class BroadcastSender : public ThreadOperatingClass
	{
		bool hamachi = false;
		void ThreadFunctionality();
		
	public:
		void SetHamachiFlag(bool flag);
	};
}

#include "MeridorMultiplayer.h"
#include <map>

namespace MeridorMultiplayer
{
	class Client {

	protected:
		//key is the time of last message received from this host
		//std::string is describing name of the host and his ip separated with " - "
		std::map<int, std::string> current_hosts;
		SOCKET *host;
		sockaddr_in addr;

		//Flag that stands for whether or not Recv Broadcast should be stopped
		bool receiving_broadcast;

	public:
		Client(SOCKET *host);
		bool RecvBroadcast(const int max_hosts, int ms_interval);
		bool Connect(const std::string ip);
		std::string GetIpFromMapValue(const std::string value);
		void FinishBroadcast() { receiving_broadcast = false; }
		std::vector<std::string> GetCurrentHosts();
		bool Recv(SOCKET socket, char *buffer, const int flags);
	};
}



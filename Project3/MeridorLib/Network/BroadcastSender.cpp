#include "BroadcastSender.h"

void NetworkConnector::BroadcastSender::Broadcasting()
{
	//UDP protocol to broadcast messages to all addresses in local network and virtual local network if flag is set
	SOCKET broadcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in sock_addr = {};
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(NetworkConnector::Constants::port_number);
	char option = '1';
	char host_name[50];

	Validate(broadcast_socket < 0);
	Validate(setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &option, sizeof(int)) < 0);
	Validate(gethostname(host_name, sizeof(host_name)) < 0);
	while (this->thread_active)
	{
		bool result = false;
		for (int i = 0; i < 255; ++i)
		{
			InetPton(AF_INET, ("192.168." + std::to_string(i) + ".255").c_str(), &sock_addr.sin_addr.s_addr);
			if (!(sendto(broadcast_socket, host_name, sizeof(host_name), 0, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr)) < 0))
			{
				result = true;
			}
		}
		if (hamachi)
		{
			InetPton(AF_INET, "25.255.255.255", &sock_addr.sin_addr.s_addr);
			if (!(sendto(broadcast_socket, host_name, sizeof(host_name), 0, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr)) < 0))
			{
				result = true;
			}
		}
		Validate(!result);
		std::chrono::milliseconds ms(NetworkConnector::Constants::ms_delay);
		std::this_thread::sleep_for(ms);
	}
	closesocket(broadcast_socket);
}

void NetworkConnector::BroadcastSender::Stop()
{
	thread_active = false;
	if (main_thread.joinable())
	{
		main_thread.join();
	}
}

void NetworkConnector::BroadcastSender::Start(bool hamachi)
{
	this->hamachi = hamachi;
	thread_active = true;
	main_thread = std::thread(&BroadcastSender::Broadcasting, this);
}

NetworkConnector::BroadcastSender::~BroadcastSender()
{
	Stop();
}

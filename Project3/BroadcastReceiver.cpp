#include "BroadcastReceiver.h"

void NetworkConnector::BroadcastReceiver::Receiving()
{
	//UDP protocol to broadcast messages to all addresses in local network and virtual local network if flag is set
	SOCKET intercept_brodcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in addr = {};
	int addr_size = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(NetworkConnector::Constants::port_number);
	addr.sin_addr.s_addr = ADDR_ANY;

	Validate(intercept_brodcast_socket < 0);
	Validate(bind(intercept_brodcast_socket, reinterpret_cast<sockaddr*>(&addr), addr_size));

	char host_name_buffer[50];
	while (thread_active)
	{
		int recv_result = recvfrom(intercept_brodcast_socket, host_name_buffer, 50, 0, reinterpret_cast<sockaddr*>(&addr), &addr_size);
		in_addr temp_addr = {};
		hostent* hostent = gethostbyname(host_name_buffer);
		if (hostent != nullptr && recv_result != -1)
		{
			for (int i = 0; hostent->h_addr_list[i] != 0; ++i)
			{
				memcpy(&temp_addr, hostent->h_addr_list[i], sizeof(struct in_addr));
			}
			std::string intercepted_host = static_cast<std::string>(host_name_buffer) + " - " + static_cast<std::string>(inet_ntoa(temp_addr));

			broadcast_mutex.lock();
			bool finded = false;
			for (size_t i = 0; i < hosts_broadcasting.size(); ++i)
			{
				if (hosts_broadcasting[i] == intercepted_host)
				{
					finded = true;
					break;
				}
			}
			if (!finded)
			{
				hosts_broadcasting.emplace_back(intercepted_host);
			}
			broadcast_mutex.unlock();
		}
	}
	closesocket(intercept_brodcast_socket);
}

void NetworkConnector::BroadcastReceiver::Stop()
{
	thread_active = false;
	if (main_thread.joinable())
	{
		main_thread.join();
	}
}

void NetworkConnector::BroadcastReceiver::Start(bool hamachi)
{
	this->hamachi = hamachi;
	thread_active = true;
	main_thread = std::thread(&BroadcastReceiver::Receiving, this);
}

std::vector<std::string> NetworkConnector::BroadcastReceiver::GetHostVector()
{
	return hosts_broadcasting;
}

void NetworkConnector::BroadcastReceiver::ResetHostVector()
{
	broadcast_mutex.lock();
	hosts_broadcasting.clear();
	broadcast_mutex.unlock();
	std::chrono::milliseconds ms(NetworkConnector::Constants::ms_delay);
	std::this_thread::sleep_for(ms);
}

NetworkConnector::BroadcastReceiver::~BroadcastReceiver()
{
	Stop();
}




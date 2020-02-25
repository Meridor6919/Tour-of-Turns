#include	"MeridorMultiplayerHost.h"

void MeridorMultiplayer::Host::Broadcast(const unsigned long addr_range, const int ms_interval)
{
	//UDP protocol to broadcast messages to all addresses in local network and virtual local network if flag is set
	SOCKET broadcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in sock_addr = {};
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(Constants::port_number);
	sock_addr.sin_addr.s_addr = addr_range;
	broadcast_running = true;

	if (broadcast_socket < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
		WSACleanup();
		exit(0);
	}
	char option = '1';
	if (setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &option, sizeof(int)) < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
		WSACleanup();
		exit(0);
	}

	char host_name[50];
	if (gethostname(host_name, sizeof(host_name)) < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
		WSACleanup();
		exit(0);
	}
	while (broadcast_running)
	{
		if (sendto(broadcast_socket, host_name, sizeof(host_name), 0, (sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
		{
			MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
			WSACleanup();
			exit(0);
		}
		std::chrono::milliseconds ms(ms_interval);
		std::this_thread::sleep_for(ms);

	}
	closesocket(broadcast_socket);
}
void MeridorMultiplayer::Host::AcceptClients(const int max)
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sock_addr = {};
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(Constants::port_number);
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	int addr_size = sizeof(sockaddr_in);
	accepting_running = true;

	if (sock < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
		WSACleanup();
		exit(0);
	}
	if (bind(sock, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sockaddr)) < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
		WSACleanup();
		exit(0);
	}
	listen(sock, max);

	while (accepting_running)
	{
		//Creating temporary socket to check if accepted connection is valid
		SOCKET temp = accept(sock, reinterpret_cast<sockaddr*>(&sock_addr), &addr_size);
		if (temp != INVALID_SOCKET && accepting_running)
		{
			bool playable = true;

			if (static_cast<int>(clients.size()) >= max)
			{
				playable = false;
			}
			for (int i = 0; i < static_cast<int>(clients.size()); ++i)
			{
				if (sock_addr.sin_addr.s_addr == clients[i].second.sin_addr.s_addr)
				{
					playable = false;
					break;
				}
			}
			for (int i = 0; i < static_cast<int>(black_list.size()); ++i)
			{
				if (black_list[i].sin_addr.s_addr == sock_addr.sin_addr.s_addr)
				{
					playable = false;
					break;
				}
			}
			if (playable)
			{
				clients.push_back(std::make_pair(temp, sock_addr));
				accept(clients[static_cast<int>(clients.size()) - 1].first, reinterpret_cast<sockaddr*>(&sock_addr), &addr_size);
			}
		}
	}
	closesocket(sock);
}
void MeridorMultiplayer::Host::StopAcceptingClients()
{
	//Connecting with myself to process accept function that would otherwise wait forever
	SOCKET temp = socket(AF_INET, SOCK_STREAM, 0);
	accepting_running = false;

	if (temp < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
		WSACleanup();
		exit(0);
	}
	sockaddr_in SocketAddress = {};
	SocketAddress.sin_family = AF_INET;
	SocketAddress.sin_port = htons(Constants::port_number);
	SocketAddress.sin_addr.s_addr = inet_addr(Constants::ip_loopback.c_str());

	connect(temp, reinterpret_cast<sockaddr*>(&SocketAddress), sizeof(SocketAddress));
	closesocket(temp);
}
void MeridorMultiplayer::Host::CloseActiveConnections()
{
	handling_connection = false;
	for (int i = 0; i < static_cast<int>(clients.size()); ++i)
	{
		closesocket(clients[i].first);
	}
	for (int i = 0; i < static_cast<int>(clients.size()); ++i)
	{
		recv_threads[i].join();
	}
}
std::string MeridorMultiplayer::Host::GetThisIp(const sockaddr_in sock_addr)
{
	char helper[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &sock_addr.sin_addr, helper, INET_ADDRSTRLEN);

	return static_cast<std::string>(helper);
}

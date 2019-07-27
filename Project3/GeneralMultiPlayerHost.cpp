#include "GeneralMultiPlayer.h"


GeneralMultiPlayer::Host::Host()
{
	port = 6919;
}
void GeneralMultiPlayer::Host::Broadcast(unsigned long addr_range, int ms_interval)
{
	broadcast_running = true;

	//UDP protocol to broadcast messages to all addresses in local network and virtual local network if flag is set
	SOCKET broadcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in sock_addr;
	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(6919);
	sock_addr.sin_addr.s_addr = addr_range;

	if (broadcast_socket < 0)
	{
		MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		WSACleanup();
		exit(0);
	}
	char option = '1';
	if (setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &option, sizeof(int)) < 0)
	{
		MessageBox(0, ("Socket option error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		WSACleanup();
		exit(0);
	}

	char host_name[50];
	if (gethostname(host_name, sizeof(host_name)) < 0)
	{
		MessageBox(0, ("Gethostname error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		WSACleanup();
		exit(0);
	}
	while (broadcast_running)
	{
		if (sendto(broadcast_socket, host_name, sizeof(host_name), 0, (sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
		{
			MessageBox(0, ("Sending error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
			WSACleanup();
			exit(0);
		}
		std::chrono::milliseconds ms(ms_interval);
		std::this_thread::sleep_for(ms);

	}
	closesocket(broadcast_socket);
}
void GeneralMultiPlayer::Host::AcceptClients(int max)
{
	accepting_running = true;
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sock_addr;
	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(6919);
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	int addr_size = sizeof(sockaddr_in);

	if (sock < 0)
	{
		MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		WSACleanup();
		exit(0);
	}
	if (bind(sock, (sockaddr *)& sock_addr, sizeof(sockaddr)) < 0)
	{
		MessageBox(0, ("Binding error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		WSACleanup();
		exit(0);
	}
	listen(sock, max);

	while (accepting_running)
	{
		//Creating temporary socket to check if accepted connection is valid
		SOCKET temp = accept(sock, (sockaddr *)& sock_addr, &addr_size);
		if (temp != INVALID_SOCKET && accepting_running)
		{
			bool good = true;

			if (clients.size() >= max)
			{
				good = false;
			}
			for (int i = 0; i < clients.size(); i++)
			{
				if (sock_addr.sin_addr.s_addr == clients[i].second.sin_addr.s_addr)
				{
					good = false;
					break;
				}
			}
			for (int i = 0; i < black_list.size(); i++)
			{
				if (black_list[i].sin_addr.s_addr == sock_addr.sin_addr.s_addr)
				{
					good = false;
					break;
				}
			}
			if (good)
			{
				clients.push_back(std::make_pair(temp, sock_addr));
				accept(clients[clients.size() - 1].first, (sockaddr *)& sock_addr, &addr_size);
			}
		}
	}
	closesocket(sock);
}
void GeneralMultiPlayer::Host::StopAcceptingClients()
{
	accepting_running = false;

	//Connecting with myself to process accept function that would otherwise wait forever
	SOCKET temp = socket(AF_INET, SOCK_STREAM, 0);

	if (temp < 0)
	{
		MessageBox(0, ("Socket error: " + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		WSACleanup();
		exit(0);
	}
	sockaddr_in SocketAddress;
	memset(&SocketAddress, 0, sizeof(SocketAddress));
	SocketAddress.sin_family = AF_INET;
	SocketAddress.sin_port = htons(6919);
	SocketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(temp, (sockaddr *)&SocketAddress, sizeof(SocketAddress));
	closesocket(temp);
}
void GeneralMultiPlayer::Host::CloseActiveConnections()
{
	handling_connection = false;
	for (int i = 0; i < clients.size(); i++)
		closesocket(clients[i].first);

	for (int i = 0; i < clients.size(); i++)
		recv_threads[i].join();
}
std::string GeneralMultiPlayer::Host::GetThisIp(sockaddr_in sock_addr)
{
	char helper[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &sock_addr.sin_addr, helper, INET_ADDRSTRLEN);

	return helper;
}

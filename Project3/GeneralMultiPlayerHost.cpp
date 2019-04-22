#include "GeneralMultiPlayer.h"


GeneralMultiPlayer::Host::Host()
{
	port = 6919;
}
void GeneralMultiPlayer::Host::Broadcast(unsigned long addr_range, int ms_interval)
{
	broadcast_running = true;

	//creating socket in udp protocole for broadcasting msgs
	SOCKET broadcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (broadcast_socket < 0)
	{
		MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		WSACleanup();
		exit(0);
	}

	//setting socket to broadcast
	char option = 1;
	if (setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &option, sizeof(int)) < 0)
	{
		MessageBox(0, ("Socket option error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		WSACleanup();
		exit(0);
	}
	sockaddr_in sock_addr;
	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(6919);
	sock_addr.sin_addr.s_addr = addr_range;

	//getting host's name
	char host_name[50];

	if (gethostname(host_name, sizeof(host_name)) < 0)
	{
		MessageBox(0, ("Gethostname error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		WSACleanup();
		exit(0);
	}

	//sending msgs with host's name to all addresses in given range
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

	//socket for accepting clients
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0)
	{
		MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		WSACleanup();
		exit(0);
	}
	sockaddr_in sock_addr;
	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(6919);
	sock_addr.sin_addr.s_addr = INADDR_ANY;

	int addr_size = sizeof(sockaddr_in);

	if (bind(sock, (sockaddr *)& sock_addr, sizeof(sockaddr)) < 0)
	{
		MessageBox(0, ("Binding error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		WSACleanup();
		exit(0);
	}

	listen(sock, max);

	//main accepting loop
	while (accepting_running)
	{
		//creating temporary socket to check if accepted connection is valid
		SOCKET temp = accept(sock, (sockaddr *)& sock_addr, &addr_size);
		if (temp != INVALID_SOCKET && accepting_running)
		{
			bool good = true;

			//checking size
			if (clients.size() >= max)
			{
				good = false;
			}

			//checking if client is already in base
			for (int i = 0; i < clients.size(); i++)
			{
				if (sock_addr.sin_addr.s_addr == clients[i].second.sin_addr.s_addr)
				{
					good = false;
					break;
				}
			}
			//checking black list
			for (int i = 0; i < black_list.size(); i++)
			{
				if (black_list[i].sin_addr.s_addr == sock_addr.sin_addr.s_addr)
				{
					good = false;
					break;
				}
			}
			//adding client to the vector and accepting connection
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

	//connecting with myself to proc accept function that will wait forever otherwise
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
std::string GeneralMultiPlayer::Host::GetIP(sockaddr_in sock_addr)
{
	char helper[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &sock_addr.sin_addr, helper, INET_ADDRSTRLEN);

	return helper;
}

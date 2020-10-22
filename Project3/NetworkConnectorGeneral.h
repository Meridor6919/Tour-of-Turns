#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <vector>
#include <WinSock2.h>
#include <thread>
#include <string>
#include <ws2tcpip.h>
#include <vector>
#include <mutex>

#pragma comment(lib, "Ws2_32.lib")

namespace NetworkConnector
{
	static bool network_initialized = false;
	namespace ErrorTitle
	{
		const std::string winsock = "Winsock Error";
		const std::string disconnect = "Disconnected from game";
		const std::string connection = "Connection error";
		const std::string initialization = "Initialization error";
	}
	namespace Constants
	{
		const std::vector<std::string> host_multiplayer_lobby = { "Start Game", "Ban players", "Unban players", "Back" };
		constexpr int buffer_size = 255;
		constexpr int port_number = 6919;
		constexpr short max_ip_size = 17;
		constexpr int ms_delay = 100;
		const std::string ip_loopback = "127.0.0.1";
	}
	namespace ErrorMsg
	{
		const std::string connection = "Player probably finished hosting";
		const std::string initialization = "Only one network instance allowed";
	}
	inline void Validate(bool error)
	{
		if (error)
		{
			MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
			WSACleanup();
			exit(0);
		}
	}
}
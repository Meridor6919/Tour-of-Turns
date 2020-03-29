#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <vector>
#include <WinSock2.h>
#include <thread>
#include <string>
#include <ws2tcpip.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

namespace MeridorMultiplayer
{
	namespace ErrorTitle
	{
		const std::string winsock = "Winsock Error";
		const std::string disconnect = "Disconnected from game";
		const std::string connection = "Connection error";
	}
	namespace Constants
	{
		const int buffer_size = 255;
		const int port_number = 6919;
		const std::string ip_loopback = "127.0.0.1";
	}
	namespace ErrorMsg
	{
		const std::string connection = "Player probably finished hosting";
	}
}
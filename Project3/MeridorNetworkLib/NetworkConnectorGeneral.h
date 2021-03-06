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
	void Initialize();
	void Validate(bool error);
	bool Recv(SOCKET socket, std::string* msg, int flags = 0);
	bool SendRequest(SOCKET socket, const std::string request, int flags = 0);

	namespace ErrorTitle
	{
		const std::string winsock = "Winsock Error";
		const std::string disconnect = "Disconnected from game";
		const std::string connection = "Connection error";
		const std::string initialization = "Initialization error";
		const std::string address_missing = "Address missing";
		const std::string msg_size = "Too big message";
		const std::string ip_extraction = "Couldn't extract crucial data";
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
		const std::string address_missing = "Strangly selected target vanished before any action was performed";
		const std::string msg_size = "This library cannot send messages larger than pre-defined constant\n Try sending couple smaller messages";
		const std::string ip_extraction = "Error while converting host's signature to ip address";
	}

}
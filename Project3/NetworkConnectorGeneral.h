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
	inline void Validate(bool error)
	{
		if (error)
		{
			MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
			WSACleanup();
			abort();
		}
	}
	inline bool Recv(SOCKET socket, std::string *msg, int flags = 0)
	{
		char buffer[Constants::buffer_size];
		if (recv(socket, buffer, Constants::buffer_size, flags) < 0)
		{
			MessageBox(0, ErrorMsg::connection.c_str(), ErrorTitle::disconnect.c_str(), 0);
			return false;
		}
		(*msg) = buffer;
		return true;
	}
	inline bool SendRequest(SOCKET socket, const std::string request, int flags = 0)
	{
		char buffer[Constants::buffer_size] = "";
		size_t request_size = request.size();
		if (request_size > NetworkConnector::Constants::buffer_size)
		{
			MessageBox(0, ErrorMsg::msg_size.c_str(), ErrorTitle::msg_size.c_str(), 0);
			abort();
		}
		for (size_t i = 0; i < request_size; ++i)
		{
			buffer[i] = request[i];
		}
		if(send(socket, buffer, NetworkConnector::Constants::buffer_size, 0))
		{
			MessageBox(0, ErrorMsg::connection.c_str(), ErrorTitle::disconnect.c_str(), 0);
			return false;
		}
		return true;
	}
	inline void Initialize()
	{
		static bool network_initialized = false;

		if (network_initialized)
		{
			WSAData wsa_data;
			network_initialized = true;
			Validate(WSAStartup(MAKEWORD(2, 2), &wsa_data));
		}
		
	}
}
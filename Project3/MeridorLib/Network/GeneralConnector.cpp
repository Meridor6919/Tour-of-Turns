#include "GeneralConnector.h"

namespace MeridorNetwork
{
	void MeridorNetwork::Initialize()
	{
		static bool network_initialized = false;

		if (!network_initialized)
		{
			WSAData wsa_data;
			network_initialized = true;
			Validate(WSAStartup(MAKEWORD(2, 2), &wsa_data));
		}

	}
	void MeridorNetwork::Validate(bool error)
	{
		if (error)
		{
			MessageBoxA(0, std::to_string(WSAGetLastError()).c_str(), MeridorNetwork::ErrorTitle::winsock.c_str(), MB_TOPMOST);
			WSACleanup();
			abort();
		}
	}
	bool MeridorNetwork::Recv(SOCKET socket, std::string* msg, int flags)
	{
		char buffer[Constants::buffer_size];
		if (recv(socket, buffer, Constants::buffer_size, flags) < 0)
		{
			MessageBoxA(0, ErrorMsg::connection.c_str(), ErrorTitle::disconnect.c_str(), MB_TOPMOST);
			return false;
		}
		(*msg) = buffer;
		return true;
	}
	bool MeridorNetwork::SendRequest(SOCKET socket, const std::string request, int flags)
	{
		char buffer[Constants::buffer_size] = "";
		size_t request_size = request.size();
		if (request_size > MeridorNetwork::Constants::buffer_size)
		{
			MessageBoxA(0, ErrorMsg::msg_size.c_str(), ErrorTitle::msg_size.c_str(), MB_TOPMOST);
			abort();
		}
		for (size_t i = 0; i < request_size; ++i)
		{
			buffer[i] = request[i];
		}
		if (send(socket, buffer, MeridorNetwork::Constants::buffer_size, 0))
		{
			MessageBoxA(0, ErrorMsg::connection.c_str(), ErrorTitle::disconnect.c_str(), MB_TOPMOST);
			return false;
		}
		return true;
	}
}
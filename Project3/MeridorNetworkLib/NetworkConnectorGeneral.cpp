#include "NetworkConnectorGeneral.h"

void NetworkConnector::Initialize()
{
	static bool network_initialized = false;

	if (network_initialized)
	{
		WSAData wsa_data;
		network_initialized = true;
		Validate(WSAStartup(MAKEWORD(2, 2), &wsa_data));
	}

}
void NetworkConnector::Validate(bool error)
{
	if (error)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
		WSACleanup();
		abort();
	}
}
bool NetworkConnector::Recv(SOCKET socket, std::string* msg, int flags)
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
bool NetworkConnector::SendRequest(SOCKET socket, const std::string request, int flags)
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
	if (send(socket, buffer, NetworkConnector::Constants::buffer_size, 0))
	{
		MessageBox(0, ErrorMsg::connection.c_str(), ErrorTitle::disconnect.c_str(), 0);
		return false;
	}
	return true;
}


#include "NetworkConnectorClient.h"

bool NetworkConnectorClient::Recv(SOCKET socket, char* buffer, int len, const int flags)
{
	if ((recv(socket, buffer, len, flags) < 0) <= 0)
	{
		MessageBox(0, NetworkConnector::ErrorMsg::connection.c_str(), NetworkConnector::ErrorTitle::disconnect.c_str(), 0);
		return false;
	}
	return true;
}

std::string NetworkConnectorClient::GetIPFromHostID(std::string host_id)
{
	for (int i = host_id.size() - 1; i >= 0; --i)
	{
		if (host_id[i] == ' ')
		{
			return std::string(host_id.begin() + i + 1, host_id.end());
		}
	}
	return std::string();
}

bool NetworkConnectorClient::Connect(const std::string host_id)
{
	const std::string ip = GetIPFromHostID(host_id);
	host = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in address = {};
	address.sin_family = AF_INET;
	address.sin_port = htons(NetworkConnector::Constants::port_number);
	address.sin_addr.s_addr = address.sin_addr.s_addr = inet_addr(ip.c_str());

	if (host == INVALID_SOCKET)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
		abort();
	}
	if (connect(host, reinterpret_cast<sockaddr*>(&address), sizeof(address)))
	{
		MessageBox(0, NetworkConnector::ErrorMsg::connection.c_str(), NetworkConnector::ErrorTitle::disconnect.c_str(), 0);
		return false;
	}
	return true;
}


NetworkConnectorClient::NetworkConnectorClient()
{
	if (!NetworkConnector::network_initialized)
	{
		WSAData wsa_data;
		WSAStartup(MAKEWORD(2, 2), &wsa_data);
		NetworkConnector::network_initialized = true;
	}
}

void NetworkConnectorClient::SendRequest(const std::string request)
{
	char buffer[NetworkConnector::Constants::buffer_size];
	size_t request_size = request.size();
	if (request_size > NetworkConnector::Constants::buffer_size && host == INVALID_SOCKET)
	{
		abort();
	}
	for (size_t i = 0; i < request_size; ++i)
	{
		buffer[i] = request[i];
	}
	send(host, buffer, NetworkConnector::Constants::buffer_size, 0);
}

bool NetworkConnectorClient::GetConnectionFlag()
{
	return connected;
}

std::string NetworkConnectorClient::GetResponse()
{
	char buffer[NetworkConnector::Constants::buffer_size];
	if (host == INVALID_SOCKET)
	{
		abort();
	}
	if (!Recv(host, buffer, NetworkConnector::Constants::buffer_size, 0))
	{
		//TODO disconnect
	}
	
	return std::string(buffer);
}

void NetworkConnectorClient::CloseAllConnections()
{
	broadcast_receiver.Stop();
	shutdown(host, SD_BOTH);
	closesocket(host);
}

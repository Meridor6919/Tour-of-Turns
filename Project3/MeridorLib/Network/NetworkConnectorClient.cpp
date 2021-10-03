#include "NetworkConnectorClient.h"


std::string NetworkConnectorClient::GetIPFromHostID(std::string host_id)
{
	for (size_t i = host_id.size(); i > 0; --i)
	{
		if (host_id[i - 1] == ' ')
		{
			return std::string(host_id.begin() + i, host_id.end());
		}
	}
	MessageBox(0, NetworkConnector::ErrorMsg::ip_extraction.c_str(), NetworkConnector::ErrorTitle::ip_extraction.c_str(), MB_TOPMOST);
	abort();
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
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), MB_TOPMOST);
		abort();
	}
	if (connect(host, reinterpret_cast<sockaddr*>(&address), sizeof(address)))
	{
		MessageBox(0, NetworkConnector::ErrorMsg::connection.c_str(), NetworkConnector::ErrorTitle::disconnect.c_str(), MB_TOPMOST);
		return false;
	}
	return true;
}
NetworkConnectorClient::NetworkConnectorClient()
{
	NetworkConnector::Initialize();
}
bool NetworkConnectorClient::GetConnectionFlag()
{
	return connected;
}
void NetworkConnectorClient::SendRequest(std::string request)
{
	if (!NetworkConnector::SendRequest(host, request))
	{
		closesocket(host);
		connected = false;
	}
}
std::string NetworkConnectorClient::GetResponse()
{
	std::string response;
	if (!NetworkConnector::Recv(host, &response))
	{
		closesocket(host);
		connected = false;
	}
	return response;
}
void NetworkConnectorClient::CloseAllConnections()
{
	broadcast_receiver.Stop();
	shutdown(host, SD_BOTH);
	closesocket(host);
}

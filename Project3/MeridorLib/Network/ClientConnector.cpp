#include "ClientConnector.h"

namespace MeridorNetwork
{
	std::string ConnectorClient::GetIPFromHostID(std::string host_id)
	{
		for (size_t i = host_id.size(); i > 0; --i)
		{
			if (host_id[i - 1] == ' ')
			{
				return std::string(host_id.begin() + i, host_id.end());
			}
		}
		MessageBoxA(0, MeridorNetwork::ErrorMsg::ip_extraction.c_str(), MeridorNetwork::ErrorTitle::ip_extraction.c_str(), MB_TOPMOST);
		abort();
	}
	bool ConnectorClient::Connect(const std::string host_id)
	{
		const std::string ip = GetIPFromHostID(host_id);
		host = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in address = {};
		address.sin_family = AF_INET;
		address.sin_port = htons(MeridorNetwork::Constants::port_number);
		address.sin_addr.s_addr = address.sin_addr.s_addr = inet_addr(ip.c_str());

		if (host == INVALID_SOCKET)
		{
			MessageBoxA(0, std::to_string(WSAGetLastError()).c_str(), MeridorNetwork::ErrorTitle::winsock.c_str(), MB_TOPMOST);
			abort();
		}
		if (connect(host, reinterpret_cast<sockaddr*>(&address), sizeof(address)))
		{
			MessageBoxA(0, MeridorNetwork::ErrorMsg::connection.c_str(), MeridorNetwork::ErrorTitle::disconnect.c_str(), MB_TOPMOST);
			return false;
		}
		return true;
	}
	ConnectorClient::ConnectorClient()
	{
		MeridorNetwork::Initialize();
	}
	bool ConnectorClient::GetConnectionFlag()
	{
		return connected;
	}
	void ConnectorClient::SendRequest(std::string request)
	{
		if (!MeridorNetwork::SendRequest(host, request))
		{
			closesocket(host);
			connected = false;
		}
	}
	std::string ConnectorClient::GetResponse()
	{
		std::string response;
		if (!MeridorNetwork::Recv(host, &response))
		{
			closesocket(host);
			connected = false;
		}
		return response;
	}
	void ConnectorClient::CloseAllConnections()
	{
		broadcast_receiver.Stop();
		shutdown(host, SD_BOTH);
		closesocket(host);
	}
}
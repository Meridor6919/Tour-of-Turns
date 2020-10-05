#include "NetworkConnectorClient.h"

bool NetworkConnectorClient::Recv(SOCKET socket, char* buffer, int len, const int flags)
{
	if ((recv(socket, buffer, len, flags) < 0) || (((std::string)buffer).size() > len) || buffer[0] == '\0')
	{
		MessageBox(0, NetworkConnector::ErrorMsg::connection.c_str(), NetworkConnector::ErrorTitle::disconnect.c_str(), 0);
		return false;
	}
	return true;
}

void NetworkConnectorClient::Connect(const std::string ip)
{
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
		abort();
	}
}

void NetworkConnectorClient::BroadcastSearch(bool hamachi)
{
	//UDP protocol to broadcast messages to all addresses in local network and virtual local network if flag is set
	SOCKET intercept_brodcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in addr;
	int addr_size = sizeof(addr);
	memset(&addr, 0, addr_size);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(NetworkConnector::Constants::port_number);
	addr.sin_addr.s_addr = ADDR_ANY;
	broadcast_active = true;
	const char* ms_delay = std::to_string(NetworkConnector::Constants::ms_delay).c_str();

	if (intercept_brodcast_socket < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
		abort();
	}
	if (setsockopt(intercept_brodcast_socket, SOL_SOCKET, SO_RCVTIMEO, ms_delay, sizeof(ms_delay)) < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
		abort();
	}
	if (bind(intercept_brodcast_socket, reinterpret_cast<sockaddr*>(&addr), addr_size))
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
		abort();
	}
	char host_name_buffer[50];
	while (broadcast_active)
	{
		int recv_result = recvfrom(intercept_brodcast_socket, host_name_buffer, 50, 0, reinterpret_cast<sockaddr*>(&addr), &addr_size);
		in_addr temp_addr;
		hostent* hostent = gethostbyname(host_name_buffer);

		if (hostent != nullptr && recv_result != -1)
		{
			for (int i = 0; hostent->h_addr_list[i] != 0; ++i)
			{
				memcpy(&temp_addr, hostent->h_addr_list[i], sizeof(struct in_addr));
			}
			std::string intercepted_host = static_cast<std::string>(host_name_buffer) + " - " + static_cast<std::string>(inet_ntoa(temp_addr));

			network_mutex.lock();
			bool finded = false;
			for (size_t i = 0; i < hosts_broadcasting.size(); ++i)
			{
				if (hosts_broadcasting[i] == intercepted_host)
				{
					finded = true;
					break;
				}
			}
			if (!finded)
			{
				hosts_broadcasting.emplace_back(intercepted_host);
			}
			network_mutex.unlock();
		}
	}
	closesocket(intercept_brodcast_socket);
}
NetworkConnectorClient::NetworkConnectorClient()
{
	if (NetworkConnector::network_initialized)
	{
		MessageBox(0, NetworkConnector::ErrorMsg::initialization.c_str(), NetworkConnector::ErrorTitle::initialization.c_str(), 0);
	}
	else
	{
		WSAData wsa_data;
		WSAStartup(MAKEWORD(2, 2), &wsa_data);
		NetworkConnector::network_initialized = true;
	}	
}

void NetworkConnectorClient::StartLookingForHosts(bool hamachi)
{
	broadcast_searching = std::make_unique<std::thread>(&NetworkConnectorClient::BroadcastSearch, this, hamachi);
}

void NetworkConnectorClient::StopLookingForHosts()
{
	broadcast_active = false;
	if (broadcast_searching->joinable())
	{
		broadcast_searching->join();
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
	Recv(host, buffer, NetworkConnector::Constants::buffer_size, 0);
	
	return std::string(buffer);
}

std::vector<std::string> NetworkConnectorClient::GetHostsBroadcasting()
{
	network_mutex.lock();
	const std::vector<std::string>& ret = hosts_broadcasting;
	network_mutex.unlock();
	return ret;
}

void NetworkConnectorClient::ResetHostsBroadcastingVector()
{
	network_mutex.lock();
	hosts_broadcasting.clear();
	network_mutex.unlock();
	std::chrono::milliseconds ms(NetworkConnector::Constants::ms_delay);
	std::this_thread::sleep_for(ms);
}

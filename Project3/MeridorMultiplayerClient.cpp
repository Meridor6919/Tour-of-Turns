#include "MeridorMultiplayerClient.h"

MeridorMultiplayer::Client::Client(SOCKET *host)
{
	this->host = host;
}
bool MeridorMultiplayer::Client::RecvBroadcast(const int max_hosts, int ms_interval)
{
	//UDP protocol to broadcast messages to all addresses in local network and virtual local network if flag is set
	SOCKET intercept_brodcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in addr;
	int addr_size = sizeof(addr);
	memset(&addr, 0, addr_size);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Constants::port_number);
	addr.sin_addr.s_addr = ADDR_ANY;
	receiving_broadcast = true;

	if (intercept_brodcast_socket < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
		return false;
	}
	//Reducing time that program will wait for messages
	if (setsockopt(intercept_brodcast_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&ms_interval), sizeof(ms_interval)) < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
		return false;
	}
	if (bind(intercept_brodcast_socket, reinterpret_cast<sockaddr*>(&addr), addr_size))
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
		return false;
	}
	char host_name_buffer[50];
	while (receiving_broadcast)
	{
		int recv_result = recvfrom(intercept_brodcast_socket, host_name_buffer, 50, 0, reinterpret_cast<sockaddr*>(&addr), &addr_size);
		in_addr temp_addr;
		hostent *hostent = gethostbyname(host_name_buffer);

		if (hostent != nullptr && recv_result != -1)
		{
			//Getting ip address from hostname
			for (int i = 0; hostent->h_addr_list[i] != 0; ++i)
			{
				memcpy(&temp_addr, hostent->h_addr_list[i], sizeof(struct in_addr));
			}
			if (current_hosts.size() <= max_hosts)
			{
				std::string value = static_cast<std::string>(host_name_buffer) + " - " + static_cast<std::string>(inet_ntoa(temp_addr));
				
				bool finded = false;
				//Refreshing time of known addresses
				for (std::map<int, std::string>::iterator it = current_hosts.begin(); it != current_hosts.end(); ++it)
				{
					if (it->second == value)
					{
						auto new_record = current_hosts.find(it->first);
						if (new_record != current_hosts.end())
						{
							std::swap(current_hosts[static_cast<int>(clock() + (ms_interval)*1.5f)], new_record->second);
							current_hosts.erase(new_record);
							finded = true;
							break;
						}
					}
				}
				//Adding new addresses
				if (!(finded))
				{
					current_hosts.insert(std::make_pair(clock() + (ms_interval)*1.5, value));
				}
			}
		}
		//Erasing outdated addresses
		std::map<int, std::string>::iterator it = current_hosts.begin();
		while (it != current_hosts.end())
		{
			if (it->first < clock())
				current_hosts.erase(it++);
		}
	}
	closesocket(intercept_brodcast_socket);
	return true;
}
bool MeridorMultiplayer::Client::Connect(const std::string ip)
{
	*host = socket(AF_INET, SOCK_STREAM, 0);
	int addr_size = sizeof(addr);
	memset(&addr, 0, addr_size);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6919);
	addr.sin_addr.s_addr = addr.sin_addr.s_addr = inet_addr(ip.c_str());

	if (*host == INVALID_SOCKET)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
		return false;
	}
	if (connect(*host, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)))
	{
		MessageBox(0, ErrorMsg::connection.c_str(), ErrorTitle::connection.c_str(), 0);
		return false; 
	}
	char temp[6] = "";
	if (!Recv(*host, temp, 0) || temp != "start")
	{
		return false;
	}
	return true;
}
std::string MeridorMultiplayer::Client::GetIpFromMapValue(const std::string value)
{
	return value.substr(value.find_last_of(" ") + 1, value.size() - value.find_last_of(" ") - 1);
}
std::vector<std::string> MeridorMultiplayer::Client::GetCurrentHosts()
{
	std::vector<std::string> ret = {};
	for (auto it = current_hosts.begin(); it != current_hosts.end(); ++it)
	{
		ret.emplace_back(it->second);
	}
	return ret;
}
bool MeridorMultiplayer::Client::Recv(SOCKET socket, char * buffer, const int flags)
{
	if ((recv(socket, buffer, Constants::buffer_size, flags) < 0) || (((std::string)buffer).size() > Constants::buffer_size) || buffer[0] == '\0')
	{
		MessageBox(0, "", ErrorTitle::disconnect.c_str(), 0);
		return false;
	}
	return true;
}

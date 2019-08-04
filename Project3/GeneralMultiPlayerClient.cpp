#include "GeneralMultiPlayer.h"

GeneralMultiPlayer::Client::Client(SOCKET *host)
{
	this->host = host;
	port = 6919;
}
bool GeneralMultiPlayer::Client::RecvBroadcast(int max_hosts, int ms_interval)
{
	receiving_broadcast = true;

	//UDP protocol to broadcast messages to all addresses in local network and virtual local network if flag is set
	SOCKET intercept_brodcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in addr;
	int addr_size = sizeof(addr);
	memset(&addr, 0, addr_size);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = ADDR_ANY;

	if (intercept_brodcast_socket < 0)
	{
		MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		return false;
	}
	//Reducing time that program will wait for messages
	if (setsockopt(intercept_brodcast_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&ms_interval), sizeof(ms_interval)) < 0)
	{
		MessageBox(0, ("Socket option error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		return false;
	}
	if (bind(intercept_brodcast_socket, (sockaddr*)&addr, addr_size))
	{
		MessageBox(0, ("Binding error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		return false;
	}

	char host_name_buffer[50];
	while (receiving_broadcast)
	{
		int recv_result = recvfrom(intercept_brodcast_socket, host_name_buffer, 50, 0, (sockaddr *)&addr, &addr_size);
		in_addr temp_addr;
		hostent *hostent = gethostbyname(host_name_buffer);

		if (hostent != nullptr && recv_result != -1)
		{
			//Getting ip address from hostname
			for (int i = 0; hostent->h_addr_list[i] != 0; ++i)
				memcpy(&temp_addr, hostent->h_addr_list[i], sizeof(struct in_addr));

			if (current_hosts.size() <= max_hosts)
			{
				std::string value = (std::string)host_name_buffer + " - " + (std::string)inet_ntoa(temp_addr);
				
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
bool GeneralMultiPlayer::Client::Connect(std::string ip)
{
	*host = socket(AF_INET, SOCK_STREAM, 0);
	int addr_size = sizeof(addr);
	memset(&addr, 0, addr_size);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6919);
	addr.sin_addr.s_addr = addr.sin_addr.s_addr = inet_addr(ip.c_str());

	if (*host == INVALID_SOCKET)
	{
		MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		return false;
	}
	if (connect(*host, (sockaddr *)&addr, sizeof(addr)))
	{
		int result = WSAGetLastError();
		if (result == 10061)
		{
			MessageBox(0, "Target stopped hosting", "Error", 0);
			return false;
		}
		MessageBox(0, ("Connection error" + std::to_string(result)).c_str(), "Error", 0);
		return false;
	}
	//Waiting until host is ready for handling connection
	char temp[6] = "";
	while ((std::string)temp != (std::string)"start")
	{
		if (!GeneralMultiPlayer::Recv(*host, temp, 6, 0))
		{
			return false;
			break;
		}
	}
	return true;
}

#include "GeneralMultiPlayer.h"

GeneralMultiPlayer::Client::Client()
{
	port = 6919;
}
void GeneralMultiPlayer::Client::RecvBroadcast(int max_hosts, int ms_interval)
{
	receiving_broadcast = true;	//started broadcasting

	//socket in udp protocole that will recive broadcast msgs from all possible addresses on given port
	SOCKET intercept_brodcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in addr;
	int addr_size = sizeof(addr);
	memset(&addr, 0, addr_size);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = ADDR_ANY;

	//checking socket
	if (intercept_brodcast_socket < 0)
	{
		MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		throw 1;
		return;
	}
	//setting socket's recv time to given value
	if (setsockopt(intercept_brodcast_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&ms_interval), sizeof(ms_interval)) < 0)
	{
		MessageBox(0, ("Socket option error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		throw 2;
		return;
	}
	//binding
	if (bind(intercept_brodcast_socket, (sockaddr*)&addr, addr_size))
	{
		MessageBox(0, ("Binding error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		throw 4;
		return;
	}
	//buffer that will contain host's name
	char recv_buffer[50];

	//main loop
	while (receiving_broadcast)
	{
		//reciving host's name
		int recived = recvfrom(intercept_brodcast_socket, recv_buffer, 50, 0, (sockaddr *)&addr, &addr_size);

		in_addr temp_addr;
		hostent *hostent = gethostbyname(recv_buffer);

		//if valid msg was recived
		if (hostent != nullptr && recived != -1)
		{
			//getting ip address from host's name
			for (int i = 0; hostent->h_addr_list[i] != 0; ++i)
				memcpy(&temp_addr, hostent->h_addr_list[i], sizeof(struct in_addr));

			//chcking size of name_ip map;
			if (name_ip.size() <= max_hosts)
			{
				//value contains ip and name with " - " between them
				std::string value = (std::string)recv_buffer + " - " + (std::string)inet_ntoa(temp_addr);
				//searching for record with the same name and ip
				bool finded = false;
				for (std::map<int, std::string>::iterator it = name_ip.begin(); it != name_ip.end(); ++it)
				{
					//if it has been finded - refresh map key that indicates time
					if (it->second == value)
					{
						auto new_record = name_ip.find(it->first);
						if (new_record != name_ip.end())
						{
							std::swap(name_ip[clock() + (ms_interval)*1.5], new_record->second);
							name_ip.erase(new_record);
							finded = true;
							break;
						}
					}
				}
				//else - add new record
				if (!(finded))
				{
					name_ip.insert(std::make_pair(clock() + (ms_interval)*1.5, value));
				}
			}
		}
		//checking if records are still valid
		else
		{
			std::map<int, std::string>::iterator it = name_ip.begin();
			while (it != name_ip.end())
			{
				if (it->first < clock())
					name_ip.erase(it++);
				else
					break;
			}
		}
		//reducing speed for optimalization purposes 
		std::chrono::milliseconds ms(ms_interval);
		std::this_thread::sleep_for(ms);
	}
	closesocket(intercept_brodcast_socket);
}
void GeneralMultiPlayer::Client::Connect(std::string ip, int recv_time)
{
	//setting socket for direct connection with host
	host = socket(AF_INET, SOCK_STREAM, 0);
	int addr_size = sizeof(addr);
	memset(&addr, 0, addr_size);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6919);
	addr.sin_addr.s_addr = addr.sin_addr.s_addr = inet_addr(ip.c_str());

	//checking socket
	if (host == INVALID_SOCKET)
	{
		MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
		throw 1;
		return;
	}

	//connect to host
	if (connect(host, (sockaddr *)&addr, sizeof(addr)))
	{
		int result = WSAGetLastError();
		if (result == 10061)
		{
			MessageBox(0, "Target stopped hosting", "Error", 0);
			throw 10;
			return;
		}
		MessageBox(0, ("Connection error" + std::to_string(result)).c_str(), "Error", 0);
		throw 3;
		return;
	}
	//waiting until the host is ready
	char temp[6] = "";
	while ((std::string)temp != (std::string)"start")
	{
		if (!GeneralMultiPlayer::Recv(host, temp, 6, 0))
		{
			throw 10;
			break;
		}
	}
}

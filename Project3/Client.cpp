#include "NetworkRole.h"
#include <set>

Client::Client(Window &main_window) : SinglePlayer(main_window)
{
	this->main_window = &main_window;
	StartNetwork();
}
void Client::StartNetwork()
{
	std::set<std::string> name_ip;
	std::chrono::milliseconds ms(100);
	bool thread_active = true;

	//recv hosts from local network
	std::thread thread([&]() {

		int recv_time = 100;
		SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (sock == INVALID_SOCKET)
		{
			MessageBox(0, "Socket error", "Error", 0);
			WSACleanup();
			closesocket(sock);
			exit(0);
		}
		if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&recv_time), sizeof(recv_time)) < 0)
		{
			MessageBox(0, "Socket option error", "Error", 0);
			WSACleanup();
			closesocket(sock);
			exit(0);
		}

		sockaddr_in addr;
		int addr_size = sizeof(addr);
		memset(&addr, 0, addr_size);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(6919);	//port number
		addr.sin_addr.s_addr = INADDR_ANY;

		if (bind(sock, (sockaddr*)&addr, addr_size))
		{
			MessageBox(0, "binding error", "Error", 0);
			WSACleanup();
			closesocket(sock);
			exit(0);
		}

		char recv_buffer[50];

		while (thread_active)
		{

			recvfrom(sock, recv_buffer, 50, 0, (sockaddr *)&addr, &addr_size);

			in_addr addr2;
			hostent *hostent = gethostbyname(recv_buffer);
			if (hostent != nullptr)
			{
				for (int i = 0; hostent->h_addr_list[i] != 0; ++i)
					memcpy(&addr2, hostent->h_addr_list[i], sizeof(struct in_addr));
			
				name_ip.insert((std::string)recv_buffer + (std::string)inet_ntoa(addr2));
			}			
			std::this_thread::sleep_for(ms);
		}
	});
	std::cin.get();


	//changed local vertical choose

	thread_active = false;
	thread.join();
}
void Client::GetTourNames(std::vector<std::string>&tours)
{

}
void Client::GetCarNames(std::vector<std::string>&cars, std::string tour)
{

}
void Client::GetTireNames(std::vector<std::string>&tires)
{

}
std::vector<int> Client::GetCarParameters(std::string path)
{
	std::vector<int> temp;
	return temp;
}
std::vector<std::string> Client::GetTireParameters(std::string path)
{
	std::vector<std::string> temp;
	return temp;
}
void Client::GetTourParameters(std::string path)
{

}
void Client::GetOtherParticipants(std::vector<Participant*> &participants, int ais)
{

}
void Client::GetRankingInfo(std::vector<std::string>&names, std::vector<int>&scores)
{

}
void Client::GetInfobox(InfoBox *infobox)
{

}
void Client::GetCurrentAtribs(float &durability, float current_speed)
{

}
void Client::GetTargetList(std::vector<std::string>&names)
{

}
void Client::SendInfo(std::string special_text, std::string text)
{

}
std::pair<int, int> Client::TakeAction()
{
	return std::make_pair(1, 4);
}
void Client::SendTarget(int ranking_position)
{

}
int Client::Possible_AIs()
{
	return 0;
}
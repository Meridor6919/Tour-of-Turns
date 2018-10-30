#include "NetworkRole.h"

Host::Host(Window &main_window) : SinglePlayer(main_window)
{
	this->main_window = &main_window;
	if(!StartNetwork())
	{
		for(int i = 0; i < clients.size(); i++)
			closesocket(clients[i]);
		throw 1;
	}
}
bool Host::StartNetwork()
{
	bool threads_active = true;

	std::thread broadcast([&]() {

		std::chrono::seconds s(1);
		char broadcast = 'a';
		SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (sock == INVALID_SOCKET)
		{
			MessageBox(0, "Socket hostname error", "Error", 0);
			WSACleanup();
			exit(0);
		}
		if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
		{
			MessageBox(0, "Socket broadcast error", "Error", 0);
			closesocket(sock);
			WSACleanup();
			exit(0);
		}
		sockaddr_in sock_addr;
		memset(&sock_addr, 0, sizeof(sock_addr));
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_port = htons(6919);
		sock_addr.sin_addr.s_addr = inet_addr("192.168.x.x");
		if (main_window->GetHamachiConnectionFlag())
			sock_addr.sin_addr.s_addr |= inet_addr("25.x.x.x");
		
		char HostName[50];

		if (gethostname(HostName, sizeof(HostName)) == SOCKET_ERROR)
		{
			MessageBox(0, "Socket hostname error", "Error", 0);
			WSACleanup();
			closesocket(sock);
			exit(0);
		}
		while (threads_active)
		{
			if (sendto(sock, HostName, sizeof(HostName), 0, (sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
			{
				MessageBox(0, "Socket send error", "Error", 0);
				WSACleanup();
				closesocket(sock);
				exit(0);
			}
			std::this_thread::sleep_for(s);
		}
	});
	std::thread accepting_clients([&]() {
	
		std::chrono::milliseconds ms(100);
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			MessageBox(0, "Socket error", "Error", 0);
			WSACleanup();
			exit(0);
		}

		sockaddr_in sock_addr;
			memset(&sock_addr, 0, sizeof(sock_addr));
			sock_addr.sin_family = AF_INET;
			sock_addr.sin_port = htons(6919);
			sock_addr.sin_addr.s_addr = INADDR_ANY;

		int addr_size = sizeof(struct sockaddr_in);

		if (bind(sock, (sockaddr *)& sock, sizeof(sockaddr)))
		{
			MessageBox(0, "Binding error", "Error", 0);
			WSACleanup();
			exit(0);
		}
		while (threads_active)
		{
			clients.push_back(accept(sock, (struct sockaddr *) & sock_addr, &addr_size));
			std::this_thread::sleep_for(ms);
		}
		closesocket(sock);
			
	
	});
	std::vector<std::string> lobby_options = { "Start game", "Kick player", "Back" };
	int pos = 0;

	pos = Text::Choose::Veritcal(lobby_options, pos, { (short)main_window->GetWidth() / 2, 15 }, 3, Text::center, true, *main_window);
	switch (pos)
	{
		case 0: // start game
		{
			threads_active = false;
			broadcast.join();
			return true;
		}
		case 1: // kick players
		{

		}
		case 2: //back
		{
			threads_active = false;
			broadcast.join();
			return false;
		}
	}
}
void Host::GetOtherParticipants(std::vector<Participant*> &participants, int ais)
{

}
void Host::GetRankingInfo(std::vector<std::string>&names, std::vector<int>&scores)
{

}
void Host::GetInfobox(InfoBox *infobox)
{

}
void Host::GetCurrentAtribs(float &durability, float current_speed)
{

}
void Host::GetTargetList(std::vector<std::string>&names)
{

}
void Host::SendInfo(std::string special_text, std::string text)
{

}
std::pair<int, int> Host::TakeAction()
{
	return std::make_pair(1, 4);
}
void Host::SendTarget(int ranking_position)
{

}
int Host::Possible_AIs()
{
	return 7 - (int)clients.size();
}
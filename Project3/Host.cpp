#include "NetworkRole.h"

Host::Host(Window &main_window, std::vector<Participant*> *participants) : SinglePlayer(main_window)
{
	this->main_window = &main_window;
	if(!StartNetwork(participants)) //if player will decide to go back throw the exception, and close all sockets (constructor issue) 
	{
		for (int i =0; i < clients.size(); i++)
		{
			closesocket(clients[i].first);
		}
		throw 1;
	}
}
bool Host::StartNetwork(std::vector<Participant*> *participants)
{
	bool threads_active = true;
	HANDLE handle = main_window->GetHandle();
	

	std::thread broadcast([&]() {	//sends broadcast msg to all other machines in local network (+hamachi if option is active) with hostname

		SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (sock < 0)
		{
			MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
			WSACleanup();
			exit(0);
		}

		char broadcast = 'a';
		if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(int)) < 0)
		{
			MessageBox(0, ("Socket option error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
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
		
		char host_name[50];

		if (gethostname(host_name, sizeof(host_name)) < 0)
		{
			MessageBox(0, ("Gethostname error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
			WSACleanup();
			exit(0);
		}
		while (threads_active)
		{
			if (sendto(sock, host_name, sizeof(host_name), 0, (sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
			{
				MessageBox(0, ("Sending error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
				WSACleanup();
				exit(0);
			}
			std::chrono::seconds s(1);
			std::this_thread::sleep_for(s);
		}
		closesocket(sock);
	});
	std::thread accepting_clients([&]() {
	
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

		if (sock < 0)
		{
			MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
			WSACleanup();
			exit(0);
		}
		sockaddr_in sock_addr;
			memset(&sock_addr, 0, sizeof(sock_addr));
			sock_addr.sin_family = AF_INET;
			sock_addr.sin_port = htons(6919);
			sock_addr.sin_addr.s_addr = INADDR_ANY;

		int addr_size = sizeof(sockaddr_in);

		if (bind(sock, (sockaddr *)& sock_addr, sizeof(sockaddr)) < 0)
		{
			MessageBox(0, ("Binding error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
			WSACleanup();
			exit(0);
		}

		listen(sock, 7);

		while (threads_active)
		{
			std::chrono::milliseconds ms(100);
			std::this_thread::sleep_for(ms);
			SOCKET temp = accept(sock, (sockaddr *) & sock_addr, &addr_size);
			if (temp != INVALID_SOCKET && threads_active)
			{
				bool equal = false;

				for (int i = 0; i < clients.size(); i++)
				{
					if (sock_addr.sin_addr.s_addr == clients[i].second.sin_addr.s_addr)
					{
						equal = true;
						closesocket(temp);
					}
				}
				
				if (!equal)
				{
					clients.push_back(std::make_pair(temp, sock_addr));

					SetConsoleCursorPosition(handle, { 0, 18 + 2 * (short)clients.size() });
					SetConsoleTextAttribute(handle, main_window->color2);

					in_addr ip_addr = sock_addr.sin_addr;
					char helper[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, &ip_addr, helper, INET_ADDRSTRLEN);
					std::cout << helper;
				}
			}
		}
		closesocket(sock);
		
	});
	
	SetConsoleCursorPosition(handle, { 0, 18 });
	SetConsoleTextAttribute(handle, main_window->color1);
	std::cout << "Players in lobby";

	while (true)
	{
		std::vector<std::string> lobby_options = { "Start game", "Kick player", "Back" };
		int pos = 0;

		pos = Text::Choose::Veritcal(lobby_options, pos, { (short)main_window->GetWidth() / 2, 15 }, 3, Text::center, true, *main_window);

		switch (pos)
		{
			case 0: // start game
			{
				SOCKET temp = socket(AF_INET, SOCK_STREAM, 0);
				threads_active = false;

				if (temp < 0)
				{
					MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
					broadcast.join();
					accepting_clients.join();
					WSACleanup();
					exit(0);
				}
				sockaddr_in SocketAddress;
				memset(&SocketAddress, 0, sizeof(SocketAddress));
				SocketAddress.sin_family = AF_INET;
				SocketAddress.sin_port = htons(6919);
				SocketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

				connect(temp, (sockaddr *)&SocketAddress, sizeof(SocketAddress));	//connecting with themself to proc accept function that will wait forever otherwise
				closesocket(temp);

				broadcast.join();
				accepting_clients.join();
				network_device = new MultiplayerDevice(participants, &clients, this, 0);
				return true;
			}
			case 1: // kick players
			{
				std::vector<std::string> text; //for vertical choose

				for (int i = 0; i < clients.size(); i++)
				{
					in_addr ip_addr = clients[i].second.sin_addr;
					char helper[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, &ip_addr, helper, INET_ADDRSTRLEN);
					text.push_back(helper);
				}
				text.push_back("back");

				while (true)
				{
					int kicked_player = Text::Choose::Veritcal(text, 0, { (short)main_window->GetWidth() / 2, 15 }, 3, Text::center, true, *main_window);
					if (kicked_player != text.size() - 1) //if host kicked somebody
					{
						for (int i = 0; i < clients.size(); i++)
						{
							SetConsoleCursorPosition(handle, { 0, 18 + 2 * (short)clients.size() });
							std::cout << "                ";
						}

						text.erase(text.begin() + kicked_player);
						clients.erase(clients.begin() + kicked_player);

						for (int i = 0; i < static_cast<int>(text.size()) - 1; i++)
						{
							SetConsoleCursorPosition(handle, { 0, 18 + 2 * (short)clients.size() });
							SetConsoleTextAttribute(handle, main_window->color2);
							std::cout << text[i];
						}
					}
					else
						break;
				}
				break;
			}
			case 2: //back
			{
				SOCKET temp = socket(AF_INET, SOCK_STREAM, 0);
				threads_active = false;

				if (temp < 0)
				{
					MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
					broadcast.join();
					accepting_clients.join();
					WSACleanup();
					exit(0);
				}
				sockaddr_in SocketAddress;
				memset(&SocketAddress, 0, sizeof(SocketAddress));
				SocketAddress.sin_family = AF_INET;
				SocketAddress.sin_port = htons(6919);
				SocketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

				connect(temp, (sockaddr *)&SocketAddress, sizeof(SocketAddress));	//connecting with themself to proc accept function that will wait forever otherwise
				closesocket(temp);
				broadcast.join();
				accepting_clients.join();
				return false;
			}
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
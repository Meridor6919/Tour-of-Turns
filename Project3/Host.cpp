#include "NetworkRole.h"

Host::Host(Window &main_window) : SinglePlayer(main_window)
{
	this->main_window = &main_window;
	if(!StartNetwork())
	{
		for(auto it = clients.begin(); it != clients.end(); ++it)
			closesocket(*it);
		throw 1;
	}
}
bool Host::StartNetwork()
{
	bool threads_active = true;
	HANDLE handle = main_window->GetHandle();
	std::vector<std::pair<SOCKET, sockaddr_in>> clients_in_queue;

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

		int addr_size = sizeof(sockaddr_in);

		if (bind(sock, (sockaddr *)& sock_addr, sizeof(sockaddr)))
		{
			MessageBox(0, "Binding error", "Error", 0);
			WSACleanup();
			exit(0);
		}
		listen(sock, 7);
		while (threads_active)
		{

			SOCKET temp = accept(sock, (sockaddr *) & sock_addr, &addr_size);
			if (temp != INVALID_SOCKET && threads_active)
			{
				int i = clients.size();
				clients.insert(temp);
				clients_in_queue.push_back(std::make_pair(temp, sock_addr));
				if (i != clients.size())
				{
					SetConsoleCursorPosition(handle, { 0, 18 + 2*(short)clients.size() });
					SetConsoleTextAttribute(handle, main_window->color2);

					in_addr ip_addr = sock_addr.sin_addr;
					char helper[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, &ip_addr, helper, INET_ADDRSTRLEN);
					std::cout << helper;
				}
			}

			std::this_thread::sleep_for(ms);
		}
		closesocket(sock);
			
	
	});
	
	SetConsoleCursorPosition(handle, { 0, 18 });
	SetConsoleTextAttribute(handle, main_window->color1);
	std::cout << "Players in lobby";

	std::vector<std::string> lobby_options = { "Start game", "Kick player", "Back" };
	int pos = 0;
	while (true)
	{
		pos = Text::Choose::Veritcal(lobby_options, pos, { (short)main_window->GetWidth() / 2, 15 }, 3, Text::center, true, *main_window);

		switch (pos)
		{
			case 0: // start game
			{
				threads_active = false;
				broadcast.join();
				accepting_clients.join();
				return true;
			}
			case 1: // kick players
			{
				std::vector<std::string> text;
				for (int i = 0; i < clients_in_queue.size(); i++)
				{
					in_addr ip_addr = clients_in_queue[i].second.sin_addr;
					char helper[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, &ip_addr, helper, INET_ADDRSTRLEN);
					text.push_back(helper);
				}
				text.push_back("back");

				int kicked_player;
				while (true)
				{
					kicked_player = Text::Choose::Veritcal(text, 0, { (short)main_window->GetWidth() / 2, 15 }, 3, Text::center, true, *main_window);
					if (kicked_player != text.size() - 1)
					{
						for (auto it = clients.begin(); it != clients.end(); ++it)
						{
							SetConsoleCursorPosition(handle, { 0, 18 + 2 * (short)clients.size() });
							std::cout << "                ";
						}

						clients.erase(clients_in_queue[kicked_player].first);
						text.erase(text.begin() + kicked_player);
						clients_in_queue.erase(clients_in_queue.begin() + kicked_player);

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
				if (temp == INVALID_SOCKET)
				{
					std::cout << "error: " << WSAGetLastError();
					MessageBox(0, "Socket error", "Error", 0);
					WSACleanup();
					broadcast.join();
					accepting_clients.join();
					exit(0);
				}
				sockaddr_in SocketAddress;
				memset(&SocketAddress, 0, sizeof(SocketAddress));
				SocketAddress.sin_family = AF_INET;
				SocketAddress.sin_port = htons(6919);
				SocketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

				if (connect(temp, (sockaddr *)&SocketAddress, sizeof(SocketAddress)))
				{
					MessageBox(0, "Chosen user stopped hosting", "Error", 0);
					WSACleanup();
					threads_active = false;
					broadcast.join();
					accepting_clients.join();
					exit(0);
				}
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
#include "NetworkRole.h"

Host::Host(ToT_Window &main_window, std::vector<Participant*> *participants) : SinglePlayer(main_window)
{
	this->main_window = &main_window;
	this->infobox = new InfoBox(10, Text::TextAlign::left, { 0,56 }, 1, main_window);
	stage = 0;
	if(!StartNetwork(participants)) //if player will decide to go back throw the exception, and close all sockets (constructor issue) 
	{
		for (int i =0; i < (*clients).size(); i++)
		{
			closesocket((*clients)[i].first);
		}
		throw 1;
	}
}
bool Host::StartNetwork(std::vector<Participant*> *participants)
{
	HANDLE handle = main_window->GetHandle();
	bool showing_clients = true;
	host = new GeneralMultiPlayer::Host();

	unsigned long addr_range = inet_addr("192.168.x.x");

	if (main_window->GetHamachiConnectionFlag())
		addr_range |= inet_addr("25.x.x.x");

	std::thread broadcast(&GeneralMultiPlayer::Host::Broadcast, host, addr_range, 200);
	std::thread accept_clients(&GeneralMultiPlayer::Host::AcceptClients, host, 8);
	std::thread show_clients([&]() {
		std::chrono::milliseconds ms(100);
		std::vector<std::pair<SOCKET, sockaddr_in>> clients;
		while (showing_clients)
		{
			std::this_thread::sleep_for(ms);
			clients = *host->GetClientsPtr();
			for (int i = 0; i < clients.size(); i++)
			{
				SetConsoleCursorPosition(handle, { 0, 25 + 2 * static_cast<short>(i) });
				SetConsoleTextAttribute(handle, main_window->color2);
				char h[30];
				getnameinfo((sockaddr *)(&clients[i].second), sizeof(clients[i].second), h, sizeof(h), NULL, NULL,0);

				std::string clientname = (std::string)h + " - " + host->GetIP(clients[i].second);

				for (int j = 0; j < 50; j++)
				{
					if (clientname.size() > j)
						std::cout << clientname[j];
					else
						std::cout << " ";
				}	
			}
			SetConsoleCursorPosition(handle, { 0, 25 + 2 * static_cast<short>(clients.size()) });
			SetConsoleTextAttribute(handle, main_window->color2);
			std::cout << "                                                 ";
		}
		for (short i = 0; i < static_cast<short>(clients.size()); i++)
		{
			SetConsoleCursorPosition(handle, { 0, 25 + 2 * i });
			SetConsoleTextAttribute(handle, main_window->color2);
			std::cout << "                                                 ";
		}
	});
	
	SetConsoleCursorPosition(handle, { 0, 23 });
	SetConsoleTextAttribute(handle, main_window->color1);
	std::cout << "Players in lobby";

	while (true)
	{
		std::vector<std::string> lobby_options = { "Start game", "Kick player", "Back" };
		int pos = 0;

		pos = Text::Choose::Veritcal(lobby_options, pos, { (short)main_window->GetWidth() / 2, 20 }, 3, Text::center, true, *main_window);
		clients = host->GetClientsPtr();

		switch (pos)
		{
			case 0: // start game
			{
				host->StopBroadcasting();
				host->StopAcceptingClients();
				showing_clients = false;
				show_clients.join();
				broadcast.join();
				accept_clients.join();
				
				network_device = new MultiplayerDevice(participants, clients, this, stage);

				for (short i = 0; i < static_cast<int>((*clients).size()+1); i++)
				{
					SetConsoleCursorPosition(handle, { 0, 23 + 2 * i });
					std::cout << "                    ";
				}
				return true;
			}
			case 1: // kick players
			{
				std::vector<std::string> text; //for vertical choose

				for (int i = 0; i < (*clients).size(); i++)
				{
					in_addr ip_addr = (*clients)[i].second.sin_addr;
					char helper[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, &ip_addr, helper, INET_ADDRSTRLEN);
					text.push_back(helper);
				}
				text.push_back("back");

				while (true)
				{
					int kicked_player = Text::Choose::Veritcal(text, 0, { (short)main_window->GetWidth() / 2, 20 }, 3, Text::center, true, *main_window);
					if (kicked_player != text.size() - 1) //if host kicked somebody
					{
						for (int i = 0; i < (*clients).size(); i++)
						{
							SetConsoleCursorPosition(handle, { 0, 23 + 2 * (short)(*clients).size() });
							std::cout << "                ";
						}

						std::vector<sockaddr_in> *black_list = host->GetBlackListPtr();
						black_list->push_back(((*clients).begin() + kicked_player)->second);

						text.erase(text.begin() + kicked_player);
						(*clients).erase((*clients).begin() + kicked_player);

						for (int i = 0; i < static_cast<int>(text.size()) - 1; i++)
						{
							SetConsoleCursorPosition(handle, { 0, 23 + 2 * (short)(*clients).size() });
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
				host->StopBroadcasting();
				host->StopAcceptingClients();
				showing_clients = false;
				show_clients.join();
				broadcast.join();
				accept_clients.join();
				return false;
			}
		}
	}
	
}
void Host::GetOtherParticipants(std::vector<Participant*> &participants, int ais, std::string tour)
{
	if(network_device != nullptr)
		network_device->HandleClientConnection(tour);

	while (true)
	{
		bool breaking = true;

		for (int i = 0; i < participants.size(); i++)
		{
			if (participants[i] == nullptr)
				breaking = false;
		}
		if (breaking)
			break;

		main_window->Pause(500);
	}
	SinglePlayer::GetOtherParticipants(participants, ais, tour);
	stage = 3;
}

std::vector<std::pair<float, std::string>> Host::GetRankingInfo(std::vector<Participant*> &participants)
{
	return SinglePlayer::GetRankingInfo(participants);
}
bool Host::GetCurrentAtribs(std::vector<Participant*> &participants, int ais, std::string field)
{
	stage = 3;
	return SinglePlayer::GetCurrentAtribs(participants, ais - (*clients).size(), field);
}
void Host::Attack(std::vector<Participant*> &participants, int ais, bool alive)
{
	network_device->ClientsReadyForNewStage();
	stage = 1;
	SinglePlayer::Attack(participants, ais, alive);
}
void Host::TakeAction(Participant* &participants)
{
	SinglePlayer::TakeAction(participants);
}
void Host::GetOthersAction(std::vector<Participant*>& participants, int ais, std::vector<std::string>& tour)
{
	stage = 2;
	network_device->ClientsReadyForNewStage();
	SinglePlayer::GetOthersAction(participants, ais, tour);
	network_device->ClientsReadyForNewStage();
}
int Host::Possible_AIs()
{
	return 7 - (int)(*clients).size();
}
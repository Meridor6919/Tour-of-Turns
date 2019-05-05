#include "NetworkRole.h"

Host::Host(ToT_Window &main_window, std::vector<Participant*> *participants) : SinglePlayer(main_window, participants)
{
	this->participants = participants;
	this->main_window = &main_window;
	this->infobox = new InfoBox(10, Text::TextAlign::left, { 0,56 }, 1, main_window);
	stage = 0;
	if(!StartNetwork()) //if player will decide to go back throw the exception, and close all sockets (constructor issue) 
	{
		for (int i =0; i < (*clients).size(); i++)
		{
			closesocket((*clients)[i].first);
		}
		throw 1;
	}
}
bool Host::StartNetwork()
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
void Host::MsgHandling(std::string msg, int client_id)
{
	
	int code = atoi(msg.substr(0, 2).c_str());
	msg = msg.substr(2, msg.size() - 2);

	//first two chars describe key code and next will represent some sort of value
	switch (code)
	{
	case 0://get tour
	{
		send((*clients)[client_id].first, tour.c_str(), tour.size() + 1, 0);
		break;
	}
	case 1://get cars
	{
		std::vector<std::string> cars;
		GetCarNames(cars, tour);

		for (int i = 0; i < cars.size(); i++)
			send((*clients)[client_id].first, cars[i].c_str(), cars[i].size() + 1, 0);

		send((*clients)[client_id].first, "exit", 5, 0);
		break;
	}
	case 2://get tires
	{
		std::vector<std::string> tires;
		GetTireNames(tires);

		for (int i = 0; i < tires.size(); i++)
			send((*clients)[client_id].first, tires[i].c_str(), tires[i].size() + 1, 0);

		send((*clients)[client_id].first, "exit", 5, 0);
		break;
	}
	case 3://get tour params
	{
		std::vector<std::string> ret = GetTourParameters(tour);

		for (int i = 0; i < ret.size(); i++)
			send((*clients)[client_id].first, ret[i].c_str(), ret[i].size() + 1, 0);

		send((*clients)[client_id].first, "exit", 5, 0);
		break;
	}
	case 4://get car params
	{
		std::vector<int> car_params;
		car_params = GetCarParameters(msg);

		for (int i = 0; i < car_params.size(); i++)
			send((*clients)[client_id].first, std::to_string(car_params[i]).c_str(), car_params.size()+1, 0);
		break;
	}
	case 5://get tires params
	{
		std::vector<std::string> tires_params;
		tires_params = GetTireParameters(msg);

		for (int i = 0; i < tires_params.size(); i++)
			send((*clients)[client_id].first, tires_params[i].c_str(), tires_params[i].size()+1, 0);
		break;
	}
	case 6://get ranking info 
	{
		std::vector<std::pair<float, std::string>> ranking = GetRankingInfo();

		for (int i = 0; i < ranking.size(); i++)
		{
			send((*clients)[client_id].first, std::to_string(ranking[i].first).c_str(), std::to_string(ranking[i].first).size()+1, 0);
			send((*clients)[client_id].first, ranking[i].second.c_str(), ranking[i].second.size()+1, 0);
		}

		send((*clients)[client_id].first, "exit", 5, 0);
		break;
	}
	case 7://get attack 
	{
		std::vector<std::string> rival_name;
		std::vector<int> rival_id;

		for (int i = 0; i < (*participants).size(); i++)
		{
			if (i == client_id + 1)
				continue;

			if ((*participants)[i]->score < (*participants)[client_id + 1]->score + 5 && (*participants)[i]->score >(*participants)[client_id + 1]->score - 5)
			{
				send((*clients)[client_id].first, (*participants)[i]->name.c_str(), (*participants)[i]->name.size()+1, 0);
				send((*clients)[client_id].first, std::to_string(i).c_str(), std::to_string(i).size()+1, 0);
			}
		}
		send((*clients)[client_id].first, "exit", 5, 0);
	}
	/*
	case 63://get atribs
	{
		if ((*clients).size() < client_id + 1)
		{
			send((*clients_sockets)[client_id].first, std::to_string((*clients)[client_id + 1]->current_speed).c_str(), 255, 0);
			send((*clients_sockets)[client_id].first, std::to_string((*clients)[client_id + 1]->current_durability).c_str(), 255, 0);
			send((*clients_sockets)[client_id].first, std::to_string((*clients)[client_id + 1]->score).c_str(), 255, 0);
		}
		else
		{
			send((*clients_sockets)[client_id].first, std::to_string(0).c_str(), 255, 0);
			send((*clients_sockets)[client_id].first, std::to_string(0).c_str(), 255, 0);
			closesocket((*clients_sockets)[client_id].first);
			(*clients_sockets).erase((*clients_sockets).begin() + client_id);
			break;
		}


		for (int i = 0; i < (*clients)[0]->network_role->infobox->info.size(); i++)
		{
			send((*clients_sockets)[client_id].first, (*clients)[0]->network_role->infobox->info[i].substr(0, (*clients)[0]->network_role->infobox->info[i].find("  ")).c_str(), 255, 0);
			send((*clients_sockets)[client_id].first, (*clients)[0]->network_role->infobox->info[i].substr((*clients)[0]->network_role->infobox->info[i].find("  ") + 2, (*clients)[0]->network_role->infobox->info[i].size() - (*clients)[0]->network_role->infobox->info[i].find("  ")).c_str(), 255, 0);
		}
		send((*clients_sockets)[client_id].first, "exit", 255, 0);

		client_current_game_stage[client_id] = 3;

		break;
	}
	case 59://receving client participant info 
	{
		std::string info[3];
		char temp[255];

		for (int i = 0; i < 3; i++)
		{
			if (!recv((*clients_sockets)[client_id].first, temp, 255, 0) < 0)
				MessageBox(0, "GetTireNames method failed", "Error", 0);

			info[i] = (std::string)temp;
		}
		(*clients)[client_id + 1] = new Participant(info[0], info[1], info[2], *host);
		break;
	}
	case 70://speed up
	{
		while (*current_stage != 2)
		{
			std::chrono::milliseconds ms(100);
			std::this_thread::sleep_for(ms);
		}

		int value = atoi(message.substr(2, message.size() - 2).c_str());
		if ((*clients)[client_id + 1]->car_modifiers[CarModifiers::max_accelerating] >= value)
		{
			(*clients)[client_id + 1]->current_speed += value;
			if ((*clients)[client_id + 1]->current_speed > (*clients)[client_id + 1]->car_modifiers[CarModifiers::max_speed])
				(*clients)[client_id + 1]->current_speed = (*clients)[client_id + 1]->car_modifiers[CarModifiers::max_speed];
			(*clients)[client_id + 1]->current_speed = (*clients)[client_id + 1]->current_speed*0.9f;
		}

		client_current_game_stage[client_id] = 2;
		break;
	}
	case 71://braking
	{
		while (*current_stage != 2)
		{
			std::chrono::milliseconds ms(100);
			std::this_thread::sleep_for(ms);
		}

		int value = atoi(message.substr(2, message.size() - 2).c_str());
		if ((*clients)[client_id]->car_modifiers[CarModifiers::max_braking] >= value)
		{
			(*clients)[client_id + 1]->current_speed += value;
			if ((*clients)[client_id + 1]->current_speed < 0)
				(*clients)[client_id + 1]->current_speed = 0;
			(*clients)[client_id]->current_speed = (*clients)[client_id + 1]->current_speed*0.9f;
		}

		client_current_game_stage[client_id] = 2;
		break;
	}
	case 72://hand braking
	{
		while (*current_stage != 2)
		{
			std::chrono::milliseconds ms(100);
			std::this_thread::sleep_for(ms);
		}
		if ((*clients)[client_id + 1]->current_speed > 0)
		{
			if ((*clients)[client_id + 1]->current_speed > 40)
				(*clients)[client_id + 1]->drift = true;
			(*clients)[client_id + 1]->current_speed -= static_cast<float>((*clients)[client_id + 1]->car_modifiers[CarModifiers::hand_brake_value]);
			if ((*clients)[client_id + 1]->current_speed < 0)
				(*clients)[client_id + 1]->current_speed = 0.0f;
			(*clients)[client_id + 1]->current_speed = (*clients)[client_id + 1]->current_speed*0.9f;
		}

		client_current_game_stage[client_id] = 2;
		break;
	}
	case 73://do nothing
	{
		while (*current_stage != 2)
		{
			std::chrono::milliseconds ms(100);
			std::this_thread::sleep_for(ms);
		}
		if ((*clients)[client_id + 1]->current_speed > 0)
			(*clients)[client_id + 1]->current_speed = (*clients)[client_id + 1]->current_speed*0.9f;

		client_current_game_stage[client_id] = 2;
		break;
	}
	case 74://abaddon race
	{
		while (*current_stage != 2)
		{
			std::chrono::milliseconds ms(100);
			std::this_thread::sleep_for(ms);
		}

		(*clients)[client_id + 1]->current_durability = 0.0f;

		client_current_game_stage[client_id] = 2;

	}*/
	}
}
void Host::GetOtherParticipants(int ais, std::string tour)
{
	this->tour = tour;
	for (int i = 0; i < clients->size(); i++)
		(*participants).push_back(nullptr);
	host->HandleConnection<Host>(&Host::MsgHandling, this);

	while (true)
	{
		bool breaking = true;

		for (int i = 0; i < (*participants).size(); i++)
		{
			if ((*participants)[i] == nullptr)
				breaking = false;
		}
		if (breaking)
			break;

		main_window->Pause(500);
	}
	SinglePlayer::GetOtherParticipants(ais, tour);
	stage = 3;
}
std::vector<std::pair<float, std::string>> Host::GetRankingInfo()
{
	return SinglePlayer::GetRankingInfo();
}
bool Host::GetCurrentAtribs(int ais, std::string field)
{
	stage = 3;
	return SinglePlayer::GetCurrentAtribs(ais - (*clients).size(), field);
}
void Host::Attack(int ais, bool alive)
{
	network_device->ClientsReadyForNewStage();
	stage = 1;
	SinglePlayer::Attack(ais, alive);
}
void Host::TakeAction()
{
	SinglePlayer::TakeAction();
}
void Host::GetOthersAction(int ais, std::vector<std::string>& tour)
{
	stage = 2;
	network_device->ClientsReadyForNewStage();
	SinglePlayer::GetOthersAction(ais, tour);
	network_device->ClientsReadyForNewStage();
}
int Host::Possible_AIs()
{
	return 7 - (int)(*clients).size();
}
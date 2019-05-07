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
				request_handler = new GeneralMultiPlayer::RequestHandler(clients->size());
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
	if (msg.size() < 2)
		return;

	int code = atoi(msg.substr(0, 2).c_str());
	msg = msg.substr(2, msg.size() - 2);
	char buffer[254];

	//first two chars describe key code and next will represent some sort of value
	switch (code)
	{
	case 1://get tour
	{
		strcpy(buffer, tour.c_str());
		send((*clients)[client_id].first, buffer, 254, 0);
		break;
	}
	case 2://get cars
	{
		std::vector<std::string> cars;
		GetCarNames(cars, tour);

		for (int i = 0; i < cars.size(); i++)
		{
			strcpy(buffer, cars[i].c_str());
			send((*clients)[client_id].first, buffer, 254, 0);
		}
		send((*clients)[client_id].first, "exit", 5, 0);
		break;
	}
	case 3://get tires
	{
		std::vector<std::string> tires;
		GetTireNames(tires);

		for (int i = 0; i < tires.size(); i++)
		{
			strcpy(buffer, tires[i].c_str());
			send((*clients)[client_id].first, buffer,254, 0);
		}
		send((*clients)[client_id].first, "exit", 5, 0);
		break;
	}
	case 4://get tour params
	{
		std::vector<std::string> ret = GetTourParameters(tour);

		for (int i = 0; i < ret.size(); i++)
		{
			strcpy(buffer, ret[i].c_str());
			send((*clients)[client_id].first,buffer, 254, 0);
		}

		send((*clients)[client_id].first, "exit", 5, 0);
		break;
	}
	case 5://get car params
	{
		std::vector<int> car_params;
		car_params = GetCarParameters(msg);

		for (int i = 0; i < car_params.size(); i++)
		{
			strcpy(buffer, std::to_string(car_params[i]).c_str());
			send((*clients)[client_id].first, buffer, 254, 0);
		}
		break;
	}
	case 6://get tires params
	{
		std::vector<std::string> tires_params;
		tires_params = GetTireParameters(msg);

		for (int i = 0; i < tires_params.size(); i++)
		{
			strcpy(buffer, tires_params[i].c_str());
			send((*clients)[client_id].first, buffer, 254, 0);
		}
		break;
	}
	case 7://get ranking info 
	{
		std::vector<std::pair<float, std::string>> ranking = GetRankingInfo();

		for (int i = 0; i < ranking.size(); i++)
		{
			strcpy(buffer, std::to_string(ranking[i].first).c_str());
			send((*clients)[client_id].first, buffer, 254, 0);
			strcpy(buffer, ranking[i].second.c_str());
			send((*clients)[client_id].first, buffer, 254, 0);
		}

		send((*clients)[client_id].first, "exit", 5, 0);
		break;
	}
	case 8://get attack targets
	{
		std::vector<std::string> rival_name;
		std::vector<int> rival_id;

		for (int i = 0; i < (*participants).size(); i++)
		{
			if (i == client_id + 1)
				continue;

			if ((*participants)[i]->score < (*participants)[client_id + 1]->score + 5 && (*participants)[i]->score >(*participants)[client_id + 1]->score - 5 && (*participants)[i]->alive)
			{
				strcpy(buffer, (*participants)[i]->name.c_str());
				send((*clients)[client_id].first, buffer, 254, 0);
				strcpy(buffer, std::to_string(i).c_str());
				send((*clients)[client_id].first, buffer, 254, 0);
			}
		}
		send((*clients)[client_id].first, "exit", 5, 0);
		break;
	}
	case 9://get atribs
	{
		strcpy(buffer, std::to_string((*participants)[client_id + 1]->current_speed).c_str());
		send((*clients)[client_id].first, buffer, 254, 0);
		strcpy(buffer, std::to_string((*participants)[client_id + 1]->current_durability).c_str());
		send((*clients)[client_id].first, buffer,254, 0);
		strcpy(buffer, std::to_string((*participants)[client_id + 1]->score).c_str());
		send((*clients)[client_id].first, buffer, 254, 0);

		for (int i = 0; i < infobox->info.size(); i++)
		{
			strcpy(buffer, (*participants)[0]->network_role->infobox->info[i].substr(0, (*participants)[0]->network_role->infobox->info[i].find("  ")).c_str());
			send((*clients)[client_id].first, buffer, 254, 0);
			strcpy(buffer, (*participants)[0]->network_role->infobox->info[i].substr((*participants)[0]->network_role->infobox->info[i].find("  ") + 2, (*participants)[0]->network_role->infobox->info[i].size() - (*participants)[0]->network_role->infobox->info[i].find("  ")).c_str());
			send((*clients)[client_id].first, buffer, 254, 0);
		}
		send((*clients)[client_id].first, "exit", 255, 0);
		break;
	}
	case 10://get current game stage
	{

		strcpy(buffer, std::to_string(stage).c_str());
		send((*clients)[client_id].first, buffer, 254, 0);
		break;
	}
	default:
	{
		request_handler->SaveMsg(std::to_string(code) + msg, client_id);
		break;
	}
	}
}
void Host::GetOtherParticipants(int ais, std::string tour)
{
	this->tour = tour;
	host->HandleConnection<Host>(&Host::MsgHandling, this);

	for (int i = clients->size() - 1; i >= 0; i--)
	{
		std::vector<std::string>* msgs;
		std::string name = "";
		std::string tires_path = "";
		std::string car_path = "";
		bool error = false; 
		std::chrono::milliseconds ms(20);

		while (((name == "") || (tires_path == "") || (car_path == "")) && !error)
		{
			msgs = request_handler->GetMsgsPtr(i);
			
			for (int j = msgs->size()-1; j >= 0 ; j--)
			{
				int code = atoi((*msgs)[j].substr(0, 2).c_str());
				if (code == 51)
				{
					name = (*msgs)[j].substr(2, (*msgs)[j].size() - 2);
					msgs->erase(msgs->begin()+j);
				}
				else if (code == 52)
				{
					car_path = (*msgs)[j].substr(2, (*msgs)[j].size() - 2);
					msgs->erase(msgs->begin() + j);
				}
				else if (code == 53)
				{
					tires_path = (*msgs)[j].substr(2, (*msgs)[j].size() - 2);
					msgs->erase(msgs->begin() + j);
				}
			}
			std::this_thread::sleep_for(ms);
		}
		(*participants).push_back(new Participant(name, car_path, tires_path, *this));

	}
	SinglePlayer::GetOtherParticipants(ais, tour);
}
std::vector<std::pair<float, std::string>> Host::GetRankingInfo()
{
	stage = 1;
	return SinglePlayer::GetRankingInfo();
}
bool Host::GetCurrentAtribs(int ais, std::string field)
{
	stage = 0;
	return SinglePlayer::GetCurrentAtribs(ais - (*clients).size(), field);
}
void Host::Attack(int ais)
{
	SinglePlayer::Attack(ais);
}
void Host::TakeAction()
{
	SinglePlayer::TakeAction();
}
void Host::GetOthersAction(int ais, std::vector<std::string>& tour)
{
	SinglePlayer::GetOthersAction(ais, tour);
}
int Host::Possible_AIs()
{
	return 7 - (int)(*clients).size();
}
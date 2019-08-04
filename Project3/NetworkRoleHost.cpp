#include "NetworkRole.h"

Host::Host(ToT_Window &main_window) : SinglePlayer(main_window)
{
	this->participants = participants;
	this->main_window = &main_window;
	COORD infobox_position = { 0,static_cast<short>(main_window.GetHeight() - 12) };
	this->infobox = std::make_shared<InfoBox>(10, Text::TextAlign::left, infobox_position, 1, main_window);
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
Host::~Host()
{
	//TODO disconnects
	std::vector<std::string>* msgs;
	std::chrono::milliseconds ms(20);

	stage++;
	for (int i = clients->size() - 1; i >= 0; i--)
	{
		bool done = false;
		while (!done)
		{
			msgs = request_handler->GetMsgsPtr(i);
			for (int j = msgs->size() - 1; j >= 0; j--)
			{
				int code = atoi((*msgs)[j].substr(0, 2).c_str());
				if (code == 60)
				{
					send((*clients)[i].first, "exit", 254, 0);
					done = true;
				}
			}
			std::this_thread::sleep_for(ms);
		}
	}
	host->CloseActiveConnections();
}
bool Host::StartNetwork()
{
	//TODO use mutexes
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

				std::string clientname = (std::string)h + " - " + host->GetThisIp(clients[i].second);

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
	
	SetConsoleCursorPosition(handle, { 0, 22 });
	SetConsoleTextAttribute(handle, main_window->color1);
	std::cout << "Players in lobby";

	while (true)
	{
		std::vector<std::string> lobby_options = { "Start game", "Kick player", "Back" };
		int pos = 0;

		pos = Text::Choose::Veritcal(lobby_options, pos, { (short)main_window->GetWidth() / 2, 25 }, 3, Text::center, true, *main_window);
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
					SetConsoleCursorPosition(handle, { 0, 22 + 2 * i });
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
					short kicked_player = Text::Choose::Veritcal(text, 0, { (short)main_window->GetWidth() / 2, 25 }, 3, Text::center, true, *main_window);
					if (kicked_player != text.size() - 1) //if host kicked somebody
					{
						for (int i = 0; i < (*clients).size(); i++)
						{
							SetConsoleCursorPosition(handle, { 0, 22 + 2 * (short)(*clients).size() });
							std::cout << "                ";
						}

						std::vector<sockaddr_in> *black_list = host->GetBlackListPtr();
						black_list->push_back(((*clients).begin() + kicked_player)->second);

						text.erase(text.begin() + kicked_player);
						(*clients).erase((*clients).begin() + kicked_player);

						for (int i = 0; i < static_cast<int>(text.size()) - 1; i++)
						{
							SetConsoleCursorPosition(handle, { 0, 22 + 2 * (short)(*clients).size() });
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
				for (short i = 0; i < static_cast<int>((*clients).size() + 1); i++)
				{
					SetConsoleCursorPosition(handle, { 0, 22 + 2 * i });
					std::cout << "                    ";
				}
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
		std::vector<std::string> cars = GetCarNames(tour);

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
		std::vector<std::string> tires = GetTireNames();

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
		std::vector<std::string> ret = GetTourParameters(0, INT_MAX);

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
		std::vector<std::pair<float, std::string>> ranking = GetRankingInfo(current_field);

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
		send((*clients)[client_id].first, "Don't attack", 254, 0);
		send((*clients)[client_id].first, "10", 254, 0);
		for (int i = 0; i < participants.size(); i++)
		{
			if (i == client_id + 1)
				continue;

			if (participants[i].score < participants[client_id + 1].score + 5 && participants[i].score >participants[client_id + 1].score - 5 && participants[i].alive)
			{
				strcpy(buffer, participants[i].name.c_str());
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
		strcpy(buffer, std::to_string(participants[client_id + 1].current_speed).c_str());
		send((*clients)[client_id].first, buffer, 254, 0);
		strcpy(buffer, std::to_string(participants[client_id + 1].current_durability).c_str());
		send((*clients)[client_id].first, buffer,254, 0);
		strcpy(buffer, std::to_string(participants[client_id + 1].score).c_str());
		send((*clients)[client_id].first, buffer, 254, 0);

		for (int i = 0; i < infobox->info.size(); i++)
		{
			std::string info = participants[0].network_role->infobox->info[i];

			strcpy(buffer, info.substr(0, info.find("  ")).c_str());
			send((*clients)[client_id].first, buffer, 254, 0);

			strcpy(buffer, info.substr(participants[0].network_role->infobox->info[i].find("  ") + 2, info.size() - info.find("  ")).c_str());
			send((*clients)[client_id].first, buffer, 254, 0);
		}
		send((*clients)[client_id].first, "exit", 254, 0);
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
void Host::GetParticipants(std::string name, int ais, std::string tour, std::string car, std::string tire)
{
	//TODO validate this
	this->tour = tour;
	this->ais = ais;
	host->HandleConnection<Host>(&Host::MsgHandling, this);
	std::vector<std::string>* msgs;
	std::chrono::milliseconds ms(20);

	participants.push_back(Participant(name, car, tire, *this));

	for (int i = clients->size() - 1; i >= 0; i--)
	{
		std::string name = "";
		std::string tires_path = "";
		std::string car_path = "";	

		while (((name == "") || (tires_path == "") || (car_path == "")))
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
		participants.push_back(Participant(name, car_path, tires_path, *this));
	}
	for (int i = 0; i < ais; i++)
		participants.emplace_back(Participant(this, tour));
	stage =1;
}
std::vector<std::pair<float, std::string>> Host::GetRankingInfo(std::string current_field)
{
	return SinglePlayer::GetRankingInfo();
}
bool Host::GetCurrentAtribs()
{
	bool ret = SinglePlayer::GetCurrentAtribs();
	stage++;
	return ret;
}
void Host::Attack()
{
	SinglePlayer::Attack();

	std::vector<std::string>* msgs;
	std::chrono::milliseconds ms(20);

	for (int i = clients->size() - 1; i >= 0; i--)
	{
		if (participants[i+1].current_durability <= 0)
		{
			continue;
		}

		bool attacked = false;

		while (!attacked)
		{
			msgs = request_handler->GetMsgsPtr(i);
			for (int j = msgs->size() - 1; j >= 0; j--)
			{
				int code = atoi((*msgs)[j].substr(0, 2).c_str());
				if (code == 54)
				{
					int target = atoi((*msgs)[j].substr(2, (*msgs)[j].size() - 2).c_str());
					if (target < participants.size())
					{
						participants[target].attacked++;
					}
					msgs->erase(msgs->begin() + j);
					attacked = true;
				}
			}
			std::this_thread::sleep_for(ms);
		}
	}
}
void Host::TakeAction()
{
	SinglePlayer::TakeAction();
}
void Host::GetOthersAction(std::vector<std::string>& tour)
{
	//TODO basic anticheat
	std::vector<std::string>* msgs;
	std::chrono::milliseconds ms(20);

	for (int i = clients->size() - 1; i >= 0; i--)
	{
		if (participants[i+1].current_durability <= 0)
		{
			continue;
		}

		bool action = false;

		while (!action)
		{
			msgs = request_handler->GetMsgsPtr(i);
			for (int j = msgs->size() - 1; j >= 0; j--)
			{
				int code = atoi((*msgs)[j].substr(0, 2).c_str());

				if (code == 55)
				{
					switch ((*msgs)[j][2])
					{
					case '0':
					{
						participants[i + 1].current_speed += atoi((*msgs)[j].substr(3, (*msgs)[j].size() - 3).c_str());
						if (participants[i + 1].current_speed > static_cast<float>(participants[i + 1].car_modifiers[CarModifiers::max_speed]))
							participants[i + 1].current_speed = static_cast<float>(participants[i + 1].car_modifiers[CarModifiers::max_speed]);
						participants[i + 1].current_speed = participants[i + 1].current_speed*0.9f;
						msgs->erase(msgs->begin() + j);
						action = true;
						break;
					}
					case '1':
					{
						participants[i + 1].current_speed -= atoi((*msgs)[j].substr(3, (*msgs)[j].size() - 3).c_str());
						if (participants[i + 1].current_speed < 0)
							participants[i + 1].current_speed = 0;
						participants[i+1].current_speed = participants[i+1].current_speed*0.9f;
						msgs->erase(msgs->begin() + j);
						action = true;
						break;
					}
					case '2':
					case '3':
					case '4':
					{
						if ((*msgs)[j][2] == '4')
						{
							participants[i+1].current_durability = 0;
							action = true;
							msgs->erase(msgs->begin() + j);
							break;
						}

						if (participants[i+1].current_speed > 0)
						{
							if ((*msgs)[j][2] == '2')
							{
								if (participants[i+1].current_speed > 40)
									participants[i+1].drift = true;
								participants[i+1].current_speed -= static_cast<float>(participants[i+1].car_modifiers[CarModifiers::hand_brake_value]);
								if (participants[i+1].current_speed < 0)
									participants[i+1].current_speed = 0.0f;
							}
							participants[i+1].current_speed = participants[i+1].current_speed*0.9f;
							action = true;
							msgs->erase(msgs->begin() + j);
							break;
						}
					}
					}
				}
			}
		}
	}
	SinglePlayer::GetOthersAction(0);
}
int Host::Possible_AIs()
{
	return 7 - (int)(*clients).size();
}
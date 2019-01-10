#include "MultiplayerDevice.h"
#include "NetworkRole.h"

MultiplayerDevice::MultiplayerDevice(std::vector<Participant*> *clients, std::vector<std::pair<SOCKET, sockaddr_in>> *clients_sockets, Host *host, int &current_stage)
{

	this->clients = clients;
	this->clients_sockets = clients_sockets;
	this->host = host;
	this->current_stage = &current_stage;
}
void MultiplayerDevice::HandleClientConnection(std::string tour)
{
	this->tour = tour;
	char buffer[254];
	std::thread *recv_threads;
	recv_threads = new std::thread[(int)(*clients_sockets).size()];

	for (int i = 0; i < (int)(*clients_sockets).size(); i++)
		send((*clients_sockets)[i].first, "start", 255, 0);

	for (int i = 0; i < clients_sockets->size(); i++)
		clients->push_back(nullptr);

	for (int i = 0; i < clients_sockets->size(); i++)
		client_current_game_stage.push_back(0);
	
	auto recv_function = [&](int i) {

		while (true)
		{
			auto h = recv((*clients_sockets)[i].first, buffer, 254, 0);
			ValidateClientAction((std::string)buffer, i);
		}
	};

	for (int i = 0; i < (int)(*clients_sockets).size(); i++)
		recv_threads[i] = std::thread(recv_function, i);

}
void MultiplayerDevice::ClientsReadyForNewStage()
{
	bool ready = true;
	std::chrono::milliseconds ms(100);
	do
	{
		ready = true;
		for (int i = 0; i < client_current_game_stage.size(); i++)
		{
			if (client_current_game_stage[i] != *current_stage)
				ready = false;
		}
		std::this_thread::sleep_for(ms);
	} while (!ready);
}
bool MultiplayerDevice::ValidateClientAction(std::string message, int client_id)
{

	int code = atoi(message.substr(0, 2).c_str());
	//first two chars describe key code and next will represent some sort of value
	switch (code)
	{
		case 50://get tour
		{
			send((*clients_sockets)[client_id].first, tour.c_str(), 255, 0);
			break;
		}
		case 51://get cars
		{
			std::vector<std::string> cars;
			host->GetCarNames(cars, tour);

			for (int i = 0; i < cars.size(); i++)
				send((*clients_sockets)[client_id].first, cars[i].c_str(), 255, 0);
		
			send((*clients_sockets)[client_id].first, "exit", 255, 0);
			break;
		}
		case 52://get tires
		{
			std::vector<std::string> tires;
			host->GetTireNames(tires);

			for (int i = 0; i < tires.size(); i++)
				send((*clients_sockets)[client_id].first, tires[i].c_str(), 255, 0);

			send((*clients_sockets)[client_id].first, "exit", 255, 0);
			break;
		}
		case 53://get tour params
		{
			std::vector<std::string> ret = host->GetTourParameters(tour);

			for (int i = 0; i < ret.size(); i++)
				send((*clients_sockets)[client_id].first, ret[i].c_str(), 255, 0);

			send((*clients_sockets)[client_id].first, "exit", 255, 0);
			break;
		}
		case 54://get car params
		{
			std::vector<int> car_params;
			car_params = host->GetCarParameters(message.substr(2, message.size() - 2));

			for (int i = 0; i < car_params.size(); i++)
				send((*clients_sockets)[client_id].first, std::to_string(car_params[i]).c_str(), 255, 0);
			break;
		}
		case 55://get tires params
		{
			std::vector<std::string> tires_params;
			tires_params = host->GetTireParameters(message.substr(2, message.size()-2));

			for (int i = 0; i < tires_params.size(); i++)
				send((*clients_sockets)[client_id].first, tires_params[i].c_str(), 255, 0);
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
		case 61://get ranking info 
		{
			while (*current_stage != 3)//stage 2 - action
			{
				std::chrono::milliseconds ms(100);
				std::this_thread::sleep_for(ms);
			}
			std::vector<std::pair<float, std::string>> ranking = (*clients)[0]->network_role->GetRankingInfo(*clients);

			for (int i = 0; i < ranking.size(); i++)
			{
				send((*clients_sockets)[client_id].first, std::to_string(ranking[i].first).c_str(), 255, 0);
				send((*clients_sockets)[client_id].first, ranking[i].second.c_str(), 255, 0);
			}

			send((*clients_sockets)[client_id].first, "exit", 255, 0);
			break;
		}
		case 62://get attack 
		{
			while (*current_stage != 1)//stage 1 - attack
			{
				std::chrono::milliseconds ms(100);
				std::this_thread::sleep_for(ms);
			}
			std::vector<std::string> rival_name;
			std::vector<int> rival_id;

			send((*clients_sockets)[client_id].first, "Don't attack", 255, 0);
			send((*clients_sockets)[client_id].first, "10", 255, 0);

			for (int i = 1; i < (*clients).size(); i++)
			{
				if ((*clients)[i]->score < (*clients)[0]->score + 5 && (*clients)[i]->score >(*clients)[0]->score - 5)
				{
					send((*clients_sockets)[client_id].first, (*clients)[i]->name.c_str(), 255, 0);
					send((*clients_sockets)[client_id].first, std::to_string(i).c_str(), 255, 0);
				}
			}
			send((*clients_sockets)[client_id].first, "exit", 255, 0);

			char temp[4];
			recv((*clients_sockets)[client_id].first, temp, 4, 0);
			(*clients)[atoi(static_cast<std::string>(temp).c_str())]->attacked++;

			client_current_game_stage[client_id] = 1;

			break;
		}
		case 63://get atribs
		{
			while (*current_stage != 3)//stage 3 - get atribs
			{
				std::chrono::milliseconds ms(100);
				std::this_thread::sleep_for(ms);
			}
			send((*clients_sockets)[client_id].first, std::to_string((*clients)[client_id + 1]->current_speed).c_str(), 255, 0);
			send((*clients_sockets)[client_id].first, std::to_string((*clients)[client_id + 1]->current_durability).c_str(), 255, 0);
			send((*clients_sockets)[client_id].first, std::to_string((*clients)[client_id + 1]->score).c_str(), 255, 0);

			for (int i = 0; i < (*clients)[0]->network_role->infobox->info.size(); i++)
			{
				send((*clients_sockets)[client_id].first, (*clients)[0]->network_role->infobox->info[i].substr(0, (*clients)[0]->network_role->infobox->info[i].find("  ")).c_str(), 255, 0);
				send((*clients_sockets)[client_id].first, (*clients)[0]->network_role->infobox->info[i].substr((*clients)[0]->network_role->infobox->info[i].find("  ")+2, (*clients)[0]->network_role->infobox->info[i].size() - (*clients)[0]->network_role->infobox->info[i].find("  ")).c_str(), 255, 0);
			}
			send((*clients_sockets)[client_id].first, "exit", 255, 0);

			client_current_game_stage[client_id] = 3;

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
			if ((*clients)[client_id+1]->car_modifiers[CarModifiers::max_accelerating] >= value)
			{
				(*clients)[client_id+1]->current_speed += value;
				if ((*clients)[client_id+1]->current_speed > (*clients)[client_id+1]->car_modifiers[CarModifiers::max_speed])
					(*clients)[client_id+1]->current_speed = (*clients)[client_id+1]->car_modifiers[CarModifiers::max_speed];
				(*clients)[client_id+1]->current_speed = (*clients)[client_id+1]->current_speed*0.9f;
			}
			else
				return false;

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
				(*clients)[client_id+1]->current_speed += value;
				if ((*clients)[client_id+1]->current_speed < 0)
					(*clients)[client_id+1]->current_speed = 0;
				(*clients)[client_id]->current_speed = (*clients)[client_id+1]->current_speed*0.9f;
			}
			else
				return false;

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
			if ((*clients)[client_id+1]->current_speed > 0)
			{
				if ((*clients)[client_id+1]->current_speed > 40)
					(*clients)[client_id+1]->drift = true;
				(*clients)[client_id+1]->current_speed -= static_cast<float>((*clients)[client_id+1]->car_modifiers[CarModifiers::hand_brake_value]);
				if ((*clients)[client_id+1]->current_speed < 0)
					(*clients)[client_id+1]->current_speed = 0.0f;
				(*clients)[client_id+1]->current_speed = (*clients)[client_id+1]->current_speed*0.9f;
			}
			else
				return false;

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
			if ((*clients)[client_id+1]->current_speed > 0)
				(*clients)[client_id+1]->current_speed = (*clients)[client_id+1]->current_speed*0.9f;
			else
				return false;

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
			
			(*clients)[client_id+1]->current_durability = 0.0f;
			
			return false;
		}
		default:
		{
			return false;
		}

	}
	return true;
}
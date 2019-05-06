#include "GeneralMultiPlayer.h"


GeneralMultiPlayer::RequestHandler::RequestHandler(int number_of_clients)
{
	this->number_of_clients = number_of_clients;
	for (int i = 0; i < number_of_clients; i++)
	{
		std::vector<std::string> temp;
		temp.clear();
		requests.push_back(temp);
	}
}

std::vector<std::string>* GeneralMultiPlayer::RequestHandler::GetMsgsPtr(int client_id)
{
	return &requests[client_id];
}

void GeneralMultiPlayer::RequestHandler::SaveMsg(std::string msg, int client_id)
{
	if (client_id < number_of_clients)
		requests[client_id].push_back(msg);
}
/*
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
#include "MultiplayerDevice.h"
#include "NetworkRole.h"

MultiplayerDevice::MultiplayerDevice(std::vector<Participant*> *clients, std::vector<std::pair<SOCKET, sockaddr_in>> *clients_sockets, Host *host, int current_stage)
{
	this->clients = clients;
	this->clients_sockets = clients_sockets;
	this->host = host;
	this->current_stage = current_stage;
}
void MultiplayerDevice::HandleClientConnection()
{
	char buffer[254];
	std::thread *recv_threads;
	recv_threads = new std::thread[clients->size()];
	
	auto recv_function = [&](int i) {

		while (true)
		{
			recv((*clients_sockets)[i].first, buffer, 254, 0);
			ValidateClientAction(buffer, i);
		}
	};

	for (int i = 0; i < clients->size(); i++)
		recv_threads[i] = std::thread(recv_function, i);
}
bool MultiplayerDevice::ClientsReadyForNewStage()
{
	bool ready = true;

	for (int i = 0; i < client_current_game_stage.size(); i++)
	{
		if (client_current_game_stage[i] != current_stage)
			ready = false;
	}
	return ready;
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
		}
		case 52://get tire
		{
			std::vector<std::string> tires;
			host->GetTireNames(tires);

			for (int i = 0; i < tires.size(); i++)
				send((*clients_sockets)[client_id].first, tires[i].c_str(), 255, 0);

			send((*clients_sockets)[client_id].first, "exit", 255, 0);
		}
		default:
		{
			return false;
		}

	}
}
#include "MultiplayerDevice.h"
#include "NetworkRole.h"

MultiplayerDevice::MultiplayerDevice(std::vector<Participant> *clients, std::vector<SOCKET> *clients_sockets, Host *host, int *current_stage)
{
	this->clients = clients;
	this->clients_sockets = clients_sockets;
	this->host = host;
	this->current_stage = current_stage;
}
void MultiplayerDevice::HandleClientConnection()
{
	//all things that will be working inside a thread for managing clients
}
bool MultiplayerDevice::ClientsReadyForNewStage()
{
	bool ready = true;

	for (int i = 0; i < client_current_game_stage.size(); i++)
	{
		if (client_current_game_stage[i] != *current_stage)
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
		//all cases of possible actions
		default:
		{
			return false;
		}

	}
}
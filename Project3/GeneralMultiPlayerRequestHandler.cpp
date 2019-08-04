#include "GeneralMultiPlayer.h"


GeneralMultiPlayer::RequestHandler::RequestHandler(const int number_of_clients)
{
	this->number_of_clients = number_of_clients;
	for (int i = 0; i < number_of_clients; ++i)
	{
		std::vector<std::string> temp;
		temp.clear();
		requests.push_back(temp);
	}
}

std::vector<std::string>* GeneralMultiPlayer::RequestHandler::GetMsgsPtr(const int client_id)
{
	return &requests[client_id];
}

void GeneralMultiPlayer::RequestHandler::SaveMsg(const std::string msg, const int client_id)
{
	if (client_id < number_of_clients)
	{
		requests[client_id].push_back(msg);
	}
}
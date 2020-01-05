#include "GeneralMultiPlayer.h"

std::vector<std::string>* GeneralMultiPlayer::RequestHandler::GetMsgsPtr(const int client_id)
{
	return &requests[client_id];
}

void GeneralMultiPlayer::RequestHandler::SaveMsg(const std::string msg, const int client_id)
{
	requests[client_id].push_back(msg);
}
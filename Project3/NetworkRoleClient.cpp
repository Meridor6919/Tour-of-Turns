#include "NetworkRoleClient.h"

Client::Client(ToT_Window &main_window) : SinglePlayer(main_window)
{
	this->participants = participants;
	this->main_window = &main_window;
	if (!StartNetwork())
	{
		closesocket(host);
		throw 1;
	}
}
bool Client::StartNetwork()
{
	return false;
}
void Client::ValidateAttack(int target, int participant)
{
}
void Client::ValidateAction(std::pair<int, int> action, int participant)
{
}
int Client::Possible_AIs()
{
	return 0;
}
void Client::GetParticipants(std::string name, std::string tour, std::string car, std::string tire)
{
}
void Client::GetCurrentAttributes()
{
}
std::string Client::GetTour()
{
	return std::string();
}
bool Client::GameLobby()
{
	return false;
}

void Client::AttackPhase()
{
}
void Client::ActionPhase()
{
}
void Client::Leaderboard(bool clear)
{
}
bool Client::VisionBox(int turn)
{
	return false;
}
void Client::Interface()
{
}
void Client::Finish()
{
}
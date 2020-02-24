#include "NetworkRoleHost.h"

Host::Host(ToT_Window &main_window) : SinglePlayer(main_window)
{
	this->main_window = &main_window;
	if (!StartNetwork()) //if player will decide to go back throw the exception
	{
		throw 1;
	}
}
bool Host::StartNetwork()
{
	return false;
}
void Host::HandleClientConnection(std::string msg, int client_id)
{
}
int Host::Possible_AIs()
{
	return 0;
}
void Host::GetParticipants(std::string name, std::string tour, std::string car, std::string tire)
{
}
void Host::GetCurrentAttributes()
{
}
bool Host::GameLobby()
{
	return false;
}
void Host::AttackPhase()
{
}
void Host::ActionPhase()
{
}
void Host::Leaderboard(bool clear)
{
}
bool Host::VisionBox(int turn)
{
	return false;
}
void Host::Interface()
{
}
void Host::Finish()
{
}

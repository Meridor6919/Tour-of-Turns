#include "NetworkRole.h"

Client::Client(Window &main_window) : SinglePlayer(main_window)
{
	this->main_window = &main_window;
}
void Client::StartNetwork()
{

}
void Client::GetTourNames(std::vector<std::string>&tours)
{

}
void Client::GetCarNames(std::vector<std::string>&cars, std::string tour)
{

}
void Client::GetTireNames(std::vector<std::string>&tires)
{

}
std::vector<int> Client::GetCarParameters(std::string path)
{
	std::vector<int> temp;
	return temp;
}
std::vector<std::string> Client::GetTireParameters(std::string path)
{
	std::vector<std::string> temp;
	return temp;
}
void Client::GetTourParameters(std::string path)
{

}
void Client::GetOtherParticipants(std::vector<Participant*> &participants, int ais)
{

}
void Client::GetRankingInfo(std::vector<std::string>&names, std::vector<int>&scores)
{

}
void Client::GetInfobox(InfoBox *infobox)
{

}
void Client::GetCurrentAtribs(float &durability, float current_speed)
{

}
void Client::GetTargetList(std::vector<std::string>&names)
{

}
void Client::SendInfo(std::string special_text, std::string text)
{

}
void Client::SendCurrentAction(int action, int value)
{

}
void Client::SendTarget(int ranking_position)
{

}
int Client::Possible_AIs()
{
	return 0;
}
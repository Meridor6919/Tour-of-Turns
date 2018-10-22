#include "NetworkRole.h"

Host::Host(Window &main_window) : SinglePlayer(main_window)
{
	this->main_window = &main_window;
	StartNetwork();
}
void Host::StartNetwork()
{

}
std::vector<int> Host::GetCarParameters(std::string path)
{
	std::vector<int> temp;
	return temp;
}
std::vector<std::string> Host::GetTireParameters(std::string path)
{
	std::vector<std::string> temp;
	return temp;
}
void Host::GetTourParameters(std::string path)
{

}
void Host::GetOtherParticipants(std::vector<Participant*> &participants, int ais)
{

}
void Host::GetRankingInfo(std::vector<std::string>&names, std::vector<int>&scores)
{

}
void Host::GetInfobox(InfoBox *infobox)
{

}
void Host::GetCurrentAtribs(float &durability, float current_speed)
{

}
void Host::GetTargetList(std::vector<std::string>&names)
{

}
void Host::SendInfo(std::string special_text, std::string text)
{

}
void Host::SendCurrentAction(int action, int value)
{

}
void Host::SendTarget(int ranking_position)
{

}
int Host::Possible_AIs()
{
	return 7 - (int)clients.size();
}
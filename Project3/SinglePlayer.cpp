#include "NetworkRole.h"

SinglePlayer::SinglePlayer(ToT_Window &main_window)
{
	this->main_window = &main_window;
}

void SinglePlayer::GetTourNames(std::vector<std::string>&tours)
{
	std::fstream fvar;
	std::string helper;
	fvar.open("Tour.txt");
	while (getline(fvar, helper))
		tours.push_back(helper);
}
void SinglePlayer::GetCarNames(std::vector<std::string>&cars, std::string tour)
{
	cars.clear();
	std::fstream fvar;
	std::string helper;
	fvar.open(tour.c_str());
	while (getline(fvar, helper) && helper != "")
		cars.push_back(helper);
}
void SinglePlayer::GetTireNames(std::vector<std::string>&tires)
{
	std::fstream fvar;
	std::string helper;
	fvar.open("Tire.txt");
	while (getline(fvar, helper))
		tires.push_back(helper);
}
std::vector<int> SinglePlayer::GetCarParameters(std::string path)
{
	std::fstream fvar;
	std::string helper;
	std::vector<int> car_parameters;
	fvar.open(path.c_str(), std::ios::in);
	while (std::getline(fvar, helper))
		car_parameters.push_back(std::atoi(helper.c_str()));

	return car_parameters;
}
std::vector<std::string> SinglePlayer::GetTireParameters(std::string path)
{
	std::fstream fvar;
	std::string helper;
	std::vector<std::string> tire_parameters;
	fvar.open(path.c_str(), std::ios::in);
	while (std::getline(fvar, helper))
		tire_parameters.push_back(helper);

	return tire_parameters;
}
void SinglePlayer::GetOtherParticipants(std::vector<Participant*> &participants, int ais, std::string tour)
{
	for (int i = 0; i < ais; i++)
		participants.emplace_back(new Participant(participants[i]->network_role, tour));
}
std::vector<std::string> SinglePlayer::GetTourParameters(std::string path)
{
	std::vector<std::string>x = { "" };
	return x;
}
void SinglePlayer::GetRankingInfo(std::vector<std::string>&names, std::vector<int>&scores)
{

}
void SinglePlayer::GetInfobox(InfoBox *infobox)
{

}
void SinglePlayer::GetCurrentAtribs(float &durability, float current_speed)
{

}
void SinglePlayer::GetTargetList(std::vector<std::string>&names)
{

}
void SinglePlayer::SendInfo(std::string special_text, std::string text)
{

}
std::pair<int, int> SinglePlayer::TakeAction()
{
	return std::make_pair(1, 4);
}
void SinglePlayer::SendTarget(int ranking_position)
{

}
int SinglePlayer::Possible_AIs()
{
	return 7;
}
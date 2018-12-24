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
	std::vector<std::string> ret;
	std::fstream fvar;
	std::string helper;

	fvar.open(path.c_str());
	do
	{ 
		std::getline(fvar, helper);
	}
	while (helper != "");
	while (std::getline(fvar, helper))
	{
		ret.push_back(helper);
	}
	fvar.close();

	return ret;
}
std::vector<std::pair<float, std::string>> SinglePlayer::GetRankingInfo(std::vector<Participant*> &participants)
{
	std::vector<std::pair<float, std::string>> ret;

	std::vector<float> scores;
	std::vector<std::string> names;

	for (int i = 0; i < participants.size(); i++)
		ret.push_back(std::make_pair(participants[i]->score, participants[i]->name));

	bool flag = true;
	float fhelper;
	std::string shelper;

	while (flag)
	{
		flag = false;
		for (int i = 0; i < participants.size() - 1; i++)
		{
			if (ret[i].first > ret[i+1].first)
			{
				flag = true;
				fhelper = ret[i].first;
				shelper = ret[i].second;
				ret[i].first = ret[i+1].first;
				ret[i].second = ret[i+1].second;
				ret[i+1].first = fhelper;
				ret[i+1].second = shelper;
			}
		}
	}
	return ret;
}
void SinglePlayer::GetInfobox(InfoBox *infobox)
{

}
void SinglePlayer::GetCurrentAtribs(float &durability, float current_speed)
{

}
void SinglePlayer::Attack(std::vector<Participant*> &participants)
{
	std::vector<std::string> rival_name;
	std::vector<int> rival_id;
	HANDLE handle = main_window->GetHandle();

	rival_name.push_back("Don't attack");
	rival_id.push_back(10);
	for (int i = 1; i < participants.size(); i++)
	{
		if (participants[i]->score < participants[0]->score + 5 && participants[i]->score > participants[0]->score - 5)
		{
			rival_name.push_back(participants[i]->name);
			rival_id.push_back(i);
		}
	}
	if (rival_id.size() == 1)
		return;

	int i = Text::Choose::Veritcal(rival_name, 0, {static_cast<short>(main_window->GetWidth() - 22), 48 }, 2, Text::TextAlign::center, true, *main_window);
	if (rival_id[i] != 10)
		participants[rival_id[i]]->attacked++;
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
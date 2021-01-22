#include ".\RankingManagement.h"

using namespace MeridorConsoleLib;

std::vector<std::string> RankingManagement::LoadRankingData(const char* path)
{
	std::vector<std::string> ret = {};
	std::ifstream fvar;
	std::string temp;

	fvar.open(path);
	for (int i = 0; std::getline(fvar, temp); ++i)
	{
		ret.push_back(temp);
	}
	fvar.close();

	return ret;
}
void RankingManagement::AddNewRecord(std::vector<std::string>& ranking_data, const std::string& name)
{
	ranking_data.push_back(name);
	for (int i = 0; i < Validation::ranking_details - 1; ++i)
	{
		ranking_data.push_back("");
	}
}
void RankingManagement::UpdateRecord(std::vector<std::string>& ranking_data, const std::vector<std::string>& ranking_details, int classification, int index)
{
	std::string temp;

	int racer_pos = GetRankedRacerPosition(ranking_data, ranking_details[RankingInfo::Name]);

	for (int i = 0; i < Validation::ranking_details; ++i)
	{
		if(ranking_data[racer_pos + i] == "")
		{
			ranking_data[racer_pos + i] = "\t\t ";
		}
		for (int j = 0; j < Validation::ranking_classification; ++j)
		{
			if (classification & Power(2, j))
			{
				switch (i)
				{
					case RankingInfo::Attacks:
					case RankingInfo::Burning:
					case RankingInfo::Drifts:
					case RankingInfo::Place:
					case RankingInfo::Score:
					case RankingInfo::Crashes:
					case RankingInfo::GamesPlayed:
					case RankingInfo::GamesWon:
					{

						temp = std::to_string(atoi(GetSeparatedValue(ranking_data[racer_pos + i].c_str(), j).c_str()) + atoi(ranking_details[i].c_str()));
						ranking_data[racer_pos + i] = SetSeparatedValue(ranking_data[racer_pos + i], temp, j);
						break;
					}
					case RankingInfo::BestScore:
					{
						int prev = atoi(GetSeparatedValue(ranking_data[racer_pos + i].c_str(), j).c_str());
						int curr = atoi(ranking_details[i].c_str());
						if (prev < curr)
						{
							ranking_data[racer_pos + i] = SetSeparatedValue(ranking_data[racer_pos + i], ranking_details[i], j);
						}
						break;
					}
					case RankingInfo::FavCar:
					case RankingInfo::FavTire:
					{
						temp = UpdateRankingFavorites(GetSeparatedValue(ranking_data[racer_pos + i], j), ranking_details[i]);
						ranking_data[racer_pos + i] = SetSeparatedValue(ranking_data[racer_pos + i], temp, j);
						break;
					}
				}
			}
		}
	}
}
void RankingManagement::SaveRankingData(std::vector<std::string>& ranking_data, const char* path)
{
	std::ofstream fvar;
	size_t ranking_size = ranking_data.size();

	fvar.open(path);
	for (int i = 0; i < ranking_size; ++i)
	{
		fvar << ranking_data[i] + '\n';
	}
	fvar.close();
}
void RankingManagement::AdjustRankingDetails(std::vector<std::string>& ranking_details)
{
	/*
	int finished_games = atoi(ret[1].c_str()) - atoi(ret[8].c_str());
	if (!finished_games)
	{
		for (int i = 2; i < 6 + 3 * (ret[1] == "0"); ++i)
		{
			ret[i] = "";
		}
		for (int i = 9; i < Validation::ranking_details; ++i)
		{
			ret[i] = "";
		}
	}
	else
	{
		double f = atof(ret[2].c_str()) / atof(ret[1].c_str()) * 100.0f;
		ret[2] = std::to_string(f).substr(0, static_cast<int>(std::to_string(f).size()) - 4) + '%';

		for (int i = 0; i < 5; ++i)
		{
			int x = i + 3 + (i > 1) * 4;
			ret[i + 3 + (i > 1) * 4] = std::to_string(static_cast<int>(round(atof(ret[i + 3 + (i > 1) * 4].c_str()) / static_cast<float>(finished_games))));
		}
	}
	*/
}
std::vector<std::string> RankingManagement::GetRankingDetails(const RankingDetails& ranking_info)
{
	std::vector<std::string> ret = {};
	ret.resize(RankingInfo::last);
	ret[RankingInfo::Attacks] = std::to_string(ranking_info.attacks);
	ret[RankingInfo::BestScore] = std::to_string(ranking_info.score);
	ret[RankingInfo::Score] = std::to_string(ranking_info.score);
	ret[RankingInfo::Burning] = std::to_string(ranking_info.durability_burning);
	ret[RankingInfo::Crashes] = std::to_string(ranking_info.crash);
	ret[RankingInfo::Drifts] = std::to_string(ranking_info.drifts);
	ret[RankingInfo::FavCar] = ranking_info.car;
	ret[RankingInfo::FavTire] = ranking_info.tires;
	ret[RankingInfo::GamesPlayed] = "1";
	ret[RankingInfo::GamesWon] = std::to_string(ranking_info.place == 1);
	ret[RankingInfo::Name] = ranking_info.name;
	ret[RankingInfo::Place] = std::to_string(ranking_info.place);
	return ret;
}
void RankingManagement::ValidateRankingDetails(std::vector<std::string>& ranking_details)
{
	size_t vector_size = ranking_details.size();
	if (vector_size < Validation::ranking_details)
	{
		ranking_details.resize(Validation::ranking_details, "");
	}
	else if (vector_size % Validation::ranking_details != 0)
	{
		ranking_details.resize(vector_size + (vector_size % Validation::ranking_details), "");
	}
}
int RankingManagement::GetRankedRacerPosition(const std::vector<std::string>& ranking_data, std::string name)
{
	for (int i = 0; i < static_cast<int>(ranking_data.size()); i += Validation::ranking_details)
	{
		if (ranking_data[i] == name)
		{
			return i;
		}
	}
	return -1;
}
std::string RankingManagement::GetRankingFavourite(const std::string& text)
{
	std::string ret = "";
	int highest_value = 0;

	for (int i = 0; true; ++i)
	{
		std::string instance = GetSeparatedValue(text, i, ';');
		if (instance.size() > 0)
		{
			int occurences = atoi(GetSeparatedValue(instance, 1, ':').c_str());
			if (occurences >= highest_value)
			{
				highest_value = occurences;
				ret = GetSeparatedValue(instance, 0, ':');
			}
		}
		else
		{
			break;
		}
	}
	return ret;
}
std::string RankingManagement::UpdateRankingFavorites(const std::string& text, const std::string& record_id)
{
	std::string ret = "";
	int highest_value = 0;
	for (int i = 0; true; ++i)
	{
		std::string instance = GetSeparatedValue(text, i, ';');
		if (instance.size() > 0)
		{
			std::string subtext = GetSeparatedValue(instance, 0, ':');
			if (subtext == record_id)
			{
				int occurences = atoi(GetSeparatedValue(instance, 1, ':').c_str());
				std::string first_part = SetSeparatedValue(instance, std::to_string(occurences + 1), 1, ':');
				return SetSeparatedValue(text, first_part, i, ';');
			}
		}
		else
		{
			if (i == 0)
			{
				return text + record_id + ":1";
			}
			else
			{
				return text + ';' + record_id + ":1";
			}
		}
	}
	return text;
}

std::vector<std::string> RankingManagement::GetRankedRacersNames(std::string tour)
{
	std::vector<std::string> ret = {};
	std::ifstream fvar;
	std::string temp;

	fvar.open(tour.c_str());
	for (int i = 0; std::getline(fvar, temp); ++i)
	{
		if (!(i % Validation::ranking_details))
		{
			ret.push_back(temp);
		}
	}
	fvar.close();

	return ret;
}
void RankingManagement::SaveRanking(RankingDetails ranking_info)
{
	RemoveExtension(ranking_info.tour, ExtName::tour);
	RemoveExtension(ranking_info.car, ExtName::car);
	RemoveExtension(ranking_info.tires, ExtName::tire);

	std::string ranking_path = FolderName::ranking + '\\' + ranking_info.tour + ExtName::ranking;
	std::vector<std::string> ranking_data = RankingManagement::LoadRankingData(ranking_path.c_str());
	int record_index = GetRankedRacerPosition(ranking_data, ranking_info.name);

	if (record_index < 0)
	{
		record_index = static_cast<int>(ranking_data.size());
		AddNewRecord(ranking_data, ranking_info.name);
	}
	std::vector<std::string> ranking_details = GetRankingDetails(ranking_info);
	
	int classification = RankingClassification::all_games;
	if (ranking_info.ais == GameConstants::max_ais)
	{
		classification += RankingClassification::ai8;
	}
	if (ranking_info.multiplayer_flag)
	{
		classification += RankingClassification::multiplayer;
	}

	UpdateRecord(ranking_data, ranking_details, classification, record_index);
	SaveRankingData(ranking_data, ranking_path.c_str());
}
std::vector<std::string> RankingManagement::GetRankingDetails(std::string tour, int racer_pos, int classification_type)
{
	std::vector<std::string> ret = {};
	std::ifstream fvar;
	std::string temp;

	fvar.open(tour.c_str());

	//skip data before racer position
	for (int i = 0; i < Validation::ranking_details * racer_pos && std::getline(fvar, temp); ++i);

	//Get Details
	for (int i = 0; i < Validation::ranking_details && std::getline(fvar, temp); ++i)
	{
		switch (i)
		{
			case RankingInfo::Name:
			{
				ret.push_back(temp);
				break;
			}
			case RankingInfo::Attacks:
			case RankingInfo::Burning:
			case RankingInfo::Drifts:
			case RankingInfo::Place:
			case RankingInfo::Score:
			case RankingInfo::BestScore:
			case RankingInfo::Crashes:
			case RankingInfo::GamesPlayed:
			case RankingInfo::GamesWon:
			{
				ret.push_back(GetSeparatedValue(temp, classification_type));
				break;
			}
			case RankingInfo::FavCar:
			case RankingInfo::FavTire:
			{
				ret.push_back(GetRankingFavourite(temp));
			}
		}
	}
	fvar.close();

	ValidateRankingDetails(ret);
	AdjustRankingDetails(ret);
	return ret;
}

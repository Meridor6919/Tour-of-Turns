#include ".\RankingManagement.h"

using namespace MeridorConsoleLib;

std::vector<std::string> RankingManagement::LoadRawData(const char* path)
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
void RankingManagement::AddEmptyRecord(std::vector<std::string>& ranking_data, const std::string& name)
{
	ranking_data.push_back(name);
	for (int i = 0; i < Validation::ranking_details - 1; ++i)
	{
		ranking_data.push_back("");
	}
}
void RankingManagement::UpdateRecordFinished(std::vector<std::string>& ranking_data, const std::vector<std::string>& record_data, int classification, int index)
{
	std::string temp;

	for (int i = 0; i < Validation::ranking_details; ++i)
	{
		if(ranking_data[index + i] == "")
		{
			ranking_data[index + i] = "\t\t ";
		}
		for (int j = 0; j < Validation::ranking_classification; ++j)
		{
			if (classification & Power(2, j))
			{
				switch (i)
				{
					case RankingInfo::Attacks:
					case RankingInfo::Drifts:
					case RankingInfo::Place:
					case RankingInfo::Crashes:
					case RankingInfo::GamesPlayed:
					case RankingInfo::GamesWon:
					{

						temp = std::to_string(atoi(GetSeparatedValue(ranking_data[index + i].c_str(), j).c_str()) + atoi(record_data[i].c_str()));
						ranking_data[index + i] = SetSeparatedValue(ranking_data[index + i], temp, j);
						break;
					}
					case RankingInfo::Burning:
					case RankingInfo::Score:
					{
						temp = std::to_string(atof(GetSeparatedValue(ranking_data[index + i].c_str(), j).c_str()) + atof(record_data[i].c_str()));
						ranking_data[index + i] = SetSeparatedValue(ranking_data[index + i], temp, j);
						break;
					}
					case RankingInfo::BestScore:
					{
						double prev = atof(GetSeparatedValue(ranking_data[index + i].c_str(), j).c_str());
						double curr = atof(record_data[i].c_str());
						if (prev > curr || prev == 0)
						{
							ranking_data[index + i] = SetSeparatedValue(ranking_data[index + i], record_data[i], j);
						}
						break;
					}
					case RankingInfo::FavCar:
					case RankingInfo::FavTire:
					{
						temp = UpdateFavorite(GetSeparatedValue(ranking_data[index + i], j), record_data[i]);
						ranking_data[index + i] = SetSeparatedValue(ranking_data[index + i], temp, j);
						break;
					}
				}
			}
		}
	}
}
void RankingManagement::UpdateRecordCrashed(std::vector<std::string>& ranking_data, int classification, int index)
{
	std::string temp;

	for (int i = 0; i < Validation::ranking_details; ++i)
	{
		if (ranking_data[index + i] == "")
		{
			ranking_data[index + i] = "\t\t ";
		}
		for (int j = 0; j < Validation::ranking_classification; ++j)
		{
			if (classification & Power(2, j))
			{
				switch (i)
				{
					case RankingInfo::Crashes:
					case RankingInfo::GamesPlayed:
					{

						temp = std::to_string(atoi(GetSeparatedValue(ranking_data[index + i].c_str(), j).c_str()) + 1);
						ranking_data[index + i] = SetSeparatedValue(ranking_data[index + i], temp, j);
						break;
					}
				}
			}
		}
	}
}
void RankingManagement::SaveData(std::vector<std::string>& ranking_data, const char* path)
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
void RankingManagement::AdjustStringVector(std::vector<std::string>& record_data)
{
	int games_played = atoi(record_data[RankingInfo::GamesPlayed].c_str());
	int finished_games = games_played - atoi(record_data[RankingInfo::Crashes].c_str());

	if (games_played)
	{
		record_data[RankingInfo::GamesWon] = std::to_string(atof(record_data[RankingInfo::GamesWon].c_str()) / games_played * 100.0) ;
		record_data[RankingInfo::GamesWon] = SetPrecision(record_data[RankingInfo::GamesWon], 2);
		record_data[RankingInfo::GamesWon] += '%';
	}
	if (finished_games)
	{
		for (int i = 0; i < Validation::ranking_details; ++i)
		{
			switch (i)
			{
				case RankingInfo::Attacks:
				case RankingInfo::Drifts:
				case RankingInfo::Place:
				case RankingInfo::Burning:
				case RankingInfo::Score:
				{
					record_data[i] = std::to_string(atof(record_data[i].c_str()) / finished_games);
					[[fallthrough]];
				}
				case RankingInfo::BestScore:
				{
					record_data[i] = SetPrecision(record_data[i], 2);
					break;
				}
			}
		}
	}
}
std::vector<std::string> RankingManagement::RankingInfoToStringVector(const RacerLeaderboardInfo& racer_leaderboard_info)
{
	std::vector<std::string> ret = {};
	ret.resize(RankingInfo::last);

	ret[RankingInfo::Attacks] = std::to_string(racer_leaderboard_info.attacks);
	ret[RankingInfo::BestScore] = std::to_string(racer_leaderboard_info.score);
	ret[RankingInfo::Score] = std::to_string(racer_leaderboard_info.score);
	ret[RankingInfo::Burning] = std::to_string(racer_leaderboard_info.durability_burning);
	ret[RankingInfo::Crashes] = "0";
	ret[RankingInfo::Drifts] = std::to_string(racer_leaderboard_info.drifts);
	ret[RankingInfo::FavCar] = racer_leaderboard_info.car;
	ret[RankingInfo::FavTire] = racer_leaderboard_info.tires;
	ret[RankingInfo::GamesPlayed] = "1";
	ret[RankingInfo::GamesWon] = std::to_string(racer_leaderboard_info.place == 1);
	ret[RankingInfo::Name] = racer_leaderboard_info.name;
	ret[RankingInfo::Place] = std::to_string(racer_leaderboard_info.place);
	return ret;
}
void RankingManagement::SanitizeRankingDetails(std::vector<std::string>& record_data)
{
	size_t vector_size = record_data.size();
	if (vector_size < Validation::ranking_details)
	{
		record_data.resize(Validation::ranking_details, "");
	}
	else if (vector_size % Validation::ranking_details != 0)
	{
		record_data.resize(vector_size + (vector_size % Validation::ranking_details), "");
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
std::string RankingManagement::GetFavourite(const std::string& text)
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
std::string RankingManagement::UpdateFavorite(const std::string& text, const std::string& record_id)
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
void RankingManagement::Save(RacerLeaderboardInfo racer_leaderboard_info)
{
	RemoveExtension(racer_leaderboard_info.tour, ExtName::tour);
	RemoveExtension(racer_leaderboard_info.car, ExtName::car);
	RemoveExtension(racer_leaderboard_info.tires, ExtName::tire);

	std::string ranking_path = FolderName::ranking + '\\' + racer_leaderboard_info.tour + ExtName::ranking;
	std::vector<std::string> ranking_data = RankingManagement::LoadRawData(ranking_path.c_str());
	int record_index = GetRankedRacerPosition(ranking_data, racer_leaderboard_info.name);

	if (record_index < 0)
	{
		record_index = static_cast<int>(ranking_data.size());
		AddEmptyRecord(ranking_data, racer_leaderboard_info.name);
	}

	int classification = RankingClassification::all_games;
	if (racer_leaderboard_info.ais == GameConstants::max_ais)
	{
		classification += RankingClassification::ai8;
	}
	if (racer_leaderboard_info.multiplayer_flag)
	{
		classification += RankingClassification::multiplayer;
	}

	if (racer_leaderboard_info.crash)
	{
		UpdateRecordCrashed(ranking_data, classification, record_index);
	}
	else
	{
		std::vector<std::string> record_data = RankingInfoToStringVector(racer_leaderboard_info);
		UpdateRecordFinished(ranking_data, record_data, classification, record_index);
	}
	SaveData(ranking_data, ranking_path.c_str());
}
std::vector<std::string> RankingManagement::GetTextToDisplay(std::string tour, int racer_pos, int classification_type)
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
				ret.push_back(GetFavourite(GetSeparatedValue(temp, classification_type)));
			}
		}
	}
	fvar.close();

	SanitizeRankingDetails(ret);
	AdjustStringVector(ret);
	return ret;
}

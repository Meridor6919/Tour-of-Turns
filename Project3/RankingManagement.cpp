#include ".\RankingManagement.h"

using namespace MeridorConsoleLib;

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
	const std::string ranking_path = FolderName::ranking + '\\' + ranking_info.tour.substr(0, static_cast<int>(ranking_info.tour.size()) - static_cast<int>(ExtName::tour.size())) + ExtName::ranking;
	int record_index;
	ranking_info.car = ranking_info.car.substr(0, static_cast<int>(ranking_info.car.size()) - static_cast<int>(ExtName::car.size()));
	ranking_info.tires = ranking_info.tires.substr(0, static_cast<int>(ranking_info.tires.size()) - static_cast<int>(ExtName::tire.size()));

	
	std::vector<std::string> ranking_data = RankingManagement::LoadRankingData(ranking_path.c_str());

	record_index = GetRankedRacerPosition(ranking_data, ranking_info.name);
	if (record_index < 0)
	{
		record_index = static_cast<int>(ranking_data.size());
		AddNewRecord(ranking_data, ranking_info.name);
	}
	UpdateRecord(ranking_data, ranking_info, record_index);
	SaveRankingData(ranking_data, ranking_path.c_str());
}

std::vector<std::string> RankingManagement::LoadRankingData(const char* path)
{
	std::vector<std::string> ret = {""};
	std::ifstream fvar;

	fvar.open(path);
	for (int i = 0; std::getline(fvar, ret[i]); ++i)
	{
		ret.push_back("");
	}
	fvar.close();
	return ret;
}

void RankingManagement::AddNewRecord(std::vector<std::string>& ranking_data, const std::string &name)
{
	ranking_data[ranking_data.size()-1] = name;
	for (int i = 0; i < Validation::ranking_details - 1; ++i)
	{
		ranking_data.push_back("");
	}
	ranking_data.push_back("");
}

void RankingManagement::UpdateRecord(std::vector<std::string>& ranking_data, const RankingDetails& ranking_details, int index)
{
	const bool classification[Validation::ranking_classification] = { true, ranking_details.ais == 7, ranking_details.multiplayer_flag };
	std::string temp;

	if (!ranking_details.crash)
	{
		const std::vector<int> added_value = {
			1,
			ranking_details.place == 1,
			ranking_details.place,
			ranking_details.score,
			0,
			ranking_details.crash,
			ranking_details.attacks,
			ranking_details.drifts,
			ranking_details.durability_burning
		};
		
		for (int i = 0; i < static_cast<int>(added_value.size()); ++i)
		{
			temp = "";
			for (int j = 0; j < Validation::ranking_classification; ++j)
			{
				temp += std::to_string(atoi(GetSeparatedValue(ranking_data[index + i], j).c_str()) + added_value[i] * classification[j]) + '\t';
			}
			ranking_data[index + i] = temp.substr(0, static_cast<int>(temp.size()) - 1);
		}
		temp = "";
		for (int i = 0; i < Validation::ranking_classification; ++i)
		{
			int local_score = atoi(GetSeparatedValue(ranking_data[index + 4], i).c_str());
			if (classification[i] && (ranking_details.score < local_score || local_score == 0))
			{
				local_score = ranking_details.score;
			}
			temp += std::to_string(local_score) + '\t';
		}
		ranking_data[index + 4] = temp.substr(0, static_cast<int>(temp.size()) - 1);
	}
	else
	{
		for (int i = 0; i < 10; i += 5)
		{
			temp = "";
			for (int j = 0; j < Validation::ranking_classification; ++j)
			{
				temp += std::to_string(atoi(GetSeparatedValue(ranking_data[index + i], j).c_str()) + 1 * classification[j]) + '\t';
			}
			ranking_data[index + i] = temp.substr(0, static_cast<int>(temp.size()) - 1);
		}
	}
	for (int i = 0; i < 2; ++i)
	{
		temp = "";
		for (int j = 0; j < Validation::ranking_classification; ++j)
		{
			temp += UpdateRankingFavorites(GetSeparatedValue(ranking_data[index + 9 + i], j), i % 2 ? ranking_details.tires : ranking_details.car) + "\t";
		}
		ranking_data[index + 9 + i] = temp.substr(0, static_cast<int>(temp.size()) - 1);
	}
}

void RankingManagement::SaveRankingData(std::vector<std::string>& ranking_data, const char* path)
{
	std::ofstream fvar;

	fvar.open(path);
	for (int i = 0; i < static_cast<int>(ranking_data.size()) - 1; ++i)
	{
		if (i)
		{
			fvar << '\n';
		}
		fvar << ranking_data[i];
	}
	fvar.close();
}

int RankingManagement::GetRankedRacerPosition(const std::vector<std::string>& ranking_data, std::string name)
{
	for (int i = 0; i < static_cast<int>(ranking_data.size()); i += Validation::ranking_details)
	{
		if (ranking_data[i] == name)
		{
			return i + 1;
		}
	}
	return -1;
}

std::vector<std::string> RankingManagement::GetRankingDetails(std::string tour, int racer_pos, int classification_type)
{
	std::string line;
	std::vector<std::string> ret = {};
	for (int i = 0; i < Validation::ranking_details; ++i)
	{
		ret.push_back(" ");
	}
	std::ifstream fvar;
	fvar.open(tour.c_str());
	for (int i = 0; i < Validation::ranking_details * racer_pos && std::getline(fvar, line); ++i);
	for (int i = 0; i < Validation::ranking_details && std::getline(fvar, line); ++i)
	{
		if (i == 0)
		{
			ret[i] = line;
		}
		else if (i < 6)
		{
			ret[i] = GetSeparatedValue(line, classification_type);
		}
		else if (i < 10)
		{
			ret[i + 2] = GetSeparatedValue(line, classification_type);
		}
		else
		{
			ret[i - 4] = GetSeparatedValue(line, classification_type);
		}
	}
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
	ret[6] = GetRankingFavourite(ret[6]);
	ret[7] = GetRankingFavourite(ret[7]);
	fvar.close();
	return ret;
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
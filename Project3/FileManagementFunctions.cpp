#include "FileManagementFunctions.h"
#include "ValidationFunctions.h"

std::vector<std::string> FileManagement::GetTourNames()
{
	return MeridorConsoleLib::GetFilesInDirectory(FolderName::tour);
}
std::vector<std::string> FileManagement::GetTireNames()
{
	return MeridorConsoleLib::GetFilesInDirectory(FolderName::tire);
}
std::vector<std::string> FileManagement::GetCarNames(const std::string tour)
{
	return MeridorConsoleLib::ReadFile(FolderName::tour + '\\' + tour);
}
std::vector<std::string> FileManagement::GetRankingNames(std::string tour)
{
	std::ifstream fvar;
	std::string temp;
	std::vector<std::string> ret = {};
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
ToTGameConfig FileManagement::LoadGameConfig()
{
	std::fstream fvar;
	ToTGameConfig ret = {};

	fvar.open(FolderName::main + '\\' + FileName::game_config, std::ios::in);
	fvar >> ret.ais;
	fvar >> ret.name;
	fvar >> ret.lang;
	fvar >> ret.timer_settings;
	
	fvar.close();
	return ret;
}
ToTWindowConfig FileManagement::LoadWindowConfig()
{
	std::ifstream fvar;
	ToTWindowConfig ret = {};

	fvar.open(FolderName::main + '\\' + FileName::window_config, std::ios::in);
	fvar >> ret.window_info.title;
	fvar >> ret.window_info.characters_capacity.X;
	fvar >> ret.window_info.characters_capacity.Y;
	fvar >> ret.window_info.main_color;
	fvar >> ret.window_info.secondary_color;
	fvar >> ret.window_info.visible_cursor;
	fvar >> reinterpret_cast<std::underlying_type<MeridorConsoleLib::WindowMode>::type&>(ret.window_info.window_mode);
	fvar >> ret.music_volume;
	fvar >> ret.hamachi_flag;

	fvar.close();
	return ret;
}
void FileManagement::LoadLanguagePack(std::string path)
{
	LanguagePack::text.clear();
	LanguagePack::text.push_back({});
	std::ifstream fvar;
	std::string line;
	int position = 0;
	fvar.open(path.c_str());
	while (std::getline(fvar, line))
	{
		if (line == "&&&")
		{
			LanguagePack::text.push_back({});
			++position;
		}
		else
		{
			LanguagePack::text[position].push_back(line);
		}
	}
	fvar.close();
	position++;
}

std::vector<std::string> FileManagement::GetTourParameters(std::string tour, int position, const int visibility)
{
	std::vector<std::string> ret;
	std::fstream fvar;
	std::string helper;

	fvar.open((FolderName::tour + '\\' + tour).c_str());
	do
	{
		std::getline(fvar, helper);
	} while (helper != "");
	for (int i = 0; i < position; ++i)
	{
		std::getline(fvar, helper);
	}
	for (int i = 0; i < visibility; ++i)
	{
		if (!std::getline(fvar, helper))
		{
			break;
		}
		ret.push_back(std::move(helper));
	}
	fvar.close();
	return ret;
}
std::vector<int> FileManagement::GetCarParameters(const std::string path)
{
	const std::vector<std::string> data = MeridorConsoleLib::ReadFile(FolderName::car + '\\' + path);
	std::vector<int> car_parameters;
	for (short i = 0; i < CarAttributes::last; ++i)
	{
		if (atoi(data[i].c_str()) < 1 || atoi(data[i].c_str()))
		{
			car_parameters.push_back(atoi(data[i].c_str()));
		}
	}
	return car_parameters;
}
std::vector<std::string> FileManagement::GetTireParameters(const std::string path)
{
	return MeridorConsoleLib::ReadFile(FolderName::tire + '\\' + path);
}
std::vector<std::string> FileManagement::GetRankingDetails(std::string tour, int racer_pos, int classification_type)
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

void FileManagement::SaveRanking(RankingDetails ranking_info)
{
	const bool classification[Validation::ranking_classification] = { true, ranking_info.ais == 7, ranking_info.multiplayer_flag };
	const std::string ranking_path = FolderName::ranking + '\\' + ranking_info.tour.substr(0, static_cast<int>(ranking_info.tour.size()) - static_cast<int>(ExtName::tour.size())) + ExtName::ranking;
	std::vector<std::string> ranking_data = { "" };
	int racer_index = -1;
	std::fstream fvar;
	std::string temp;
	ranking_info.car = ranking_info.car.substr(0, static_cast<int>(ranking_info.car.size()) - static_cast<int>(ExtName::car.size()));
	ranking_info.tires = ranking_info.tires.substr(0, static_cast<int>(ranking_info.tires.size()) - static_cast<int>(ExtName::tire.size()));

	//Loading data
	fvar.open(ranking_path.c_str(), std::ios::in);
	for (int i = 0; std::getline(fvar, ranking_data[i]); ++i)
	{
		ranking_data.push_back("");
	}
	fvar.close();
	//Searching if the racer is already in the data
	for (int i = 0; i < static_cast<int>(ranking_data.size()); i += Validation::ranking_details)
	{
		if (ranking_data[i] == ranking_info.name)
		{
			racer_index = i + 1;
			break;
		}
	}
	//Add record if racer not found
	if (racer_index < 0)
	{
		racer_index = static_cast<int>(ranking_data.size());
		ranking_data[racer_index - 1] = ranking_info.name;
		for (int i = 0; i < Validation::ranking_details - 1; ++i)
		{
			ranking_data.push_back("");
		}
		ranking_data.push_back("");
	}
	//Modifying ranking_data by selected racer's performance
	if (!ranking_info.crash)
	{
		const std::vector<int> added_value = { 1, ranking_info.place == 1,ranking_info.place, ranking_info.score, 0, ranking_info.crash, ranking_info.attacks, ranking_info.drifts, ranking_info.durability_burning };
		for (int i = 0; i < static_cast<int>(added_value.size()); ++i)
		{
			temp = "";
			for (int j = 0; j < Validation::ranking_classification; ++j)
			{
				temp += std::to_string(atoi(GetSeparatedValue(ranking_data[racer_index + i], j).c_str()) + added_value[i] * classification[j]) + '\t';
			}
			ranking_data[racer_index + i] = temp.substr(0, static_cast<int>(temp.size()) - 1);
		}
		temp = "";
		for (int i = 0; i < Validation::ranking_classification; ++i)
		{
			int local_score = atoi(GetSeparatedValue(ranking_data[racer_index + 4], i).c_str());
			if (classification[i] && (ranking_info.score < local_score || local_score == 0))
			{
				local_score = ranking_info.score;
			}
			temp += std::to_string(local_score) + '\t';
		}
		ranking_data[racer_index + 4] = temp.substr(0, static_cast<int>(temp.size()) - 1);
	}
	else
	{
		for (int i = 0; i < 10; i += 5)
		{
			temp = "";
			for (int j = 0; j < Validation::ranking_classification; ++j)
			{
				temp += std::to_string(atoi(GetSeparatedValue(ranking_data[racer_index + i], j).c_str()) + 1 * classification[j]) + '\t';
			}
			ranking_data[racer_index + i] = temp.substr(0, static_cast<int>(temp.size()) - 1);
		}
	}
	for (int i = 0; i < 2; ++i)
	{
		temp = "";
		for (int j = 0; j < Validation::ranking_classification; ++j)
		{
			temp += UpdateRankingFavorites(GetSeparatedValue(ranking_data[racer_index + 9 + i], j), i % 2 ? ranking_info.tires : ranking_info.car) + "\t";
		}
		ranking_data[racer_index + 9 + i] = temp.substr(0, static_cast<int>(temp.size()) - 1);
	}
	//Saving ranking_data to file
	fvar.open(ranking_path.c_str(), std::ios::out);
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
void FileManagement::SaveGameConfig(const ToTGameConfig &game_config)
{
	std::string name = game_config.name;;
	for (int i = 0; i < static_cast<int>(name.size()); ++i)
	{
		if (name[i] == ' ')
		{
			name[i] = '_';
		}
	}
	std::ofstream fvar;
	fvar.open(FolderName::main + '\\' + FileName::game_config);
	fvar << game_config.ais << '\n';
	fvar << name + '\n';
	fvar << game_config.lang + '\n';
	fvar << game_config.timer_settings << '\n';
	fvar.close();
}
void FileManagement::SaveWindowConfig(const ToTWindowConfig& window_config)
{
	std::string title = window_config.window_info.title;;
	for (int i = 0; i < static_cast<int>(title.size()); ++i)
	{
		if (title[i] == ' ')
		{
			title[i] = '_';
		}
	}
	std::ofstream fvar;
	fvar.open(FolderName::main + '\\' + FileName::window_config);
	fvar << title << '\n';
	fvar << window_config.window_info.characters_capacity.X << '\n';
	fvar << window_config.window_info.characters_capacity.Y << '\n';
	fvar << window_config.window_info.main_color << '\n';
	fvar << window_config.window_info.secondary_color << '\n';
	fvar << window_config.window_info.visible_cursor << '\n';
	fvar << static_cast<int>(window_config.window_info.window_mode) << '\n';
	fvar << window_config.music_volume << '\n';
	fvar << window_config.hamachi_flag << '\n';
	fvar.close();
}

void FileManagement::RemoveExtension(std::vector<std::string>& vector, std::string extension)
{
	short extension_size = static_cast<short>(extension.size());
	for (int i = 0; i < static_cast<int>(vector.size()); ++i)
	{
		vector[i] = vector[i].substr(0, static_cast<short>(vector[i].size()) - extension_size);
	}
}
std::string FileManagement::GetSeparatedValue(const std::string &text, int index, char separator)
{
	int start = 0;
	int count = 0;
	int text_size = static_cast<int>(text.size());
	for (int i = 0; i < text_size; ++i)
	{
		if ((text[i] == separator) || (i + 1 == text_size))
		{
			if (!index)
			{
				count = i - start + (i + 1 == text_size);
				break;
			}
			else
			{
				--index;
				start = i + 1;
			}
		}
	}
	return text.substr(start, count);
}
std::string FileManagement::SetSeparatedValue(const std::string& original_text, const std::string& text_to_place, int index, char separator)
{
	int start = 0;
	int count = 0;
	std::string ret;
	int original_text_size = static_cast<int>(original_text.size());
	for (int i = 0; i < original_text_size; ++i)
	{
		if ((original_text[i] == separator) || (i + 1 == original_text_size))
		{
			if (!index)
			{
				count = i - start + (i + 1 == original_text_size);
				return original_text.substr(0, start) + text_to_place + original_text.substr(start + count, original_text_size - start - count);
			}
			else
			{
				--index;
				start = i + 1;
			}
		}
	}
	return original_text;
}

std::string FileManagement::GetRankingFavourite(const std::string& text)
{
	std::string ret = "";
	int highest_value = 0;

	for(int i = 0; true; ++i)
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
std::string FileManagement::UpdateRankingFavorites(const std::string& text, const std::string& record_id)
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
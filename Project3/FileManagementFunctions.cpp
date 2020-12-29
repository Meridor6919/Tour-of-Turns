#include "FileManagementFunctions.h"
#include "ValidationFunctions.h"

std::vector<std::string> FileManagement::GetTourNames()
{
	return MeridorConsoleLib::ReadFile(FolderName::tour + '\\' + FileName::tour);
}
std::vector<std::string> FileManagement::GetTireNames()
{
	return MeridorConsoleLib::ReadFile(FolderName::tire + '\\' + FileName::tire);
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
WindowConfig FileManagement::LoadWindowConfig()
{
	std::fstream fvar;
	WindowConfig ret = {};
	ret.hamachi_flag = true;
	ret.music_volume = 1.0f;
	ret.ais = 0;

	fvar.open(FolderName::main + '\\' + FileName::config, std::ios::in);
	fvar >> ret.main_color;
	fvar >> ret.secondary_color;
	fvar >> ret.music_volume;
	fvar >> ret.hamachi_flag;
	fvar >> ret.ais;
	fvar >> ret.name;
	fvar >> ret.lang;
	fvar >> ret.timer_settings;
	fvar.close();

	bool name_valid = true;
	for (int i = 0; i < static_cast<int>(ret.name.size()); ++i)
	{
		if (ret.name[i] == '_')
		{
			ret.name[i] = ' ';
		}
		else if (!(ret.name[i] >= 'a' && ret.name[i] <= 'z' || ret.name[i] >= 'A' && ret.name[i] <= 'Z'))
		{
			name_valid = false;
			break;
		}
	}
	LoadLanguagePack(FolderName::language + '\\' + ret.lang);
	if (!Validation::ValidateLanguagePack())
	{
		MessageBox(0, (ret.lang + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);

		std::vector<std::string> languages = MeridorConsoleLib::ReadFile(FolderName::language + '\\' + FileName::language);
		bool no_valid_lang_packs = true;
		for (int i = 0; i < static_cast<int>(languages.size()); ++i)
		{
			LoadLanguagePack(FolderName::language + '\\' + languages[i]);
			if (Validation::ValidateLanguagePack())
			{
				no_valid_lang_packs = false;
				ret.lang = languages[i];
				MessageBox(0, (languages[i] + ErrorMsg::placeholder_language).c_str(), ErrorTitle::placeholder_language.c_str(), 0);
				break;
			}
		}
		if (no_valid_lang_packs)
		{
			MessageBox(0, ErrorMsg::language_error.c_str(), ErrorTitle::language_error.c_str(), 0);
			exit(0);
		}
	}
	if (ret.main_color < 0 || ret.main_color > 15)
	{
		ret.main_color = 15;
	}
	if (ret.secondary_color < 0 || ret.secondary_color > 15)
	{
		ret.secondary_color = 10;
	}
	if (ret.ais < 0 || ret.ais > 7)
	{
		ret.ais = 7;
	}
	if (static_cast<int>(ret.name.size()) < 1 || static_cast<int>(ret.name.size()) > GameConstants::maximum_name_length || !name_valid)
	{
		ret.name = LanguagePack::text[LanguagePack::other_strings][OtherStrings::default_name];
	}
	if (ret.timer_settings < 0 || ret.timer_settings > GameConstants::maximum_timer)
	{
		ret.timer_settings = 0;
	}
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

void FileManagement::SaveRanking(RankingInfo ranking_info)
{
	const bool classification[Validation::ranking_classification] = { true, ranking_info.ais == 7, ranking_info.multiplayer };
	const std::string ranking_path = FolderName::tour + '\\' + ranking_info.tour.substr(0, static_cast<int>(ranking_info.tour.size()) - static_cast<int>(ExtName::tour.size())) + ExtName::ranking;
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
			temp += UpdateRankingFavorites(GetSeparatedValue(ranking_data[racer_index + 9 + i], j), i % 2 ? ranking_info.tires : ranking_info.car, 1) + "\t";
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
bool FileManagement::SaveFileNames(std::string src_path, std::string dst_path, const std::string ext, HANDLE handle)
{
	dst_path = "dir " + src_path + "\\*" + ext + " > " + dst_path + " /b";
	if (system(dst_path.c_str()))
	{
		//Clearing error message that shows in a console window by default when files not found
		SetConsoleCursorPosition(handle, { 0,0 });
		MeridorConsoleLib::Spaces(25);
		SetConsoleCursorPosition(handle, { 0,0 });
		return false;
	}
	return true;
}
void FileManagement::SaveWindowConfig(WindowConfig window_config)
{
	for (int i = 0; i < static_cast<int>(window_config.name.size()); ++i)
	{
		if (window_config.name[i] == ' ')
		{
			window_config.name[i] = '_';
		}
	}
	std::ofstream fvar;
	fvar.open(FolderName::main + '\\' + FileName::config);
	fvar << window_config.main_color << '\n';
	fvar << window_config.secondary_color << '\n';
	fvar << window_config.music_volume << '\n';
	fvar << window_config.hamachi_flag << '\n';
	fvar << window_config.ais << '\n';
	fvar << window_config.name + '\n';
	fvar << window_config.lang + '\n';
	fvar << window_config.timer_settings << '\n';
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
	for (int i = 0; i < static_cast<int>(text.size()); ++i)
	{
		if ((text[i] == '\t') || (i + 1 == static_cast<int>(text.size())))
		{
			if (!index)
			{
				count = i - start + (i + 1 == static_cast<int>(text.size()));
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
std::string FileManagement::GetRankingFavourite(std::string text)
{
	std::string current_phrase = "";
	std::string current_value = "";
	std::string ret = "";
	int highest_value = 0;
	bool phrase_value_flag = true;

	for (int i = 0; i < static_cast<int>(text.size()); ++i)
	{
		if (text[i] == ':')
		{
			phrase_value_flag = !phrase_value_flag;
			if (phrase_value_flag)
			{
				if (atoi(current_value.c_str()) > highest_value)
				{
					ret = current_phrase;
					highest_value = atoi(current_value.c_str());
					current_phrase = "";
					current_value = "";
				}
			}
		}
		else if (phrase_value_flag)
		{
			current_phrase += text[i];
		}
		else
		{
			current_value += text[i];
		}
	}
	return ret;
}
std::string FileManagement::UpdateRankingFavorites(std::string text, std::string phrase, int added_value)
{
	if (!added_value)
	{
		return "";
	}
	bool phrase_value_flag = true;
	std::string current_phrase = "";
	std::string value = "";
	int phrase_index = -1;

	for (int i = 0; i < static_cast<int>(text.size()); ++i)
	{
		if (text[i] == ':')
		{
			phrase_value_flag = !phrase_value_flag;
			if (!phrase_value_flag)
			{
				if (current_phrase == phrase)
				{
					phrase_index = i;
				}
			}
			else
			{
				if (phrase_index != -1)
				{
					return text.substr(0, phrase_index + 1) + std::to_string(atoi(value.c_str()) + added_value) + text.substr(i, static_cast<int>(text.size()) - i);
				}
				current_phrase = "";
				value = "";
			}
		}
		else if (phrase_value_flag)
		{
			current_phrase += text[i];
		}
		else
		{
			value += text[i];
		}
	}
	return text + phrase + ':' + std::to_string(added_value) + ':';
}
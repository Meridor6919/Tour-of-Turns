#include "FileManagementFunctions.h"
#include "ValidationFunctions.h"

using namespace MeridorConsoleLib;

std::vector<std::string> FileManagement::GetTourNames()
{
	return GetFilesInDirectory(FolderName::tour);
}
std::vector<std::string> FileManagement::GetTireNames()
{
	return GetFilesInDirectory(FolderName::tire);
}
std::vector<std::string> FileManagement::GetCarNames(const std::string tour)
{
	return ReadFile(FolderName::tour + '\\' + tour);
}
ToTGameConfig FileManagement::LoadGameConfig()
{
	ToTGameConfig ret = {};
	std::ifstream fvar;

	fvar.open(FolderName::main + '\\' + FileName::game_config);
	fvar >> ret.ais;
	fvar >> ret.name;
	fvar >> ret.lang;
	fvar >> ret.timer_settings;
	fvar.close();

	return ret;
}
ToTWindowConfig FileManagement::LoadWindowConfig()
{
	ToTWindowConfig ret = {};
	std::ifstream fvar;

	fvar.open(FolderName::main + '\\' + FileName::window_config);
	fvar >> ret.window_info.title;
	fvar >> ret.window_info.characters_capacity.X;
	fvar >> ret.window_info.characters_capacity.Y;
	fvar >> ret.window_info.main_color;
	fvar >> ret.window_info.secondary_color;
	fvar >> ret.window_info.visible_cursor;
	fvar >> reinterpret_cast<std::underlying_type<WindowMode>::type&>(ret.window_info.window_mode);
	fvar >> ret.music_volume;
	fvar >> ret.hamachi_flag;
	fvar.close();

	return ret;
}
void FileManagement::LoadLanguagePack(std::string path)
{
	std::ifstream fvar;
	std::string temp;
	int position = 0;

	LanguagePack::text[position].clear();
	
	fvar.open(path.c_str());
	while (std::getline(fvar, temp))
	{
		if (temp == LanguagePack::language_pack_separator)
		{
			++position;
			LanguagePack::text[position].clear();
		}
		else
		{
			LanguagePack::text[position].push_back(temp);
		}
	}
	fvar.close();
}

std::vector<std::string> FileManagement::GetTourParameters(std::string tour, int position, const int visibility)
{
	std::vector<std::string> ret = {};
	std::ifstream fvar;
	std::string temp;

	fvar.open((FolderName::tour + '\\' + tour).c_str());
	
	//skip "allowed cars" section
	do
	{
		std::getline(fvar, temp);
	} while (temp != "");

	//skip segments before specified position
	for (int i = 0; i < position; ++i)
	{
		std::getline(fvar, temp);
	}

	//read and save important data
	for (int i = 0; i < visibility; ++i)
	{
		if (!std::getline(fvar, temp))
		{
			break;
		}
		ret.push_back(std::move(temp));
	}
	fvar.close();
	return ret;
}
std::vector<int> FileManagement::GetCarParameters(const std::string path)
{
	const std::vector<std::string> data = ReadFile(FolderName::car + '\\' + path);
	std::vector<int> car_parameters;

	for (short i = 0; i < CarAttributes::last; ++i)
	{
		car_parameters.push_back(atoi(data[i].c_str()));
	}
	return car_parameters;
}
std::vector<std::string> FileManagement::GetTireParameters(const std::string path)
{
	return ReadFile(FolderName::tire + '\\' + path);
}

void FileManagement::SaveGameConfig(const ToTGameConfig &game_config)
{
	std::string name = game_config.name;
	std::ofstream fvar;

	for (int i = 0; i < static_cast<int>(name.size()); ++i)
	{
		if (name[i] == ' ')
		{
			name[i] = '_';
		}
	}
	fvar.open(FolderName::main + '\\' + FileName::game_config);
	fvar << game_config.ais << '\n';
	fvar << name + '\n';
	fvar << game_config.lang + '\n';
	fvar << game_config.timer_settings << '\n';
	fvar.close();
}
void FileManagement::SaveWindowConfig(const ToTWindowConfig& window_config)
{
	std::string title = window_config.window_info.title;
	std::ofstream fvar;

	for (int i = 0; i < static_cast<int>(title.size()); ++i)
	{
		if (title[i] == ' ')
		{
			title[i] = '_';
		}
	}
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


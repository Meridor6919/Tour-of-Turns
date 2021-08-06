#include "FileManagementFunctions.h"
#include "ValidationFunctions.h"

using namespace MeridorConsoleLib;

std::vector<std::string> FileManagement::GetAINames()
{
	std::vector<std::string> ret = GetFilesInDirectory(FolderName::ai);
	RemoveExtension(ret, ExtName::ai);
	return ret;
}
std::vector<std::string> FileManagement::GetTourNames()
{
	std::vector<std::string> ret = GetFilesInDirectory(FolderName::tour);
	RemoveExtension(ret, ExtName::tour);
	return ret;
}
std::vector<std::string> FileManagement::GetTireNames()
{
	std::vector<std::string> ret = GetFilesInDirectory(FolderName::tire);
	RemoveExtension(ret, ExtName::tire);
	return ret;
}
std::vector<std::string> FileManagement::GetCarNames(const std::string& tour)
{
	return ReadFile(FolderName::tour + '\\' + tour);
}
std::vector<std::string> FileManagement::GetTitleThemeNames()
{
	std::vector<std::string> ret = {};
	std::ifstream fvar;
	std::string temp;

	fvar.open(FolderName::main + '\\' + FileName::title_theme);

	while (std::getline(fvar, temp))
	{
		ret.push_back(GetSeparatedValue(temp, 0, '\t'));
	}
	return ret;
}
ToTGameConfig FileManagement::LoadGameConfig()
{
	ToTGameConfig ret = {};
	std::ifstream fvar;

	fvar.open(FolderName::main + '\\' + FileName::game_config);
	IntegerGetline(fvar, ret.number_of_ais);
	StringGetline(fvar, ret.name);
	IntegerGetline(fvar, ret.timer_settings);
	fvar.close();

	return ret;
}
ToTWindowConfig FileManagement::LoadWindowConfig()
{
	ToTWindowConfig ret = {};
	std::ifstream fvar;

	fvar.open(FolderName::main + '\\' + FileName::window_config);
	StringGetline(fvar, ret.window_info.title);
	StringGetline(fvar, ret.theme_name);
	StringGetline(fvar, ret.lang);
	StringGetline(fvar, ret.ai_module);
	IntegerGetline(fvar, ret.windowed_mode_window_size.X);
	IntegerGetline(fvar, ret.windowed_mode_window_size.Y);
	IntegerGetline(fvar, ret.window_info.main_color);
	IntegerGetline(fvar, ret.window_info.secondary_color);
	IntegerGetline(fvar, reinterpret_cast<std::underlying_type<WindowMode>::type&>(ret.window_info.window_mode));
	FloatGetline(fvar, ret.music_volume);
	IntegerGetline(fvar, ret.hamachi_flag);
	fvar.close();

	return ret;
}
TitleTheme FileManagement::GetTitleThemeFromString(const std::string& title_theme)
{
	TitleTheme ret;

	ret.name = GetSeparatedValue(title_theme, 0, '\t');
	ret.main_left = GetSeparatedValue(title_theme, 1, '\t');
	ret.main_right = GetSeparatedValue(title_theme, 2, '\t');
	ret.secondary_left = GetSeparatedValue(title_theme, 3, '\t');
	ret.secondary_right = GetSeparatedValue(title_theme, 4, '\t');
	ret.decoration_distance = atoi(GetSeparatedValue(title_theme, 5, '\t').c_str());
	ret.decoration_wobble = atoi(GetSeparatedValue(title_theme, 6, '\t').c_str());

	for (short j = 0; j < ret.main_left.size(); ++j)
	{
		if (ret.main_left[j] == ' ')
		{
			ret.secondary_pos_left = j;
			break;
		}
	}
	for (short j = 0; j < ret.main_right.size(); ++j)
	{
		if (ret.main_right[j] == ' ')
		{
			ret.secondary_pos_right = j;
			break;
		}
	}
	return ret;
}
std::string FileManagement::GetStringFromTitleTheme(const TitleTheme& title_theme)
{
	std::string ret = title_theme.name + '\t'
		+ title_theme.main_left + '\t'
		+ title_theme.main_right + '\t'
		+ title_theme.secondary_left + '\t'
		+ title_theme.secondary_right + '\t'
		+ std::to_string(title_theme.decoration_distance) + '\t'
		+ std::to_string(title_theme.decoration_distance) + '\t';

	return ret;
}
TitleTheme FileManagement::LoadTitleTheme(const std::string& theme_name)
{
	std::ifstream fvar;
	std::string temp;

	fvar.open(FolderName::main + '\\' + FileName::title_theme);

	while (std::getline(fvar, temp))
	{
		if (GetSeparatedValue(temp, 0, '\t') == theme_name)
		{
			return GetTitleThemeFromString(temp);
		}
	}
	return TitleTheme();
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

void FileManagement::SaveGameConfig(const ToTGameConfig& game_config)
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
	fvar << game_config.number_of_ais << '\n';
	fvar << name + '\n';
	fvar << game_config.timer_settings;
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
	fvar << window_config.theme_name << '\n';
	fvar << window_config.lang << '\n';
	fvar << window_config.ai_module << '\n';
	fvar << window_config.windowed_mode_window_size.X << '\n';
	fvar << window_config.windowed_mode_window_size.Y << '\n';
	fvar << static_cast<int>(window_config.window_info.main_color) << '\n';
	fvar << static_cast<int>(window_config.window_info.secondary_color) << '\n';
	fvar << static_cast<int>(window_config.window_info.window_mode) << '\n';
	fvar << window_config.music_volume << '\n';
	fvar << window_config.hamachi_flag;
	fvar.close();
}


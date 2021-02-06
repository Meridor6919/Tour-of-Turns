#include "ToT_Window.h"


void ToT_Window::Init()
{
	ToTWindowConfig tot_window_config = LoadWindowConfig();
	SanitizeToTWindowConfig(tot_window_config);
	tot_window_config.window_info.visible_cursor = false;

	window_info = tot_window_config.window_info;
	window_info.handle = GetStdHandle(STD_OUTPUT_HANDLE);
	window_info.hwnd = GetConsoleWindow();
	music_volume = tot_window_config.music_volume;
	hamachi_flag = tot_window_config.hamachi_flag;

	tot_game_config = LoadGameConfig();
	SanitizeToTGameConfig(tot_game_config);

	title_theme = LoadTitleTheme(tot_window_config.title_theme);
	SanitizeTitleTheme(title_theme);

	wav_transformer.Init(FolderName::main + '\\' + FileName::music);
	
	SetMusic(music_volume);
	SetLanguage(tot_game_config.lang);

	Window::Init(window_info);
}
const ToTGameConfig& ToT_Window::GetToTGameConfig()
{
	return tot_game_config;
}
const TitleTheme& ToT_Window::GetTitleTheme()
{
	return title_theme;
}
ToTWindowConfig ToT_Window::GetToTWindowConfig()
{
	return { music_volume, hamachi_flag, title_theme.name, window_info };
}
bool ToT_Window::GetHamachiConnectionFlag()
{
	return hamachi_flag;
}
int ToT_Window::GetAIs()
{
	return tot_game_config.ais;
}
std::string ToT_Window::GetName()
{
	return tot_game_config.name;
}
int ToT_Window::GetTimerSettings()
{
	return tot_game_config.timer_settings;
}
std::string ToT_Window::GetLanguage()
{
	return tot_game_config.lang;
}
float ToT_Window::GetMusicVolume()
{
	return music_volume;
}
void ToT_Window::SetTitleTheme(std::string name)
{
	title_theme = LoadTitleTheme(name);
	SanitizeTitleTheme(title_theme);
}
void ToT_Window::SetHamachiConnectionFlag(const bool flag)
{
	hamachi_flag = flag;
}
void ToT_Window::SetAIs(int number_of_ais)
{
	tot_game_config.ais = number_of_ais;
}
void ToT_Window::SetName(std::string name)
{
	tot_game_config.name = name;
}
void ToT_Window::SetTimerSettings(int timer_settings)
{
	tot_game_config.timer_settings = timer_settings;
}
void ToT_Window::SetLanguage(std::string lang)
{
	tot_game_config.lang = lang;
	LoadLanguagePack(FolderName::language + '\\' + lang);
	if (!ValidateLanguagePack())
	{
		MessageBox(0, (lang + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), MB_TOPMOST);

		std::vector<std::string> languages = GetFilesInDirectory(FolderName::language);
		bool no_valid_lang_packs = true;
		for (int i = 0; i < static_cast<int>(languages.size()); ++i)
		{
			LoadLanguagePack(FolderName::language + '\\' + languages[i]);
			if (ValidateLanguagePack())
			{
				no_valid_lang_packs = false;
				tot_game_config.lang = languages[i];
				MessageBox(0, (languages[i] + ErrorMsg::placeholder_language).c_str(), ErrorTitle::placeholder_language.c_str(), MB_TOPMOST);
				break;
			}
		}
		if (no_valid_lang_packs)
		{
			MessageBox(0, ErrorMsg::language_error.c_str(), ErrorTitle::language_error.c_str(), MB_TOPMOST);
			exit(0);
		}
	}
}
void ToT_Window::SetMusic(float volume)
{
	music_volume = volume;
	wav_transformer.SetFlags(SND_ASYNC | SND_LOOP);
	wav_transformer.StartPlaying(volume);
}

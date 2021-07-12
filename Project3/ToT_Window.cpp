#include "ToT_Window.h"


void ToT_Window::Init()
{
	
	Validation::Status status;

	ValidationCheck::FileIntegrity(status);

	ToTWindowConfig tot_window_config = LoadWindowConfig();
	ValidationCheck::ToTWindowConfig(tot_window_config, status);

	tot_window_config.window_info.visible_cursor = false;
	window_info = tot_window_config.window_info;
	window_info.handle = GetStdHandle(STD_OUTPUT_HANDLE);
	window_info.hwnd = GetConsoleWindow();
	music_volume = tot_window_config.music_volume;
	hamachi_flag = tot_window_config.hamachi_flag;
	ai_module = tot_window_config.ai_module;

	tot_game_config = LoadGameConfig();
	ValidationCheck::ToTGameConfig(tot_game_config, status);

	ValidationCheck::TitleThemes(status);
	ValidationCheck::Rankings(status);
	ValidationCheck::LanguagePacks(status);
	ValidationCheck::GameFiles(status);

	title.Init(this);
	title.SetTheme(tot_window_config.theme_name);
	

	if (status.IsFlagActive(Validation::Status::Flags::no_music))
	{
		music_volume = 0.0f;
	}
	else
	{
		wav_transformer.Init(FolderName::main + '\\' + FileName::music);
	}
	if (status.IsFlagActive(Validation::Status::Flags::corrupted))
	{
		exit(0);
	}


	SetMusic(music_volume);
	SetLanguage(tot_game_config.lang);
	Window::Init(window_info);
}
void ToT_Window::DrawTitle(bool clear)
{
	if (clear)
	{
		title.ShowDecoration(true);
	}
	else
	{
		title.ShowMainPart();
		title.ShowDecoration(false);
	}
}
std::string ToT_Window::GetTitleTheme()
{
	return title.GetTheme().name;
}
void ToT_Window::SetTitleTheme(std::string name)
{
	title.SetTheme(name);
}
const ToTGameConfig& ToT_Window::GetToTGameConfig()
{
	return tot_game_config;
}
ToTWindowConfig ToT_Window::GetToTWindowConfig()
{
	return { ai_module, music_volume, hamachi_flag, GetTitleTheme(), window_info };
}
std::string ToT_Window::GetAIModule()
{
	return ai_module;
}
bool ToT_Window::GetHamachiConnectionFlag()
{
	return hamachi_flag;
}
int ToT_Window::GetAIs()
{
	return tot_game_config.number_of_ais;
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
void ToT_Window::SetAIModule(std::string ai_module)
{
	this->ai_module = ai_module;
}
void ToT_Window::SetHamachiConnectionFlag(const bool flag)
{
	hamachi_flag = flag;
}
void ToT_Window::SetAIs(int number_of_ais)
{
	tot_game_config.number_of_ais = number_of_ais;
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
}
void ToT_Window::SetMusic(float volume)
{
	music_volume = volume;
	wav_transformer.SetFlags(SND_ASYNC | SND_LOOP);
	wav_transformer.StartPlaying(volume);
}

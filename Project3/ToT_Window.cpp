#include "ToT_Window.h"

ToT_Window::ToT_Window(const WindowInfoEx& window_info) : Window(window_info)
{
	wav_transformer.Init(FolderName::main + '\\' + FileName::music);
	window_config = LoadWindowConfig(main_color, secondary_color);
	wav_transformer.StartPlaying(window_config.music_volume);
}
bool ToT_Window::GetHamachiConnectionFlag()
{
	return window_config.hamachi_flag;
}
int ToT_Window::GetAIs()
{
	return window_config.ais;
}
std::string ToT_Window::GetName()
{
	return window_config.name;
}
int ToT_Window::GetTimerSettings()
{
	return window_config.timer_settings;
}
std::string ToT_Window::GetLanguage()
{
	return window_config.lang;
}
float ToT_Window::GetMusicVolume()
{
	return window_config.music_volume;
}
void ToT_Window::SetHamachiConnectionFlag(const bool flag)
{
	window_config.hamachi_flag = flag;
}
void ToT_Window::SetAIs(int number_of_ais)
{
	window_config.ais = number_of_ais;
}
void ToT_Window::SetName(std::string name)
{
	window_config.name = name;
}
void ToT_Window::SetTimerSettings(int timer_settings)
{
	window_config.timer_settings = timer_settings;
}
void ToT_Window::SetLanguage(std::string lang)
{
	window_config.lang = lang;
	LoadLanguagePack(FolderName::language + '\\' + lang);
	if (!ValidateLanguagePack())
	{
		MessageBox(0, (lang + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);

		std::vector<std::string> languages = ReadFile(FolderName::language + '\\' + FileName::language);
		bool no_valid_lang_packs = true;
		for (int i = 0; i < static_cast<int>(languages.size()); ++i)
		{
			LoadLanguagePack(FolderName::language + '\\' + languages[i]);
			if (!ValidateLanguagePack())
			{
				no_valid_lang_packs = false;
				window_config.lang = languages[i];
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
}
void ToT_Window::SetMusic(float volume)
{
	window_config.music_volume = volume;
	wav_transformer.SetFlags(SND_ASYNC | SND_LOOP);
	wav_transformer.StartPlaying(volume);
}

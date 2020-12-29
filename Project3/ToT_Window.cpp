#include "ToT_Window.h"

ToT_Window::ToT_Window(const WindowInfoEx& window_info) : Window(window_info)
{
	playable = true;
	enable_ranking = true;
	const std::string error_msg = " " + ErrorMsg::missing_file;
	const char* errot_title = ErrorTitle::missing_file.c_str();

	if (!SaveFileNames(FolderName::tour, FolderName::tour + '\\' + FileName::ranking, ExtName::ranking, window_info.handle))
	{
		enable_ranking = false;
	}
	if (!SaveFileNames(FolderName::tire, FolderName::tire + '\\' + FileName::tire, ExtName::tire, window_info.handle))
	{
		MessageBox(0, (ExtName::tire +error_msg).c_str(), errot_title, 0);
		playable = false;
	}
	if (!SaveFileNames(FolderName::tour, FolderName::tour + '\\' + FileName::tour, ExtName::tour, window_info.handle))
	{
		MessageBox(0, (ExtName::tour + error_msg).c_str(), errot_title, 0);
		playable = false;
	}
	if (!SaveFileNames(FolderName::language, FolderName::language + '\\' + FileName::language, ExtName::language, window_info.handle))
	{
		MessageBox(0, ErrorMsg::language_error.c_str(), ErrorTitle::language_error.c_str(), 0);
		exit(0);
	}
	enable_ranking = enable_ranking * ValidateRanking();
	playable = playable * ValidateGameFiles();
	wav_transformer.Init(FolderName::main + '\\' + FileName::music);
	auto ret = LoadWindowConfig();
	ais = ret.ais;
	hamachi_enabled = ret.hamachi_flag;
	lang = ret.lang;
	this->window_info.main_color = ret.main_color;
	music_volume = ret.music_volume;
	name = ret.name;
	this->window_info.secondary_color = ret.secondary_color;
	timer_settings = ret.timer_settings;

	wav_transformer.StartPlaying(music_volume);

	const COORD infobox_position = { 0,static_cast<short>(GetCharactersPerColumn() - 12) };
	InfoBoxDesc infobox_info = { 10, {1, static_cast<short>(GetCharactersPerColumn() - 18), 85, static_cast<short>(GetCharactersPerColumn()-2)}, 2 };
	this->infobox = std::make_shared<InfoBox>(infobox_info, window_info);
}
void ToT_Window::Title(const COORD starting_point, const TextAlign text_align)
{
	const COORD orientation_point = { starting_point.X - static_cast<short>(static_cast<float>(text_align) / 2.0f * LanguagePack::text[LanguagePack::title_main][0].size()), starting_point.Y };
	const short decoration_distance = 5;
	const std::string main_decoration = "{ }";
	const std::string additional_decoration = "*";
	const short main_title_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main].size());
	const short additional_title_size = static_cast<short>(LanguagePack::text[LanguagePack::title_additional].size());
	//Main text
	SetConsoleTextAttribute(window_info.handle, window_info.secondary_color);
	for (short i = 0; i < main_title_size; ++i)
	{
		SetConsoleCursorPosition(window_info.handle, { orientation_point.X, orientation_point.Y + i });
		std::cout << LanguagePack::text[LanguagePack::title_main][i];
	}
	SetConsoleTextAttribute(window_info.handle, window_info.main_color);
	for (short i = 0; i < additional_title_size; ++i)
	{
		const short main_line_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main][i].size());
		const short additional_line_size = static_cast<short>(LanguagePack::text[LanguagePack::title_additional][i].size());
		SetConsoleCursorPosition(window_info.handle, { orientation_point.X + main_line_size / 2 - additional_line_size / 2, orientation_point.Y + i + main_title_size / 3 });
		std::cout << LanguagePack::text[LanguagePack::title_additional][i];
	}
	//Decoration
	for (short i = 0; i < main_title_size; ++i)
	{
		const short decoration_size = static_cast<short>(decoration_distance + main_decoration.size());
		const short line_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main][i].size());
		SetConsoleTextAttribute(window_info.handle, window_info.secondary_color);
		SetConsoleCursorPosition(window_info.handle, { orientation_point.X - decoration_size - i % 2, orientation_point.Y + i });
		std::cout << main_decoration;
		SetConsoleCursorPosition(window_info.handle, { orientation_point.X + line_size + decoration_distance - 1 - i % 2, orientation_point.Y + i });
		std::cout << main_decoration;
		SetConsoleTextAttribute(window_info.handle, window_info.main_color);
		SetConsoleCursorPosition(window_info.handle, { orientation_point.X - decoration_size + 1 - i % 2, orientation_point.Y + i });
		std::cout << additional_decoration;
		SetConsoleCursorPosition(window_info.handle, { orientation_point.X + line_size + decoration_distance - i % 2, orientation_point.Y + i });
		std::cout << additional_decoration;
	}
}
bool ToT_Window::GetHamachiConnectionFlag()
{
	return hamachi_enabled;
}
int ToT_Window::GetAIs()
{
	return ais;
}
std::string ToT_Window::GetName()
{
	return name;
}
int ToT_Window::GetTimerSettings()
{
	return timer_settings;
}
std::string ToT_Window::GetLanguage()
{
	return lang;
}
bool ToT_Window::RankingFound()
{
	return enable_ranking;
}
bool ToT_Window::IsPlayable()
{
	return playable;
}
bool ToT_Window::GetMultiplayer()
{
	return multiplayer;
}
float ToT_Window::GetMusicVolume()
{
	return music_volume;
}
void ToT_Window::SetHamachiConnectionFlag(const bool flag)
{
	hamachi_enabled = flag;
}
void ToT_Window::SetAIs(int number_of_ais)
{
	ais = number_of_ais;
}
void ToT_Window::SetName(std::string name)
{
	this->name = name;
}
void ToT_Window::SetTimerSettings(int timer_settings)
{
	this->timer_settings = timer_settings;
}
void ToT_Window::SetLanguage(std::string lang)
{
	this->lang = lang;
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
				lang = languages[i];
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
void ToT_Window::SetMultiplayer(bool multiplayer)
{
	this->multiplayer = multiplayer;
}
void ToT_Window::SetMusic(float volume)
{
	this->music_volume = volume;
	wav_transformer.SetFlags(SND_ASYNC | SND_LOOP);
	wav_transformer.StartPlaying(volume);
}

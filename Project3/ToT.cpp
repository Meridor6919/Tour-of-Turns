#include "ToT.h"

ToT::ToT()
{
	main_window = std::make_shared<ToT_Window>();
	main_window->Init();

	playable = ValidateGameFiles();
	ValidateRanking();

	handle = main_window->GetHandle();
	game_window_center = static_cast<short>(main_window->GetCharactersPerRow()) / 2;
}
void ToT::ShowRankingDetails(std::string tour, int racer_pos, int classification_type, bool clearing)
{
	const int spacing = 2;
	const int paragraph_size = 2;
	const COORD base_position = { 0, 19 };
	std::vector<std::string> details = RankingManagement::GetTextToDisplay(tour, racer_pos, classification_type);

	if (clearing)
	{
		const int border_size = GameConstants::box_width;
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + 1 });
		std::cout<<Spaces(border_size);
		for (short i = 0; i < static_cast<short>(Validation::ranking_details); ++i)
		{
			SetConsoleCursorPosition(handle, { base_position.X + paragraph_size, base_position.Y + spacing * (i + 2) });
			std::cout << Spaces(static_cast<short>(LanguagePack::text[LanguagePack::ranking_details][i].size()) + static_cast<short>(details[i].size()) + 2);
		}
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + spacing * (static_cast<short>(Validation::ranking_details) + 2) });
		std::cout << Spaces(border_size);
	}
	else
	{
		SetConsoleTextAttribute(handle, *main_window->secondary_color);
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + 1 });
		std::cout << GetMonoCharacterString(GameConstants::box_width, '_');
		for (short i = 0; i < static_cast<short>(Validation::ranking_details); ++i)
		{
			SetConsoleCursorPosition(handle, { base_position.X + paragraph_size, base_position.Y + spacing * (i + 2) });
			SetConsoleTextAttribute(handle, *main_window->main_color);
			std::cout << LanguagePack::text[LanguagePack::ranking_details][i] + ": ";
			SetConsoleTextAttribute(handle, *main_window->secondary_color);
			std::cout << details[i];
		}
		SetConsoleTextAttribute(handle, *main_window->secondary_color);
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + spacing * (static_cast<short>(Validation::ranking_details) + 2) });
		std::cout << GetMonoCharacterString(GameConstants::box_width, '_');
	}
}
void ToT::MainMenu()
{
	main_window->DrawTitle();
	while (true)
	{
		Text::TextInfo text_info = { LanguagePack::text[LanguagePack::main_menu], main_menu_position, { game_window_center + 1, 25 }, TextAlign::center, 3, true};
		switch (main_menu_position = Text::Choose::Veritcal(text_info, *main_window->GetWindowInfo()))
		{
		case 0://Singleplayer
		case 1://Multiplayer
		{
			if (playable)
			{
				Game(main_menu_position);
			}
			else
			{
				MessageBox(0, ErrorMsg::unplayable, ErrorTitle::unplayable, MB_TOPMOST);
			}
			break;
		}
		case 2://AI battles
		{
			MessageBoxA(0, ErrorMsg::comming_soon_tm, ErrorTitle::comming_soon_tm, MB_TOPMOST);
			break;
		}
		case 3://Gamepedia
		{
			Info();
			break;
		}
		case 4://Ranking
		{
			Ranking();
			break;
		}
		case 5://Options
		{
			Options();
			break;
		}
		case 6://Credits
		{
			Credits();
			break;
		}
		case 7://Exit
		{
			return;
		}
		}
	}
}
void ToT::Credits()
{
	Text::TextInfo text_info = { LanguagePack::text[LanguagePack::credits], 0, { game_window_center,25 }, TextAlign::center, 3, 0 };
	OrdinaryText(text_info, *main_window->GetWindowInfo());
	_getch();
	text_info.clear_after = true;
	OrdinaryText(text_info, *main_window->GetWindowInfo());
}
void ToT::Options()
{
	unsigned int main_menu_position = 0;
	const COORD starting_point = { game_window_center+1, 25 };
	const short spacing = 3;
	bool loop = true;
	Text::TextInfo text_info = { LanguagePack::text[LanguagePack::general_options], main_menu_position, starting_point, TextAlign::center, spacing, false };

	while (loop)
	{
		main_menu_position = Text::Choose::Veritcal(text_info, *main_window->GetWindowInfo());
		const short submenu_horizontal_position = static_cast<short>(LanguagePack::text[LanguagePack::general_options][main_menu_position].size()) / 2 + 3;
		const short game_window_vertical_position = static_cast<short>(main_menu_position * spacing);
		const COORD local_starting_point = { starting_point.X + submenu_horizontal_position, starting_point.Y + game_window_vertical_position };
		switch (main_menu_position)
		{
			case 0: //Title theme
			{
				std::vector<std::string> text = GetTitleThemeNames();
				const std::string theme_name = main_window->GetTitleTheme();
				int current_theme_pos = 0;
				for (int i = 0; i < text.size(); ++i)
				{
					if (text[i] == theme_name)
					{
						current_theme_pos = i;
						break;
					}
				}
				Text::TextInfo text_info = { text, current_theme_pos, local_starting_point, TextAlign::left, 0, true };
				int new_theme_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
				if (current_theme_pos != new_theme_pos)
				{
					main_window->DrawTitle(true);
					main_window->SetTitleTheme(text[new_theme_pos]);
					main_window->DrawTitle();
				}
				break;
			}
			case 1://set primary color
			{
				std::vector<std::string> local_text = LanguagePack::text[LanguagePack::selectable_colors];
				const int starting_color = *main_window->main_color;
				local_text.erase(local_text.begin() + (*main_window->secondary_color - 1));
				unsigned int pos = static_cast<unsigned int>(*main_window->main_color - 1 - (*main_window->main_color > *main_window->secondary_color));
				Text::TextInfo text_info = { local_text, pos, local_starting_point, TextAlign::left, 0, true};
				(*main_window->main_color) = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo()) + 1;
				if (*main_window->main_color >= *main_window->secondary_color)
				{
					(*main_window->main_color) = (*main_window->main_color) + 1;
				}
				if (starting_color != *main_window->main_color)
				{
					main_window->DrawTitle();
				}
				break;
			}
			case 2://set secondary color
			{
				std::vector<std::string> local_text = LanguagePack::text[LanguagePack::selectable_colors];
				const int starting_color = *main_window->secondary_color;
				local_text.erase(local_text.begin() + (*main_window->main_color - 1));
				unsigned int pos = static_cast<unsigned int>(*main_window->secondary_color - 1 - (*main_window->secondary_color > *main_window->main_color));
				Text::TextInfo text_info = { local_text, pos, local_starting_point, TextAlign::left, 0, true };
				(*main_window->secondary_color) = (Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo()) + 1);
				if (*main_window->secondary_color >= *main_window->main_color)
				{
					(*main_window->secondary_color) = (*main_window->secondary_color) + 1;
				}
				if (starting_color != *main_window->secondary_color)
				{
					main_window->DrawTitle();
				}
				break;
			}
			case 3://set music
			{
				std::vector<std::string> text = { LanguagePack::text[LanguagePack::on_off][1] };
				for (int i = 1; i < 11; ++i)
				{
					text.push_back(std::to_string(i));
				}
				const float starting_volume = main_window->GetMusicVolume();
				Text::TextInfo text_info = { text, static_cast<size_t>(main_window->GetMusicVolume() * 10), local_starting_point, TextAlign::left, 0, true };
				const float current_volume = static_cast<float>(Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo())) / 10.0f;
				if(starting_volume != current_volume)
				{
					main_window->SetMusic(current_volume);
				}
				break;
			}
			case 4://language
			{
				std::vector<std::string> language = GetFilesInDirectory(FolderName::language);
				unsigned int starting_pos = 0;
				for (; starting_pos < static_cast<int>(language.size()); ++starting_pos)
				{
					if (language[starting_pos] == main_window->GetLanguage())
					{
						break;
					}
				}
				RemoveExtension(language, ExtName::language);
				Text::TextInfo text_info = { language, starting_pos, local_starting_point, TextAlign::left, 0, true };
				int current_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
				if (current_pos != starting_pos)
				{
					main_window->SetLanguage(language[current_pos] + ExtName::language);
					system("cls");
					main_window->DrawTitle();
				}
				break;
			}
			case 5://set hamachi flag
			{
				Text::TextInfo text_info = { LanguagePack::text[LanguagePack::on_off], !main_window->GetHamachiConnectionFlag(), local_starting_point, TextAlign::left, 0, true };
				main_window->SetHamachiConnectionFlag(!(Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo())));
				break;
			}
			case 6://display settings
			{
				unsigned int current_display_mode = static_cast<int>(main_window->GetWindowMode());
				Text::TextInfo text_info = { LanguagePack::text[LanguagePack::display_settings], current_display_mode, local_starting_point, TextAlign::left, 0, true };
				int new_display_mode = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
				if (new_display_mode != current_display_mode)
				{
					main_window->SetWindowMode(static_cast<MeridorConsoleLib::WindowMode>(new_display_mode));
				}
				break;
			}
			case 7://AI module
			{
				std::vector<std::string> ai_modules = GetAINames();
				std::string current_module_name = main_window->GetAIModule();
				int module_pos;
				for (int i = 0; i < ai_modules.size(); ++i)
				{
					if (ai_modules[i] == current_module_name)
					{
						module_pos = i;
					}
					RemoveExtension(ai_modules[i], ExtName::ai);
				}
				Text::TextInfo text_info = { ai_modules, module_pos, local_starting_point, TextAlign::left, 0, true };
				module_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
				main_window->SetAIModule(ai_modules[module_pos] + ExtName::ai);
				break;
			}
			case 8://clearing
			{
				loop = false;
				break;
			}
		}
	}
	Text::Choose::VerticalClearGUI(text_info, *main_window->GetWindowInfo());
	SaveWindowConfig(main_window->GetToTWindowConfig());
}
void ToT::Ranking()
{
	unsigned int main_menu_position = 0;
	const COORD starting_point = { game_window_center + 1, 25 };
	const short spacing = 3;
	bool loop = true;

	std::vector<std::string> maps = GetFilesInDirectory(FolderName::ranking);
	unsigned int map_pos = 0;
	RemoveExtension(maps, ExtName::ranking);

	unsigned int racer_pos = 0;
	unsigned int classification_type = 0;

	ShowRankingDetails(FolderName::ranking + '\\' + maps[map_pos] + ExtName::ranking, racer_pos, classification_type);
	Text::TextInfo text_info = { LanguagePack::text[LanguagePack::ranking_menu], main_menu_position, starting_point, TextAlign::center, spacing, false };
	while (loop)
	{
		const std::string starting_map_path = FolderName::ranking + '\\' + maps[map_pos] + ExtName::ranking;
		main_menu_position = Text::Choose::Veritcal(text_info, *main_window->GetWindowInfo());
		const short submenu_horizontal_position = static_cast<short>(LanguagePack::text[LanguagePack::ranking_menu][main_menu_position].size()) / 2 + 3;
		const short game_window_vertical_position = static_cast<short>(main_menu_position * spacing);
		const COORD local_starting_point = { starting_point.X + submenu_horizontal_position, starting_point.Y + game_window_vertical_position };
		switch (main_menu_position)
		{
			case 0://Map
			{
				const int temp = map_pos;
				Text::TextInfo text_info = { maps, map_pos, local_starting_point, TextAlign::left, 0, true };
				map_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
				if (map_pos != temp)
				{
					ShowRankingDetails(starting_map_path, racer_pos, classification_type, true);
					racer_pos = 0;
					ShowRankingDetails(FolderName::ranking + '\\' + maps[map_pos] + ExtName::ranking, racer_pos, classification_type);
				}
				break;
			}
			case 1://Player
			{
				const int temp = racer_pos;
				Text::TextInfo text_info = { RankingManagement::GetRankedRacersNames(FolderName::ranking + '\\' + maps[map_pos] + ExtName::ranking), racer_pos, local_starting_point, TextAlign::left, 0, true };
				racer_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
				if(temp != racer_pos && racer_pos >= 0)
				{
					ShowRankingDetails(starting_map_path, temp, classification_type, true);
					ShowRankingDetails(starting_map_path, racer_pos, classification_type);
				}
				break;
			}
			case 2://Classification type
			{
				const int temp = classification_type;
				Text::TextInfo text_info = { LanguagePack::text[LanguagePack::ranking_classification_types], classification_type, local_starting_point, TextAlign::left, 0, true };
				classification_type = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
				if (temp != classification_type)
				{
					ShowRankingDetails(starting_map_path, racer_pos, temp, true);
					ShowRankingDetails(starting_map_path, racer_pos, classification_type);
				}
				break;
			}
			case 3://Reset map details
			{
				ShowRankingDetails(starting_map_path, racer_pos, classification_type, true);
				std::fstream fvar((FolderName::ranking + '\\' + maps[map_pos] + ExtName::ranking).c_str(), std::ios::out);
				ShowRankingDetails(starting_map_path, racer_pos, classification_type);
				fvar.close();
				break;
			}
			case 4://Back
			{
				ShowRankingDetails(starting_map_path, racer_pos, classification_type, true);
				loop = false;
				break;
			}
		}
		Text::Choose::VerticalClearGUI(text_info, *main_window->GetWindowInfo());
	}
}
void ToT::Info()
{
	const TextAlign text_align_title = TextAlign::center;
	const TextAlign text_align_content = TextAlign::center;
	const short spacing = 3;
	const COORD text_pos = { game_window_center, 28 };
	const COORD title_pos = { game_window_center - 2, text_pos.Y - 3 };
	while (true)
	{
		Text::TextInfo ordinary_text_info = { LanguagePack::text[LanguagePack::gamepedia_introduction + info_pos], 0, text_pos, text_align_content, spacing, 0 };
		OrdinaryText(ordinary_text_info, *main_window->GetWindowInfo());
		Text::TextInfo text_info = { LanguagePack::text[LanguagePack::gamepedia_sections], info_pos, title_pos, text_align_title, 0, false };
		int temp_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
		ordinary_text_info.clear_after = true;
		OrdinaryText(ordinary_text_info, *main_window->GetWindowInfo());
		if (temp_pos == info_pos)
		{
			const short text_size = static_cast<short>(LanguagePack::text[LanguagePack::gamepedia_sections][info_pos].size());
			SetConsoleCursorPosition(handle, {title_pos.X-static_cast<short>(static_cast<float>(text_size)*GetTextAlignScalar(text_align_title)), title_pos.Y });
			std::cout << Spaces(text_size + 4);
			break;
		}
		info_pos = temp_pos;
	}
}
void ToT::Game(const bool multiplayer)
{
	std::unique_ptr<SinglePlayer> network_role;
	if (multiplayer)
	{	
		do
		{
			Text::TextInfo text_info = { LanguagePack::text[LanguagePack::multiplayer_menu], 0, { game_window_center, 25 }, TextAlign::center, 3, false };
			switch (Text::Choose::Veritcal(text_info, *main_window->GetWindowInfo()))
			{
				case 0:
				{
					network_role = std::make_unique<Host>(*main_window);
					bool temp = false;
					Text::Choose::VerticalClearGUI(text_info, *main_window->GetWindowInfo());
					break;
				}
				case 1:
				{
					bool temp = false;
					Text::Choose::VerticalClearGUI(text_info, *main_window->GetWindowInfo());
					network_role = std::make_unique<Client>(*main_window);
					break;
				}
				case 2:
				{
					bool temp = false;
					Text::Choose::VerticalClearGUI(text_info, *main_window->GetWindowInfo());
					return;
				}
			}
		} while (!network_role->isInit());
	}
	else
	{
		network_role = std::make_unique<SinglePlayer>(*main_window);
	}
	
	while (network_role->GameLobby())
	{
		network_role->Interface();
		network_role->Leaderboard(true);
		network_role->Leaderboard(false);

		for (int turn = 0; network_role->VisionBox(turn); turn++)
		{
			if (turn > 0)//attacks start with second turn 
			{
				network_role->AttackPhase();
			}
			network_role->ActionPhase();
			network_role->GetCurrentAttributes();
			network_role->Interface();
			network_role->Leaderboard(true);
			network_role->Leaderboard(false);
		}
		network_role->Finish();
		system("cls");
		main_window->DrawTitle();
	}
	system("cls");
	main_window->DrawTitle();
}
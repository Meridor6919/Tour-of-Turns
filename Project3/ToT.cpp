#include "ToT.h"

ToT::ToT()
{
	WindowInfoEx window_info = {};
	window_info.characters_capacity = { 240, 70 };
	window_info.handle = GetStdHandle(STD_OUTPUT_HANDLE);
	window_info.hwnd = GetConsoleWindow();
	window_info.main_color = 15;
	window_info.secondary_color = 10;
	window_info.title = "Tour of Turns";
	window_info.window_mode = WindowMode::windowed_fullscreen;
	window_info.visible_cursor = false;
	this->main_window = std::make_shared<ToT_Window>(window_info);

	CheckLangPackValid();
	CheckPlayablity();
	CheckRankingValid();

	handle = main_window->GetHandle();
	game_window_center = static_cast<short>(main_window->GetCharactersPerRow()) / 2;
}
void ToT::CheckPlayablity()
{
	const std::string error_msg = " " + ErrorMsg::missing_file;
	const char* errot_title = ErrorTitle::missing_file.c_str();

	playable = true;

	if (!SaveFileNames(FolderName::tire, FolderName::tire + '\\' + FileName::tire, ExtName::tire, main_window->GetHandle()))
	{
		MessageBox(0, (ExtName::tire + error_msg).c_str(), errot_title, MB_TOPMOST);
		playable = false;
	}
	if (!SaveFileNames(FolderName::tour, FolderName::tour + '\\' + FileName::tour, ExtName::tour, main_window->GetHandle()))
	{
		MessageBox(0, (ExtName::tour + error_msg).c_str(), errot_title, MB_TOPMOST);
		playable = false;
	}
	playable *= ValidateGameFiles();
}
void ToT::CheckRankingValid()
{
	ranking_enabled = true;

	if (!SaveFileNames(FolderName::tour, FolderName::tour + '\\' + FileName::ranking, ExtName::ranking, main_window->GetHandle()))
	{
		ranking_enabled = false;
	}

	ranking_enabled *= ValidateRanking();
}
void ToT::CheckLangPackValid()
{
	if (!SaveFileNames(FolderName::language, FolderName::language + '\\' + FileName::language, ExtName::language, main_window->GetHandle()))
	{
		MessageBox(0, ErrorMsg::language_error.c_str(), ErrorTitle::language_error.c_str(), MB_TOPMOST);
		exit(0);
	}
}
void ToT::ShowRankingDetails(std::string tour, int racer_pos, int classification_type, bool clearing)
{
	const int spacing = 2;
	const int paragraph_size = 2;
	const COORD base_position = { 0, 19 };
	std::vector<std::string> details = GetRankingDetails(tour, racer_pos, classification_type);
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
void ToT::Title()
{
	const COORD starting_point = { game_window_center, 1 };
	constexpr TextAlign text_align = TextAlign::center;
	const COORD orientation_point = { starting_point.X - static_cast<short>(static_cast<float>(text_align) / 2.0f * LanguagePack::text[LanguagePack::title_main][0].size()), starting_point.Y };
	const short decoration_distance = 5;
	const std::string main_decoration = "{ }";
	const std::string additional_decoration = "*";
	const short main_title_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main].size());
	const short additional_title_size = static_cast<short>(LanguagePack::text[LanguagePack::title_additional].size());
	const HANDLE handle = main_window->GetHandle();
	
	//Main text
	SetConsoleTextAttribute(handle, *main_window->secondary_color);
	for (short i = 0; i < main_title_size; ++i)
	{
		SetConsoleCursorPosition(handle, { orientation_point.X, orientation_point.Y + i });
		std::cout << LanguagePack::text[LanguagePack::title_main][i];
	}
	SetConsoleTextAttribute(handle, *main_window->main_color);
	for (short i = 0; i < additional_title_size; ++i)
	{
		const short main_line_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main][i].size());
		const short additional_line_size = static_cast<short>(LanguagePack::text[LanguagePack::title_additional][i].size());
		SetConsoleCursorPosition(handle, { orientation_point.X + main_line_size / 2 - additional_line_size / 2, orientation_point.Y + i + main_title_size / 3 });
		std::cout << LanguagePack::text[LanguagePack::title_additional][i];
	}
	//Decoration
	for (short i = 0; i < main_title_size; ++i)
	{
		const short decoration_size = static_cast<short>(decoration_distance + main_decoration.size());
		const short line_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main][i].size());
		SetConsoleTextAttribute(handle, *main_window->secondary_color);
		SetConsoleCursorPosition(handle, { orientation_point.X - decoration_size - i % 2, orientation_point.Y + i });
		std::cout << main_decoration;
		SetConsoleCursorPosition(handle, { orientation_point.X + line_size + decoration_distance - 1 - i % 2, orientation_point.Y + i });
		std::cout << main_decoration;
		SetConsoleTextAttribute(handle, *main_window->main_color);
		SetConsoleCursorPosition(handle, { orientation_point.X - decoration_size + 1 - i % 2, orientation_point.Y + i });
		std::cout << additional_decoration;
		SetConsoleCursorPosition(handle, { orientation_point.X + line_size + decoration_distance - i % 2, orientation_point.Y + i });
		std::cout << additional_decoration;
	}
}
void ToT::MainMenu()
{
	Title();
	while (true)
	{
		Text::TextInfo text_info = { LanguagePack::text[LanguagePack::main_menu], main_menu_position, { game_window_center + 1, 25 }, TextAlign::center, 3, true};
		switch (main_menu_position = Text::Choose::Veritcal(text_info, *main_window->GetWindowInfo()))
		{
		case 0:
		case 1:
		{
			if (playable)
			{
				Game(main_menu_position);
			}
			else
			{
				MessageBox(0, ErrorMsg::unplayable.c_str(), ErrorTitle::unplayable.c_str(), MB_TOPMOST);
			}
			break;
		}
		case 2:
		{
			Info();
			break;
		}
		case 3:
		{
			if (ranking_enabled)
			{
				Ranking();
			}
			else
			{
				MessageBox(0, ErrorMsg::ranking_missing.c_str(), ErrorTitle::missing_file.c_str(), MB_TOPMOST);
			}
			break;
		}
		case 4:
		{
			Options();
			break;
		}
		case 5:
		{
			Credits();
			break;
		}
		case 6:
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
	ClearOrdinaryText(text_info, *main_window->GetWindowInfo());
}
void ToT::Options()
{
	int main_menu_position = 0;
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
			case 0://set primary color
			{
				std::vector<std::string> local_text = LanguagePack::text[LanguagePack::selectable_colors];
				const int starting_color = *main_window->main_color;
				local_text.erase(local_text.begin() + (*main_window->secondary_color - 2));
				Text::TextInfo text_info = { local_text, *main_window->main_color - 2 - (*main_window->main_color > *main_window->secondary_color), local_starting_point, TextAlign::left, 0, true};
				(*main_window->main_color) = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo()) + 2;
				if (*main_window->main_color >= *main_window->secondary_color)
				{
					(*main_window->main_color) = (*main_window->main_color) + 1;
				}
				if (starting_color != *main_window->main_color)
				{
					Title();
				}
				break;
			}
			case 1://set secondary color
			{
				std::vector<std::string> local_text = LanguagePack::text[LanguagePack::selectable_colors];
				const int starting_color = *main_window->secondary_color;
				local_text.erase(local_text.begin() + (*main_window->main_color - 2));
				Text::TextInfo text_info = { local_text, *main_window->secondary_color - 2 - (*main_window->secondary_color > *main_window->main_color), local_starting_point, TextAlign::left, 0, true };
				(*main_window->secondary_color) = (Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo()) + 2);
				if (*main_window->secondary_color >= *main_window->main_color)
				{
					(*main_window->secondary_color) = (*main_window->secondary_color) + 1;
				}
				if (starting_color != *main_window->secondary_color)
				{
					Title();
				}
				break;
			}
			case 2://set music
			{
				std::vector<std::string> text = { LanguagePack::text[LanguagePack::on_off][1] };
				for (int i = 1; i < 11; ++i)
				{
					text.push_back(std::to_string(i));
				}
				const float starting_volume = main_window->GetMusicVolume();
				Text::TextInfo text_info = { text, static_cast<int>(main_window->GetMusicVolume() * 10), local_starting_point, TextAlign::left, 0, true };
				const float current_volume = static_cast<float>(Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo())) / 10.0f;
				if(starting_volume != current_volume)
				{
					main_window->SetMusic(current_volume);
				}
				break;
			}
			case 3://language
			{
				std::vector<std::string> language = ReadFile(FolderName::language+'\\'+FileName::language);
				int starting_pos = 0;
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
					Title();
				}
				break;
			}
			case 4://set hamachi flag
			{
				Text::TextInfo text_info = { LanguagePack::text[LanguagePack::on_off], !main_window->GetHamachiConnectionFlag(), local_starting_point, TextAlign::left, 0, true };
				main_window->SetHamachiConnectionFlag(!(Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo())));
				break;
			}
			case 5://clearing
			{
				loop = false;
				break;
			}
		}
	}
	Text::Choose::VerticalClearGUI(text_info, *main_window->GetWindowInfo());
	SaveWindowConfig(main_window->GetToTConfig(), *main_window->main_color, *main_window->secondary_color );
}
void ToT::Ranking()
{
	int main_menu_position = 0;
	const COORD starting_point = { game_window_center + 1, 25 };
	const short spacing = 3;
	bool loop = true;

	std::vector<std::string> maps = ReadFile(FolderName::tour + '\\' + FileName::ranking);
	int map_pos = 0;
	RemoveExtension(maps, ExtName::ranking);

	int racer_pos = 0;
	int classification_type = 0;

	ShowRankingDetails(FolderName::tour + '\\' + maps[map_pos] + ExtName::ranking, racer_pos, classification_type);
	Text::TextInfo text_info = { LanguagePack::text[LanguagePack::ranking_menu], main_menu_position, starting_point, TextAlign::center, spacing, false };
	while (loop)
	{
		const std::string starting_map_path = FolderName::tour + '\\' + maps[map_pos] + ExtName::ranking;
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
					ShowRankingDetails(FolderName::tour + '\\' + maps[map_pos] + ExtName::ranking, racer_pos, classification_type);
				}
				break;
			}
			case 1://Player
			{
				const int temp = racer_pos;
				Text::TextInfo text_info = { GetRankingNames(FolderName::tour + '\\' + maps[map_pos] + ExtName::ranking), racer_pos, local_starting_point, TextAlign::left, 0, true };
				racer_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
				if(temp != racer_pos)
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
				std::fstream fvar((FolderName::tour + '\\' + maps[map_pos] + ExtName::ranking).c_str(), std::ios::out);
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
	const Text::TextInfo ordinary_text_info = { LanguagePack::text[LanguagePack::gamepedia_introduction + info_pos], 0, text_pos, text_align_content, spacing, 0 };
	while (true)
	{
		OrdinaryText(ordinary_text_info, *main_window->GetWindowInfo());
		Text::TextInfo text_info = { LanguagePack::text[LanguagePack::gamepedia_sections], info_pos, title_pos, text_align_title, 0, false };
		int temp_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
		ClearOrdinaryText(ordinary_text_info, *main_window->GetWindowInfo());
		if (temp_pos == info_pos)
		{
			const short text_size = static_cast<short>(LanguagePack::text[LanguagePack::gamepedia_sections][info_pos].size());
			SetConsoleCursorPosition(handle, {title_pos.X-static_cast<short>(static_cast<float>(text_size)*GetTextAlignScalar(text_align_title)), title_pos.Y });
			std::cout << Spaces(text_size + 4);
			break;
		}
		info_pos = temp_pos;
	}
	ClearOrdinaryText(ordinary_text_info, *main_window->GetWindowInfo());
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
		Title();
	}
	system("cls");
	Title();
}
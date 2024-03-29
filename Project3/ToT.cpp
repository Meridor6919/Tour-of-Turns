#include "ToT.h"

ToT::ToT()
{
	main_window = std::make_shared<ToT_Window>();
	main_window->Init();

	output_handle = main_window->GetOutputHandle();
	centered_position = { static_cast<short>(main_window->GetCharactersPerRow()) / 2, main_centered_Y };
}
void ToT::ShowRankingDetails(std::string tour, int racer_pos, int classification_type)
{
	std::vector<std::string> details = RankingManagement::GetTextToDisplay(tour, racer_pos, classification_type);

	MeridorConsoleLib::SetColor(output_handle, *main_window->secondary_color);
	SetConsoleCursorPosition(output_handle, top_left_box_position);
	std::cout << GetMonoCharacterString(Validation::box_width, '_');
	for (short i = 0; i < static_cast<short>(Validation::ranking_details); ++i)
	{
		SetConsoleCursorPosition(output_handle, { top_left_box_position.X + avarage_indent, top_left_box_position.Y + small_spacing * (i + 1) });
		MeridorConsoleLib::SetColor(output_handle, *main_window->main_color);
		std::cout << LanguagePack::text[LanguagePack::ranking_details][i] + ": ";
		MeridorConsoleLib::SetColor(output_handle, *main_window->secondary_color);
		std::cout << details[i];
	}
	MeridorConsoleLib::SetColor(output_handle, *main_window->secondary_color);
	SetConsoleCursorPosition(output_handle, { top_left_box_position.X, top_left_box_position.Y + small_spacing * static_cast<short>(Validation::ranking_details + 1) });
	std::cout << GetMonoCharacterString(Validation::box_width, '_');
}
void ToT::ClearRankingDetails()
{
	const int border_size = Validation::box_width;
	SetConsoleCursorPosition(output_handle, top_left_box_position);
	std::cout << Spaces(border_size);
	for (short i = 0; i < static_cast<short>(Validation::ranking_details); ++i)
	{
		SetConsoleCursorPosition(output_handle, { top_left_box_position.X + avarage_indent, top_left_box_position.Y + small_spacing * (i + 1) });
		std::cout << Spaces(border_size);
	}
	SetConsoleCursorPosition(output_handle, { top_left_box_position.X, top_left_box_position.Y + small_spacing * static_cast<short>(Validation::ranking_details + 1) });
	std::cout << Spaces(border_size);
}
void ToT::SetTheme(const COORD& local_starting_point)
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

	Text::ChooseDesc<Text::RefVector> text_info(text);
	text_info.text_align = TextAlign::left;
	text_info.starting_index = current_theme_pos;
	text_info.spacing = 0;
	text_info.point_of_reference = local_starting_point;
	text_info.clear_after = true;

	int new_theme_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
	if (current_theme_pos != new_theme_pos)
	{
		main_window->DrawTitle(true);
		main_window->SetTitleTheme(text[new_theme_pos]);
		main_window->DrawTitle();
	}
}
void ToT::SetColor(const COORD& local_starting_point, bool main)
{
	std::vector<std::string> local_text = LanguagePack::text[LanguagePack::selectable_colors];
	MeridorConsoleLib::Color* color = main ? main_window->main_color : main_window->secondary_color;
	MeridorConsoleLib::Color* secondary_color = main ? main_window->secondary_color : main_window->main_color;
	const MeridorConsoleLib::Color starting_color = *color;

	local_text.erase(local_text.begin() + static_cast<int>(*secondary_color) - 1);
	size_t pos = static_cast<int>(*color) - 1 - (*color > *secondary_color);

	Text::ChooseDesc<Text::RefVector> text_info(local_text);
	text_info.text_align = TextAlign::left;
	text_info.starting_index = pos;
	text_info.spacing = 0;
	text_info.point_of_reference = local_starting_point;
	text_info.clear_after = true;

	(*color) = static_cast<MeridorConsoleLib::Color>(Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo()) + 1);
	if (*color >= *secondary_color)
	{
		(*color) = (*color) + 1;
	}
	if (starting_color != *color)
	{
		main_window->DrawTitle();
	}
}
void ToT::SetMusic(const COORD& local_starting_point)
{
	std::vector<std::string> text = { LanguagePack::text[LanguagePack::on_off][1] };
	for (int i = 1; i <= volume_levels; ++i)
	{
		text.push_back(std::to_string(i));
	}
	const float starting_volume = main_window->GetMusicVolume();

	Text::ChooseDesc<Text::RefVector> text_info(text);
	text_info.text_align = TextAlign::left;
	text_info.starting_index = static_cast<size_t>(main_window->GetMusicVolume() * volume_levels);
	text_info.spacing = 0;
	text_info.point_of_reference = local_starting_point;
	text_info.clear_after = true;

	const float current_volume = static_cast<float>(Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo())) / static_cast<float>(volume_levels);
	if (starting_volume != current_volume)
	{
		main_window->SetMusic(current_volume);
	}
}
void ToT::SetLanguage(const COORD& local_starting_point)
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

	Text::ChooseDesc<Text::RefVector> text_info(language);
	text_info.text_align = TextAlign::left;
	text_info.starting_index = starting_pos;
	text_info.spacing = 0;
	text_info.point_of_reference = local_starting_point;
	text_info.clear_after = true;

	int current_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
	if (current_pos != starting_pos)
	{
		main_window->SetLanguage(language[current_pos] + ExtName::language);
		system("cls");
		main_window->DrawTitle();
	}
}
void ToT::SetHamachiFlag(const COORD& local_starting_point)
{
	Text::ChooseDesc<Text::RefVector> text_info(LanguagePack::text[LanguagePack::on_off]);
	text_info.text_align = TextAlign::left;
	text_info.starting_index = !main_window->GetHamachiConnectionFlag();
	text_info.spacing = 0;
	text_info.point_of_reference = local_starting_point;
	text_info.clear_after = true;

	main_window->SetHamachiConnectionFlag(!(Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo())));
}
void ToT::SetDisplayMode(const COORD& local_starting_point)
{
	unsigned int current_display_mode = static_cast<int>(main_window->GetWindowMode());
	
	Text::ChooseDesc<Text::RefVector> text_info(LanguagePack::text[LanguagePack::display_settings]);
	text_info.text_align = TextAlign::left;
	text_info.starting_index = current_display_mode;
	text_info.spacing = 0;
	text_info.point_of_reference = local_starting_point;
	text_info.clear_after = true;

	int new_display_mode = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
	if (new_display_mode != current_display_mode)
	{
		main_window->SetWindowMode(static_cast<MeridorConsoleLib::WindowMode>(new_display_mode));
	}
	centered_position = { static_cast<short>(main_window->GetCharactersPerRow() / 2), main_centered_Y };
	system("cls");
	main_window->DrawTitle();
}
void ToT::SetAIModule(const COORD& local_starting_point)
{
	std::vector<std::string> ai_modules = GetAINames();
	std::string current_module_name = main_window->GetAIModule();
	RemoveExtension(current_module_name, ExtName::ai);

	int module_pos;
	for (int i = 0; i < ai_modules.size(); ++i)
	{
		if (ai_modules[i] == current_module_name)
		{
			module_pos = i;
		}
	}
	Text::ChooseDesc<Text::RefVector> text_info(ai_modules);
	text_info.text_align = TextAlign::left;
	text_info.starting_index = module_pos;
	text_info.spacing = 0;
	text_info.point_of_reference = local_starting_point;
	text_info.clear_after = true;

	module_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
	main_window->SetAIModule(ai_modules[module_pos] + ExtName::ai);
}
void ToT::MainMenu()
{
	main_window->DrawTitle();
	while (true)
	{
		Text::ChooseDesc<Text::RefVector> text_info(LanguagePack::text[LanguagePack::main_menu]);
		text_info.text_align = TextAlign::center;
		text_info.starting_index = main_menu_position;
		text_info.spacing = avarage_spacing;
		text_info.point_of_reference = centered_position;
		text_info.clear_after = true;

		switch (main_menu_position = Text::Choose::Vertical(text_info, *main_window->GetWindowInfo()))
		{
		case 0://Singleplayer
		case 1://Multiplayer
		{
			if (main_window->IsPlayable())
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
			if (main_window->IsRankingActive())
			{
				Ranking();
			}
			else
			{
				MessageBox(0, ErrorMsg::ranking_absent, ErrorTitle::ranking_absent, MB_TOPMOST);
			}
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
	Text::ChooseDesc<Text::RefVector> text_info(LanguagePack::text[LanguagePack::credits]);
	text_info.text_align = TextAlign::center;
	text_info.starting_index = 0;
	text_info.spacing = avarage_spacing;
	text_info.point_of_reference = centered_position;
	text_info.clear_after = false;

	OrdinaryText(text_info, *main_window->GetWindowInfo());
	_getch();
	text_info.clear_after = true;
	OrdinaryText(text_info, *main_window->GetWindowInfo());
}
void ToT::Options()
{
	unsigned int main_menu_position = 0;
	bool loop = true;

	while (loop)
	{
		Text::ChooseDesc<Text::RefVector> text_info(LanguagePack::text[LanguagePack::general_options]);
		text_info.text_align = TextAlign::center;
		text_info.starting_index = main_menu_position;
		text_info.spacing = avarage_spacing;
		text_info.point_of_reference = centered_position;
		text_info.clear_after = false;

		main_menu_position = Text::Choose::Vertical(text_info, *main_window->GetWindowInfo());
		const short submenu_horizontal_position = static_cast<short>(LanguagePack::text[LanguagePack::general_options][main_menu_position].size()) / 2 + avarage_indent;
		const short game_window_vertical_position = static_cast<short>(main_menu_position * avarage_spacing);
		const COORD local_starting_point = { centered_position.X + submenu_horizontal_position, centered_position.Y + game_window_vertical_position };
		switch (main_menu_position)
		{
		case 0:
		{
			SetTheme(local_starting_point);
			break;
		}
		case 1:
		{
			SetColor(local_starting_point, true);
			break;
		}
		case 2:
		{
			SetColor(local_starting_point, false);
			break;
		}
		case 3:
		{
			if (main_window->IsMusicAvailable())
			{
				SetMusic(local_starting_point);
			}
			else
			{
				MessageBox(0, ErrorMsg::music_unavailable, ErrorTitle::music_unavailable, MB_TOPMOST);
			}
			break;
		}
		case 4:
		{
			SetLanguage(local_starting_point);
			break;
		}
		case 5:
		{
			SetHamachiFlag(local_starting_point);
			break;
		}
		case 6:
		{
			SetDisplayMode(local_starting_point);
			break;
		}
		case 7:
		{
			SetAIModule(local_starting_point);
			break;
		}
		case 8:
		{
			loop = false;
			Text::Choose::VerticalClearGUI(text_info, *main_window->GetWindowInfo());
			SaveWindowConfig(main_window->GetToTWindowConfig());
			break;
		}
		}
	}
}
void ToT::Ranking()
{
	std::vector<std::string> maps = GetFilesInDirectory(FolderName::ranking);
	unsigned int map_pos = 0;
	RemoveExtension(maps, ExtName::ranking);

	unsigned int racer_pos = 0;
	unsigned int classification_type = 0;
	bool loop = true;

	ShowRankingDetails(FolderName::ranking + '\\' + maps[map_pos] + ExtName::ranking, racer_pos, classification_type);
	
	Text::ChooseDesc<Text::RefVector> text_info(LanguagePack::text[LanguagePack::ranking_menu]);
	text_info.text_align = TextAlign::center;
	text_info.starting_index = 0;
	text_info.spacing = avarage_spacing;
	text_info.point_of_reference = centered_position;
	text_info.clear_after = false;

	while (loop)
	{
		const std::string starting_map_path = FolderName::ranking + '\\' + maps[map_pos] + ExtName::ranking;
		text_info.starting_index = Text::Choose::Vertical(text_info, *main_window->GetWindowInfo());
		const short submenu_horizontal_position = static_cast<short>(LanguagePack::text[LanguagePack::ranking_menu][text_info.starting_index].size()) / 2 + avarage_indent;
		const short game_window_vertical_position = static_cast<short>(text_info.starting_index * avarage_spacing);
		const COORD local_starting_point = { centered_position.X + submenu_horizontal_position, centered_position.Y + game_window_vertical_position };
		switch (text_info.starting_index)
		{
		case 0://Map
		{
			const int temp = map_pos;

			Text::ChooseDesc<Text::RefVector> text_info(maps);
			text_info.text_align = TextAlign::left;
			text_info.starting_index = map_pos;
			text_info.spacing = 0;
			text_info.point_of_reference = local_starting_point;
			text_info.clear_after = true;

			map_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
			if (map_pos != temp)
			{
				ClearRankingDetails();
				racer_pos = 0;
				ShowRankingDetails(FolderName::ranking + '\\' + maps[map_pos] + ExtName::ranking, racer_pos, classification_type);
			}
			break;
		}
		case 1://Player
		{
			const int temp = racer_pos;

			Text::ChooseDesc<Text::Vector> text_info(RankingManagement::GetRankedRacersNames(FolderName::ranking + '\\' + maps[map_pos] + ExtName::ranking));
			text_info.text_align = TextAlign::left;
			text_info.starting_index = racer_pos;
			text_info.spacing = 0;
			text_info.point_of_reference = local_starting_point;
			text_info.clear_after = true;

			racer_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
			if (temp != racer_pos && racer_pos >= 0)
			{
				ClearRankingDetails();
				ShowRankingDetails(starting_map_path, racer_pos, classification_type);
			}
			break;
		}
		case 2://Classification type
		{
			const int temp = classification_type;

			Text::ChooseDesc<Text::RefVector> text_info(LanguagePack::text[LanguagePack::ranking_classification_types]);
			text_info.text_align = TextAlign::left;
			text_info.starting_index = classification_type;
			text_info.spacing = 0;
			text_info.point_of_reference = local_starting_point;
			text_info.clear_after = true;

			classification_type = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
			if (temp != classification_type)
			{
				ClearRankingDetails();
				ShowRankingDetails(starting_map_path, racer_pos, classification_type);
			}
			break;
		}
		case 3://Reset map details
		{
			ClearRankingDetails();
			std::fstream fvar((FolderName::ranking + '\\' + maps[map_pos] + ExtName::ranking).c_str(), std::ios::out);
			ShowRankingDetails(starting_map_path, racer_pos, classification_type);
			fvar.close();
			break;
		}
		case 4://Back
		{
			ClearRankingDetails();
			loop = false;
			break;
		}
		}
		Text::Choose::VerticalClearGUI(text_info, *main_window->GetWindowInfo());
	}
}
void ToT::Info()
{
	constexpr short bracket_size = 4;
	const COORD title_pos = centered_position;
	const COORD text_pos = { centered_position.X, centered_position.Y + avarage_spacing };

	while (true)
	{
		Text::ChooseDesc<Text::RefVector> ordinary_text_info(LanguagePack::text[LanguagePack::gamepedia_introduction + info_pos]);
		ordinary_text_info.text_align = TextAlign::center;
		ordinary_text_info.starting_index = 0;
		ordinary_text_info.spacing = avarage_spacing;
		ordinary_text_info.point_of_reference = text_pos;
		ordinary_text_info.clear_after = false;

		OrdinaryText(ordinary_text_info, *main_window->GetWindowInfo());

		Text::ChooseDesc<Text::RefVector> text_info(LanguagePack::text[LanguagePack::gamepedia_sections]);
		text_info.text_align = TextAlign::center;
		text_info.starting_index = info_pos;
		text_info.spacing = 0;
		text_info.point_of_reference = title_pos;
		text_info.clear_after = false;

		int temp_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());

		ordinary_text_info.clear_after = true;
		OrdinaryText(ordinary_text_info, *main_window->GetWindowInfo());

		if (temp_pos == info_pos)
		{
			const short text_size = static_cast<short>(LanguagePack::text[LanguagePack::gamepedia_sections][info_pos].size());
			SetConsoleCursorPosition(output_handle, { title_pos.X - static_cast<short>(static_cast<float>(text_size) * GetTextAlignScalar(TextAlign::center)), title_pos.Y });
			std::cout << Spaces(text_size + bracket_size);
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
			Text::ChooseDesc<Text::RefVector> text_info(LanguagePack::text[LanguagePack::multiplayer_menu]);
			text_info.text_align = TextAlign::center;
			text_info.starting_index = 0;
			text_info.spacing = avarage_spacing;
			text_info.point_of_reference = centered_position;
			text_info.clear_after = false;

			switch (Text::Choose::Vertical(text_info, *main_window->GetWindowInfo()))
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
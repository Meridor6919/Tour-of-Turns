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
	window_info.window_option = WindowOption::windowed_fullscreen;
	this->main_window = std::make_shared<ToT_Window>(window_info);

	handle = main_window->GetHandle();
	game_window_center = static_cast<short>(main_window->GetCharactersPerRow()) / 2;
}
std::vector<std::string> ToT::GetRankingNames(std::string tour)
{
	std::ifstream fvar;
	std::string temp;
	std::vector<std::string> ret = {};
	fvar.open(tour.c_str());
	for (int i = 0; std::getline(fvar, temp); ++i)
	{
		if (!(i %  GameConstants::validate_ranking_details))
		{
			ret.push_back(temp);
		}
	}
	fvar.close();
	return ret;
}
std::vector<std::string> ToT::GetRankingDetails(std::string tour, int racer_pos, int classification_type)
{
	std::string line;
	std::vector<std::string> ret = {};
	for (int i = 0; i < GameConstants::validate_ranking_details; ++i)
	{
		ret.push_back(" ");
	}
	std::ifstream fvar;
	fvar.open(tour.c_str());
	for (int i = 0; i < GameConstants::validate_ranking_details * racer_pos && std::getline(fvar, line); ++i);
	for (int i = 0; i < GameConstants::validate_ranking_details && std::getline(fvar, line); ++i)
	{
		if (i == 0)
		{
			ret[i] = line;
		}
		else if (i < 6)
		{
			ret[i] = main_window->GetClassifiedDetail(line, classification_type);
		}
		else if (i < 10)
		{
			ret[i+2] = main_window->GetClassifiedDetail(line, classification_type);
		}
		else
		{
			ret[i-4] = main_window->GetClassifiedDetail(line, classification_type);
		}
	}
	int finished_games = atoi(ret[1].c_str()) - atoi(ret[8].c_str());
	if (!finished_games)
	{
		for (int i = 2; i < 6 + 3*(ret[1] == "0"); ++i)
		{
			ret[i] = "";
		}
		for (int i = 9; i < GameConstants::validate_ranking_details; ++i)
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
std::string ToT::GetRankingFavourite(std::string text)
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
void ToT::ShowRankingDetails(std::string tour, int racer_pos, int classification_type, bool clearing)
{
	const int spacing = 2;
	const int paragraph_size = 2;
	const COORD base_position = { 0, 19 };
	std::vector<std::string> details = GetRankingDetails(tour, racer_pos, classification_type);
	if (clearing)
	{
		const int border_size = static_cast<int>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::border].size());
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + 1 });
		std::cout<<Spaces(border_size);
		for (short i = 0; i < static_cast<short>(GameConstants::validate_ranking_details); ++i)
		{
			SetConsoleCursorPosition(handle, { base_position.X + paragraph_size, base_position.Y + spacing * (i + 2) });
			std::cout << Spaces(static_cast<short>(LanguagePack::text[LanguagePack::ranking_details][i].size()) + static_cast<short>(details[i].size()) + 2);
		}
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + spacing * (static_cast<short>(GameConstants::validate_ranking_details) + 2) });
		std::cout << Spaces(border_size);
	}
	else
	{
		SetConsoleTextAttribute(handle, *main_window->secondary_color);
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + 1 });
		std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::border];
		for (short i = 0; i < static_cast<short>(GameConstants::validate_ranking_details); ++i)
		{
			SetConsoleCursorPosition(handle, { base_position.X + paragraph_size, base_position.Y + spacing * (i + 2) });
			SetConsoleTextAttribute(handle, *main_window->main_color);
			std::cout << LanguagePack::text[LanguagePack::ranking_details][i] + ": ";
			SetConsoleTextAttribute(handle, *main_window->secondary_color);
			std::cout << details[i];
		}
		SetConsoleTextAttribute(handle, *main_window->secondary_color);
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + spacing * (static_cast<short>(GameConstants::validate_ranking_details) + 2) });
		std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::border];
	}
}
void ToT::MainMenu()
{
	main_window->Title({ game_window_center, 0 }, TextAlign::center);
	while (true)
	{
		Text::TextInfo text_info = { LanguagePack::text[LanguagePack::main_menu_options], main_menu_position, { game_window_center + 1, 25 }, TextAlign::center, 3, true};
		switch (main_menu_position = Text::Choose::Veritcal(text_info, *main_window->GetWindowInfo()))
		{
		case 0:
		case 1:
		{
			if (main_window->IsPlayable())
			{
				Game(main_menu_position);
			}
			else
			{
				MessageBox(0, ErrorMsg::unplayable.c_str(), ErrorTitle::unplayable.c_str(), 0);
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
			if (main_window->RankingFound())
			{
				Ranking();
			}
			else
			{
				MessageBox(0, ErrorMsg::ranking_missing.c_str(), ErrorTitle::missing_file.c_str(), 0);
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
	Text::TextInfo text_info = { LanguagePack::text[LanguagePack::tot_general_options], main_menu_position, starting_point, TextAlign::center, spacing, false };

	while (loop)
	{
		main_menu_position = Text::Choose::Veritcal(text_info, *main_window->GetWindowInfo());
		const short submenu_horizontal_position = static_cast<short>(LanguagePack::text[LanguagePack::tot_general_options][main_menu_position].size()) / 2 + 3;
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
					(main_window->main_color) = (main_window->main_color) + 1;
				}
				if (starting_color != *main_window->main_color)
				{
					main_window->Title({ game_window_center, 0 }, TextAlign::center);
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
					main_window->Title({ game_window_center, 0 }, TextAlign::center);
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
				std::vector<std::string> language = main_window->ReadFile(FolderName::language+'\\'+FileName::language);
				int starting_pos = 0;
				for (; starting_pos < static_cast<int>(language.size()); ++starting_pos)
				{
					main_window->GetLanguage();
					if (language[starting_pos] == main_window->GetLanguage())
					{
						break;
					}
				}
				main_window->RemoveExtension(language, ExtName::language);
				Text::TextInfo text_info = { language, starting_pos, local_starting_point, TextAlign::left, 0, true };
				int current_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
				if (current_pos != starting_pos)
				{
					main_window->SetLanguage(language[current_pos] + ExtName::language);
					system("cls");
					main_window->Title({ game_window_center, 0 }, TextAlign::center);
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
	main_window->SaveAtributes();
}
void ToT::Ranking()
{
	int main_menu_position = 0;
	const COORD starting_point = { game_window_center + 1, 25 };
	const short spacing = 3;
	bool loop = true;

	std::vector<std::string> maps = main_window->ReadFile(FolderName::tour + '\\' + FileName::ranking);
	int map_pos = 0;
	main_window->RemoveExtension(maps, ExtName::ranking);

	int racer_pos = 0;
	int classification_type = 0;

	ShowRankingDetails(FolderName::tour + '\\' + maps[map_pos] + ExtName::ranking, racer_pos, classification_type);
	Text::TextInfo text_info = { LanguagePack::text[LanguagePack::ranking_search_menu], main_menu_position, starting_point, TextAlign::center, spacing, false };
	while (loop)
	{
		const std::string starting_map_path = FolderName::tour + '\\' + maps[map_pos] + ExtName::ranking;
		main_menu_position = Text::Choose::Veritcal(text_info, *main_window->GetWindowInfo());
		const short submenu_horizontal_position = static_cast<short>(LanguagePack::text[LanguagePack::ranking_search_menu][main_menu_position].size()) / 2 + 3;
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
	const Text::TextInfo ordinary_text_info = { LanguagePack::text[LanguagePack::game_information_introduction + info_pos], 0, text_pos, text_align_content, spacing, 0 };
	while (true)
	{
		OrdinaryText(ordinary_text_info, *main_window->GetWindowInfo());
		Text::TextInfo text_info = { LanguagePack::text[LanguagePack::game_information_options], info_pos, title_pos, text_align_title, 0, false };
		int temp_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo());
		ClearOrdinaryText(ordinary_text_info, *main_window->GetWindowInfo());
		if (temp_pos == info_pos)
		{
			const short text_size = static_cast<short>(LanguagePack::text[LanguagePack::game_information_options][info_pos].size());
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
	main_window->SetMultiplayer(multiplayer);
	std::unique_ptr<SinglePlayer> network_role;
	if (multiplayer)
	{	
		do
		{
			Text::TextInfo text_info = { LanguagePack::text[LanguagePack::multiplayer_before_game_lobby], 0, { game_window_center, 25 }, TextAlign::center, 3, false };
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
		main_window->Title({ game_window_center, 0 }, TextAlign::center);
	}
	system("cls");
	main_window->Title({ game_window_center, 0 }, TextAlign::center);
}
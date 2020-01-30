#include "ToT.h"

ToT::ToT()
{
	this->main_window = std::make_shared<ToT_Window>("Tour of Turns", 15, 10, 200, 70);
	handle = main_window->GetHandle();
	game_window_center = static_cast<short>(main_window->GetWidth()) / 2;
}
std::vector<std::string> ToT::GetRankingNames(std::string tour)
{
	std::ifstream fvar;
	std::string temp;
	std::vector<std::string> ret = {};
	fvar.open(tour.c_str());
	for (int i = 0; std::getline(fvar, temp); ++i)
	{
		if (!(i %  ValidationConstants::ranking_details))
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
	for (int i = 0; i < 12; ++i)
	{
		ret.push_back(" ");
	}
	std::ifstream fvar;
	fvar.open(tour.c_str());
	for (int i = 0; i < ValidationConstants::ranking_details * racer_pos && std::getline(fvar, line); ++i);
	for (int i = 0; i < ValidationConstants::ranking_details && std::getline(fvar, line); ++i)
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
		for (int i = 9; i < ValidationConstants::ranking_details; ++i)
		{
			ret[i] = "";
		}
	}
	else
	{
		ret[2] = std::to_string(static_cast<int>(atof(ret[2].c_str()) / atof(ret[1].c_str()) * 100.0f)) + '%';

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
	std::string ret = " ";
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
	const HANDLE handle = main_window->GetHandle();
	const int spacing = 2;
	const int paragraph_size = 2;
	const COORD base_position = { 0, 19 };
	std::vector<std::string> details = GetRankingDetails(tour, racer_pos, classification_type);
	if (clearing)
	{
		const int border_size = static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border].size());
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + 1 });
		for (int i = 0; i < border_size; ++i)
		{
			std::cout << " ";
		}
		for (short i = 0; i < static_cast<short>(ValidationConstants::ranking_details); ++i)
		{
			SetConsoleCursorPosition(handle, { base_position.X + paragraph_size, base_position.Y + spacing * (i + 2) });
			for (int j = 0; j < static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::ranking_details][i].size()) + static_cast<short>(details[i].size()) + 2; ++j)
			{
				std::cout << " ";
			}
		}
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + spacing * (static_cast<short>(ValidationConstants::ranking_details) + 2) });
		for (int i = 0; i < border_size; ++i)
		{
			std::cout << " ";
		}
	}
	else
	{
		SetConsoleTextAttribute(handle, main_window->color2);
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + 1 });
		std::cout << LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border];
		for (short i = 0; i < static_cast<short>(ValidationConstants::ranking_details); ++i)
		{
			SetConsoleCursorPosition(handle, { base_position.X + paragraph_size, base_position.Y + spacing * (i + 2) });
			SetConsoleTextAttribute(handle, main_window->color1);
			std::cout << LanguagePack::vector_of_strings[LanguagePack::ranking_details][i] + ": ";
			SetConsoleTextAttribute(handle, main_window->color2);
			std::cout << details[i];
		}
		SetConsoleTextAttribute(handle, main_window->color2);
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + spacing * (static_cast<short>(ValidationConstants::ranking_details) + 2) });
		std::cout << LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border];
	}
}
void ToT::MainMenu()
{
	main_window->Title({ game_window_center, 0 }, Text::TextAlign::center);
	while (true)
	{
		switch (main_menu_position = Text::Choose::Veritcal(LanguagePack::vector_of_strings[LanguagePack::main_menu], main_menu_position, { game_window_center + 1, 25 }, 3, Text::TextAlign::center, true, *main_window))
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
	OrdinaryText(LanguagePack::vector_of_strings[LanguagePack::credits], Text::TextAlign::center, 3, { game_window_center,25 }, *main_window);
	_getch();
	OrdinaryText(LanguagePack::vector_of_strings[LanguagePack::credits], Text::TextAlign::center, 3, { game_window_center,25 }, *main_window, true);
}
void ToT::Options()
{
	int main_menu_position = 0;
	const COORD starting_point = { game_window_center+1, 25 };
	const short spacing = 3;
	bool loop = true;

	while (loop)
	{
		main_menu_position = Text::Choose::Veritcal(LanguagePack::vector_of_strings[LanguagePack::game_options], main_menu_position, starting_point, spacing, Text::TextAlign::center, false, *main_window);
		const short submenu_horizontal_position = static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::game_options][main_menu_position].size()) / 2 + 3;
		const short game_window_vertical_position = static_cast<short>(main_menu_position * spacing);
		const COORD local_starting_point = { starting_point.X + submenu_horizontal_position, starting_point.Y + game_window_vertical_position };
		switch (main_menu_position)
		{
			case 0://set primary color
			{
				std::vector<std::string> local_text = LanguagePack::vector_of_strings[LanguagePack::colors];
				const int starting_color = main_window->color1;
				local_text.erase(local_text.begin() + (main_window->color2 - 2));
				main_window->color1 = Text::Choose::Horizontal(local_text, main_window->color1 - 2 - (main_window->color1 > main_window->color2), local_starting_point, Text::TextAlign::left, true, *main_window) + 2;
				if (main_window->color1 >= main_window->color2)
				{
					++main_window->color1;
				}
				if (starting_color != main_window->color1)
				{
					main_window->Title({ game_window_center, 0 }, Text::TextAlign::center);
				}
				break;
			}
			case 1://set secondary color
			{
				std::vector<std::string> local_text = LanguagePack::vector_of_strings[LanguagePack::colors];
				const int starting_color = main_window->color2;
				local_text.erase(local_text.begin() + (main_window->color1 - 2));
				main_window->color2 = Text::Choose::Horizontal(local_text, main_window->color2 - 2 - (main_window->color2 > main_window->color1), local_starting_point, Text::TextAlign::left, true, *main_window) + 2;
				if (main_window->color2 >= main_window->color1)
				{
					++main_window->color2;
				}
				if (starting_color != main_window->color2)
				{
					main_window->Title({ game_window_center, 0 }, Text::TextAlign::center);
				}
				break;
			}
			case 2://set music
			{
				std::vector<std::string> text = { LanguagePack::vector_of_strings[LanguagePack::on_off][1] };
				for (int i = 1; i < 11; ++i)
				{
					text.push_back(std::to_string(i));
				}
				const float starting_volume = main_window->GetMusicVolume();
				const float current_volume = static_cast<float>(Text::Choose::Horizontal(text, static_cast<int>(main_window->GetMusicVolume() * 10), local_starting_point, Text::TextAlign::left, true, *main_window)) / 10.0f;
				if(starting_volume != current_volume)
				{
					main_window->SetMusic(current_volume);
				}
				break;
			}
			case 3://language
			{
				std::vector<std::string> language = main_window->ReadFile(FolderName::language+"\\"+FileName::language);
				int starting_pos = 0;
				for (; starting_pos < static_cast<int>(language.size()); ++starting_pos)
				{
					if (language[starting_pos] == main_window->GetLanguage())
					{
						break;
					}
				}
				main_window->RemoveExtension(language, ExtName::language);
				int current_pos = Text::Choose::Horizontal(language, starting_pos, local_starting_point, Text::TextAlign::left, true, *main_window);
				if (current_pos != starting_pos)
				{
					main_window->SetLanguage(language[current_pos] + "\\" + ExtName::language);
					system("cls");
					main_window->Title({ game_window_center, 0 }, Text::TextAlign::center);
				}
				break;
			}
			case 4://set hamachi flag
			{
				main_window->SetHamachiConnectionFlag(!(Text::Choose::Horizontal(LanguagePack::vector_of_strings[LanguagePack::on_off], !main_window->GetHamachiConnectionFlag(), local_starting_point, Text::TextAlign::left, true, *main_window)));
				break;
			}
			case 5://clearing
			{
				loop = false;
				break;
			}
		}
	}
	for (short i = 0; i < static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::game_options].size()); ++i)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(Text::TextAlign::center) / 2.0f * static_cast<float>(LanguagePack::vector_of_strings[LanguagePack::game_options][i].size())), starting_point.Y + i * spacing });
		for (int j = 0; j < static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::game_options][i].size()); ++j)
		{
			std::cout << " ";
		}
	}
	main_window->SaveAtributes();
}
void ToT::Ranking()
{
	int main_menu_position = 0;
	const COORD starting_point = { game_window_center + 1, 25 };
	const short spacing = 3;
	bool loop = true;

	std::vector<std::string> maps = main_window->ReadFile(FolderName::tour + "\\" + FileName::ranking);
	int map_pos = 0;
	main_window->RemoveExtension(maps, ExtName::ranking);

	int racer_pos = 0;
	int classification_type = 0;

	ShowRankingDetails(FolderName::tour + "\\" + maps[map_pos] + ExtName::ranking, racer_pos, classification_type);
	while (loop)
	{
		const std::string starting_map_path = FolderName::tour + "\\" + maps[map_pos] + ExtName::ranking;
		main_menu_position = Text::Choose::Veritcal(LanguagePack::vector_of_strings[LanguagePack::ranking_search_menu], main_menu_position, starting_point, spacing, Text::TextAlign::center, false, *main_window);
		const short submenu_horizontal_position = static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::ranking_search_menu][main_menu_position].size()) / 2 + 3;
		const short game_window_vertical_position = static_cast<short>(main_menu_position * spacing);
		const COORD local_starting_point = { starting_point.X + submenu_horizontal_position, starting_point.Y + game_window_vertical_position };
		switch (main_menu_position)
		{
			case 0://Map
			{
				const int temp = map_pos;
				map_pos = Text::Choose::Horizontal(maps, map_pos, local_starting_point, Text::TextAlign::left, true, *main_window);
				if (map_pos != temp)
				{
					ShowRankingDetails(starting_map_path, racer_pos, classification_type, true);
					racer_pos = 0;
					ShowRankingDetails(FolderName::tour + "\\" + maps[map_pos] + ExtName::ranking, racer_pos, classification_type);
				}
				break;
			}
			case 1://Player
			{
				const int temp = racer_pos;
				racer_pos = Text::Choose::Horizontal(GetRankingNames(FolderName::tour+ "\\" + maps[map_pos] + ExtName::ranking), racer_pos, local_starting_point, Text::TextAlign::left, true, *main_window);
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
				classification_type = Text::Choose::Horizontal(LanguagePack::vector_of_strings[LanguagePack::ranking_classification_types], classification_type, local_starting_point, Text::TextAlign::left, true, *main_window);
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
				std::fstream fvar((FolderName::tour + "\\" + maps[map_pos] + ExtName::ranking).c_str(), std::ios::out);
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
		for (short i = 0; i < static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::ranking_search_menu].size()); ++i)
		{
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(Text::TextAlign::center) / 2.0f * static_cast<float>(LanguagePack::vector_of_strings[LanguagePack::ranking_search_menu][i].size())), starting_point.Y + i * spacing });
			for (int j = 0; j < static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::ranking_search_menu][i].size()); ++j)
			{
				std::cout << " ";
			}
		}
	}
}
void ToT::Info()
{
	const Text::TextAlign text_align_title = Text::TextAlign::center;
	const Text::TextAlign text_align_content = Text::TextAlign::center;
	const short spacing = 3;
	const COORD text_pos = { game_window_center, 28 };
	const COORD title_pos = { game_window_center - 2, text_pos.Y - 3 };
	while (true)
	{
		OrdinaryText(LanguagePack::vector_of_strings[LanguagePack::introduction + info_pos], text_align_content, spacing, text_pos, *main_window);
		int temp_pos = Text::Choose::Horizontal(LanguagePack::vector_of_strings[LanguagePack::game_information], info_pos, title_pos, text_align_title, false,* main_window);
		OrdinaryText(LanguagePack::vector_of_strings[LanguagePack::introduction + info_pos], text_align_content, spacing, text_pos, *main_window, true);
		if (temp_pos == info_pos)
		{
			const short text_size = static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::game_information][info_pos].size());
			SetConsoleCursorPosition(handle, {title_pos.X-text_size/2*text_align_title, title_pos.Y });
			for (int j = 0; j < text_size + 4; ++j)
			{
				std::cout << " ";
			}
			break;
		}
		info_pos = temp_pos;
	}
	OrdinaryText(LanguagePack::vector_of_strings[LanguagePack::introduction], text_align_content, 3, text_pos, *main_window, true);
}
void ToT::Game(const bool multiplayer)
{
	main_window->SetMultiplayer(multiplayer);
	std::shared_ptr<SinglePlayer> network_role;
	if (multiplayer)
	{	
		switch (Text::Choose::Veritcal(LanguagePack::vector_of_strings[LanguagePack::multiplayer], 0, { game_window_center, 25 }, 3, Text::TextAlign::center, true, *main_window))
		{
			case 0:
			{
				try
				{
					network_role = std::make_shared<Host>(*main_window);
				}
				catch (int err)
				{
					UNREFERENCED_PARAMETER(err);
					return;
				}
				break;
			}
			case 1:
			{
				try
				{
					network_role = std::make_shared<Client>(*main_window);
				}
				catch (int err)
				{
					UNREFERENCED_PARAMETER(err);
					return;
				}

				break;
			}
			case 2:
			{
				return;
			}
		}
	}
	else
	{
		network_role = std::make_shared<SinglePlayer>(*main_window);
	}
	if (network_role->GameLobby())
	{
		network_role->Interface();
		network_role->Leaderboard(true);
		network_role->Leaderboard(false);

		for (int turn = 0; network_role->VisionBox(turn); turn++)
		{
			if (turn > 0)//attacks start with second turn 
			{
				network_role->Attack();
			}
			network_role->TakeAction();
			network_role->GetOthersAction(turn);
			network_role->GetCurrentAtribs();
			network_role->Interface();
			network_role->Leaderboard(true);
			network_role->Leaderboard(false);
		}
		network_role->Finish();
		_getch();
		_getch();
	}
	system("cls");
	main_window->Title({ game_window_center, 0 }, Text::TextAlign::center);
}
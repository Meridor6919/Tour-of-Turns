#include "ToT.h"

void GameMode::Credits(ToT_Window &main_window)
{
	HANDLE handle = main_window.GetHandle();

	OrdinaryText(LanguagePack::vector_of_strings[LanguagePack::credits], Text::TextAlign::center, 3, 25, main_window);
	_getch();
	OrdinaryText(LanguagePack::vector_of_strings[LanguagePack::credits], Text::TextAlign::center, 3, 25, main_window, true);
}
void GameMode::Options(ToT_Window &main_window)
{
	int main_menu_position = 0;
	const short game_window_center = static_cast<short>(main_window.GetWidth()) / 2;
	const COORD starting_point = { game_window_center+1, 25 };
	const short spacing = 3;
	bool loop = true;

	while (loop)
	{
		main_menu_position = Text::Choose::Veritcal(LanguagePack::vector_of_strings[LanguagePack::game_options], main_menu_position, starting_point, spacing, Text::TextAlign::center, false, main_window);
		const short submenu_horizontal_position = static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::game_options][main_menu_position].size()) / 2 + 3;
		const short game_window_vertical_position = static_cast<short>(main_menu_position * spacing);
		switch (main_menu_position)
		{
			case 0://set primary color
			{
				std::vector<std::string> local_text = LanguagePack::vector_of_strings[LanguagePack::colors];
				const COORD local_starting_point = { starting_point.X + submenu_horizontal_position, starting_point.Y + game_window_vertical_position };
				local_text.erase(local_text.begin() + (main_window.color2 - 2));
				main_window.color1 = Text::Choose::Horizontal(local_text, main_window.color1 - 2 - (main_window.color1 > main_window.color2), local_starting_point, Text::TextAlign::left, true, main_window) + 2;
				if (main_window.color1 >= main_window.color2)
				{
					++main_window.color1;
				}
				main_window.Title({ game_window_center, 0 }, Text::TextAlign::center);
				break;
			}
			case 1://set secondary color
			{
				std::vector<std::string> local_text = LanguagePack::vector_of_strings[LanguagePack::colors];
				const COORD local_starting_point = { starting_point.X + submenu_horizontal_position, starting_point.Y + game_window_vertical_position };
				local_text.erase(local_text.begin() + (main_window.color1 - 2));
				main_window.color2 = Text::Choose::Horizontal(local_text, main_window.color2 - 2 - (main_window.color2 > main_window.color1), local_starting_point, Text::TextAlign::left, true, main_window) + 2;
				if (main_window.color2 >= main_window.color1)
				{
					++main_window.color2;
				}
				main_window.Title({ game_window_center, 0 }, Text::TextAlign::center);
				break;
			}
			case 2://set music
			{
				const COORD local_starting_point = { starting_point.X + submenu_horizontal_position, starting_point.Y + game_window_vertical_position };
				std::vector<std::string> text = { LanguagePack::vector_of_strings[LanguagePack::on_off][1] };
				for (int i = 1; i < 11; ++i)
				{
					text.push_back(std::to_string(i));
				}
				main_window.SetMusic(static_cast<float>(Text::Choose::Horizontal(text, static_cast<int>(main_window.GetMusicVolume()*10), local_starting_point, Text::TextAlign::left, true, main_window)) / 10.0f);
				break;
			}
			case 3://set hamachi flag
			{
				
				const COORD local_starting_point = { starting_point.X + submenu_horizontal_position , starting_point.Y + static_cast<short>(main_menu_position * spacing) };
				main_window.SetHamachiConnectionFlag(!(Text::Choose::Horizontal(LanguagePack::vector_of_strings[LanguagePack::on_off], !main_window.GetHamachiConnectionFlag(), local_starting_point, Text::TextAlign::left, true, main_window)));
				break;
			}
			case 4://clearing
			{
				HANDLE handle = main_window.GetHandle();
				for (short i = 0; i < static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::game_options].size()); ++i)
				{
					SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(Text::TextAlign::center) / 2.0f * static_cast<float>(LanguagePack::vector_of_strings[LanguagePack::game_options][i].size())), starting_point.Y + i * spacing });
					for (int j = 0; j < static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::game_options][i].size()); ++j)
					{
						std::cout << " ";
					}
				}
				loop = false;
			}
		}
	}
	main_window.SaveAtributes();
}
void GameMode::Ranking(ToT_Window &main_window)
{
	//TODO ranking should show players accuracy in decision making instead of showing lucky and stupid ones that scores the highest
}
void GameMode::Info(ToT_Window & main_window)
{
	HANDLE handle = main_window.GetHandle();
	int pos = 0;
	while (true)
	{
		OrdinaryText(LanguagePack::vector_of_strings[LanguagePack::introduction + pos], Text::TextAlign::center, 3, 28, main_window);
		int i = Text::Choose::Horizontal(LanguagePack::vector_of_strings[LanguagePack::game_information], 0, { static_cast<short>(main_window.GetWidth() / 2), 25 }, Text::TextAlign::center, false, main_window);
		if (i == pos)
		{
			break;
		}
		pos = i;
		
	}
	OrdinaryText(LanguagePack::vector_of_strings[LanguagePack::introduction], Text::TextAlign::center, 3, 28, main_window, true);
}
void GameMode::Game(const bool multiplayer, ToT_Window &main_window)
{
	std::shared_ptr<SinglePlayer> network_role;
	const short game_window_center = static_cast<short>(main_window.GetWidth() / 2);

	if (multiplayer)
	{	
		switch (Text::Choose::Veritcal(LanguagePack::vector_of_strings[LanguagePack::multiplayer], 0, { game_window_center, 25 }, 3, Text::TextAlign::center, true, main_window))
		{
			case 0:
			{
				try
				{
					network_role = std::make_shared<Host>(main_window);
				}
				catch (int err)
				{
					return;
				}
				break;
			}
			case 1:
			{
				try
				{
					network_role = std::make_shared<Client>(main_window);
				}
				catch (int err)
				{
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
		network_role = std::make_shared<SinglePlayer>(main_window);
	}
	if (network_role->GameLobby())
	{
		network_role->Interface();
		network_role->Ranking(false);

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
			network_role->Ranking(true);
			network_role->Ranking(false);
		}
		_getch();
		_getch();
	}
	system("cls");
	main_window.Title({ game_window_center, 0 }, Text::TextAlign::center);
}
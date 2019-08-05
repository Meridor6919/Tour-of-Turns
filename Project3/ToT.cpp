#include "ToT.h"

void GameMode::Credits(ToT_Window &main_window)
{
	HANDLE handle = main_window.GetHandle();

	std::vector<std::string> credits = {
		"Hello I am Michal Balicki and I am really happy that you are playing my game.",
		"Tour of Turns was my first real project in my programming way and I hope you like it.",
		"I would like to thank people who helped me with this project.",
		"Krystian Grzegorzewicz ",
		"without him the game wouldn't be created.",
		"He was patiently listening what problems do I have with ToT and the most important he gave me a main idea of the game.",
		"Marek Loek ",
		"for making an icon image and inspiring me to hard work.",
		"Marcin Marzec ",
		"for giving me inspiration.",
		"I admire how much work he put in Pan Zniszczenia map for HoMM3 based on Diablo 2 universum.",
		"Miroslaw Zelent ",
		"for getting me interested in programming"
	};
	std::vector<Text::OrdinaryText_atributes> credits_atribute = { Text::endl, Text::endl, Text::endl, Text::color2, Text::endl,
												Text::endl, Text::color2, Text::endl, Text::color2, Text::endl,
												Text::endl, Text::color2, Text::endl };

	OrdinaryText(credits, credits_atribute, Text::TextAlign::center, 3, 25, main_window);
	_getch();
	OrdinaryText(credits, credits_atribute, Text::TextAlign::center, 3, 25, main_window, true);
}
void GameMode::Options(ToT_Window &main_window)
{
	const std::vector<std::string> options_text = { "Main Color", "Secondary Color", "Music playing", "Hamachi connection", "Back" };
	int main_menu_position = 0;
	const COORD starting_point = { static_cast<short>(main_window.GetWidth()) / 2+1, 25 };
	const short spacing = 3;
	bool loop = true;

	while (loop)
	{
		//text allign is also needed in clearing so beware if changing them
		switch (main_menu_position = Text::Choose::Veritcal(options_text, main_menu_position, starting_point, spacing, Text::TextAlign::center, false, main_window))
		{
			case 0:
			{

				//list of colours from 2 to 17
				std::vector<std::string> local_text = { "green","cyan","red","magenta","brown","light gray","dark gray","blue","light green",
						"light cyan","light red","light magenta","yellow","white" };
				const COORD local_starting_point = { starting_point.X + static_cast<short>(options_text[main_menu_position].size()) / 2 + 3 , starting_point.Y + static_cast<short>(main_menu_position * spacing) };
				
				//erasing color that is already used by second colour
				//program crashes when local_text.begin() is equal to local.text.end() -1 so inline if is nessesary. Substracting by 2 to synchronizewith list.
				auto text_to_erase = main_window.color2 < 15 ? local_text.begin() + main_window.color2 - 2 : local_text.end() - 1;
				local_text.erase(text_to_erase);

				//setting new colour
				main_window.color1 = Text::Choose::Horizontal(local_text, main_window.color1 - 2 - (main_window.color1 > main_window.color2), local_starting_point, Text::TextAlign::left, true, main_window) + 2;
				if (main_window.color1 >= main_window.color2)
				{
					++main_window.color1;
				}
				main_window.Title({ static_cast<short>(main_window.GetWidth()) / 2, 0 }, Text::TextAlign::center);
				break;
			}
			case 1:
			{
				//list of colours from 2 to 17
				std::vector<std::string> local_text = { "green","cyan","red","magenta","brown","light gray","dark gray","blue","light green",
						"light cyan","light red","light magenta","yellow","white" };
				const COORD local_starting_point = { starting_point.X + static_cast<short>(options_text[main_menu_position].size()) / 2 + 3 , starting_point.Y + static_cast<short>(main_menu_position * spacing) };
				
				//erasing color that is already used by second colour
				//program crashes when local_text.begin() is equal to local.text.end() -1 so inline if is nessesary. Substracting by 2 to synchronizewith list.
				auto text_to_erase = main_window.color1 < 15 ? local_text.begin() + main_window.color1 - 2 : local_text.end()-1;
				local_text.erase(text_to_erase);

				//setting new colour
				main_window.color2 = Text::Choose::Horizontal(local_text, main_window.color2 - 2 - (main_window.color2 > main_window.color1), local_starting_point, Text::TextAlign::left, true, main_window) + 2;
				if (main_window.color2 >= main_window.color1)
				{
					++main_window.color2;
				}
				main_window.Title({ static_cast<short>(main_window.GetWidth()) / 2, 0 }, Text::TextAlign::center);
				break;
			}
			case 2:
			{
				static bool flag = true;
				const std::vector<std::string> local_text = { "on", "off" };
				const COORD local_starting_point = { starting_point.X + static_cast<short>(options_text[main_menu_position].size())/2 +3 , starting_point.Y + static_cast<short>(main_menu_position * spacing) };
				main_window.SetMusic("test.wav", !(flag = Text::Choose::Horizontal(local_text, flag, local_starting_point, Text::TextAlign::left, true, main_window)));
				break;
			}
			case 3:
			{
				static bool flag = false;
				const std::vector<std::string> local_text = { "on", "off" };
				const COORD local_starting_point = { starting_point.X + static_cast<short>(options_text[main_menu_position].size())/2 + 3 , starting_point.Y + static_cast<short>(main_menu_position * spacing) };
				main_window.SetHamachiConnectionFlag(!(flag = Text::Choose::Horizontal(local_text, flag, local_starting_point, Text::TextAlign::left, true, main_window)));
				break;
			}
			case 4:
			{
				HANDLE handle = main_window.GetHandle();
				//clearing options
				for (short i = 0; i < static_cast<short>(options_text.size()); ++i)
				{
					SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(Text::TextAlign::center) / 2.0f * static_cast<float>(options_text[i].size())), starting_point.Y + i * spacing });
					for (int j = 0; j < static_cast<int>(options_text[i].size()); ++j)
					{
						std::cout << " ";
					}
				}
				loop = false;
			}
		}
	}
}
void GameMode::Ranking(ToT_Window &main_window)
{
	//TODO ranking should show players accuracy in decision making instead of showing lucky and stupid ones that scores the highest
}
void GameMode::Game(const bool multiplayer, ToT_Window &main_window)
{
	std::shared_ptr<SinglePlayer> network_role;
	const std::vector<std::string> options = { "Host game", "Search game", "Back" };

	if (multiplayer)
	{	
		switch (Text::Choose::Veritcal(options, 0, { static_cast<short>(main_window.GetWidth()) / 2, 25 }, 3, Text::TextAlign::center, true, main_window))
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
	network_role->GameLobby();
	network_role->Interface();
	network_role->Ranking(false);

	for (int turn = 0; network_role->VisionBox(turn); turn++)
	{
		//attacking start with second turn 
		if (turn > 0)
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
	system("cls");
	main_window.Title({ static_cast<short>(main_window.GetWidth()) / 2, 0 }, Text::TextAlign::center);
}
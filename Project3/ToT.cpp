#include "ToT.h"


void GameMode::Credits(Window &main_window)
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
	std::vector<Text::Atributes> credits_atribute = { Text::endl, Text::endl, Text::endl, Text::color2, Text::endl,
												Text::endl, Text::color2, Text::endl, Text::color2, Text::endl,
												Text::endl, Text::color2, Text::endl };

	OrdinaryText(credits, credits_atribute, Text::TextAlign::center, 3, 20, main_window);
}
void GameMode::Options(Window &main_window)
{
	std::vector<std::string> options_text = { "Main Color", "Secondary Color", "Music playing", "Hamachi connection", "Back" };
	int main_menu_position = 0;
	COORD starting_point = { (short)main_window.GetWidth() / 2, 15 };
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
				COORD local_starting_point = { starting_point.X + options_text[main_menu_position].size() / 2 + 3 , starting_point.Y + main_menu_position * spacing };
				
				//erasing color that is already used by second colour
				//program crashes when local_text.begin() is equal to local.text.end() -1 so inline if is nessesary. Substracting by 2 to synchronizewith list.
				auto text_to_erase = main_window.color2 < 15 ? local_text.begin() + main_window.color2 - 2 : local_text.end() - 1;
				local_text.erase(text_to_erase);

				//setting new colour
				main_window.color1 = Text::Choose::Horizontal(local_text, main_window.color1 - 2 - (main_window.color1 > main_window.color2), local_starting_point, Text::TextAlign::left, true, main_window) + 2;
				if (main_window.color1 >= main_window.color2)
					main_window.color1++;
				break;
			}
			case 1:
			{
				//list of colours from 2 to 17
				std::vector<std::string> local_text = { "green","cyan","red","magenta","brown","light gray","dark gray","blue","light green",
						"light cyan","light red","light magenta","yellow","white" };
				COORD local_starting_point = { starting_point.X + options_text[main_menu_position].size() / 2 + 3 , starting_point.Y + main_menu_position * spacing };
				
				//erasing color that is already used by second colour
				//program crashes when local_text.begin() is equal to local.text.end() -1 so inline if is nessesary. Substracting by 2 to synchronizewith list.
				auto text_to_erase = main_window.color1 < 15 ? local_text.begin() + main_window.color1 - 2 : local_text.end()-1;
				local_text.erase(text_to_erase);

				//setting new colour
				main_window.color2 = Text::Choose::Horizontal(local_text, main_window.color2 - 2 - (main_window.color2 > main_window.color1), local_starting_point, Text::TextAlign::left, true, main_window) + 2;
				if (main_window.color2 >= main_window.color1)
					main_window.color2++;
				break;
			}
			case 2:
			{
				static bool flag = false;
				std::vector<std::string> local_text = { "on", "off" };
				COORD local_starting_point = { starting_point.X + options_text[main_menu_position].size()/2 +3 , starting_point.Y + main_menu_position * spacing };
				
				main_window.SetMusic(!(flag = Text::Choose::Horizontal(local_text, flag, local_starting_point, Text::TextAlign::left, true, main_window)));
				break;
			}
			case 3:
			{
				static bool flag = false;
				std::vector<std::string> local_text = { "on", "off" };
				COORD local_starting_point = { starting_point.X + options_text[main_menu_position].size()/2 + 3 , starting_point.Y + main_menu_position * spacing };

				main_window.SetHamachiConectionFlag(!(flag = Text::Choose::Horizontal(local_text, flag, local_starting_point, Text::TextAlign::left, true, main_window)));
				break;
			}
			case 4:
			{
				HANDLE handle = main_window.GetHandle();

				//clearing options
				for (short i = 0; i < options_text.size(); i++)
				{
					SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::TextAlign::center / 2 * (float)options_text[i].size()), starting_point.Y + i * spacing });
					for (int j = 0; j < options_text[i].size(); j++)
						std::cout << " ";
				}
				loop = false;
			}
		}
	}
}
void GameMode::Ranking(Window &main_window)
{
	
	HANDLE handle = main_window.GetHandle();
	std::vector<std::string> maps_rankings;
	std::vector<std::string>map_information;
	std::string tmp_string;
	std::fstream fvar;
	int position = 0;
	short tmp_int;


	fvar.open("Ranking.txt", std::ios::in);

	while (std::getline(fvar, tmp_string))
		maps_rankings.push_back(tmp_string.erase(tmp_string.size() - 5, 5));
	
	fvar.close();

	do
	{
		tmp_int = position;

		Text::TableText(map_information, 1, 7, 2, 20, main_window, true);
		map_information.clear();

		map_information = { "Name", "Car", "Tires", "AI","Time","Place","Points" };

		fvar.open((maps_rankings[position] + ".rank").c_str(), std::ios::in);

		while (std::getline(fvar, tmp_string))
			map_information.push_back(tmp_string);

		Text::TableText(map_information, 1, 7, 2, 20, main_window);
		fvar.close();

		position = Text::Choose::Horizontal(maps_rankings, position, { (short)main_window.GetWidth() / 2, 18 }, Text::TextAlign::center, true, main_window);
	} while (tmp_int != position);

	Text::TableText(map_information, 1, 7, 2, 20, main_window, true);
	map_information.clear();
}
void GameMode::Game(bool multiplayer, Window &main_window)
{

	SinglePlayer *network_role = nullptr;

	if (multiplayer)
	{
		std::vector<std::string> options = { "Host game", "Search game", "Back" };

		switch (Text::Choose::Veritcal(options, 0, { (short)main_window.GetWidth() / 2, 20 }, 3, Text::TextAlign::center, true, main_window))
		{
		case 0:
		{
			network_role = new Host(main_window);
			break;
		}
		case 1:
		{
			network_role = new Client(main_window);
			break;
		}
		case 2:
			return;
		}
	}
	else
		network_role = new SinglePlayer(main_window);
	
	Race race(main_window);
	race.Lobby(network_role);
	race.Game();
	race.Ending();

	delete network_role;
}
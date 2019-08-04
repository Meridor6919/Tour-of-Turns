#include <WinSock2.h>
#include "ToT.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	//TODO make maps cars and tires
	std::string title = "Tour of Turns";
	ToT_Window game_window(title, 15, 10, 200, 70);
	std::vector<std::string> text = { "New Game ", "Multiplayer ", "Ranking ", "Options ", "Credits ", "Exit" };
	short main_menu_position = 0;

	WSAData wsa_data;
	WSAStartup(MAKEWORD(2, 2), &wsa_data);
	game_window.Title({static_cast<short>(game_window.GetWidth())/2, 0}, Text::TextAlign::center);

	while (true)
	{
		switch (main_menu_position = Text::Choose::Veritcal(text, main_menu_position, { static_cast<short>(game_window.GetWidth()) / 2+1, 25 }, 3, Text::TextAlign::center, true, game_window))
		{
			case 0:
			case 1:
			{
				GameMode::Game(main_menu_position, game_window);
				break;
			}
			case 2:
			{
				GameMode::Ranking(game_window);
				break;
			}
			case 3:
			{
				GameMode::Options(game_window);
				break;
			}
			case 4:
			{
				GameMode::Credits(game_window);
				break;
			}
			case 5:
			{
				WSACleanup();
				exit(0);
			}
		}
	}
}
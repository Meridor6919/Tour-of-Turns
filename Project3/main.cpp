#include <WinSock2.h>
#include "ToT.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	ToT_Window game_window("Tour of Turns", 15, 10, 200, 70);
	ToT game(&game_window);
	const short game_window_center = static_cast<short>(game_window.GetWidth()) / 2;
	short main_menu_position = 0;


	WSAData wsa_data;
	WSAStartup(MAKEWORD(2, 2), &wsa_data);
	game_window.Title({game_window_center, 0}, Text::TextAlign::center);

	while (true)
	{
		switch (main_menu_position = Text::Choose::Veritcal(LanguagePack::vector_of_strings[LanguagePack::main_menu], main_menu_position, { game_window_center +1, 25 }, 3, Text::TextAlign::center, true, game_window))
		{
			case 0:
			case 1:
			{
				if (game_window.IsPlayable())
				{
					game.Game(main_menu_position);
				}
				else
				{
					MessageBox(0, (LanguagePack::vector_of_strings[LanguagePack::error_msg][ErrorMsgs::unplayable]).c_str(), LanguagePack::vector_of_strings[LanguagePack::error_title][ErrorTitles::unplayable].c_str(), 0);
				}
				break;
			}
			case 2:
			{
				game.Info();
				break;
			}
			case 3:
			{
				if (game_window.RankingFound())
				{
					game.Ranking();
				}
				else
				{
					MessageBox(0, (LanguagePack::vector_of_strings[LanguagePack::error_msg][ErrorMsgs::ranking_not_present]).c_str(), LanguagePack::vector_of_strings[LanguagePack::error_title][ErrorTitles::missing_file].c_str(), 0);
				}
				break;
			}
			case 4:
			{
				game.Options();
				break;
			}
			case 5:
			{
				game.Credits();
				break;
			}
			case 6:
			{
				WSACleanup();
				exit(0);
			}
		}
	}
}
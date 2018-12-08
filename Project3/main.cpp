#include <WinSock2.h>
#include "ToT.h"
#include "ToT_Window.h"


#pragma comment(lib, "Ws2_32.lib")

int main()
{
	char title[] = "Tour of Turns";
	ToT_Window GameWindow(title, 15, 10, 14, true); //window settings
	std::vector<std::string> text = { "New Game ", "Multiplayer ", "Ranking ", "Options ", "Credits ", "Exit" };
	int main_menu_position = 0;

	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	while (true) //game loop
	{

		switch (main_menu_position = Text::Choose::Veritcal(text, main_menu_position, { (short)GameWindow.GetWidth() / 2, 15 }, 3, Text::TextAlign::center, true, GameWindow))
		{
			case 0:
			case 1:
			{
				GameMode_::Game(main_menu_position, GameWindow);
				break;
			}
			case 2:
			{
				GameMode_::Ranking(GameWindow);
				break;
			}
			case 3:
			{
				GameMode_::Options(GameWindow);
				break;
			}
			case 4:
			{
				GameMode_::Credits(GameWindow);
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
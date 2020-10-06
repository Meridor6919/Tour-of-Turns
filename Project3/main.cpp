#include <WinSock2.h>
#include "ToT.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	ToT game;
	game.MainMenu();
	WSACleanup();
}
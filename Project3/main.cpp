#include <WinSock2.h>
#include "ToT.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSAData wsa_data;
	WSAStartup(MAKEWORD(2, 2), &wsa_data);

	LanguagePack::LoadVector(FolderName::main+"\\Ang.txt");
	ToT game;
	game.MainMenu();
	WSACleanup();
}
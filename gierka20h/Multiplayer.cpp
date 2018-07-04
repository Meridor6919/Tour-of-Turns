#include "Multiplayer.h"

#define PORT 6919



void Broadcasting()
{
	using namespace std::literals::chrono_literals;
	//--------------------------------------------------------------------Setting socket and socket's address----------------------------------------------------------//
	char broadcast = 'a';
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sock == INVALID_SOCKET)
	{
		MessageBox(0, "Socket hostname error", "Error", 0);
		WSACleanup();
		exit(0);
	}
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
	{
		MessageBox(0, "Socket broadcast error", "Error", 0);
		closesocket(sock);
		WSACleanup();
		exit(0);
	}

	sockaddr_in SocketAddress;
	memset(&SocketAddress, 0, sizeof(SocketAddress));
	SocketAddress.sin_family = AF_INET;
	SocketAddress.sin_port = htons(PORT);
	SocketAddress.sin_addr.s_addr = inet_addr("192.168.x.x" ) | inet_addr("25.x.x.x");
	//---------------------------------------------------------------------------Getting Host name -----------------------------------------------------------------------//
	char HostName[50];

	if (gethostname(HostName, sizeof(HostName)) == SOCKET_ERROR)
	{
		MessageBox(0, "Socket hostname error", "Error", 0);
		WSACleanup();
		closesocket(sock);
		exit(0);
	}
	//-------------------------------------------------------------------------------Broadcasting----------------------------------------------------------------------//
	while (BroadcastingWorking)
	{
		if (sendto(sock, HostName, sizeof(HostName), 0, (sockaddr *)&SocketAddress, sizeof(SocketAddress)) < 0)
		{
			MessageBox(0, "Socket send error", "Error", 0);
			WSACleanup();
			closesocket(sock);
			exit(0);
		}
		std::this_thread::sleep_for(500ms);
	}
	closesocket(sock);
}
int HostsLobby(SOCKET ClientsConnected[], char ClientNames[][50], int width, int color, int color2)
{
	//------------------------------------------------------------------------------Setting socket for accepting clients-----------------------------------------------------------------------------
	int ClientsAccepted = 0;
	for (int i = 0; ClientsConnected[i] != INVALID_SOCKET; i++)
		ClientsAccepted++;
	

	SOCKET HostSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (HostSocket == INVALID_SOCKET)
	{
		MessageBox(0, "Socket error", "Error", 0);
		WSACleanup();
		exit(0);
	}

	struct sockaddr_in HostAddress;
	memset(&HostAddress, 0, sizeof(HostAddress));
	HostAddress.sin_family = AF_INET;
	HostAddress.sin_port = htons(6919);
	HostAddress.sin_addr.s_addr = INADDR_ANY;

	SOCKET ClientSocket = INVALID_SOCKET;

	struct sockaddr_in ClientAddress;
	int AddressSize = sizeof(struct sockaddr_in);

	bind(HostSocket, (struct sockaddr *) & HostAddress, sizeof(struct sockaddr));

	listen(HostSocket, 7);
	//---------------------------------------------------------------------------------------------Just Lobby-------------------------------------------------------------------------------------------
	char button;
	int CursorPosition = 0;

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string LobbyOptions[] = { "Start game","Kick players","Back" };
	COORD LobbyOptionsCoords[] = { { width - 3,20 },{ width - 4,22 },{ width, 24 } };

	SetConsoleCursorPosition(handle, { 0, 18});
	SetConsoleTextAttribute(handle, color);
	std::cout << "Players in lobby";

	SetConsoleTextAttribute(handle, color2);
	for (int i = 0; i < 3; i++)
	{
		SetConsoleCursorPosition(handle, LobbyOptionsCoords[i]);
		std::cout << LobbyOptions[i];
	}

	SetConsoleCursorPosition(handle, LobbyOptionsCoords[CursorPosition]);
	SetConsoleTextAttribute(handle, color);
	std::cout << LobbyOptions[CursorPosition];

	std::thread t{ [&ClientSocket, HostSocket, &ClientAddress, &AddressSize]
	{   using namespace std::literals::chrono_literals;
		while (true) 
		{
		 ClientSocket = accept(HostSocket, (struct sockaddr *) & ClientAddress, &AddressSize); 
		 std::this_thread::sleep_for(500ms);
		} 
	} };

	do
	{
		SetConsoleTextAttribute(handle, color2);

		if (ClientSocket != INVALID_SOCKET)
		{
			ClientsConnected[ClientsAccepted] = ClientSocket;
			ClientSocket = INVALID_SOCKET;
		}
		for (short i = ClientsAccepted; i < 7; i++)
		{
			SetConsoleCursorPosition(handle, { 1, 20 + i * 2 });
			if (ClientsConnected[i] != INVALID_SOCKET)
			{
				recv(ClientsConnected[i], ClientNames[i], 50, 0);
				std::cout << ClientNames[i] << "                                                 ";
				ClientsAccepted++;
			}
			else
				std::cout << "                                                 ";
		}

		if (kbhit())
		{
			button = _getch();

			//------------------------------------------------------------------------
			if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 80)
			{
				SetConsoleCursorPosition(handle, LobbyOptionsCoords[CursorPosition]);
				std::cout << LobbyOptions[CursorPosition];

				if (CursorPosition == 2)
					CursorPosition = 0;
				else
					CursorPosition += 1;

				SetConsoleTextAttribute(handle, color);

				SetConsoleCursorPosition(handle, LobbyOptionsCoords[CursorPosition]);
				std::cout << LobbyOptions[CursorPosition];
			}
			//------------------------------------------------------------------------
			if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 72)
			{
				SetConsoleCursorPosition(handle, LobbyOptionsCoords[CursorPosition]);
				std::cout << LobbyOptions[CursorPosition];

				if (CursorPosition == 0)
					CursorPosition = 2;
				else
					CursorPosition -= 1;

				SetConsoleTextAttribute(handle, color);

				SetConsoleCursorPosition(handle, LobbyOptionsCoords[CursorPosition]);
				std::cout << LobbyOptions[CursorPosition];
			}
		}
	} while (button != 13);

	t.detach();
	t.~thread();

	for (int i = 0; i < 3; i++)
	{
		SetConsoleCursorPosition(handle, LobbyOptionsCoords[i]);
		std::cout << "            ";
	}
	return CursorPosition;
}
SOCKET ConnectingWithHost(std::string HostAddress)
{
	SOCKET HostSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (HostSocket == INVALID_SOCKET)
	{
		std::cout << "error: " << WSAGetLastError();
		MessageBox(0, "Socket error", "Error", 0);
		WSACleanup();
		exit(0);
	}
	sockaddr_in SocketAddress;
	memset(&SocketAddress, 0, sizeof(SocketAddress));
	SocketAddress.sin_family = AF_INET;
	SocketAddress.sin_port = htons(6919);
	SocketAddress.sin_addr.s_addr = inet_addr(HostAddress.c_str());

	if (connect(HostSocket, (sockaddr *)&SocketAddress, sizeof(SocketAddress)))
	{
		MessageBox(0, "Chosen user stopped hosting", "Error", 0);
		return -1;
	}
	char MyName[50];

	if (gethostname(MyName, sizeof(MyName)) == SOCKET_ERROR)
	{
		MessageBox(0, "Socket MyName error", "Error", 0);
		WSACleanup();
		exit(0);
	}
	send(HostSocket, MyName, 50, 0);
	return HostSocket;

}
int ClientsLobby(HostDataBase &HDB, short width, int color, int color2)
{
	char button;
	int RecivingTime = 100;//ms
	char recvBuff[50];
	int SockAddrSize = sizeof(struct sockaddr_in);
	int CursorPosition = 0;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string helper;

	//--------------------------------------------------------------------Setting socket and socket's address----------------------------------------------------------//
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET)
	{
		MessageBox(0, "Socket error", "Error", 0);
		WSACleanup();
		closesocket(sock);
		exit(0);
	}
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&RecivingTime), sizeof(RecivingTime)) < 0)
	{
		MessageBox(0, "Socket option error", "Error", 0);
		WSACleanup();
		closesocket(sock);
		exit(0);
	}

	sockaddr_in SocketAddress;
	memset(&SocketAddress, 0, sizeof(SocketAddress));
	SocketAddress.sin_family = AF_INET;
	SocketAddress.sin_port = htons(PORT);
	SocketAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (sockaddr*)&SocketAddress, sizeof(SocketAddress)) < 0)
	{
		MessageBox(0, "Socket bind error", "Error", 0);
		WSACleanup();
		closesocket(sock);
		exit(0);
	}

	//------------------------------------------------------------------- Reciving Broadcast message from host ------------------------------------------------------------//
	do
	{
		recvfrom(sock, recvBuff, 50, 0, (sockaddr *)&SocketAddress, &SockAddrSize);

		struct in_addr addr;
		struct hostent *Hostent = gethostbyname(recvBuff);
		if (Hostent != nullptr)
		{
			for (int i = 0; Hostent->h_addr_list[i] != 0; ++i)
			{
				memcpy(&addr, Hostent->h_addr_list[i], sizeof(struct in_addr));
			}
			HDB.AddNewHost(recvBuff, inet_ntoa(addr));
		}


		SetConsoleTextAttribute(handle, color2);
		for (int i = 0; i < HDB.UsedSpace; i++)
		{
			SetConsoleCursorPosition(handle, { static_cast<short>(width - HDB.Hosts[i].size() / 2 + 2), static_cast<short>(i * 2 + 20) });
			std::cout << HDB.Hosts[i];
		}
		SetConsoleCursorPosition(handle, { static_cast<short>(width), static_cast<short>(HDB.UsedSpace * 2 + 20) });
		std::cout << "Back";

		SetConsoleTextAttribute(handle, color);
		helper = (CursorPosition == HDB.UsedSpace) ? "Back" : HDB.Hosts[CursorPosition];
		SetConsoleCursorPosition(handle, { static_cast<short>(width - helper.size() / 2 + 2), static_cast<short>(CursorPosition * 2 + 20) });
		std::cout << helper;

		if (kbhit())
		{
			button = _getch();

			if (CursorPosition > HDB.UsedSpace)
				helper = HDB.Hosts[CursorPosition];
			else
				helper = "Back";

			SetConsoleTextAttribute(handle, color2);
			//------------------------------------------------------------------------
			if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 80)
			{
				for (size_t i = 0; i < helper.size(); i++)
				{
					std::cout << "\b";
				}
				for (size_t i = 0; i < helper.size(); i++)
				{
					std::cout << " ";
				}
				helper = (CursorPosition == HDB.UsedSpace) ? "Back" : HDB.Hosts[CursorPosition];
				SetConsoleCursorPosition(handle, { static_cast<short>(width - helper.size() / 2 + 2), static_cast<short>(CursorPosition * 2 + 20) });
				std::cout << helper;

				if (CursorPosition == HDB.UsedSpace)
					CursorPosition = 0;
				else
					CursorPosition += 1;

				SetConsoleTextAttribute(handle, color);
				helper = (CursorPosition == HDB.UsedSpace) ? "Back" : HDB.Hosts[CursorPosition];
				SetConsoleCursorPosition(handle, { static_cast<short>(width - helper.size() / 2 + 2), static_cast<short>(CursorPosition * 2 + 20) });
				std::cout << helper;
			}
			//------------------------------------------------------------------------
			if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 72)
			{
				for (size_t i = 0; i < helper.size(); i++)
				{
					std::cout << "\b";
				}
				for (size_t i = 0; i < helper.size(); i++)
				{
					std::cout << " ";
				}
				helper = (CursorPosition == HDB.UsedSpace) ? "Back" : HDB.Hosts[CursorPosition];
				SetConsoleCursorPosition(handle, { static_cast<short>(width - helper.size() / 2 + 2), static_cast<short>(CursorPosition * 2 + 20) });
				std::cout << helper;

				if (CursorPosition <= 0)
					CursorPosition = HDB.UsedSpace;
				else
					CursorPosition -= 1;
				SetConsoleTextAttribute(handle, color);
				helper = (CursorPosition == HDB.UsedSpace) ? "Back" : HDB.Hosts[CursorPosition];
				SetConsoleCursorPosition(handle, { static_cast<short>(width - helper.size() / 2 + 2), static_cast<short>(CursorPosition * 2 + 20) });
				std::cout << helper;
			}
		}
	} while (button != 13);
	for (int i = 0; i <= HDB.UsedSpace; i++)
	{
		helper = (i == HDB.UsedSpace) ? "Back" : HDB.Hosts[CursorPosition];
		SetConsoleCursorPosition(handle, { static_cast<short>(width - helper.size() / 2 + 2), static_cast<short>(i * 2 + 20) });
		std::cout << "                                                                      ";
	}
	closesocket(sock);
	return CursorPosition;
}

int HostDataBase::AddNewHost(std::string Name, std::string IP)
{
	if (UsedSpace == MemoryAllocated)
		return 0;

	for (int i = 0; i < UsedSpace; i++)
	{
		if (Hosts[i] == Name)
			return 2;
	}

	Hosts[UsedSpace] = Name;
	Addresses[UsedSpace] = IP;
	UsedSpace++;
	return 1;
}
void HostDataBase::ChangeSize(int Howmany)
{
	std::string* temporaryH = new std::string[Howmany];
	std::string* temporaryA = new std::string[Howmany];

	for (int i = 0; i < UsedSpace && i < Howmany; i++)
	{
		temporaryH[i] = Hosts[i];
		temporaryA[i] = Addresses[i];
	}

	if (UsedSpace > Howmany)
		UsedSpace = Howmany;

	delete[] Hosts;
	delete[] Addresses;

	MemoryAllocated = Howmany;
	Hosts = new std::string[Howmany];
	Hosts = temporaryH;
	Addresses = new std::string[Howmany];
	Addresses = temporaryA;
}
HostDataBase::HostDataBase(int HowMany)
{
	MemoryAllocated = HowMany;
	UsedSpace = 0;
	Hosts = new std::string[HowMany];
	Addresses = new std::string[HowMany];
}
void MultiPlayer::SendArr(SOCKET Client, std::vector<std::string> Tour, int position)
{
	for (int i = 0; i < Atrib[Visibility]; i++)
		send(Client, Tour[i + position].c_str(), 254, 0);
}
void MultiPlayer::VisionBox(SOCKET host, int color, int color2)
{
	std::string *TourArr = new std::string[Atrib[Visibility]];
	char buffer[254];
	int Number;
	std::string helper;
	HANDLE window = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string Distance[] = { "In front of you: ", "Close to you: ", "At some distance: ", "A little further: ",
		"At a considerable distance: ", "Far ahead: ", "Barely noticeable: " };


	for (Number = 0; Number < Atrib[Visibility]; Number++)
	{
		recv(host, buffer, 254, 0);
		if (buffer == "6")
			break;

		TourArr[Number] = buffer;
	}
	for (int i = 0; i < Number; i++)
	{
		helper = TourArr[i];

		SetConsoleTextAttribute(window, color);
		SetConsoleCursorPosition(window, { 1,static_cast<short>(25 + 2 * i) });
		std::cout << Distance[i];
		SetConsoleTextAttribute(window, color2);

		switch (TourArr[i][0])
		{
		case '0':
		{
			if (TourArr[i].size() > 1)
				std::cout << "Turn on asphalt - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Straight road, terrain is asphalt" << "                    ";
			break;
		}
		case '1':
		{
			if (TourArr[i].size() > 1)
				std::cout << "Turn on grass - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Straight grassy road" << "                    ";
			break;
		}
		case '2':
		{
			if (TourArr[i].size() > 1)
				std::cout << "Turn on gravel - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Straight road, gravel" << "                    ";
			break;
		}
		case '3':
		{
			if (TourArr[i].size() > 1)
				std::cout << "Turn on sand - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Linear part of tour, but sandy" << "                    ";
			break;
		}
		case '4':
		{
			if (TourArr[i].size() > 1)
				std::cout << "Turn on mud - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Muddy road, without turn" << "                    ";
			break;
		}
		case '5':
		{
			if (TourArr[i].size() > 1)
				std::cout << "Turn on ice - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Slippery road with ice" << "                    ";
			break;
		}
		}
	}
	if (Number < Atrib[Visibility])
	{
		SetConsoleCursorPosition(window, { 1,static_cast<short>(25 + 2 * (Number+1)) });
		SetConsoleTextAttribute(window, color);
		std::cout << Distance[(Number + 1)];
		SetConsoleTextAttribute(window, color2);
		std::cout << "META                                              ";
		SetConsoleCursorPosition(window, { 1,static_cast<short>(27 + 2 * (Number + 1)) });
		std::cout << "                                                   ";
	}
}
void MultiPlayer::Init1(SOCKET Client, std::string file[], std::string ext[])
{

	std::fstream fvar;
	
	std::string helper, car, tire;
	char buffer[254];

	for (int j = 0; j < 2; j++)
	{
		int number = 0;

		fvar.open(file[j]);
		for (; getline(fvar, helper) && helper != ""; number++);
		send(Client, NumberToString(number).c_str(), 254, 0);
		fvar.close();

		fvar.open(file[j]);
		while (getline(fvar, helper) && helper != "")
			send(Client, helper.c_str(), 254, 0);
		fvar.close();
	}

	recv(Client, buffer, 254, 0);
	name = buffer;
	recv(Client, buffer, 254, 0);
	car = buffer;
	recv(Client, buffer, 254, 0);
	tire = buffer;

	LoadParam(car+ext[0], tire+ext[1]);

	for (int i = 0; i < 8; i++)
		send(Client, NumberToString(Atrib[i]).c_str(), 254, 0);			//sending attribs [8]
	for (int i = 0; i < 6; i++)
		send(Client, Terrain[i].c_str(), 254, 0);						//sending terrain [6]


}
void MultiPlayer::Init(SOCKET Host, int width, int color, int color2) //Client
{
	char buffer[254];
	std::string ChoosenOptions[2];
	std::string Names = "Racer";
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	int HowMany;
	for (int j = 0; j < 2; j++)
	{
		recv(Host, buffer, 254, 0);
		HowMany = atoi(buffer);

		std::string* Options = new std::string[HowMany];
		COORD* coords = new COORD[HowMany];
		int position = 0;

		for (int i = 0; i < HowMany; i++)
		{
			recv(Host, buffer, 254, 0);
			Options[i] = buffer;
			Options[i].erase(Options[i].find("."), Options[i].size() - Options[i].find("."));
			coords[i] = { static_cast<short>(width - Options[i].size()/2 + 2), static_cast<short>(20 + i * 2) };
		}
		Choose(Options, coords, HowMany, position, color, color2);
		ChoosenOptions[j] = Options[position];
		for (int i = 0; i < HowMany; i++)
		{
			SetConsoleCursorPosition(handle, coords[i]);
			std::cout << "                           ";
		}
	}

	char namechar;
	int charpos = Names.size();

	SetConsoleCursorPosition(handle, { static_cast<short>(width - 5),18 });
	std::cout << "CHOOSE YOUR NAME";
	while (true)
	{
		SetConsoleCursorPosition(handle, { static_cast<short>(width - Names.size() / 2),21 });
		SetConsoleTextAttribute(handle, color);
		std::cout << " < ";
		SetConsoleTextAttribute(handle, color2);
		std::cout << Names;
		SetConsoleTextAttribute(handle, color);
		std::cout << " > ";

		namechar = _getch();

		if (namechar == 8 && charpos > 0)
		{
			Names.erase(Names.size() - 1, 1);
			charpos--;
		}
		else if (namechar == 13)
		{
			SetConsoleCursorPosition(handle, { static_cast<short>(width + 9),19 });
			std::cout << "                          ";
			SetConsoleCursorPosition(handle, { static_cast<short>(width + 9),19 });
			Spaces(20);
			break;
		}
		else if (charpos < 16 && namechar >31)
		{
			Names += namechar;
			charpos++;
		}
	}
	SetConsoleCursorPosition(handle, { static_cast<short>(width - 5),21 });
	std::cout << "                             ";
	SetConsoleCursorPosition(handle, { static_cast<short>(width - Names.size() / 2),21 });
	std::cout << "                             ";
	send(Host, Names.c_str(), 254, 0);//wybor imienia
	send(Host, ChoosenOptions[0].c_str(), 254, 0);
	send(Host, ChoosenOptions[1].c_str(), 254, 0);

	for (int i = 0; i < 8; i++)
	{
		recv(Host, buffer, 254, 0);
		Atrib[i] = atof(buffer);
	}
	for (int i = 0; i < 6; i++)
	{
		recv(Host, buffer, 254, 0);
		Terrain[i] = buffer;
	}
	SetConsoleCursorPosition(handle, { static_cast<short>(width - 5),18 });
	std::cout << "                        ";
	name = Names;
	current_speed = 0;
	Score = 0;
}
bool MultiPlayer::Action(float &value, int &position, int color, int color2)
{
	COORD coord[] = { { 2, 39 },{ 2, 41 },{ 2, 43 },{ 2, 45 },{ 2, 47 } };
	std::string actions[] = { "Speed up","Slow down","Hand-Brake","Do nothing","Abaddon Race" };
	HANDLE window = GetStdHandle(STD_OUTPUT_HANDLE);
	char button;

	while (true)
	{
		Choose(actions, coord, 5, position, color, color2);
		switch (position)
		{
		case 0:
		case 1:
		{
			if (current_speed == 0 && position == 1)
			{
				SetConsoleCursorPosition(window, { coord[position].X + static_cast<short>(actions[position].size()), coord[position].Y });
				std::cout << " - You can't do this because you aren't moving...";
				Sleeping(1000);
				SetConsoleCursorPosition(window, { coord[position].X + static_cast<short>(actions[position].size()), coord[position].Y });
				std::cout << "                                                 ";
				break;
			}
			SetConsoleCursorPosition(window, { coord[position].X + static_cast<short>(actions[position].size()), coord[position].Y });
			std::cout << ": ";

			int NumberPosition = 0;
			char Number;
			while (true)
			{
				Number = _getch();

				if ((Number >= '0' && Number <= '9' && value * 10.0f + static_cast<float>(Number - 48) <= Atrib[position * 2]) && (Number > 48 || NumberPosition > 0))
				{
					value = value * 10.0f + static_cast<float>(Number - 48);
					NumberPosition++;

					SetConsoleCursorPosition(window, { coord[position].X + static_cast<short>(actions[position].size()), coord[position].Y });
					std::cout << ": " << value;
				}
				else if (Number == 8 && NumberPosition > 0)
				{
					value = static_cast<float>(static_cast<int>(value / 10.0f));
					NumberPosition--;

					if (NumberPosition == 0)
						value = 0;

					std::cout << "\b \b";
				}
				else if (Number == 13 && value != 0)
				{
					for (int i = 0; i < NumberPosition + 2; i++)
						std::cout << "\b";

					Spaces(NumberPosition + 2);
					return true;
				}
				else if (Number == 27 || Number == 13)
				{
					for (int i = 0; i < NumberPosition + 2; i++)
						std::cout << "\b";

					Spaces(NumberPosition + 2);
					break;
				}
			}
			break;
		}
		case 2:
		case 3:
		case 4:
		{
			while (true)
			{
				SetConsoleCursorPosition(window, { coord[position].X + static_cast<short>(actions[position].size()), coord[position].Y });
				std::cout << " - Do you really want to do this ? (Y/N) ";
				button = _getch();
				if (button == 'y' || button == 'Y')
				{
					if (position == 4)
					{
						value = 0;
						return false;
					}
					if (current_speed > 0)
					{
						value = 0;
						SetConsoleCursorPosition(window, { coord[position].X + static_cast<short>(actions[position].size()), coord[position].Y });
						std::cout << "                                                 ";
						return true;
					}
					else
					{
						SetConsoleCursorPosition(window, { coord[position].X + static_cast<short>(actions[position].size()), coord[position].Y });
						std::cout << " - You can't do this because you aren't moving...";
						Sleeping(1000);
						SetConsoleCursorPosition(window, { coord[position].X + static_cast<short>(actions[position].size()), coord[position].Y });
						std::cout << "                                                 ";
						break;
					}
					
				}
				else if (button == 13 || button == 27 || button == 'n' || button == 'N')
					break;
				
			}
			SetConsoleCursorPosition(window, { coord[position].X + static_cast<short>(actions[position].size()), coord[position].Y });
			std::cout << "                                         ";
		}
		}
	}

}
void MultiPlayer::AssignAction(int position, int value)
{
	if (position == 0)
	{
		if (value > Atrib[Acceleration])
			value = Atrib[Acceleration];
		else if (value < 0)
			value = 0;
		current_speed += static_cast<int>(value);
		if (current_speed > static_cast<int>(Atrib[MaxSpeed]))
			current_speed = static_cast<int>(Atrib[MaxSpeed]);
	}
	else if (position == 1)
	{
		if (value > Atrib[MaxBraking])
			value = Atrib[MaxBraking];
		else if (value < 0)
			value = 0;
		current_speed -= static_cast<int>(value);
		if (current_speed < 0)
			current_speed = 0;
	}
	else if (position == 2)
	{
		if (value > Atrib[MaxBraking])
			value = Atrib[MaxBraking];
		else if (value < 0)
			value = 0;
		current_speed -= static_cast<int>(Atrib[HandBrake]);
		if (current_speed > 40)
			drift = true;
		if (current_speed < 0)
			current_speed = 0;
	}
	else if (position == 4)
	{
		Atrib[Durability] = 0;
		//player quits
	}
}
bool MultiPlayer::Test(std::string field, bool show, int color, int color2)
{
	if (field.size() == 1)
	{
		int r = rand() % 8;
		int lost = (attacks - r)*rand() % 5 * 5;
		if (attacks > r)
		{
			current_speed /= attacks - r;
			Atrib[Durability] -= lost;
			if (show)
			{
				message1.push_back( name + " lost " + NumberToString(lost) + " durability" );
				message2.push_back( ", because of enemies attacks");
			}
		}
		
		attacks = 0;
		return true;
	}



	const char Chelper = field[0];
	field.erase(0, 1);
	std::string tire = Terrain[atoi(&Chelper)];
	std::string helper = tire;
	int find = tire.find("x");
	int reqired_tests = atoi(helper.erase(find, helper.size() - find).c_str());
	int number_of_tests = atoi(tire.erase(0, find + 1).c_str());
	int passed_tests = 0;
	int max = 0, min = 100;
	float local_score;
	float formula;
	float base = static_cast<float>(current_speed) - static_cast<float>(atof(field.c_str()));


	if (base < 0)
		base = 0;

	if (drift == true)
	{
		base *= 100.0f / static_cast<float>(Atrib[DriftEffectivnes]) + static_cast<float>(5 * attacks);
		if (base > 100.0f)
			base = 100.0f;
		formula = (current_speed + base) / 2;
	}
	else
	{
		base *= 100.0f / static_cast<float>(Atrib[TurnEffectivnes]) + static_cast<float>(0.15f * attacks);
		if (base > 100.0f)
			base = 100.0f;
		formula = 1.0f / 3.0f*sqrt(10000.0f - Power((100.0f - base), 2.0f)) + 2.0f / 3.0f*base;
	}

	attacks = 0;

	for (int i = 0; i < number_of_tests; i++)
	{
		local_score = static_cast<float>(rand() % 100) + static_cast<float>(rand() % 100 + 1) / 100.0f;

		if (local_score > formula)
		{
			if (local_score > max)
				max = static_cast<int>(local_score);

			passed_tests++;
		}
		else if (local_score < min)
		{
			min = static_cast<int>(local_score);
		}
	}

	if (passed_tests >= reqired_tests)
	{
		if (show)
		{
			
			message1.push_back( name + " have manage to turn, ");
			message2.push_back("required - " + NumberToString(static_cast<int>(formula)) + " highest roll - " + NumberToString(max));
		}
		

		return true;
	}
	else
	{
		if (show)
		{
			
			message1.push_back( name + " had mistaken, ");
			message2.push_back( "required - " + NumberToString(static_cast<int>(formula)) + " lowest roll - " + NumberToString(min));
		}

		if (formula > static_cast<float>(min + 50))
		{
			Atrib[Durability] -= current_speed * (100.0f + formula - static_cast<float>(min)) / 50.0f;
			if (show)
				message1.push_back(name + " badly crashed !!! ");
				message2.push_back(name + " lost " + NumberToString(current_speed * (100 + static_cast<int>(formula) - min) / 50) + " durability");
			current_speed = 0;
		}
		else if (formula > static_cast<float>(min + 40))
		{
			Atrib[Durability] -= current_speed * (100.0f + formula - static_cast<float>(min)) / 75.0f;
			if (show)
				infobox(name + " crashed !!!, ", name + " lost " + NumberToString(current_speed * (100 + static_cast<int>(formula) - min) / 75) + " durability", color, color2);
			current_speed = 0;
		}
		else if (formula > static_cast<float>(min + 30))
		{

			Atrib[Durability] -= current_speed * (100.0f + formula - static_cast<float>(min)) / 120.0f;
			if (show)
			{
				message1.push_back(name + " had an dangerous accident, ");
				message2.push_back(name + " lost " + NumberToString(current_speed * (100 + static_cast<int>(formula) - min) / 120) + " durability");
			}
			current_speed /= 10;
		}
		else if (formula > static_cast<float>(min + 20))
		{
			Atrib[Durability] -= current_speed;
			if (show)
			{
				message1.push_back(name + " got off the route, ");
				message2.push_back(name + " lost " + NumberToString(current_speed) + " durability");
			}
			current_speed /= 5;
		}
		else if (formula > static_cast<float>(min + 10))
		{
			Atrib[Durability] -= current_speed / 2;
			if (show)
			{
				message1.push_back(name + " fell into a dangerous slip, ");
				message2.push_back(name + " lost " + NumberToString(current_speed / 2) + " durability");
			}
			current_speed /= 2;
		}
		else
		{
			current_speed = static_cast<int>(static_cast<int>(current_speed) / 1.2f);
			if (show)
			{
				message1.push_back(name + " slipped, ");
				message2.push_back(name + " lost a little bit of speed");
			}
		}

		if (Atrib[Durability] <= 0)
		{
			message1.push_back("RIP, " + name + " dezintegrated his vehichle...");
			message2.push_back("");
			return false;
		}
		else
			return true;
	}
}
void MultiPlayer::SendData(SOCKET client, int PlayersAlive)
{
	
	send(client, NumberToString(Score).c_str(), 254, 0);
	send(client, NumberToString(current_speed).c_str(), 254, 0);
	send(client, NumberToString(Atrib[Durability]).c_str(), 254, 0);
	send(client, NumberToString(message1.size()).c_str(), 254, 0);
	for (int i = 0; i < message1.size(); i++)
	{
		send(client, message1[i].c_str(), 254, 0);
		send(client, message2[i].c_str(), 254, 0);
	}
	message1.clear();
	message2.clear();

}
void MultiPlayer::RecvData(SOCKET host, int color, int color2)
{
	HANDLE window = GetStdHandle(STD_OUTPUT_HANDLE);
	char buffer[254];
	recv(host, buffer, 254, 0);
	Score = atof(buffer);
	recv(host, buffer, 254, 0);
	current_speed = atoi(buffer);
	recv(host, buffer, 254, 0);
	Atrib[Durability] = atoi(buffer);
	recv(host, buffer, 254, 0);
	int messages = atoi(buffer);
	for (int i = 0; i < messages; i++)
	{
		recv(host, buffer, 254, 0);
		std::string msg1 = buffer;
		recv(host, buffer, 254, 0);
		std::string msg2 = buffer;
		infobox(msg1, msg2, color, color2);
	}
	SetConsoleTextAttribute(window, color);
	SetConsoleCursorPosition(window, { 0, 16 });
	std::cout << " You're racing as: ";
	SetConsoleTextAttribute(window, color2);
	std::cout << name;
	SetConsoleTextAttribute(window, color);
	SetConsoleCursorPosition(window, { 0, 18 });
	std::cout << " Your current speed is: ";
	SetConsoleTextAttribute(window, color2);
	std::cout << current_speed << "     ";
	SetConsoleTextAttribute(window, color);
	SetConsoleCursorPosition(window, { 0, 20 });
	std::cout << " Your vehice has ";
	SetConsoleTextAttribute(window, color2);
	std::cout << Atrib[Durability] << " durability   ";
}
bool MultiPlayer::Alive()
{
	if (Atrib[Durability] <= 0)
		return false;
	return true;
}
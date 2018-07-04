#include "Racer.h"
#include "Player.h"
#include "Enemy.h"
#include "ToT.h"
#include "Multiplayer.h"

bool BroadcastingWorking = true;

int main(void)
{
	system("title Tour of Turns");
	srand(static_cast<int>(time(0)));								
	system("dir *.rank  > Ranking.txt /b");							//Updates list of avaible files
	system("dir *.tour  > Tour.txt /b");
	system("dir *.tire  > Tire.txt /b");

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD windowsize = GetLargestConsoleWindowSize(handle);

	CONSOLE_FONT_INFOEX ConsoleFontInfoEx = { 0 };					//Changes size of font
	ConsoleFontInfoEx.cbSize = sizeof(ConsoleFontInfoEx);
	ConsoleFontInfoEx.dwFontSize.Y = 14;
	wcscpy_s(ConsoleFontInfoEx.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(handle, NULL, &ConsoleFontInfoEx);

	CONSOLE_CURSOR_INFO Cursor;										//Sets cursor visibility to false
	GetConsoleCursorInfo(handle, &Cursor);
	Cursor.bVisible = false;

	system("mode 650");												//Fullscreens window
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	SetConsoleScreenBufferSize(handle, windowsize);

	char GameTitle[256];                                            //Gets window width
	RECT WindowSize;
	GetWindowText(GetForegroundWindow(), GameTitle, sizeof(GameTitle));
	GetWindowRect(FindWindow(NULL, GameTitle), &WindowSize);
	short width = static_cast<short>(WindowSize.right - WindowSize.left - 20) / 16;

	//PlaySound("test.wav", 0, SND_LOOP | SND_ASYNC);					//Plays music

	int	color = 10, color2 = 15, music = 0;

	Title(width, color, color2);

	while (true)
	{

		//--------------------------------------------------Main menu----------------------------------------------------------------------------
		std::string MainMenuTexts[] = { "Singleplayer","Multiplayer","Rankings","Options","Credits","Exit" };
		COORD MainMenuTextCoords[] = { { width - 5,20 },{ width - 4, 22 },{ width - 3,24 },{ width - 2,26 },{ width - 2,28 },{ width,30 } };

		SetConsoleCursorInfo(handle, &Cursor);
		static int MainMenuCursorPosition = 0;
		Choose(MainMenuTexts, MainMenuTextCoords, 6, MainMenuCursorPosition, color, color2);
		for (int i = 0; i < 7; i++)
		{
			SetConsoleCursorPosition(handle, MainMenuTextCoords[i]);
			std::cout << "             ";
		}
		//----------------------------------------------------------------------------------------------------------------------------------------

		switch (MainMenuCursorPosition)
		{
			//------------------------------------------------------------New game----------------------------------------------------------------
		case 0:
		{
			std::string Tour;
			std::string  helper;

			std::string Car;
			std::string Tire;
			std::string Name;
			Name = "Racer";

			int NumberOfAIs = 0, AliveAIs;
			int CurrentPosition = 0;

			while (true)
			{
				std::string MainText[] = { "Your Name","Number of Rivals", "Continue" };
				COORD MainTextCoords[] = { { width - 1, 19 },{ width - 5, 21 },{ width, 23 } };
				std::string SecondaryText[] = { "none","1","2","3","4","5","6","7" };

				Choose(MainText, MainTextCoords, 3, CurrentPosition, color, color2);
				if (CurrentPosition == 0)
				{
					char namechar;
					int charpos = Name.size();

					while (true)
					{
						SetConsoleCursorPosition(handle, { width + 13,19 });
						SetConsoleTextAttribute(handle, color);
						std::cout << "< ";
						SetConsoleTextAttribute(handle, color2);
						std::cout << Name;
						SetConsoleTextAttribute(handle, color);
						std::cout << " > ";

						namechar = _getch();

						if (namechar == 8 && charpos > 0)
						{
							Name.erase(Name.size() - 1, 1);
							charpos--;
						}
						else if (namechar == 13)
						{
							SetConsoleCursorPosition(handle, { width + 9,19 });
							std::cout << "                          ";
							SetConsoleCursorPosition(handle, { width + 9,19 });
							Spaces(20);
							break;
						}
						else if (charpos < 16 && namechar >31)
						{
							Name += namechar;
							charpos++;
						}
					}
				}
				else if (CurrentPosition == 1)
					HorizontalChoose(SecondaryText, { width + 14,21 }, 8, NumberOfAIs, color, color2);
				else
				{
					for (int i = 0; i < 3; i++)
					{
						SetConsoleCursorPosition(handle, { MainTextCoords[i] });
						std::cout << "                             ";
					}
					break;
				}
			}

			Tour = NewRaceStart(".tour", "CHOOSE A TOUR THAT YOU WOULD LIKE TO PLAY", "Tour.txt", width, color, color2);
			Car = NewRaceStart(".car", "CHOOSE A CAR THAT YOU WOULD LIKE TO DRIVE", Tour, width, color, color2);
			Tire = NewRaceStart(".tire", "CHOOSE THE BEST TIRE FOR THIS RACE", "Tire.txt", width, color, color2);

			CurrentPosition = 0;

			std::fstream fvar;
			std::vector<std::string> TourArr;
			bool start = true;
			int target = 0;

			Player Players(Name, Car, Tire);
			std::vector<Enemy*> Enemies;
			std::string* RacersNames = new std::string[NumberOfAIs];
			float* RacersScores = new float[NumberOfAIs];


			for (int i = 0; i < NumberOfAIs; i++)
				Enemies.push_back(new Enemy(Tour));

			fvar.open(Tour.c_str(), std::ios::in);

			do	getline(fvar, helper);	while (helper != "");
			for (int i = 0; getline(fvar, helper); i++)
				TourArr.push_back(helper);

			fvar.close();
			Players.Interface(color, color2, width * 2);
			Players.VisionBox(TourArr, CurrentPosition, color, color2);

			AliveAIs = NumberOfAIs;

			while (static_cast<int>(TourArr.size()) > CurrentPosition) //main game loop
			{
				for (int i = 0; i < AliveAIs; i++)
				{
					RacersScores[i] = Enemies[i]->Score;
					RacersNames[i] = Enemies[i]->name;
				}
				Players.Ranking(RacersScores, RacersNames, AliveAIs, width * 2, color, color2);

				if (!(Players.Action(start, color, color2)))
					break;

				for (int i = 0; i < AliveAIs; i++)
				{
					if (!(Enemies[i]->Action(Tour, CurrentPosition + 1, color, color2)))
					{
						Enemies[i] = Enemies[AliveAIs - 1];
						AliveAIs -= 1;
					}
				}

				Players.EstimateScore(TourArr, CurrentPosition + 1, color, color2);
				Players.VisionBox(TourArr, CurrentPosition + 1, color, color2);
				

				int target = Players.Attack(RacersScores, RacersNames, AliveAIs, width, color, color2);
				if (target != 10)
					Enemies[target]->attacks += 1;

				for (int i = 0; i < AliveAIs; i++)
				{
					RacersScores[i] = Players.Score;
					target = Enemies[i]->Attack(RacersScores, AliveAIs);
					RacersScores[i] = Enemies[i]->Score;

					if (target == i)
						Players.attacks += 1;
					else if (target != 10)
						Enemies[i]->attacks += 1;
				}
				start = false;
				CurrentPosition++;
			}
			Players.SaveHighScore(Tour.erase(Tour.size() - 5, 5) + ".rank", Players.Ranking(RacersScores, RacersNames, NumberOfAIs, width * 2, color, color2), NumberOfAIs);
			for (int i = 0; i < 10; i++)
				infobox("", "", 8, 8);
			system("cls");
			Title(width, color, color2);

			break;
		}
		//----------------------------------------------------------Multiplayer---------------------------------------------------------------
		case 1:
		{
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);
			int MultiplayerMenuCursorPosition = 0;
			COORD MultiplayerMenuCoords[4] = { { width - 3,20 },{ width - 4,22 },{ width,24 } };
			std::string MultiplayerMenuText[4] = { "Host game", "Search game", "Back" };

			Choose(MultiplayerMenuText, MultiplayerMenuCoords, 3, MultiplayerMenuCursorPosition, color, color2);
			for (int i = 0; i < 3; i++)
			{
				SetConsoleCursorPosition(handle, MultiplayerMenuCoords[i]);
				std::cout << "            ";
			}


			if (MultiplayerMenuCursorPosition == 0) //Host game
			{
				char ClientNames[7][50];
				int LobbyOption;
				SOCKET  Clients[7];
				int CurrentPosition = 0;
				memset(Clients, -1, sizeof(Clients));
				std::string Name = "Racer";

				while (1)
				{
					int NumberOfPlayers = 0;
					BroadcastingWorking = true;
					std::thread Broadcast(Broadcasting);
					std::vector<std::thread> RacerInit;
					LobbyOption = HostsLobby(Clients, ClientNames, width, 10, 15);
					BroadcastingWorking = false;
					Broadcast.join();
					if (LobbyOption == 0)//start 
					{
						std::string Tour = NewRaceStart(".tour", "CHOOSE A TOUR THAT YOU WOULD LIKE TO PLAY", "Tour.txt", width, color, color2);
						std::string a[] = { Tour.c_str(), "Tire.txt" };
						std::string b[] = { ".car", ".tire" };
						for (int i = 0; i < 7; i++)
						{
							if (Clients[i] != INVALID_SOCKET)
								NumberOfPlayers++;
							else
								break;
						}
						MultiPlayer *Racers = new MultiPlayer[NumberOfPlayers];

						for (int i = 0; i < NumberOfPlayers; i++)
							RacerInit.emplace_back(&MultiPlayer::Init1, &Racers[i], Clients[i], a, b);

						char namechar;
						int charpos = Name.size();

						SetConsoleCursorPosition(handle, { static_cast<short>(width - 5),18 });
						std::cout << "CHOOSE YOUR NAME";
						while (true)
						{
							SetConsoleCursorPosition(handle, { static_cast<short>(width - Name.size() / 2),21 });
							SetConsoleTextAttribute(handle, color);
							std::cout << " < ";
							SetConsoleTextAttribute(handle, color2);
							std::cout << Name;
							SetConsoleTextAttribute(handle, color);
							std::cout << " > ";

							namechar = _getch();

							if (namechar == 8 && charpos > 0)
							{
								Name.erase(Name.size() - 1, 1);
								charpos--;
							}
							else if (namechar == 13)
							{
								SetConsoleCursorPosition(handle, { width + 9,19 });
								std::cout << "                          ";
								SetConsoleCursorPosition(handle, { width + 9,19 });
								Spaces(20);
								break;
							}
							else if (charpos < 16 && namechar >31)
							{
								Name += namechar;
								charpos++;
							}
						}
						SetConsoleCursorPosition(handle, { static_cast<short>(width - 5),21 });
						std::cout << "                             ";
						SetConsoleCursorPosition(handle, { static_cast<short>(width - Name.size() / 2),21 });
						std::cout << "                             ";
						std::string Car = NewRaceStart(".car", "CHOOSE A CAR THAT YOU WOULD LIKE TO DRIVE", Tour, width, color, color2);
						std::string Tire = NewRaceStart(".tire", "CHOOSE THE BEST TIRE FOR THIS RACE", "Tire.txt", width, color, color2);
						Player host(Name, Car, Tire);

						for (int i = 0; i < NumberOfPlayers; i++)
							RacerInit[i].join();

						//start interface
						std::fstream fvar;
						std::string helper;
						std::vector<std::string> TourArr;
						fvar.open(Tour.c_str(), std::ios::in);

						do	getline(fvar, helper);	while (helper != "");
						for (int i = 0; getline(fvar, helper); i++)
							TourArr.push_back(helper);

						std::vector<std::thread> CommunicationThreads;
						bool start = true;
						std::string* RacersNames = new std::string[NumberOfPlayers];
						float* RacersScores = new float[NumberOfPlayers];
						int PlayersAlive = NumberOfPlayers;

						host.Interface(color, color2, width * 2);
						host.VisionBox(TourArr, CurrentPosition, color, color2);
						for (int i = 0; i < NumberOfPlayers; i++)
							CommunicationThreads.emplace_back(&MultiPlayer::SendArr, Racers[i], Clients[i], TourArr, CurrentPosition);

						for (int i = 0; i < NumberOfPlayers; i++)
							CommunicationThreads[i].join();

						CommunicationThreads.clear();

						while (static_cast<int>(TourArr.size()) > CurrentPosition)
						{


							for (int i = 0; i < PlayersAlive; i++)
							{
								RacersScores[i] = Racers[i].Score;
								RacersNames[i] = Racers[i].name;
							}


							for (int i = 0; i < NumberOfPlayers; i++)
							{
								CommunicationThreads.emplace_back([=]() {

									send(Clients[i], NumberToString(NumberOfPlayers).c_str(), 254, 0);
									send(Clients[i], NumberToString(host.Score).c_str(), 254, 0);
									send(Clients[i], host.name.c_str(), 254, 0);
									RacersScores[i] = RacersScores[PlayersAlive - 1];
									RacersNames[i] = RacersNames[PlayersAlive - 1];
									for (int j = 0; j < PlayersAlive - 1; j++)
										send(Clients[i], NumberToString(RacersScores[j]).c_str(), 254, 0);
									for (int j = 0; j < PlayersAlive - 1; j++)
										send(Clients[i], RacersNames[j].c_str(), 254, 0);
								});
							}

							host.Ranking(RacersScores, RacersNames, PlayersAlive, width * 2, color, color2);

							for (int i = 0; i < NumberOfPlayers; i++)
								CommunicationThreads[i].join();

							CommunicationThreads.clear();
							for (int i = 0; i < PlayersAlive; i++)
							{
								CommunicationThreads.emplace_back([=]() {
									char buffer[254];
									float value;
									int position;

									recv(Clients[i], buffer, 254, 0);
									value = atof(buffer);
									recv(Clients[i], buffer, 254, 0);
									position = atoi(buffer);
									Racers[i].AssignAction(position, value);
									Racers[i].EstimateScore(TourArr, CurrentPosition + 1, color, color2);
									//sprawdzic czy zyje
								});
							}
							if (!(host.Action(start, color, color2)))
								break;

							for (int i = 0; i < PlayersAlive; i++)
								CommunicationThreads[i].join();

							CommunicationThreads.clear();

							host.EstimateScore(TourArr, CurrentPosition + 1, color, color2);
							host.VisionBox(TourArr, CurrentPosition + 1, color, color2);

							for (int i = 0; i < NumberOfPlayers; i++)
							{
								CommunicationThreads.emplace_back([=]() {

									send(Clients[i], NumberToString(NumberOfPlayers).c_str(), 254, 0);
									send(Clients[i], NumberToString(host.Score).c_str(), 254, 0);
									send(Clients[i], host.name.c_str(), 254, 0);
									RacersScores[i] = RacersScores[PlayersAlive - 1];
									RacersNames[i] = RacersNames[PlayersAlive - 1];
									for (int j = 0; j < PlayersAlive - 1; j++)
										send(Clients[i], NumberToString(RacersScores[j]).c_str(), 254, 0);
									for (int j = 0; j < PlayersAlive - 1; j++)
										send(Clients[i], RacersNames[j].c_str(), 254, 0);

									Racers[i].SendData(Clients[i], PlayersAlive);
								});
							}
							for (int i = 0; i < PlayersAlive; i++)
								CommunicationThreads[i].join();

							CommunicationThreads.clear();

							for (int i = 0; i < NumberOfPlayers; i++)
								CommunicationThreads.emplace_back(&MultiPlayer::SendArr, Racers[i], Clients[i], TourArr, CurrentPosition+1);

							for (int i = 0; i < NumberOfPlayers; i++)
								CommunicationThreads[i].join();

							CommunicationThreads.clear();
							
							int target = host.Attack(RacersScores, RacersNames, PlayersAlive, width, color, color2);
							if (target != 10)
								Racers[target].attacks += 1;
							
							for (int i = 0; i < PlayersAlive; i++)
							{
								CommunicationThreads.emplace_back([&]() {

									char buffer[254];
									recv(Clients[i], buffer, 254, 0);

									if (atoi(buffer) != 10)
									{
										if(atoi(buffer) == i)
											host.attacks += 1;
										else
											Racers[atoi(buffer)].attacks += 1;
									}
								});
							}

							for (int i = 0; i < NumberOfPlayers; i++)
								CommunicationThreads[i].join();
							for (int i = 0; i < NumberOfPlayers; i++)
								Racers[i].SendData(Clients[i], PlayersAlive);
								

							CommunicationThreads.clear();
							
							start = false;
							CurrentPosition++;
						}
						break;
					}
					else if (LobbyOption == 1)//Kicking players
					{
						COORD ClientCoords[8];
						std::string ClientNamesStr[8];
						short i = 0;
						int position = 0;

						for (i = 0; i < 7; i++)
						{
							if (Clients[i] != INVALID_SOCKET)
							{
								ClientCoords[i] = { 1,20 + i * 2 };
								ClientNamesStr[i] = ClientNames[i];
							}
							else
							{
								SetConsoleCursorPosition(handle, { 1,20 + i * 2 });
								std::cout << "                                                 ";
								break;
							}

						}
						ClientCoords[i] = { 1,20 + i * 2 };
						ClientNamesStr[i] = "Back                                             ";

						Choose(ClientNamesStr, ClientCoords, i + 1, position, color, color2);
						if (position != i)
						{
							while (position + 1 < i)
							{
								strncpy(ClientNames[position], ClientNames[position + 1], sizeof(ClientNames[position]) - 1);
								ClientNames[position][sizeof(ClientNames[position]) - 1] = 0;
								Clients[position] = Clients[position + 1];
								position++;
							}
							Clients[i - 1] = INVALID_SOCKET;
						}
					}
					else //back
					{
						SetConsoleCursorPosition(handle, { 0,18 });
						std::cout << "                 ";
						break;
					}

				}

			}
			else if (MultiplayerMenuCursorPosition == 1)//client
			{
				SOCKET HostSocket = INVALID_SOCKET;
				MultiPlayer I;
				do
				{
					HostDataBase HostsRegistred(10);
					for (int i = 20; i < 50; i++)
					{
						SetConsoleCursorPosition(handle, { width - 38,20 });
						std::cout << "                                                                                                                       ";
					}

					int LobbyNumber = ClientsLobby(HostsRegistred, width, 10, 15);
					if (!(LobbyNumber == HostsRegistred.UsedSpace))
					{
						HostSocket = ConnectingWithHost(HostsRegistred.Addresses[LobbyNumber]);
						I.Init(HostSocket, width, color, color2);
					}
					else
						break;

				} while (HostSocket == INVALID_SOCKET);

				I.Interface(color, color2, width * 2);
				I.VisionBox(HostSocket, color, color2);

				float RacersScores[7];
				std::string RacersNames[7];
				int NumberOfRivals = 0;
				int position = 0;

				while (true)
				{
					char buffer[254];
					recv(HostSocket, buffer, 254, 0);

					NumberOfRivals = atoi(buffer);
					
					for (int i = 0; i < NumberOfRivals; i++)
					{
						recv(HostSocket, buffer, 254, 0);
						RacersScores[i] = atof(buffer);
						recv(HostSocket, buffer, 254, 0);
						RacersNames[i] = buffer;
					}
					
					I.Ranking(RacersScores, RacersNames, NumberOfRivals, width * 2, color, color2);
					/* sending action */
					float value = 0;
					if (!(I.Action(value, position, color, color2)))
					{
						break;
					}
					send(HostSocket, NumberToString(value).c_str(), 254, 0);
					send(HostSocket, NumberToString(position).c_str(), 254, 0);

					recv(HostSocket, buffer, 254, 0);

					NumberOfRivals = atoi(buffer);

					for (int i = 0; i < NumberOfRivals; i++)
					{
						recv(HostSocket, buffer, 254, 0);
						RacersScores[i] = atof(buffer);
						recv(HostSocket, buffer, 254, 0);
						RacersNames[i] = buffer;
					}
					I.RecvData(HostSocket, color, color2);
					I.VisionBox(HostSocket, color, color2);
					
					int target = I.Attack(RacersScores, RacersNames, NumberOfRivals, width, color, color2);

					send(HostSocket, NumberToString(target).c_str(), 254, 0);
					I.RecvData(HostSocket, color, color2);
					
				}
				system("cls");
				Title(width, color, color2);
				WSACleanup();
			}
			else
				break;
			break;
		}
		//---------------------------------------------------------------------------------------------------------------------------------
		case 2:
		{
			int helperint = 0;
			std::fstream fvar;
			std::string helper;
			int position = 0;

			fvar.open("Ranking.txt", std::ios::in);

			if (!fvar.good())
			{
				MessageBox(0, "Your game can't even load file that was created a milisecond ago XD", "error", 0);
				break;
			}
			while (std::getline(fvar, helper))
			{
				helperint += 1;
			}
			const int howmany = helperint;
			helperint = 0;
			std::string *maps_rankings = new std::string[howmany];

			if (howmany < 1)
			{
				MessageBox(0, "Your game doesn't have any .rank files", "error", 0);
				break;
			}
			fvar.close();
			fvar.open("Ranking.txt", std::ios::in);
			for (int i = 0; i < howmany; i++)
			{
				std::getline(fvar, helper);
				helper.erase(helper.size() - 5, 5);
				maps_rankings[i] = helper;
			}
			fvar.close();


			do
			{
				int getlines = 0;
				SetConsoleCursorPosition(handle, { 0,25 });
				for (int i = 0; i < 12; i++)
					Spaces(width * 2);
				helperint = position;
				helper = maps_rankings[position] + ".rank";

				fvar.open(helper.c_str());
				SetConsoleCursorPosition(handle, { 0,25 });
				SetConsoleTextAttribute(handle, color);
				std::cout << "Name";
				SetConsoleCursorPosition(handle, { width / 2 - 1,25 });
				std::cout << "AI";
				SetConsoleCursorPosition(handle, { width - 2,25 });
				std::cout << "Time";
				SetConsoleCursorPosition(handle, { width / 2 * 3 - 5,25 });
				std::cout << "Place";
				SetConsoleCursorPosition(handle, { width * 2 - 10,25 });
				std::cout << "Points";
				SetConsoleTextAttribute(handle, color2);

				while (std::getline(fvar, helper))
				{

					switch (getlines % 5)
					{
					case 4:
					{SetConsoleCursorPosition(handle, { 0,static_cast<short>(27 + getlines / 5) }); break; }
					case 3:
					{SetConsoleCursorPosition(handle, { width / 2 - 1,static_cast<short>(27 + getlines / 5) }); break; }
					case 2:
					{SetConsoleCursorPosition(handle, { width - 2,static_cast<short>(27 + getlines / 5) }); break; }
					case 1:
					{SetConsoleCursorPosition(handle, { width / 2 * 3 - 5,static_cast<short>(27 + getlines / 5) }); break; }
					case 0:
					{SetConsoleCursorPosition(handle, { width * 2 - 10,static_cast<short>(27 + getlines / 5) }); break; }
					}
					std::cout << helper;
					getlines++;
				}
				fvar.close();
			} while (HorizontalChoose(maps_rankings, { width - 4 ,20 }, howmany, position, color, color2) && helperint != position);

			SetConsoleCursorPosition(handle, { 0,25 });
			for (int i = 0; i < 12; i++)
				Spaces(width * 2);
			delete[] maps_rankings;
			break;



		}
		case 3:
		{
			int OptionMenuCursorPosition = 0;
			COORD OptionMenuCoords[] = { { width - 2,24 },{ width - 5,26 },{ width - 5,28 },{ width,30 } };
			std::string OptionMenuTexts[] = { " Music  ", " First Color  ", " Second Color ","Back" };

			bool optionloop = true;
			while (optionloop)
			{
				Choose(OptionMenuTexts, OptionMenuCoords, 4, OptionMenuCursorPosition, color, color2);
				switch (OptionMenuCursorPosition)
				{

				case 0://music
				{
					std::string text[2] = { "on","off" };
					int playing = music;

					HorizontalChoose(text, { width + 5,24 }, 2, music, color, color2);
					if (playing - music)
					{
						if (music)	PlaySound(0, 0, 0);
						else		PlaySound("test.wav", 0, SND_LOOP | SND_ASYNC);
					}
					break;
				}
				case 1://First color
				{
					int helperint = color - 2;
					std::string helper;
					std::string colors[14] = { "green","cyan","red","magenta","brown","light gray","dark gray","blue","light green",
						"light cyan","light red","light magenta","yellow","white" };
					helper = colors[color2 - 2];
					colors[color2 - 2] = colors[13];

					if (helperint == 13)
						helperint = color2 - 2;

					HorizontalChoose(colors, { width + 8,26 }, 13, helperint, color, color2);
					color = helperint + 2;

					if (color == color2)
						color = 15;

					Title(width, color, color2);
					break;
				}
				case 2://color2
				{
					int helperint = color2 - 2;
					std::string helper;
					std::string colors[14] = { "green","cyan","red","magenta","brown","light gray","dark gray","blue","light green",
						"light cyan","light red","light magenta","yellow","white" };
					helper = colors[color - 2];
					colors[color - 2] = colors[13];

					if (helperint == 13)
						helperint = color - 2;

					HorizontalChoose(colors, { width + 9,28 }, 13, helperint, color, color2);
					color2 = helperint + 2;

					if (color == color2)
						color2 = 15;

					Title(width, color, color2);
					break;
				}
				case 3:
				{
					optionloop = false;
					for (int i = 0; i < 6; i++)
					{
						SetConsoleCursorPosition(handle, OptionMenuCoords[i]);
						std::cout << "                 ";
					}
				}
				}
			}
			break;
		}
		case 4:
		{
			SetConsoleTextAttribute(handle, color2);
			SetConsoleCursorPosition(handle, { width - 38,20 });
			std::cout << "Hello I am Michal Balicki and I am really happy that you are playing my game.";
			SetConsoleCursorPosition(handle, { width - 43,22 });
			std::cout << "Tour of Turns was my first real project in my programming way and I hope you like it.";
			SetConsoleCursorPosition(handle, { width - 31,24 });
			std::cout << "I would like to thank people who helped me with this project.";
			SetConsoleCursorPosition(handle, { width - 32,27 });
			SetConsoleTextAttribute(handle, color);
			std::cout << "Krystian Grzegorzewicz ";
			SetConsoleTextAttribute(handle, color2);
			std::cout << "without him the game wouldn't be created.";
			SetConsoleCursorPosition(handle, { width - 55,29 });
			std::cout << "He was patiently listening what problems do I have with ToT and the most important he gave me a main idea of the game.";
			SetConsoleCursorPosition(handle, { width - 34,31 });
			SetConsoleTextAttribute(handle, color);
			std::cout << "Marek Loek ";
			SetConsoleTextAttribute(handle, color2);
			std::cout << "for making an icon image and inspiring me to hard work.";
			SetConsoleCursorPosition(handle, { width - 20,33 });
			SetConsoleTextAttribute(handle, color);
			std::cout << "Marcin Marzec ";
			SetConsoleTextAttribute(handle, color2);
			std::cout << "for giving me inspiration.";
			SetConsoleCursorPosition(handle, { width - 46,35 });
			std::cout << "I admire how much work he put in Pan Zniszczenia map for HoMM3 based on Diablo 2 universum.";
			SetConsoleCursorPosition(handle, { width - 29,37 });
			SetConsoleTextAttribute(handle, color);
			std::cout << "Miroslaw Zelent ";
			SetConsoleTextAttribute(handle, color2);
			std::cout << "for getting me interested in programming";
			_getch();
			SetConsoleCursorPosition(handle, { width - 38,20 });
			std::cout << "                                                                                                                       ";
			SetConsoleCursorPosition(handle, { width - 43,22 });
			std::cout << "                                                                                                                       ";
			SetConsoleCursorPosition(handle, { width - 31,24 });
			std::cout << "                                                                                                                       ";
			SetConsoleCursorPosition(handle, { width - 32,27 });
			std::cout << "                                                                                                                       ";
			SetConsoleCursorPosition(handle, { width - 55,29 });
			std::cout << "                                                                                                                       ";
			SetConsoleCursorPosition(handle, { width - 34,31 });
			std::cout << "                                                                                                                       ";
			SetConsoleCursorPosition(handle, { width - 20,33 });
			std::cout << "                                                                                                                       ";
			SetConsoleCursorPosition(handle, { width - 46,35 });
			std::cout << "                                                                                                                       ";
			SetConsoleCursorPosition(handle, { width - 29,37 });
			std::cout << "                                                                                                                       ";
			break;
		}
		case 5:
			return 0;
		}
	}
	_getch();
	return 0;
}
#include "Player.h"
#include "ToT.h"

Player::Player(std::string current_name, std::string car, std::string tire)
{
	name = current_name;
	LoadParam(car, tire);
	
}
bool Player::Action(bool reset, int color, int color2)
{
	COORD coord[] = { { 2, 39},{ 2, 41 },{ 2, 43 },{ 2, 45 },{ 2, 47} };
	std::string actions[] = { "Speed up","Slow down","Hand-Brake","Do nothing","Abaddon Race" };
	static int position = 0;
	HANDLE window = GetStdHandle(STD_OUTPUT_HANDLE);
	char button;

	if (reset)
		position = 0;

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
				std::cout<< ": ";

				int NumberPosition = 0;
				float value = 0;
				char Number;
				while (true)
				{
					Number = _getch();

					if ((Number >= '0' && Number <= '9' && value * 10.0f + static_cast<float>(Number-48)<= Atrib[position*2]) && (Number > 48 || NumberPosition > 0))
					{
						value = value * 10.0f + static_cast<float>(Number - 48);
						NumberPosition++;

						SetConsoleCursorPosition(window, { coord[position].X + static_cast<short>(actions[position].size()), coord[position].Y });
						std::cout <<": "<< value;
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

						if (position == 0)
						{
							current_speed += static_cast<int>(value);
							if (current_speed > static_cast<int>(Atrib[MaxSpeed]))
								current_speed = static_cast<int>(Atrib[MaxSpeed]);
						}
						else if(position == 1)
						{
							current_speed -= static_cast<int>(value);
							if (current_speed < 0)
								current_speed = 0;
						}
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
					std::cout<< " - Do you really want to do this ? (Y/N) ";
					button = _getch();
					if (button == 'y' || button == 'Y')
					{
						if (position == 4)
						{
							Atrib[Durability] = 0;
							return false;
						}
							
						
						if (current_speed > 0)
						{
							if (position == 2)
							{
								current_speed -= static_cast<int>(Atrib[HandBrake]);
								if(current_speed>40)
									drift = true;
								if (current_speed < 0)
									current_speed = 0;
							}
							SetConsoleCursorPosition(window, { coord[position].X + static_cast<short>(actions[position].size()), coord[position].Y });
							std::cout << "                                                 ";
							current_speed = static_cast<int>(static_cast<float>(current_speed)*0.9f);
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
					{
						
						break;
					}
				}
				SetConsoleCursorPosition(window, { coord[position].X + static_cast<short>(actions[position].size()), coord[position].Y });
				std::cout << "                                         ";
			}
		}
	}
	
}
int Player::Attack(float Scores[], std::string Names[], int rivals, int width, int color, int color2)
{
	COORD *coord = new COORD[rivals+1];
	std::string *List_str = new std::string[rivals+1];
	int *List_int = new int[rivals + 1];
	int l = 1;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	width *= 2;
	
	List_str[0] = "Don't attack";
	List_int[0] = 10;
	coord[0] = { static_cast<short>(width - 44), 57 };
	
	for (int i = 0; i < rivals; i++)
	{
		if (Scores[i]<Score + 5 && Scores[i]>Score - 5)
		{
			List_str[l] = Names[i];
			List_int[l] = i;
			coord[l] = { static_cast<short>(width - 44), static_cast<short>(57+l)};
			SetConsoleCursorPosition(handle, coord[l]);
			std::cout << List_str[l];
			l++;
		}
	}
	if (rivals == 0 || l == 1)
		return 10;
	rivals = 0;

	Choose(List_str, coord, l, rivals, color, color2);

	for (int i = 0; i < l; i++)
	{
		SetConsoleCursorPosition(handle, coord[i]);
		std::cout << "                          ";
	}

	return List_int[rivals];
	
}
int Player::Ranking(float EnemyScore[], std::string EnemyName[], int rivals, int width, int color, int color2)
{
	HANDLE window = GetStdHandle(STD_OUTPUT_HANDLE);
	const int HowMany = rivals + 1;
	float *ScoresArray = new float [HowMany];
	std::string *NamesArray = new std::string[HowMany];
	int ReturnValue;

	for (int i = 0; i < rivals; i++)
	{
		ScoresArray[i] = EnemyScore[i];
		NamesArray[i] = EnemyName[i];
	}
	ScoresArray[rivals] = Score;
	NamesArray[rivals] = name;

	bool flag = true;
	float fhelper;
	std::string shelper;

	while (flag)
	{
		flag = false;
		for (int i = 0; i < rivals; i++)
		{
			if (ScoresArray[i] > ScoresArray[i + 1])
			{
				flag = true;
				fhelper = ScoresArray[i];
				shelper = NamesArray[i];
				ScoresArray[i] = ScoresArray[i + 1];
				NamesArray[i] = NamesArray[i + 1];
				ScoresArray[i + 1] = fhelper;
				NamesArray[i + 1] = shelper;
			}
		}
	}

	SetConsoleTextAttribute(window, color);
	SetConsoleCursorPosition(window, { static_cast<short>(width- 35), 17});
	std::cout <<"RACER";
	SetConsoleCursorPosition(window, { static_cast<short>(width - 7), 17});
	std::cout <<"SCORE";
	SetConsoleTextAttribute(window, color2);

	for (int i = 0; i < HowMany; i++)
	{
		SetConsoleCursorPosition(window, { static_cast<short>(width - 35), static_cast<short>(19 + i) });
		std::cout << i+1 << ". " << NamesArray[i]<<"                ";
		SetConsoleCursorPosition(window, { static_cast<short>(width - 7), static_cast<short>(19 + i) });
		std::cout << static_cast<float>(static_cast<int>(ScoresArray[i]*10.0f))/10.0f<<"  ";
		if (NamesArray[i] == name)
			ReturnValue = i+1;
	}
	for (int i = HowMany; i < 9; i++)
	{
		SetConsoleCursorPosition(window, { static_cast<short>(width - 35), static_cast<short>(19 + i) });
		std::cout << "                                  ";
	}
	return ReturnValue;
}
void Player::VisionBox(std::vector<std::string> tour, int starting_pos, int color, int color2)
{
	HANDLE window = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string helper;
	std::string Distance[] = { "In front of you: ", "Close to you: ", "At some distance: ", "A little further: ",
								"At a considerable distance: ", "Far ahead: ", "Barely noticeable: " };

	if (static_cast<int>(tour.size()) < starting_pos)
		return;

	for (int i = 0; i <Atrib[Visibility]; i++)
	{
		if (tour.size() == starting_pos + i)
		{
			SetConsoleCursorPosition(window, { 1,static_cast<short>(25 + 2 * i) });
			SetConsoleTextAttribute(window, color);
			std::cout << Distance[i];
			SetConsoleTextAttribute(window, color2);
			std::cout << "META                                              ";
			SetConsoleCursorPosition(window, { 1,static_cast<short>(27 + 2 * i) });
			std::cout << "                                                   ";
			break;
		}

		helper = tour[starting_pos + i];
		
		SetConsoleTextAttribute(window, color);
		SetConsoleCursorPosition(window,{ 1,static_cast<short>(25+2*i) });
		std::cout << Distance[i];
		SetConsoleTextAttribute(window, color2);

		switch (tour[starting_pos][0])
		{
			case '0':
			{
				if (tour[starting_pos+i].size() > 1)
					std::cout << "Turn on asphalt - safe speed is around "<< helper.erase(0,1) << "          ";
				else
					std::cout << "Straight road, terrain is asphalt"<< "                    ";
				break;
			}
			case '1':
			{
				if (tour[starting_pos+i].size() > 1)
					std::cout << "Turn on grass - safe speed is around " << helper.erase(0, 1) << "          ";
				else
					std::cout << "Straight grassy road" << "                    ";
				break;
			}
			case '2':
			{
				if (tour[starting_pos +i].size() > 1)
					std::cout << "Turn on gravel - safe speed is around " << helper.erase(0, 1) << "          ";
				else
					std::cout << "Straight road, gravel" << "                    ";
				break;
			}
			case '3':
			{
				if (tour[starting_pos + i].size() > 1)
					std::cout << "Turn on sand - safe speed is around " << helper.erase(0, 1)<<"          ";
				else
					std::cout << "Linear part of tour, but sandy" << "                    ";
				break;
			}
			case '4':
			{
				if (tour[starting_pos + i].size() > 1)
					std::cout << "Turn on mud - safe speed is around " << helper.erase(0, 1) << "          ";
				else
					std::cout << "Muddy road, without turn" << "                    ";
				break;
			}
			case '5':
			{
				if (tour[starting_pos + i].size() > 1)
					std::cout << "Turn on ice - safe speed is around " << helper.erase(0, 1) << "          ";
				else
					std::cout << "Slippery road with ice" << "                    ";
				break;
			}
		}
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
bool Player::EstimateScore(std::vector<std::string> tour, int starting_pos, int color, int color2)
{
	HANDLE window = GetStdHandle(STD_OUTPUT_HANDLE);
	bool flag;

	flag = Test(tour[starting_pos - 1], true, color, color2);

	current_speed = static_cast<int>(static_cast<float>(current_speed)*0.9f);

	if (drift == true)
	{
		drift = false;
		Score += 1.5;
	}
	else
		Score += 100 / (1 + current_speed * 10.0f / 36.0f);

	if (flag == false)
		_getch();

	return flag;
}
void Player::Interface(int color, int color2, int width)
{
	HANDLE window = GetStdHandle(STD_OUTPUT_HANDLE);
	
	std::string string[5] = { 
		"You can speed up by (0 to "+NumberToString(static_cast<int>(Atrib[Acceleration]))+")   ",
		"You can slow down by (0 to "+NumberToString(static_cast<int>(Atrib[MaxBraking]))+")   ",
		"Hand brake value is - "+NumberToString(static_cast<int>(Atrib[HandBrake]))+"   ",
		"You can just wait if you're moving...",
		"Remember only weak ones surrenders" };
	
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

	SetConsoleTextAttribute(window, 8);
	for (int i = 0; i < 5; i++)
	{
		SetConsoleCursorPosition(window, { 2, static_cast<short>(40 + i*2) });
		std::cout << string[i];
	}
	SetConsoleCursorPosition(window, { 0, 55 });
	SetConsoleTextAttribute(window, color2);
	std::cout << "                          Infobox                         "<<std::endl;
	std::cout << "----------------------------------------------------------";
	SetConsoleCursorPosition(window, { 0, 67 });
	std::cout << "----------------------------------------------------------";

	SetConsoleTextAttribute(window, color2);
	SetConsoleCursorPosition(window, { static_cast<short>(width - 30),54 });
	std::cout << "Action Box";
	SetConsoleCursorPosition(window, { static_cast<short>(width - 47),55 });
	std::cout << "---------------------------------------------";
	SetConsoleCursorPosition(window, { static_cast<short>(width - 47),67 });
	std::cout << "---------------------------------------------";

}
void Player::SaveHighScore(std::string Path, int Place, int Rivals)
{
		if (Atrib[Durability] <= 0)
			return;

		std::fstream fvar;
		std::string helper[50];
		int HowMany = 0;
		bool flag = true;
		int Points = (Rivals + 2 - Place) * static_cast<int>(10000.0f / Score);

		fvar.open(Path.c_str(), std::ios::in);
		for (; getline(fvar, helper[HowMany]) && HowMany<44; HowMany++);
		fvar.close();

		fvar.open(Path.c_str(), std::ios::out);
		for (int number = 0; number<HowMany || number < 5; number += 5)
		{
			if (Points > atoi(helper[number].c_str()) && flag)
			{
				fvar << NumberToString(Points) << std::endl;
				fvar << NumberToString(Place) << std::endl;
				fvar << NumberToString(static_cast<int>(Score)) << std::endl;
				fvar << NumberToString(Rivals) << std::endl;
				fvar << name << std::endl;
				std::cout << NumberToString(Points) << std::endl;
				std::cout << NumberToString(Place) << std::endl;
				std::cout << NumberToString(static_cast<int>(Score)) << std::endl;
				std::cout << NumberToString(Rivals) << std::endl;
				std::cout << name << std::endl;
				number -= 5;
				flag = false;
			}
			else
			{
				for (int i = 0; i<5; i++)
				{
					fvar << helper[number + i] << std::endl;
					std::cout << helper[number + i] << std::endl;
				}
			}
		}
		fvar.close();
}

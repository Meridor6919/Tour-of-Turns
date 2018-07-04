#include "ToT.h"

void Title(int width, int color, int color2)
{ 
	HANDLE windowtot = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(windowtot, color);
	SetConsoleCursorPosition(windowtot, { 0,0 });
	
	Spaces(width - 20); std::cout << "[ ]     _______  ___         ___      [ ]\n";
	Spaces(width - 20); std::cout << "[ ]        |    |   | |   | |   |     [ ]\n";
	Spaces(width - 20); std::cout << "[ ]        |    |   | |   | |___|     [ ]\n";
	Spaces(width - 20); std::cout << "[ ]        |    |   | |   | |  \\      [ ]\n";
	Spaces(width - 20); std::cout << "[ ]        |    |___| |___| |   \\     [ ]\n";
	Spaces(width - 20); std::cout << "[ ]                                   [ ]\n";
	Spaces(width - 20); std::cout << "[ ]                                   [ ]\n";
	Spaces(width - 20); std::cout << "[ ]                                   [ ]\n";
	Spaces(width - 20); std::cout << "[ ]                                   [ ]\n";
	Spaces(width - 20); std::cout << "[ ]                                   [ ]\n";
	Spaces(width - 20); std::cout << "[ ]   _______        ___   __   __    [ ]\n";
	Spaces(width - 20); std::cout << "[ ]      |    |   | |   | |  | |      [ ]\n";
	Spaces(width - 20); std::cout << "[ ]      |    |   | |___| |  | |__    [ ]\n";
	Spaces(width - 20); std::cout << "[ ]      |    |   | |  \\  |  |    |   [ ]\n";
	Spaces(width - 20); std::cout << "[ ]      |    |___| |   \\ |  | ___|   [ ]\n";
	
	SetConsoleTextAttribute(windowtot, color2);
	for (short i = 0; i < 15; i++)
	{
		SetConsoleCursorPosition(windowtot, { static_cast<short>(width-19),i });
		std::cout << "*";
		SetConsoleCursorPosition(windowtot, { static_cast<short>(width+19),i });
		std::cout << "*";
	}
	SetConsoleCursorPosition(windowtot, { static_cast<short>(width - 3),5 }); std::cout << " ___    ___";
	SetConsoleCursorPosition(windowtot, { static_cast<short>(width - 3),6 }); std::cout << "|   |  |";
	SetConsoleCursorPosition(windowtot, { static_cast<short>(width - 3),7 }); std::cout << "|   |  |_";
	SetConsoleCursorPosition(windowtot, { static_cast<short>(width - 3),8 }); std::cout << "|   |  |";
	SetConsoleCursorPosition(windowtot, { static_cast<short>(width - 3),9 }); std::cout << "|___|  |";
}
void Sleeping(int time)
{
	HANDLE windowtot = GetStdHandle(STD_INPUT_HANDLE);
	DWORD  consolesettings;

	GetConsoleMode(windowtot, &consolesettings);		
	SetConsoleMode(windowtot, 0 & ~ENABLE_ECHO_INPUT);	
	Sleep(time);
	FlushConsoleInputBuffer(windowtot);					
	SetConsoleMode(windowtot, consolesettings);			
}
void Choose(std::string text[], COORD coord[], int HowMany, int &pos, int color, int color2)
{
	char button;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string helper;

	SetConsoleTextAttribute(handle, color2);
	for (int i = 0; i<HowMany; i++)                          
	{
		SetConsoleCursorPosition(handle, coord[i]);
		std::cout << text[i];
	}
	SetConsoleCursorPosition(handle, coord[pos]);
	SetConsoleTextAttribute(handle, color);
	std::cout << text[pos];                                   

	do                               
	{
		button = _getch();
		SetConsoleTextAttribute(handle, color2);
		//------------------------------------------------------------------------
		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 80)
		{
			helper = text[pos];                               
			for (size_t i = 0; i<helper.size(); i++)
			{
				std::cout << "\b";
			}
			for (size_t i = 0; i<helper.size(); i++)
			{
				std::cout << " ";
			}
			SetConsoleCursorPosition(handle, coord[pos]); std::cout << text[pos];
			if (pos >= HowMany - 1)
				pos = 0;
			else
				pos+=1;
			SetConsoleTextAttribute(handle, color);
			SetConsoleCursorPosition(handle, coord[pos]);
			std::cout << text[pos];
		}
		//------------------------------------------------------------------------
		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 72)
		{
			helper = *text;
			for (size_t i = 0; i<helper.size(); i++)
			{
				std::cout << "\b";
			}
			for (size_t i = 0; i<helper.size(); i++)
			{
				std::cout << " ";
			}
			SetConsoleCursorPosition(handle, coord[pos]); std::cout << text[pos];
			if (pos <= 0)
				pos = HowMany-1;
			else
				pos -= 1;
			SetConsoleTextAttribute(handle, color);
			SetConsoleCursorPosition(handle, coord[pos]);
			std::cout << text[pos];
		}
	} while (button != 13);
}
bool HorizontalChoose(std::string text[], COORD coord, int HowMany, int &pos, int color, int color2)
{
	HANDLE windowtot = GetStdHandle(STD_OUTPUT_HANDLE);
	char button;
	std::string helper;

	SetConsoleCursorPosition(windowtot, coord);
	SetConsoleTextAttribute(windowtot, color);
	std::cout << "< ";
	SetConsoleTextAttribute(windowtot, color2);
	std::cout << text[pos];
	SetConsoleTextAttribute(windowtot, color);
	std::cout << " >";

	do
	{
		button = _getch();
		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 75 && pos > 0)
			pos -= 1;
		else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 77 && pos < HowMany - 1)
			pos += 1;
		else if (button == 27)
			return false;

		helper = text[pos];
		SetConsoleCursorPosition(windowtot, coord);
		SetConsoleTextAttribute(windowtot, color);
		std::cout << "< ";
		SetConsoleTextAttribute(windowtot, color2);
		std::cout << text[pos];
		SetConsoleTextAttribute(windowtot, color);
		std::cout << " >              "; Spaces(helper.size() - helper.size());
	} while (button != 13);

	SetConsoleCursorPosition(windowtot, coord);
	Spaces(helper.size() + 4);	
	return true;
}
std::string NewRaceStart(std::string ext, std::string text, std::string file, short width, int color, int color2)
{	
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string helper;
	std::fstream fvar;
	int NumberOfLines = 0;
	int position = 0;
	

	SetConsoleCursorPosition(handle, { static_cast<short>(width - text.size()/2 - 7),18 });
	SetConsoleTextAttribute(handle, color2);
	std::cout <<"        "<< text<<"        ";

	fvar.open(file.c_str());
	for (; std::getline(fvar, helper) && helper != ""; NumberOfLines++);
	fvar.close();

	std::string *Names = new std::string[NumberOfLines];
	COORD *Coord = new COORD[NumberOfLines];

	fvar.open(file.c_str());
	for (int i = 0; i<NumberOfLines; i++)
	{
		std::getline(fvar, helper);
		Names[i] = helper.erase(helper.size() - ext.size(), ext.size());
		Coord[i] = { static_cast<short>(width - helper.size() / 2 +2), static_cast<short>(22 + i * 2) };
		if (helper.size() % 2 == 1)
			Coord[i].X -= 1;
	}
	fvar.close();

	if (NumberOfLines > 12)
		NumberOfLines = 12;

	Choose(Names, Coord, NumberOfLines, position, color, color2);
	
	SetConsoleCursorPosition(handle, { static_cast<short>(width - text.size() / 2 - 7),18 });
	std::cout << "                                                                             ";
	for (; NumberOfLines + 1; NumberOfLines--)
	{
		SetConsoleCursorPosition(handle, Coord[NumberOfLines]);
		std::cout << "                                       ";
	}
	
	helper = Names[position] + ext;

	delete[] Names;
	delete[] Coord;
	return helper;
}
void infobox(std::string text1, std::string text2, int color, int color2)
{
	static std::string info[10] = { "","","","","","","","","",""};
	std::string helper;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 8); 

	for (int i = 9; i>0; i--)
	{
		info[i] = info[i - 1];
		SetConsoleCursorPosition(handle, { 1, static_cast<short>(66 - i) });
		std::cout << info[i] << "                        ";
	}
	info[0] = text1 + text2;
	SetConsoleCursorPosition(handle, { 1, 66 });
	SetConsoleTextAttribute(handle, color);
	std::cout << text1;
	SetConsoleTextAttribute(handle, color2);
	std::cout << " " << text2<<"                        ";
}
float factorial(float number)
{
	if (number == 0)
		return 1;
	return number * factorial(number - 1);
}
float Power(float number, float ToThePowerOf)
{
	float result = 1.0f;
	for (int i = 0; i < static_cast<int>(ToThePowerOf); i++)
	{
		result = result * number;
	}
	return result;
}

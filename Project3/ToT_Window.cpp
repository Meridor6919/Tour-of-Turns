#include "ToT_Window.h"

ToT_Window::ToT_Window(char title[], int color1, int color2, int font_size, bool fullscreen, int chars_in_rows, int chars_in_columns) : Window(title, color1, color2, font_size, fullscreen, chars_in_rows, chars_in_columns)
{
	LoadFiles("Ranking.txt", "rank");
	if (!LoadFiles("Tire.txt", "tire"))
		MessageBox(0, "No tire files found, please reinstall your game or reapair missing files", "File Error", 0);
	if (!LoadFiles("Tour.txt", "tour"))
		MessageBox(0, "No tour files found, please reinstall your game or reapair missing files", "File Error", 0);
}
bool ToT_Window::LoadFiles(std::string file_name, std::string ext)
{
	file_name = "dir *." + ext + " > " + file_name + " /b";
	if (system(file_name.c_str()))//Updates list of avaible files
	{
		SetConsoleCursorPosition(window_handle, { 0,0 });
		std::cout << "                       ";
		SetConsoleCursorPosition(window_handle, { 0,0 });
		return false;
	}
	return true;
}
void ToT_Window::SetHamachiConnectionFlag(bool flag)
{
	hamachi_enabled = flag;
}
void ToT_Window::Title()
{
	SetConsoleTextAttribute(window_handle, color2);
	SetConsoleCursorPosition(window_handle, { 0,0 });

	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,0 }); std::cout << "[ ]     _______  ___         ___      [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,1 }); std::cout << "[ ]        |    |   | |   | |   |     [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,2 }); std::cout << "[ ]        |    |   | |   | |___|     [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,3 }); std::cout << "[ ]        |    |   | |   | |  \\      [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,4 }); std::cout << "[ ]        |    |___| |___| |   \\     [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,5 }); std::cout << "[ ]                                   [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,6 }); std::cout << "[ ]                                   [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,7 }); std::cout << "[ ]                                   [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,8 }); std::cout << "[ ]                                   [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,9 }); std::cout << "[ ]                                   [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,10 }); std::cout << "[ ]   _______        ___    __   __   [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,11 }); std::cout << "[ ]      |    |   | |   |  |  | |     [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,12 }); std::cout << "[ ]      |    |   | |___|  |  | |__   [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,13 }); std::cout << "[ ]      |    |   | |  \\   |  |    |  [ ]";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X / 2) - 22,14 }); std::cout << "[ ]      |    |___| |   \\  |  | ___|  [ ]";

	SetConsoleTextAttribute(window_handle, color1);
	for (short i = 0; i < 15; i++)
	{
		SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X/2 - 21),i });
		std::cout << "*";
		SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X/2 + 17),i });
		std::cout << "*";
	}
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X/2) - 5,5 }); std::cout << " ___    ___";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X/2) - 5,6 }); std::cout << "|   |  |";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X/2) - 5,7 }); std::cout << "|   |  |_";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X/2) - 5,8 }); std::cout << "|   |  |";
	SetConsoleCursorPosition(window_handle, { static_cast<short>(window_size.X/2) - 5,9 }); std::cout << "|___|  |";
}
#include "ToT_Window.h"

ToT_Window::ToT_Window(char title[], int color1, int color2, int chars_in_rows, int chars_in_columns) : Window(title, color1, color2, chars_in_rows, chars_in_columns)
{
	LoadFiles("Ranking.txt", "rank");
	if (!LoadFiles("Tire.txt", "tire"))
		MessageBox(0, "No tire files found, please reinstall your game or repair missing files", "File Error", 0);
	if (!LoadFiles("Tour.txt", "tour"))
		MessageBox(0, "No tour files found, please reinstall your game or repair missing files", "File Error", 0);
}
bool ToT_Window::LoadFiles(std::string path, std::string ext)
{
	path = "dir *." + ext + " > " + path + " /b";
	if (system(path.c_str()))
	{
		//if file does't exist we need to clear error message that shows in a console window by default
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
void ToT_Window::Title(COORD starting_point, Text::TextAlign text_align)
{
	SetConsoleTextAttribute(window_handle, color2);
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y }); std::cout << "[ ]     _______  ___         ___      [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 1 }); std::cout << "[ ]        |    |   | |   | |   |     [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 2 }); std::cout << "[ ]        |    |   | |   | |___|     [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 3 }); std::cout << "[ ]        |    |   | |   | |  \\      [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 4 }); std::cout << "[ ]        |    |___| |___| |   \\     [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 5 }); std::cout << "[ ]                                   [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 6 }); std::cout << "[ ]                                   [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 7 }); std::cout << "[ ]                                   [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 8 }); std::cout << "[ ]                                   [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 9 }); std::cout << "[ ]                                   [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 10 }); std::cout << "[ ]   _______        ___    __   __   [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 11 }); std::cout << "[ ]      |    |   | |   |  |  | |     [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 12 }); std::cout << "[ ]      |    |   | |___|  |  | |__   [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 13 }); std::cout << "[ ]      |    |   | |  \\   |  |    |  [ ]";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42), starting_point.Y + 14 }); std::cout << "[ ]      |    |___| |   \\  |  | ___|  [ ]";

	SetConsoleTextAttribute(window_handle, color1);
	for (short i = 0; i < 15; i++)
	{
		SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42)+1, starting_point.Y+i });
		std::cout << "*";
		SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42)+39, starting_point.Y+i });
		std::cout << "*";
	}
	
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42) + 17, starting_point.Y + 5 }); std::cout << " ___   ___";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42) + 17, starting_point.Y + 6 }); std::cout << "|   | |";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42) + 17, starting_point.Y + 7 }); std::cout << "|   | |_";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42) + 17, starting_point.Y + 8 }); std::cout << "|   | |";
	SetConsoleCursorPosition(window_handle, { starting_point.X - static_cast<short>((float)text_align / 2 * 42) + 17, starting_point.Y + 9 }); std::cout << "|___| |";
}
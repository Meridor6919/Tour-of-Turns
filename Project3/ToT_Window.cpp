#include "ToT_Window.h"

ToT_Window::ToT_Window(const std::string title, const int color1, const int color2, const short chars_in_rows, const short chars_in_columns) : Window(title, color1, color2, chars_in_rows, chars_in_columns)
{
	LoadFiles("Ranking.txt", "rank");
	if (!LoadFiles("Tire.txt", "tire"))
	{
		MessageBox(0, "No tire files found, please reinstall your game or repair missing files", "File Error", 0);
	}
	if (!LoadFiles("Tour.txt", "tour"))
	{
		MessageBox(0, "No tour files found, please reinstall your game or repair missing files", "File Error", 0);
	}
}
bool ToT_Window::LoadFiles(std::string path, const std::string ext)
{
	path = "dir *." + ext + " > " + path + " /b";
	if (system(path.c_str()))
	{
		//Clearing error message that shows in a console window by default when files not found
		SetConsoleCursorPosition(window_handle, { 0,0 });
		std::cout << "                       ";
		SetConsoleCursorPosition(window_handle, { 0,0 });
		return false;
	}
	return true;
}
void ToT_Window::SetHamachiConnectionFlag(const bool flag)
{
	hamachi_enabled = flag;
}
void ToT_Window::Title(const COORD starting_point, const Text::TextAlign text_align)
{
	const std::vector<std::string> main_text = {
		"  _______  ___         ___     ",
		"     |    |   | |   | |   |    ",
		"     |    |   | |   | |___|    ",
		"     |    |   | |   | |  \\     ",
		"     |    |___| |___| |   \\    ",
		"                               ",
		"                               ",
		"                               ",
		"                               ",
		"                               ",
		"_______        ___    __   __  ",
		"   |    |   | |   |  |  | |    ",
		"   |    |   | |___|  |  | |__  ",
		"   |    |   | |  \\   |  |    | ",
		"   |    |___| |   \\  |  | ___| ",
	};
	const std::vector<std::string> additional_text = {
		" ___   ___",
		"|   | |   ",
		"|   | |_  ",
		"|   | |   ",
		"|___| |   ",
	};
	const COORD orientation_point = { starting_point.X - static_cast<short>((float)text_align / 2 * main_text[0].size()), starting_point.Y };
	const short decoration_distance = 5;
	const std::string main_decoration = "{ }";
	const std::string additional_decoration = "*";
	//Main text
	SetConsoleTextAttribute(window_handle, color2);
	for (short i = 0; i < static_cast<short>(main_text.size()); ++i)
	{
		SetConsoleCursorPosition(window_handle, { orientation_point.X, orientation_point.Y + i });
		std::cout << main_text[i];
	}
	SetConsoleTextAttribute(window_handle, color1);
	for (short i = 0; i < static_cast<short>(additional_text.size()); ++i)
	{
		SetConsoleCursorPosition(window_handle, { orientation_point.X + static_cast<short>(main_text[i].size()/2 - additional_text[i].size()/2), orientation_point.Y + i + static_cast<short>(main_text.size()) / 3 });
		std::cout << additional_text[i];
	}
	//Decoration
	SetConsoleTextAttribute(window_handle, color2);
	for (short i = 0; i < static_cast<short>(main_text.size()); ++i)
	{
		SetConsoleCursorPosition(window_handle, { orientation_point.X - static_cast<short>(decoration_distance + main_decoration.size()) - i % 2, orientation_point.Y + i });
		std::cout << main_decoration;
		SetConsoleCursorPosition(window_handle, { orientation_point.X + static_cast<short>(main_text[i].size() + decoration_distance - 1) - i % 2, orientation_point.Y + i });
		std::cout << main_decoration;
	}
	SetConsoleTextAttribute(window_handle, color1);
	for (short i = 0; i < static_cast<short>(main_text.size()); ++i)
	{
		SetConsoleCursorPosition(window_handle, { orientation_point.X - static_cast<short>(decoration_distance + main_decoration.size()) + 1 - i % 2, orientation_point.Y + i });
		std::cout << additional_decoration;
		SetConsoleCursorPosition(window_handle, { orientation_point.X + static_cast<short>(main_text[i].size() + decoration_distance - 1) + 1 - i % 2, orientation_point.Y + i });
		std::cout << additional_decoration;
	}
}
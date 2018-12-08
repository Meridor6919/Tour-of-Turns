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

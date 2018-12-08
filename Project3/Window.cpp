#include "Window.h"


Window::Window(char title[], int color1, int color2, int font_size, bool fullscreen, int chars_in_rows, int chars_in_columns)
{

	strcpy(this->title, title);
	this->color1 = color1;
	this->color2 = color2;
	this->font_size = font_size;
	window_size = { (short)chars_in_rows, (short)chars_in_columns };
	window_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	window_hwnd = GetConsoleWindow();

	char temp_char[254] = "title ";
	system(strcat(temp_char, title));
	srand(static_cast<int>(time(0)));

	//Font setup
	CONSOLE_FONT_INFOEX ConsoleFontInfoEx = { 0 };
	ConsoleFontInfoEx.cbSize = sizeof(ConsoleFontInfoEx);
	ConsoleFontInfoEx.dwFontSize.Y = font_size;
	wcscpy_s(ConsoleFontInfoEx.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(window_handle, NULL, &ConsoleFontInfoEx);
	GetCurrentConsoleFontEx(window_handle, NULL, &ConsoleFontInfoEx);

	//window setup
	if (fullscreen)
		window_size = GetLargestConsoleWindowSize(window_handle);
	MoveWindow(window_hwnd, 0, 0, window_size.X * ConsoleFontInfoEx.dwFontSize.X, window_size.Y*ConsoleFontInfoEx.dwFontSize.Y, true);

	//Cursor visibility								
	GetConsoleCursorInfo(window_handle, &console_cursor);
	SetCursor(false);
}
int Window::GetFontSize()
{
	return font_size;
}
HANDLE Window::GetHandle()
{
	return window_handle;
}
HWND Window::GetHWND()
{
	return window_hwnd;
}
int Window::GetHeight()
{
	return window_size.Y;
}
int Window::GetWidth()
{
	return window_size.X;
}
void Window::SetWindowFlags(int flags)
{
	flags |= SWP_NOMOVE | SWP_NOSIZE;


	SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, flags);
	SetConsoleCursorInfo(window_handle, &console_cursor);
}
void Window::SetCursor(bool visible)
{
	console_cursor.bVisible = visible;
	SetConsoleCursorInfo(window_handle, &console_cursor);
}
void Window::SetMusic(std::string sound_file, bool playing)
{
	if (playing)
		PlaySound(sound_file.c_str(), 0, SND_LOOP | SND_ASYNC);
	else
		PlaySound(0, 0, 0);
}
void Window::Pause(int miliseconds)
{
	DWORD  consolesettings;

	GetConsoleMode(window_handle, &consolesettings);
	SetConsoleMode(window_handle, 0 & ~ENABLE_ECHO_INPUT);
	Sleep(miliseconds);
	FlushConsoleInputBuffer(window_handle);
	SetConsoleMode(window_handle, consolesettings);
}

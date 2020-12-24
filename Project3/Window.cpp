#include "Window.h"


Window::Window(const std::string title, const int color1, const int color2, const short chars_in_rows, const short chars_in_columns)
{
	//Saving crucial data
	this->color1 = color1;
	this->color2 = color2;
	this->window_size = { chars_in_rows, chars_in_columns };
	this->window_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	this->window_hwnd = GetConsoleWindow();
	SetConsoleTitle(title.c_str());
	srand(static_cast<int>(time(0)));

	//Automatically detecting appropriate font size
	CONSOLE_FONT_INFOEX ConsoleFontInfoEx = { 0 };
	ConsoleFontInfoEx.cbSize = sizeof(ConsoleFontInfoEx);
	ConsoleFontInfoEx.dwFontSize.Y = 64;
	wcscpy_s(ConsoleFontInfoEx.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(window_handle, NULL, &ConsoleFontInfoEx);

	for(COORD c = GetLargestConsoleWindowSize(window_handle); (c.X < chars_in_rows || c.Y < chars_in_columns) && ConsoleFontInfoEx.dwFontSize.Y > 0; c = GetLargestConsoleWindowSize(window_handle))
	{
		--ConsoleFontInfoEx.dwFontSize.Y;
		SetCurrentConsoleFontEx(window_handle, NULL, &ConsoleFontInfoEx);
	}
	font_size = ConsoleFontInfoEx.dwFontSize.Y;
	
	//Setting window size
	window_size = GetLargestConsoleWindowSize(window_handle);
	SetConsoleScreenBufferSize(window_handle, { window_size.X - 1,window_size.Y - 1 });
	SetWindowLongA(window_hwnd, GWL_STYLE, GetWindowLong(window_hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_CAPTION);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	
	//Cursor visibility	set to false by default							
	SetCursor(false);
}
std::vector<std::string> Window::ReadFile(const std::string path)
{
	std::vector<std::string> data;
	std::fstream fvar;
	std::string helper;
	fvar.open(path);
	while (getline(fvar, helper) && helper != "")
	{
		data.push_back(std::move(helper));
	}
	fvar.close();
	return data;
}
void Window::Pause(const int miliseconds)
{
	DWORD consolesettings;
	HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(input_handle, &consolesettings);
	SetConsoleMode(input_handle, 0 & ~ENABLE_ECHO_INPUT);
	Sleep(miliseconds);
	FlushConsoleInputBuffer(input_handle);
	SetConsoleMode(input_handle, consolesettings);
}
int Window::GetWidth()
{
	return window_size.X;
}
int Window::GetHeight()
{
	return window_size.Y;
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
float Window::GetMusicVolume()
{
	return music_volume;
}
void Window::SetCursor(const bool visible)
{
	CONSOLE_CURSOR_INFO console_cursor;
	GetConsoleCursorInfo(window_handle, &console_cursor);
	console_cursor.bVisible = visible;
	SetConsoleCursorInfo(window_handle, &console_cursor);
}
void Window::SetMusic(float volume)
{
	this->music_volume = volume;
	wav_transformer.SetFlags(SND_ASYNC | SND_LOOP);
	wav_transformer.StartPlaying(volume);
}
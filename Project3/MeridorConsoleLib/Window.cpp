#include "Window.h"


void MeridorConsoleLib::Window::AdjustFontSize()
{
	CONSOLE_FONT_INFOEX ConsoleFontInfoEx = { 0 };
	ConsoleFontInfoEx.cbSize = sizeof(ConsoleFontInfoEx);
	ConsoleFontInfoEx.dwFontSize.Y = 64;
	wcscpy_s(ConsoleFontInfoEx.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(window_info.handle, NULL, &ConsoleFontInfoEx);
	
	for (COORD c = GetLargestConsoleWindow();
		(c.X < window_info.characters_capacity.X || c.Y < window_info.characters_capacity.Y) && ConsoleFontInfoEx.dwFontSize.Y > 0;
		c = GetLargestConsoleWindow())
	{
		--ConsoleFontInfoEx.dwFontSize.Y;
		SetCurrentConsoleFontEx(window_info.handle, NULL, &ConsoleFontInfoEx);
	}
	font_size = ConsoleFontInfoEx.dwFontSize.Y;
}

void MeridorConsoleLib::Window::SetWindowSize()
{
	LONG flags;
	if (window_info.window_option == WindowOption::fullscreen)
	{
		flags = WS_POPUPWINDOW & ~WS_CAPTION & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_HSCROLL & ~WS_VSCROLL;
	}
	else
	{
		flags = WS_CAPTION | WS_POPUPWINDOW | WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_HSCROLL & ~WS_VSCROLL;
	}
	window_info.characters_capacity = GetLargestConsoleWindow();
	SetConsoleScreenBufferSize(window_info.handle, { window_info.characters_capacity.X - 1, window_info.characters_capacity.Y - 1 });
	SetWindowLongA(window_info.hwnd, GWL_STYLE, flags);
	ShowWindow(GetConsoleWindow(), SW_SHOWMAXIMIZED);
}

COORD MeridorConsoleLib::Window::GetLargestConsoleWindow()
{
	CONSOLE_FONT_INFO font_info;
	GetCurrentConsoleFont(window_info.handle, false, &font_info);
	
	short caption_size = GetSystemMetrics(SM_CYCAPTION) * (window_info.window_option != WindowOption::fullscreen);
	short x = GetSystemMetrics(SM_CXSCREEN) / font_info.dwFontSize.X;
	short y = (GetSystemMetrics(SM_CYSCREEN) - caption_size) / font_info.dwFontSize.Y;

	return { x, y };
}

void MeridorConsoleLib::Window::SetConsoleEditMode(bool enable)
{
	DWORD prev_mode;
	DWORD quick_edit_flag = enable ? ENABLE_QUICK_EDIT_MODE : ~ENABLE_QUICK_EDIT_MODE;
	HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(input_handle, &prev_mode);
	SetConsoleMode(input_handle, prev_mode & quick_edit_flag | ENABLE_EXTENDED_FLAGS);
}

MeridorConsoleLib::Window::Window(const WindowInfoEx& window_info_ex)
{
	this->window_info = window_info_ex;
	main_color = &window_info.main_color;
	secondary_color = &window_info.secondary_color;

	SetConsoleTitle(window_info_ex.title.c_str());
	AdjustFontSize();
	SetWindowSize();					
	SetCursor(false);
	SetConsoleEditMode(false);

	//change this rng
	srand(static_cast<int>(time(0)));
}
std::vector<std::string> MeridorConsoleLib::Window::ReadFile(const std::string path)
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
void MeridorConsoleLib::Window::Pause(const int miliseconds)
{
	DWORD consolesettings;
	HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(input_handle, &consolesettings);
	SetConsoleMode(input_handle, 0 & ~ENABLE_ECHO_INPUT);
	Sleep(miliseconds);
	FlushConsoleInputBuffer(input_handle);
	SetConsoleMode(input_handle, consolesettings);
}
int MeridorConsoleLib::Window::GetCharactersPerRow()
{
	return window_info.characters_capacity.X;
}
int MeridorConsoleLib::Window::GetCharactersPerColumn()
{
	return window_info.characters_capacity.Y;
}
int MeridorConsoleLib::Window::GetFontSize()
{
	return font_size;
}
HANDLE MeridorConsoleLib::Window::GetHandle()
{
	return window_info.handle;
}
HWND MeridorConsoleLib::Window::GetHWND()
{
	return window_info.hwnd;
}
float MeridorConsoleLib::Window::GetMusicVolume()
{
	return music_volume;
}
const MeridorConsoleLib::WindowInfo* MeridorConsoleLib::Window::GetWindowInfo()
{
	return &window_info;
}
const MeridorConsoleLib::WindowInfoEx* MeridorConsoleLib::Window::GetWindowInfoEx()
{
	return &window_info;
}
void MeridorConsoleLib::Window::SetCursor(const bool visible)
{
	CONSOLE_CURSOR_INFO console_cursor;
	GetConsoleCursorInfo(window_info.handle, &console_cursor);
	console_cursor.bVisible = visible;
	SetConsoleCursorInfo(window_info.handle, &console_cursor);
}
void MeridorConsoleLib::Window::SetMusic(float volume)
{
	this->music_volume = volume;
	wav_transformer.SetFlags(SND_ASYNC | SND_LOOP);
	wav_transformer.StartPlaying(volume);
}
#include "Window.h"


void MeridorConsoleLib::Window::AdjustFontSize()
{
	CONSOLE_FONT_INFOEX ConsoleFontInfoEx = {};
	ConsoleFontInfoEx.cbSize = sizeof(ConsoleFontInfoEx);
	ConsoleFontInfoEx.dwFontSize.Y = 64;
	wcscpy_s(ConsoleFontInfoEx.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(window_info.handle, NULL, &ConsoleFontInfoEx);
	
	COORD window_size_with_current_font_size = GetLargestConsoleWindow();
	while ((window_size_with_current_font_size.X < window_info.characters_capacity.X || window_size_with_current_font_size.Y < window_info.characters_capacity.Y) && ConsoleFontInfoEx.dwFontSize.Y > 0)
	{
		--ConsoleFontInfoEx.dwFontSize.Y;
		SetCurrentConsoleFontEx(window_info.handle, NULL, &ConsoleFontInfoEx);
		window_size_with_current_font_size = GetLargestConsoleWindow();
	}
	font_size = ConsoleFontInfoEx.dwFontSize.Y;
}

void MeridorConsoleLib::Window::SetWindowSize()
{
	LONG flags;
	if (window_info.window_mode == WindowMode::fullscreen)
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
	
	short caption_size = GetSystemMetrics(SM_CYCAPTION) * (window_info.window_mode != WindowMode::fullscreen);
	short x = GetSystemMetrics(SM_CXSCREEN) / font_info.dwFontSize.X;
	short y = (GetSystemMetrics(SM_CYSCREEN) - caption_size) / font_info.dwFontSize.Y;

	return { x, y };
}

void MeridorConsoleLib::Window::Init(const WindowInfoEx& window_info_ex)
{
	this->window_info = window_info_ex;
	main_color = &window_info.main_color;
	secondary_color = &window_info.secondary_color;

	window_immobilizer.Init(this);
	SetConsoleTitle(window_info_ex.title.c_str());
	AdjustFontSize();
	SetWindowSize();
	SetCursor(window_info.visible_cursor);
	SetConsoleEditMode(false);

	if (window_info.window_mode == WindowMode::windowed)
	{
		window_immobilizer.Start();
	}
}

void MeridorConsoleLib::Window::SetConsoleEditMode(bool enable)
{
	DWORD prev_mode;
	DWORD quick_edit_flag = enable ? ENABLE_QUICK_EDIT_MODE : ~ENABLE_QUICK_EDIT_MODE;
	HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(input_handle, &prev_mode);
	SetConsoleMode(input_handle, prev_mode & quick_edit_flag | ENABLE_EXTENDED_FLAGS);
}
void MeridorConsoleLib::Window::BlockingSleep(const int miliseconds)
{
	DWORD console_settings;
	HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(input_handle, &console_settings);
	SetConsoleMode(input_handle, 0 & ~ENABLE_ECHO_INPUT);
	Sleep(miliseconds);
	FlushConsoleInputBuffer(input_handle);
	SetConsoleMode(input_handle, console_settings);
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
void MeridorConsoleLib::Window::SetWindowMode(WindowMode window_mode)
{
	if (window_mode == WindowMode::fullscreen)
	{
		window_immobilizer.Stop();
	}
	else
	{
		window_immobilizer.Start();
	}
	window_info.window_mode = window_mode;
	SetWindowSize();
}

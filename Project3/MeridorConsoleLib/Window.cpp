#include "Window.h"

void MeridorConsoleLib::Window::AdjustFontSize()
{
	CONSOLE_FONT_INFOEX console_font_info = {};
	console_font_info.cbSize = sizeof(console_font_info);
	console_font_info.dwFontSize.Y = 64;

	wcscpy_s(console_font_info.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(window_info.handle, NULL, &console_font_info);
	
	COORD window_size_with_current_font_size = GetMetricsWithSelectedFontSize();
	short upper_bound = 128;
	short lower_bound = 1;

	while (upper_bound > lower_bound)
	{
		const short midpoint = (lower_bound + upper_bound) / 2;

		console_font_info.dwFontSize.Y = midpoint;
		SetCurrentConsoleFontEx(window_info.handle, NULL, &console_font_info);
		window_size_with_current_font_size = GetMetricsWithSelectedFontSize();
		
		if (upper_bound - lower_bound <= 1)
		{
			if (midpoint == upper_bound)
			{
				console_font_info.dwFontSize.Y = lower_bound;
				SetCurrentConsoleFontEx(window_info.handle, NULL, &console_font_info);
				window_size_with_current_font_size = GetMetricsWithSelectedFontSize();
			}
			break;
		}
		else if (window_size_with_current_font_size.Y < window_info.characters_capacity_height)
		{
			upper_bound = midpoint;
		}
		else
		{
			lower_bound = midpoint;
		}
	}
	font_size = console_font_info.dwFontSize.Y;
}
void MeridorConsoleLib::Window::SetWindowSize()
{
	LONG flags;
	switch (window_info.window_mode)
	{
		case WindowMode::fullscreen:
		{
			flags = WS_POPUPWINDOW & ~WS_CAPTION & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_HSCROLL & ~WS_VSCROLL;
			break;
		}
		case WindowMode::windowed_fullscreen:
		case WindowMode::windowed:
		{
			flags = WS_CAPTION | WS_POPUPWINDOW | WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_HSCROLL & ~WS_VSCROLL;
			break;
		}
	}
	SetWindowLongA(window_info.hwnd, GWL_STYLE, flags);
	if (window_info.window_mode != WindowMode::windowed)
	{
		ShowWindow(window_info.hwnd, SW_SHOWMAXIMIZED);
	}
	else
	{
		MoveWindow(window_info.hwnd, 0, 0, window_info.window_size.X, window_info.window_size.Y, true);
		ShowWindow(window_info.hwnd, SW_NORMAL);
	}
}
COORD MeridorConsoleLib::Window::GetMetricsWithSelectedFontSize()
{
	CONSOLE_FONT_INFO font_info;
	GetCurrentConsoleFont(window_info.handle, false, &font_info);

	short caption_size = GetSystemMetrics(SM_CYCAPTION) * (window_info.window_mode != WindowMode::fullscreen);
	short x = window_info.window_size.X / font_info.dwFontSize.X;
	short y = (window_info.window_size.Y - caption_size) / font_info.dwFontSize.Y;

	return { x, y };
}
void MeridorConsoleLib::Window::SetBufferSize()
{
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(window_info.handle, &screen_buffer_info);
	COORD buffer_size;
	buffer_size.X = screen_buffer_info.srWindow.Right - screen_buffer_info.srWindow.Left + 1;
	buffer_size.Y = screen_buffer_info.srWindow.Bottom - screen_buffer_info.srWindow.Top + 1;

	SetConsoleScreenBufferSize(window_info.handle, buffer_size);
}
void MeridorConsoleLib::Window::Init(const WindowInfoEx& window_info_ex)
{
	this->window_info = window_info_ex;
	main_color = &window_info.main_color;
	secondary_color = &window_info.secondary_color;
	if (window_info.window_mode != WindowMode::windowed)
	{
		window_info.window_size = { static_cast<short>(GetSystemMetrics(SM_CXSCREEN)), static_cast<short>(GetSystemMetrics(SM_CYSCREEN)) };
	}

	window_immobilizer.Init(this);
	SetConsoleTitleA(window_info_ex.title.c_str());
	SetWindowSize();
	AdjustFontSize();
	SetBufferSize();
	SetCursor(window_info.visible_cursor);
	SetConsoleEditMode(false);

	if (window_info.window_mode == WindowMode::windowed_fullscreen)
	{
		window_immobilizer.Start();
	}
}
void MeridorConsoleLib::Window::SetConsoleEditMode(bool enable)
{
	DWORD prev_mode;
	HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(input_handle, &prev_mode);
	if (enable)
	{
		SetConsoleMode(input_handle, prev_mode | ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
	}
	else
	{
		SetConsoleMode(input_handle, prev_mode & ~ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
	}
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
	CONSOLE_FONT_INFOEX console_font_info = {};
	GetCurrentConsoleFontEx(window_info.handle, false, &console_font_info);
	return console_font_info.dwFontSize.X;
}
int MeridorConsoleLib::Window::GetCharactersPerColumn()
{
	CONSOLE_FONT_INFOEX console_font_info = {};
	GetCurrentConsoleFontEx(window_info.handle, false, &console_font_info);
	return console_font_info.dwFontSize.Y;
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
MeridorConsoleLib::WindowMode MeridorConsoleLib::Window::GetWindowMode()
{
	return window_info.window_mode;
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
	SetCursor(window_info.visible_cursor);
}

#include "Functions.h"
namespace MeridorConsoleLib
{
	namespace Window
	{
		void BlockingSleep(int miliseconds)
		{
			DWORD console_settings;
			HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);

			GetConsoleMode(input_handle, &console_settings);
			SetConsoleMode(input_handle, 0 & ~ENABLE_ECHO_INPUT);

			Sleep(miliseconds);

			FlushConsoleInputBuffer(input_handle);
			SetConsoleMode(input_handle, console_settings);
		}
		void SetConsoleEditMode(BOOL enable)
		{
			DWORD prev_mode;
			HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
			GetConsoleMode(input_handle, &prev_mode);
			if (enable == TRUE)
			{
				SetConsoleMode(input_handle, prev_mode | ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
			}
			else
			{
				SetConsoleMode(input_handle, (prev_mode & ~ENABLE_QUICK_EDIT_MODE) | ENABLE_EXTENDED_FLAGS);
			}
		}
		void SetCursor(HANDLE output_handle, BOOL visible)
		{
			CONSOLE_CURSOR_INFO console_cursor;
			GetConsoleCursorInfo(output_handle, &console_cursor);
			console_cursor.bVisible = visible;
			SetConsoleCursorInfo(output_handle, &console_cursor);
		}
		bool IsMinimizeButtonPressed(HANDLE input_handle)
		{
			DWORD number_of_events;

			GetNumberOfConsoleInputEvents(input_handle, &number_of_events);
			INPUT_RECORD buffer[window_immobilizer_buffer_size];
			ReadConsoleInputA(input_handle, buffer, window_immobilizer_buffer_size, &number_of_events);

			for (DWORD i = 0; i < number_of_events; ++i)
			{
				if (buffer[i].EventType == FOCUS_EVENT && buffer[i].Event.MenuEvent.dwCommandId == 0)
				{
					return true;
				}
			}
			return false;
		}
		LONG GetWindowFlags(WindowMode window_mode)
		{
			switch (window_mode)
			{
			case WindowMode::fullscreen:
			{
				return (WS_POPUPWINDOW & ~WS_CAPTION & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_HSCROLL & ~WS_VSCROLL);
			}
			case WindowMode::windowed_fullscreen:
			case WindowMode::windowed:
			{
				return ((WS_CAPTION | WS_POPUPWINDOW | WS_MINIMIZEBOX) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_HSCROLL & ~WS_VSCROLL);
			}
			}
			abort();
		}
	}
}
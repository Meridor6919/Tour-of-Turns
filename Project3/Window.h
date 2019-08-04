#pragma once
#include <Windows.h>
#include <time.h>
#include <string>

#pragma comment(lib, "winmm.lib")
#pragma warning(disable : 4996)

class Window
{
protected:
	HANDLE window_handle;
	HWND window_hwnd;
	COORD window_size;
	int font_size;
	char title[254];
	CONSOLE_CURSOR_INFO console_cursor;
public:
	int color1;
	int color2;

	Window(const std::string title, const int color1, const int color2, const short chars_in_rows, const short chars_in_columns);
	int GetWidth();
	int GetHeight();
	int GetFontSize();
	HANDLE GetHandle();
	HWND GetHWND();
	void SetCursor(const bool visible);
	void SetMusic(const std::string sound_file,const bool playing);
	void Pause(const int miliseconds);
};
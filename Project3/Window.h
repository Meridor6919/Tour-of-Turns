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

	Window(char title[], int color1, int color2, int font_size, bool fullscreen, int chars_in_rows = 0, int chars_in_columns = 0);

	int GetWidth();
	int GetHeight();
	int GetFontSize();
	HANDLE GetHandle();
	HWND GetHWND();

	void SetWindowFlags(int flags);
	void SetCursor(bool visible);
	void SetMusic(std::string sound_file, bool playing);
	void Pause(int miliseconds);

};
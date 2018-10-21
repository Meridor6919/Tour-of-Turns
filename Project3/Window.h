#pragma once
#include <Windows.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")
#pragma warning(disable : 4996)

class Window
{
	HANDLE window_handle;
	COORD window_size;
	int font_size;
	char title[254];
	CONSOLE_CURSOR_INFO console_cursor;
	bool hamachi_enabled;

public:

	int color1;
	int color2;

	Window(char title[], int color1, int color2, bool music_playing, int font_size, bool fullscreen, int chars_in_rows = 0, int chars_in_columns = 0);

	int GetWidth();
	int GetHeight();
	int GetFontSize();
	HANDLE GetHandle();

	void SetCursor(bool visible);
	void SetMusic(bool playing);
	void SetHamachiConectionFlag(bool flag);
	void Pause(int miliseconds);
	
};
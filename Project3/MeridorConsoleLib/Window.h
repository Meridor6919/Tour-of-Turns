#pragma once
#include <Windows.h>
#include <time.h>
#include <string>
#include <vector>
#include "..\MeridorMiscModules\WavTransformer.h"

#pragma warning(disable : 4996)

namespace MeridorConsoleLib
{
	class Window
	{
	protected:
		float music_volume;
		HANDLE window_handle;
		HWND window_hwnd;
		COORD window_size;
		int font_size;
		WavTransformer wav_transformer;
	public:
		int color1;
		int color2;

		//Genera
		Window(const std::string title, const int color1, const int color2, const short chars_in_rows, const short chars_in_columns);
		std::vector<std::string> ReadFile(std::string path);
		void Pause(const int miliseconds);

		//Get methods
		int GetWidth();
		int GetHeight();
		int GetFontSize();
		HANDLE GetHandle();
		HWND GetHWND();
		float GetMusicVolume();

		//Set methods
		void SetCursor(const bool visible);
		void SetMusic(float volume);
	};
}
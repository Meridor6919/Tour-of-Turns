#pragma once
#include <Windows.h>
#include <time.h>
#include <string>
#include <vector>

#include "..\MeridorMiscModules\WavTransformer.h"
#include "DataStructures.h"

#pragma warning(disable : 4996)

namespace MeridorConsoleLib
{
	class Window
	{
	protected:

		WindowInfoEx window_info;
		WavTransformer wav_transformer;
		int font_size;
		float music_volume;

		void AdjustFontSize();
		void SetWindowSize();

	public:

		int* main_color;
		int* secondary_color;
		
		Window(const WindowInfoEx& window_info);
		std::vector<std::string> ReadFile(std::string path);
		void Pause(const int miliseconds);

		const WindowInfo* GetWindowInfo();
		const WindowInfoEx* GetWindowInfoEx();

		int GetCharactersPerRow();
		int GetCharactersPerColumn();
		int GetFontSize();
		HANDLE GetHandle();
		HWND GetHWND();
		float GetMusicVolume();
		
		void SetCursor(const bool visible);
		void SetMusic(float volume);
	};
}
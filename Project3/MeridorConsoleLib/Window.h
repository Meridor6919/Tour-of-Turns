#pragma once
#include <Windows.h>
#include <time.h>
#include <string>
#include <vector>

#include "..\MeridorMiscModules\WavTransformer.h"
#include "DataStructures.h"
#include "WindowImmobilizer.h"

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
		COORD GetLargestConsoleWindow();

		WindowImmobilizer window_immobilizer;

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
		
		//There is a bug where edit-mode causes SetWindowPosition to crash console window - don't use with windowed fullscreen
		void SetConsoleEditMode(bool enable);
		void SetCursor(const bool visible);
		void SetMusic(float volume);

		friend class WindowImmobilizer;
	};
}
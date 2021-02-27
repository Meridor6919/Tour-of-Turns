#pragma once
#include "DataStructures.h"
#include "WindowImmobilizer.h"

namespace MeridorConsoleLib
{
	class Window
	{
	protected:

		WindowInfoEx window_info;
		WindowImmobilizer window_immobilizer;
		int font_size;

		void AdjustFontSize();
		void SetWindowSize();
		COORD GetLargestConsoleWindow();

		void Init(const WindowInfoEx& window_info_ex);
	public:

		int* main_color;
		int* secondary_color;

		void BlockingSleep(const int miliseconds);

		const WindowInfo* GetWindowInfo();
		const WindowInfoEx* GetWindowInfoEx();
		int GetCharactersPerRow();
		int GetCharactersPerColumn();
		int GetFontSize();
		HANDLE GetHandle();
		HWND GetHWND();
		WindowMode GetWindowMode();

		//There is a bug where edit-mode causes SetWindowPosition to crash console window - don't use with windowed fullscreen
		void SetConsoleEditMode(bool enable);
		void SetCursor(const bool visible);
		void SetWindowMode(WindowMode window_mode);

		friend class WindowImmobilizer;
	};
}
#pragma once
#include "WindowUtility.h"
#include "WindowImmobilizer.h"

namespace MeridorConsoleLib
{
	class Window
	{
	protected:

		WindowInfo window_info;
		WindowImmobilizer window_immobilizer;
		COORD font_size;
		COORD base_character_capacity;

		void AdjustFontSize();
		void SetWindowSize();
		COORD GetMetricsWithSelectedFontSize();
		void SetBufferSize();
		void UpdateWindowInformation();

		Window() {};
		void Init(const WindowInfo& window_info_ex);

	public:

		const WindowInfo* GetWindowInfo();
		int GetCharactersPerRow();
		int GetCharactersPerColumn();
		COORD GetFontSize();
		HANDLE GetOutputHandle();
		HANDLE GetInputHandle();
		HWND GetHWND();
		WindowMode GetWindowMode();

		void SetConsoleEditMode(bool enable);
		void SetCursor(const bool visible);
		void SetWindowMode(WindowMode window_mode, COORD window_size = MeridorConsoleLib::minimum_window_size);

		void BlockingSleep(const int miliseconds);
		
		friend class WindowImmobilizer;
	};
}